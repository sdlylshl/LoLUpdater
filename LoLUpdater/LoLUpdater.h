#define VC_EXTRALEAN

#include <tchar.h>
#include <direct.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <ShlObj.h>

#include <D3D9.h>
#include <D3dx9core.h>

#include <stdint.h>
#include <intrin.h>
	inline void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
	{
		__cpuidex(abcd, eax, ecx);
	}

	inline int check_xcr0_ymm()
	{
		uint32_t xcr0;
		xcr0 = (uint32_t)_xgetbv(0);
		return ((xcr0 & 6) == 6);
	}

	inline int check_4th_gen_intel_core_features()
	{
		int abcd[4];
		uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
		uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);
		run_cpuid(1, 0, abcd);
		if ((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask)
			return 0;
		if (!check_xcr0_ymm())
			return 0;
		run_cpuid(7, 0, abcd);
		if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
			return 0;
		run_cpuid(0x80000001, 0, abcd);
		if ((abcd[2] & (1 << 5)) == 0)
			return 0;
		return 1;
	}

	static int can_use_intel_core_4th_gen_features()
	{
		static int the_4th_gen_features_available = -1;
		if (the_4th_gen_features_available < 0)
			the_4th_gen_features_available = check_4th_gen_intel_core_features();
		return the_4th_gen_features_available;
	}
