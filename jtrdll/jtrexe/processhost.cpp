#include<string>
#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdarg.h>
#ifdef _WIN32
#include<windows.h>
#include<AccCtrl.h>
#include<Aclapi.h>
#include<io.h>
#endif
#include"cpuinformation.h"
#include"jtrexe.h"

////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
typedef HANDLE PIPETYPE;
#define	THREADCALL DWORD WINAPI

BOOL GetUserSid(SID *pSidUser)
{
	PTOKEN_USER pTokenUser = NULL;
	HANDLE      hToken;
	DWORD       dwLength;
	DWORD       cbName = 250;
	DWORD       cbDomainName = 250;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	if (!GetTokenInformation(hToken,       // handle of the access token
		TokenUser,    // type of information to retrieve
		pTokenUser,   // address of retrieved information 
		0,            // size of the information buffer
		&dwLength     // address of required buffer size
		))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			pTokenUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLength);
			if (pTokenUser == NULL)
			{
				CloseHandle(hToken);
				return FALSE;
			}
		}
		else
		{
			CloseHandle(hToken);
			return FALSE;
		}
	}
	else
	{
		CloseHandle(hToken);
		return FALSE;
	}

	if (!GetTokenInformation(hToken,     // handle of the access token
		TokenUser,  // type of information to retrieve
		pTokenUser, // address of retrieved information 
		dwLength,   // size of the information buffer
		&dwLength   // address of required buffer size
		))
	{
		HeapFree(GetProcessHeap(), 0, pTokenUser);
		CloseHandle(hToken);
		return FALSE;
	}

	if (!CopySid(SECURITY_MAX_SID_SIZE, pSidUser, pTokenUser->User.Sid))
	{
		HeapFree(GetProcessHeap(), 0, pTokenUser);
		CloseHandle(hToken);
		return FALSE;
	}

	HeapFree(GetProcessHeap(), 0, pTokenUser);
	CloseHandle(hToken);
	return TRUE;
}
#else
#error "Implement me"
#endif

PIPETYPE create_control_pipe(std::string & ctlpipe_name)
{
#ifdef _WIN32
	UUID uuid;
	UuidCreate(&uuid);

	RPC_CSTR uuidstr;
	UuidToString(&uuid, &uuidstr);

	std::string pipename = "\\\\.\\pipe\\jtrdll_";
	pipename += (const char *)uuidstr;

	ctlpipe_name = pipename;

	RpcStringFree(&uuidstr);

	SECURITY_DESCRIPTOR sd;
	memset(&sd, 0, sizeof(SECURITY_DESCRIPTOR));
	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		return NULL;
	}

	char usersidbuf[SECURITY_MAX_SID_SIZE];
	SID *usersid = (SID *)usersidbuf;

	if (!GetUserSid(usersid))
	{
		return NULL;
	}

	EXPLICIT_ACCESS ea;
	ea.grfAccessPermissions = SPECIFIC_RIGHTS_ALL | STANDARD_RIGHTS_ALL;
	ea.grfAccessMode = SET_ACCESS;
	ea.grfInheritance = NO_INHERITANCE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName = (LPTSTR)usersid;

	PACL acl = NULL;
	SetEntriesInAcl(1, &ea, NULL, &acl);
	if (!SetSecurityDescriptorDacl(&sd, TRUE, acl, FALSE))
	{
		LocalFree(acl);
		return NULL;
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = FALSE;

	HANDLE nphandle = CreateNamedPipe(pipename.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 65536, 65536, 2000, &sa);

	LocalFree(acl);

	return nphandle;
#else
#error "implement me"
#endif
}

void destroy_control_pipe(PIPETYPE pipe)
{
#ifdef _WIN32
	FlushFileBuffers(pipe);
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);
#else
#error "implement me"
#endif
}

bool wait_for_pipe_client(PIPETYPE pipe)
{
#ifdef _WIN32
	if (!ConnectNamedPipe(pipe, NULL))
	{
		DWORD err = GetLastError();
		if (err != ERROR_PIPE_CONNECTED)
		{
			return false;
		}
	}
	return true;
#else
#error "implement me"
#endif
}

void create_command_thread(void * entrypoint, void *tparam)
{
#ifdef _WIN32
	DWORD tid;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entrypoint, tparam, 0, &tid);
	CloseHandle(hThread);
#else
#error "implement me"
#endif
}

