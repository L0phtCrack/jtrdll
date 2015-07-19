#ifndef __INC_CPUINFORMATION
#define __INC_CPUINFORMATION

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <array>

class CPUInformation
{
	// forward declarations
	class InstructionSet_Internal;

public:

	// getters
	virtual int CoreCount(void);
	virtual std::string Vendor(void);
	virtual std::string Brand(void);

	virtual bool SSE3(void);
	virtual bool PCLMULQDQ(void);
	virtual bool MONITOR(void);
	virtual bool SSSE3(void);
	virtual bool FMA(void);
	virtual bool CMPXCHG16B(void);
	virtual bool SSE41(void);
	virtual bool SSE42(void);
	virtual bool MOVBE(void);
	virtual bool POPCNT(void);
	virtual bool AES(void);
	virtual bool XSAVE(void);
	virtual bool OSXSAVE(void);
	virtual bool AVX(void);
	virtual bool F16C(void);
	virtual bool RDRAND(void);

	virtual bool MSR(void);
	virtual bool CX8(void);
	virtual bool SEP(void);
	virtual bool CMOV(void);
	virtual bool CLFSH(void);
	virtual bool MMX(void);
	virtual bool FXSR(void);
	virtual bool SSE(void);
	virtual bool SSE2(void);

	virtual bool FSGSBASE(void);
	virtual bool BMI1(void);
	virtual bool HLE(void);
	virtual bool AVX2(void);
	virtual bool BMI2(void);
	virtual bool ERMS(void);
	virtual bool INVPCID(void);
	virtual bool RTM(void);
	virtual bool AVX512F(void);
	virtual bool RDSEED(void);
	virtual bool ADX(void);
	virtual bool AVX512PF(void);
	virtual bool AVX512ER(void);
	virtual bool AVX512CD(void);
	virtual bool SHA(void);

	virtual bool PREFETCHWT1(void);

	virtual bool LAHF(void);
	virtual bool LZCNT(void);
	virtual bool ABM(void);
	virtual bool SSE4a(void);
	virtual bool XOP(void);
	virtual bool TBM(void);

	virtual bool SYSCALL(void);
	virtual bool MMXEXT(void);
	virtual bool RDTSCP(void);
	virtual bool _3DNOWEXT(void);
	virtual bool _3DNOW(void);

private:

	class InstructionSet_Internal
	{
	public:
		InstructionSet_Internal();

		int nIds_;
		int nExIds_;
		std::string vendor_;
		std::string brand_;
		bool isIntel_;
		bool isAMD_;
		std::bitset<32> f_1_ECX_;
		std::bitset<32> f_1_EDX_;
		std::bitset<32> f_7_EBX_;
		std::bitset<32> f_7_ECX_;
		std::bitset<32> f_81_ECX_;
		std::bitset<32> f_81_EDX_;
		std::vector<std::array<int, 4>> data_;
		std::vector<std::array<int, 4>> extdata_;
	};

	InstructionSet_Internal CPU_Rep;
};

#endif