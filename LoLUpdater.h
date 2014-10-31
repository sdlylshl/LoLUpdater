#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#if XP == FALSE

#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)

#include <immintrin.h>

int check_4th_gen_intel_core_features()
{
	const int the_4th_gen_features =
		(_FEATURE_AVX2 | _FEATURE_FMA | _FEATURE_BMI | _FEATURE_LZCNT | _FEATURE_MOVBE);
	return _may_i_use_cpu_feature(the_4th_gen_features);
}

#else /* non-Intel compiler */

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif

bool avxSupported = false;

// If Visual Studio 2010 SP1 or later
#if (_MSC_FULL_VER >= 160040219)
// Checking for AVX requires 3 things:
// 1) CPUID indicates that the OS uses XSAVE and XRSTORE
//     instructions (allowing saving YMM registers on context
//     switch)
// 2) CPUID indicates support for AVX
// 3) XGETBV indicates the AVX registers will be saved and
//     restored on context switch
//
// Note that XGETBV is only available on 686 or later CPUs, so
// the instruction needs to be conditionally run.
#include "intrin.h"
int cpuInfo[4];
__cpuid(cpuInfo, 1);

bool osUsesXSAVE_XRSTORE = cpuInfo[2] & (1 << 27) || false;
bool cpuAVXSuport = cpuInfo[2] & (1 << 28) || false;
#endif
inline void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
#if defined(_MSC_VER)
	__cpuidex(abcd, eax, ecx);
#else
	uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
	/* in case of PIC under 32-bit EBX cannot be clobbered */
	__asm__("movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
	__asm__("cpuid" : "+b" (ebx),
# endif
		"+a" (eax), "+c" (ecx), "=d" (edx));
	abcd[0] = eax; abcd[1] = ebx; abcd[2] = ecx; abcd[3] = edx;
#endif
}

inline int check_xcr0_ymm()
{
	uint32_t xcr0;
#if defined(_MSC_VER)
	xcr0 = (uint32_t)_xgetbv(0);  /* min VS2010 SP1 compiler is required */
#else
	__asm__("xgetbv" : "=a" (xcr0) : "c" (0) : "%edx");
#endif
	return ((xcr0 & 6) == 6); /* checking if xmm and ymm state are enabled in XCR0 */
}


inline int check_4th_gen_intel_core_features()
{
	int abcd[4];
	uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
	uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);

	/* CPUID.(EAX=01H, ECX=0H):ECX.FMA[bit 12]==1   &&
	CPUID.(EAX=01H, ECX=0H):ECX.MOVBE[bit 22]==1 &&
	CPUID.(EAX=01H, ECX=0H):ECX.OSXSAVE[bit 27]==1 */
	run_cpuid(1, 0, abcd);
	if ((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask)
		return 0;

	if (!check_xcr0_ymm())
		return 0;

	/*  CPUID.(EAX=07H, ECX=0H):EBX.AVX2[bit 5]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI1[bit 3]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI2[bit 8]==1  */
	run_cpuid(7, 0, abcd);
	if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		return 0;

	/* CPUID.(EAX=80000001H):ECX.LZCNT[bit 5]==1 */
	run_cpuid(0x80000001, 0, abcd);
	if ((abcd[2] & (1 << 5)) == 0)
		return 0;

	return 1;
}

#endif /* non-Intel compiler */


static int can_use_intel_core_4th_gen_features()
{
	static int the_4th_gen_features_available = -1;
	/* test is performed once */
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();

	return the_4th_gen_features_available;
}
#endif

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif
using namespace std;

inline bool file_exists(wstring(fileName))
{
	ifstream infile(fileName);
	return infile.good();
}



// Buffers
wstringstream buff_c[MAX_PATH + 1];
wstringstream tbb0[MAX_PATH + 1];
wstringstream airfile[MAX_PATH + 1];
wstringstream airdir[MAX_PATH + 1];
wstringstream flashfile[MAX_PATH + 1];
wstringstream flashdir[MAX_PATH + 1];
wstringstream cgbin[MAX_PATH + 1];
wstringstream cginstunblock[MAX_PATH + 1];
wstringstream airinstunblock[MAX_PATH + 1];
wstringstream strair[MAX_PATH + 1];
wstringstream airunblock[MAX_PATH + 1];
wstringstream cgd3d9bin[MAX_PATH + 1];
wstringstream cgglbin[MAX_PATH + 1];
wstringstream cgpath[MAX_PATH + 1];
wstringstream cgglpath[MAX_PATH + 1];
wstringstream tbbunblock[MAX_PATH + 1];
wstringstream flashunblock[MAX_PATH + 1];
wstringstream cgd3d9path[MAX_PATH + 1];
wstringstream cgglunblock[MAX_PATH + 1];
wstringstream cgunblock[MAX_PATH + 1];
wstringstream cgd3d9unblock[MAX_PATH + 1];
wstringstream airpath_f[MAX_PATH + 1];
wstringstream slnpath_f[MAX_PATH + 1];
wstringstream airinst[MAX_PATH + 1];
wstringstream cginst[MAX_PATH + 1];
vector<wchar_t> cwd(MAX_PATH + 1, 0);
vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);

// Constants
wstring unblock(L":Zone.Identifier");
wstring air(L"Adobe AIR.dll");
wstring flash(L"Resources\\NPSWF32.dll");
wstring cgfile(L"Cg.dll");
wstring cgglfile(L"CgGL.dll");
wstring cgd3d9file(L"CgD3D9.dll");
wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
wstring tbbfile(L"tbb.dll");
wstring airwin(L"air15_win.exe");
wstring unblockfiles[] = { cgunblock[0].str(), cgglunblock[0].str(), cgd3d9unblock[0].str(), tbbunblock[0].str(), airunblock[0].str(), flashunblock[0].str() };