bool send_command(PIPETYPE h, const char *name, int size = 0, const void *buf = NULL)
{
#ifdef _WIN32
	DWORD nlen = (DWORD)strlen(name);
	DWORD dwBytesWritten;
	if (nlen == 0)
	{
		return false;
	}

	if (!WriteFile(h, &nlen, sizeof(int), &dwBytesWritten, NULL) || dwBytesWritten != sizeof(int))
	{
		return false;
	}

	if (!WriteFile(h, name, nlen, &dwBytesWritten, NULL) || dwBytesWritten != nlen)
	{
		return false;
	}

	if (!WriteFile(h, &size, sizeof(size), &dwBytesWritten, NULL) || dwBytesWritten != sizeof(size))
	{
		return false;
	}

	if (size > 0)
	{
		if (!WriteFile(h, buf, size, &dwBytesWritten, NULL) || dwBytesWritten != size)
		{
			return false;
		}
	}

	FlushFileBuffers(h);
	return true;
#else
#error "implement me"
#endif

}


bool writeStdOut(const char *fmt, ...)
{

#ifdef _WIN32
	va_list va;
	va_start(va, fmt);

	/*
	vprintf(fmt, va);
	fflush(stdout);
	*/

	char bufdata[2048];
	char *buf = bufdata;
	
	unsigned required = _vsnprintf_s(buf, 2048, _TRUNCATE, fmt, va);
	if (required >= 2048)
	{
		buf = (char *)malloc(required + 1);
		_vsnprintf_s(buf, required + 1, _TRUNCATE, fmt, va);
	}
	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	DWORD dwBytesWritten;
	DWORD dwTotalBytesWritten = 0;
	DWORD dwBytesToWrite = required;
	while (dwBytesToWrite > 0)
	{
		if (!WriteFile(hStdOut, buf + dwTotalBytesWritten, dwBytesToWrite, &dwBytesWritten, NULL))
		{
			if(buf != bufdata)
				free(buf);
			return false;
		}
		dwBytesToWrite -= dwBytesWritten;
		dwTotalBytesWritten += dwBytesWritten;
	}
	
	if(buf != bufdata)
		free(buf);

	FlushFileBuffers(hStdOut);
	
#else
#error "implement me"
#endif

	return true;
}



////////////////////////////////////////////////////////////////////////

struct THREADPARAMS_JTRDLL_MAIN {
	std::string ctlpipe_name;
	PIPETYPE ctlpipe_handle;

	int argc;
	std::vector<char *> argv;
	JTRDLL_HOOKS hooks;
};

void jtrdll_main_stdout_hook(void *ctx, const char *str)
{
	THREADPARAMS_JTRDLL_MAIN *tparam = (THREADPARAMS_JTRDLL_MAIN *)ctx;
	send_command(tparam->ctlpipe_handle, "stdout", (DWORD)strlen(str), str);
}

void jtrdll_main_stderr_hook(void *ctx, const char *str)
{
	THREADPARAMS_JTRDLL_MAIN *tparam = (THREADPARAMS_JTRDLL_MAIN *)ctx;
	send_command(tparam->ctlpipe_handle, "stderr", (DWORD)strlen(str), str);
}

THREADCALL jtrdll_main_thread(THREADPARAMS_JTRDLL_MAIN *tparam)
{
	if (!wait_for_pipe_client(tparam->ctlpipe_handle))
	{
		destroy_control_pipe(tparam->ctlpipe_handle);
		delete tparam;
		return -1;
	}

	int ret = jtrdll_main(tparam->argc, &(tparam->argv[0]), &tparam->hooks);
	if (tparam->hooks.caught_sigill)
	{
		send_command(tparam->ctlpipe_handle, "sigill", sizeof(ret), &ret);
	}
	else
	{
		send_command(tparam->ctlpipe_handle, "return", sizeof(ret), &ret);
	}

	destroy_control_pipe(tparam->ctlpipe_handle);
	delete tparam;
	return 0;
}

////////////////////////////////////////////////////////////////////////

THREADCALL jtrdll_abort_thread(void *)
{
	jtrdll_abort();
	return 0;
}

////////////////////////////////////////////////////////////////////////


struct THREADPARAMS_JTRDLL_GET_STATUS {
	std::string ctlpipe_name;
	PIPETYPE ctlpipe_handle;
};

THREADCALL jtrdll_get_status_thread(THREADPARAMS_JTRDLL_GET_STATUS *tparam)
{
	if (!wait_for_pipe_client(tparam->ctlpipe_handle))
	{
		destroy_control_pipe(tparam->ctlpipe_handle);
		delete tparam;
		return -1;
	}

	JTRDLL_STATUS status;
	memset(&status, 0, sizeof(status));
	jtrdll_get_status(&status);
	send_command(tparam->ctlpipe_handle, "status", sizeof(status), &status);

	destroy_control_pipe(tparam->ctlpipe_handle);
	delete tparam;
	return 0;
}

////////////////////////////////////////////////////////////////////////


struct THREADPARAMS_JTRDLL_GET_CHARSET_INFO {
	std::string ctlpipe_name;
	PIPETYPE ctlpipe_handle;

