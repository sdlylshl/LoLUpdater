#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>
#include <iostream>
#include <sstream>
#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)

#include <immintrin.h>

int check_4th_gen_intel_core_features()
{
	const int the_4th_gen_features =
		(_FEATURE_AVX2 | _FEATURE_FMA | _FEATURE_BMI | _FEATURE_LZCNT | _FEATURE_MOVBE);
	return _may_i_use_cpu_feature(the_4th_gen_features);
}

#else

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif
#include <fstream>
#include <vector>

inline void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
#if defined(_MSC_VER)
	__cpuidex(abcd, eax, ecx);
#else
	uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
	__asm__("movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
	__asm__("cpuid" : "+b" (ebx),
# endif
		"+a" (eax), "+c" (ecx), "=d" (edx));
	abcd[0] = eax;
	abcd[1] = ebx;
	abcd[2] = ecx;
	abcd[3] = edx;
#endif
}

inline int check_xcr0_ymm()
{
	uint32_t xcr0;
#if defined(_MSC_VER)
	//Todo: Find conversion-method for long to uint to remove C-style cast.  
	xcr0 = (uint32_t)_xgetbv(0);
#else
	__asm__("xgetbv" : "=a" (xcr0) : "c" (0) : "%edx");
#endif
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

#endif


static int can_use_intel_core_4th_gen_features()
{
	static int the_4th_gen_features_available = -1;
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();

	return the_4th_gen_features_available;
}


#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#else
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif


#undef CONTEXT_XSTATE

#if defined(_M_X64)
#define CONTEXT_XSTATE                      (0x00100040)
#else
#define CONTEXT_XSTATE                      (0x00010040)
#endif


#define XSTATE_MASK_AVX                     (XSTATE_MASK_GSSE)

typedef DWORD64(WINAPI * PGETENABLEDXSTATEFEATURES)();

inline bool file_exists(std::wstring(fileName))
{
	std::ifstream infile(fileName);
	return infile.good();
}

inline std::string GetExeFileName()
{
	char buffer[261];
	GetModuleFileNameA(nullptr, buffer, 261);
	return std::string(buffer);
}

inline std::string GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
}


// Not sure of how to remove C-Style cast here
PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetEnabledXStateFeatures");
DWORD64 FeatureMask = pfnGetEnabledXStateFeatures();


// 261 = Max_PATH without null delimiter (260+1)
// Buffers
std::wstringstream buff_c[261];
std::wstringstream tbb0[261];
std::wstringstream airfile[261];
std::wstringstream airdir[261];
std::wstringstream flashfile[261];
std::wstringstream flashdir[261];
std::wstringstream cgbin[261];
std::wstringstream cginstunblock[261];
std::wstringstream airinstunblock[261];
std::wstringstream strair[261];
std::wstringstream airunblock[261];
std::wstringstream cgd3d9bin[261];
std::wstringstream cgglbin[261];
std::wstringstream cgpath[261];
std::wstringstream cgglpath[261];
std::wstringstream tbbunblock[261];
std::wstringstream flashunblock[261];
std::wstringstream cgd3d9path[261];
std::wstringstream cgglunblock[261];
std::wstringstream cgunblock[261];
std::wstringstream cgd3d9unblock[261];
std::wstringstream airpath_f[261];
std::wstringstream slnpath_f[261];
std::vector<wchar_t> cgbinpath[261];

// Constants
std::wstring cwd(std::to_wstring(GetExePath()[0]));

std::wstring drive(&cwd[0]);

std::wstring envget(std::to_wstring(GetEnvironmentVariableW(L"CG_BIN_PATH",
	&*cgbinpath[0].begin(),
260)));

std::wstring unblock(L":Zone.Identifier");
std::wstring air(L"Adobe AIR.dll");
std::wstring flash(L"Resources\\NPSWF32.dll");
std::wstring cgfile(L"Cg.dll");
std::wstring cgglfile(L"CgGL.dll");
std::wstring cgd3d9file(L"CgD3D9.dll");
std::wstring prog(L":\\Program Files (x86)");
std::wstring progx86(L":\\Program Files");
std::wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
std::wstring tbbfile(L"tbb.dll");
std::wstring adobepath(L"\\Common Files\\Adobe AIR\\Versions\\1.0\\");
std::wstring airwin(L"air15_win.exe");

std::wstring unblockfiles[] = { cgunblock[0].str(), cgglunblock[0].str(), cgd3d9unblock[0].str(), tbbunblock[0].str(), airunblock[0].str(), flashunblock[0].str() };