	char path[MAX_PATH];
};

THREADCALL jtrdll_get_charset_info_thread(THREADPARAMS_JTRDLL_GET_CHARSET_INFO *tparam)
{
	if (!wait_for_pipe_client(tparam->ctlpipe_handle))
	{
		destroy_control_pipe(tparam->ctlpipe_handle);
		delete tparam;
		return -1;
	}

	JTRDLL_STATUS status;
	memset(&status, 0, sizeof(status));

	unsigned char charmin;
	unsigned char charmax;
	unsigned char len;
	unsigned char count;
	unsigned char allchars[256];

	int ret = jtrdll_get_charset_info(tparam->path, &charmin, &charmax, &len, &count, allchars);

	send_command(tparam->ctlpipe_handle, "charmin", sizeof(charmin), &charmin);
	send_command(tparam->ctlpipe_handle, "charmax", sizeof(charmax), &charmax);
	send_command(tparam->ctlpipe_handle, "len", sizeof(len), &len);
	send_command(tparam->ctlpipe_handle, "count", sizeof(count), &count);
	send_command(tparam->ctlpipe_handle, "allchars", sizeof(allchars), &allchars);
	send_command(tparam->ctlpipe_handle, "return", sizeof(ret), &ret);

	destroy_control_pipe(tparam->ctlpipe_handle);
	delete tparam;
	return 0;
}

////////////////////////////////////////////////////////////////////////

int run_processhost(void)
{
	bool done = false;
	std::string line;
	int ret = 0;

	while (!done && std::getline(std::cin, line))
	{
		try
		{
			// jtrdll_main / appdatadir / argc / arguments *
			if (line == "jtrdll_main")
			{
				THREADPARAMS_JTRDLL_MAIN * tp_jtrdll_main = new THREADPARAMS_JTRDLL_MAIN;

				std::getline(std::cin, line);
				strcpy_s(tp_jtrdll_main->hooks.appdatadir, sizeof(tp_jtrdll_main->hooks.appdatadir), line.c_str());

				tp_jtrdll_main->hooks.caught_sigill = false;
				tp_jtrdll_main->hooks.ctx = tp_jtrdll_main;
				tp_jtrdll_main->hooks.stderr_hook = jtrdll_main_stderr_hook;
				tp_jtrdll_main->hooks.stdout_hook = jtrdll_main_stdout_hook;

				std::getline(std::cin, line);
				tp_jtrdll_main->argc = atoi(line.c_str());

				tp_jtrdll_main->argv.resize(tp_jtrdll_main->argc + 1);
				for (int arg = 0; arg < tp_jtrdll_main->argc; arg++)
				{
					std::getline(std::cin, line);
					tp_jtrdll_main->argv[arg] = _strdup(line.c_str());
				}
				tp_jtrdll_main->argv[tp_jtrdll_main->argc] = NULL;

				tp_jtrdll_main->ctlpipe_handle = create_control_pipe(tp_jtrdll_main->ctlpipe_name);
				writeStdOut("pipe=%s\n", tp_jtrdll_main->ctlpipe_name.c_str());

				create_command_thread(jtrdll_main_thread, tp_jtrdll_main);
			}
			else if (line == "jtrdll_abort")
			{
				create_command_thread(jtrdll_abort_thread, NULL);
			}
			else if (line == "jtrdll_get_status")
			{
				THREADPARAMS_JTRDLL_GET_STATUS * tp_jtrdll_get_status = new THREADPARAMS_JTRDLL_GET_STATUS;
				tp_jtrdll_get_status->ctlpipe_handle = create_control_pipe(tp_jtrdll_get_status->ctlpipe_name);

				create_command_thread(jtrdll_get_status_thread, tp_jtrdll_get_status);
			}
			else if (line == "jtrdll_get_charset_info")
			{
				THREADPARAMS_JTRDLL_GET_CHARSET_INFO * tp_jtrdll_get_charset_info = new THREADPARAMS_JTRDLL_GET_CHARSET_INFO;

				std::getline(std::cin, line);
				strcpy_s(tp_jtrdll_get_charset_info->path, sizeof(tp_jtrdll_get_charset_info->path), line.c_str());

				tp_jtrdll_get_charset_info->ctlpipe_handle = create_control_pipe(tp_jtrdll_get_charset_info->ctlpipe_name);
				writeStdOut("pipe=%s\n", tp_jtrdll_get_charset_info->ctlpipe_name.c_str());

				create_command_thread(jtrdll_get_charset_info_thread, tp_jtrdll_get_charset_info);
			}
			else if (line == "exit")
			{
				done = true;
			}
		}
		catch (...)
		{
			done = true;
			ret = 1;
		}
	}

	return 0;
}


