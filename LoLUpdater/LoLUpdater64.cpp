// LoLUpdater.cpp : Defines the entry point for the console application.
//
#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)

#include <immintrin.h>

int check_4th_gen_intel_core_features()
{
	const int the_4th_gen_features = 
		(_FEATURE_AVX2 | _FEATURE_FMA | _FEATURE_BMI | _FEATURE_LZCNT | _FEATURE_MOVBE);
	return _may_i_use_cpu_feature( the_4th_gen_features );
}

#else /* non-Intel compiler */

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif

void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
#if defined(_MSC_VER)
	__cpuidex(abcd, eax, ecx);
#else
	uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
	/* in case of PIC under 32-bit EBX cannot be clobbered */
	__asm__ ( "movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
	__asm__ ( "cpuid" : "+b" (ebx),
# endif
		"+a" (eax), "+c" (ecx), "=d" (edx) );
	abcd[0] = eax; abcd[1] = ebx; abcd[2] = ecx; abcd[3] = edx;
#endif
}

int check_xcr0_ymm()
{
	uint32_t xcr0;
#if defined(_MSC_VER)
	xcr0 = (uint32_t)_xgetbv(0); /* min VS2010 SP1 compiler is required */
#else
	__asm__("xgetbv" : "=a" (xcr0) : "c" (0) : "%edx");
#endif
	return ((xcr0 & 6) == 6); /* checking if xmm and ymm state are enabled in XCR0 */
}


int check_4th_gen_intel_core_features()
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
#include <tchar.h>
#include <stdio.h>
#include "ShlObj.h"
#include <VersionHelpers.h>
#include "Urlmon.h"
#include "Windows.h"
#include "stdlib.h"
#include <string.h>
#include <string>
#include "Shlwapi.h"

static wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}

int _tmain(int argc, _TCHAR* argv[])
{
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

	char* cgbinpath = getenv("CG_BIN_PATH");

	if (cgbinpath == NULL)
	{
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			"Cg-3.1_April2012_Setup.exe",
			0,
			nullptr
			);
		DeleteFileA("Cg-3.1_April2012_Setup.exe:Zone.Identifier");

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = L"Cg-3.1_April2012_Setup.exe";
		ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	char buffer_1[MAX_PATH] = "";
	char *lpStr1;
	lpStr1 = buffer_1;

	// String for balance of path name.
	char* buffer_3 = "\\Cg.dll";
	char *lpStr3;
	lpStr3 = buffer_3;

	// String for directory name.
	char *buffer_2 = cgbinpath;
	char *lpStr2;
	lpStr2 = buffer_2;

	// String for directory name.
	char* buffer_4 = "\\CgGL.dll";
	char *lpStr4;
	lpStr4 = buffer_4;

	// String for directory name.
	char* buffer_5 = "\\CgD3D9.dll";
	char *lpStr5;
	lpStr5 = buffer_5;
	PathCombineA(lpStr1, lpStr3, lpStr2);
	CopyFileA(
		lpStr1,
		"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\Cg.dll",
		false
		);
	PathCombineA(lpStr1, lpStr4, lpStr2);
	CopyFileA(
		lpStr1,
		"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgGL.dll",
		false
		);
	PathCombineA(lpStr1, lpStr5, lpStr2);
	CopyFileA(
		lpStr1,
		"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgD3D9.dll",
		false
		);
	DeleteFileA("RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\Cg.dll:Zone.Identifier");
	DeleteFileA("RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgGL.dll:Zone.Identifier");
	DeleteFileA("RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgD3D9.dll:Zone.Identifier");



	URLDownloadToFileA(
		nullptr,
		"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		"air15_win.exe",
		0,
		nullptr
		);
	DeleteFileA("air15_win.exe:Zone.Identifier");
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = L"air15_win.exe";
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);


	char* szPath = nullptr;

	SHGetFolderPathA(nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		0,
		szPath);

	char buffer_01[MAX_PATH] = "";
	char *lpStr01;
	lpStr01 = buffer_01;

	// String for balance of path name.
	char* buffer_03 = "\\Common Files\\Adobe AIR\\Versions\\1.0\\Adobe AIR.dll";
	char *lpStr03;
	lpStr03 = buffer_03;

	// String for directory name.
	char *buffer_02 = szPath;
	char *lpStr02;
	lpStr02 = buffer_02;

	// String for balance of path name.
	char* buffer_04 = "\\Common Files\\Adobe AIR\\Versions\\1.0\\Resources\\NPSWF32.dll";
	char *lpStr04;
	lpStr04 = buffer_04;
	PathCombineA(lpStr01, lpStr03, lpStr02);
	CopyFileA(
		lpStr01,
		"RADS\\projects\\lol_air_client\\releases\\0.0.1.114\\deploy\\Adobe AIR\\versions\\1.0\\Adobe AIR.dll",
		false
		);
	PathCombineA(lpStr01, lpStr04, lpStr02);
	CopyFileA(
		lpStr01,
		"RADS\\projects\\lol_air_client\\releases\\0.0.1.114\\deploy\\Adobe AIR\\versions\\1.0\\Resources\\NPSWF32.dll",
		false
		);

	char* tbb = "RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\tbb.dll";
	if (can_use_intel_core_4th_gen_features())
	{
		URLDownloadToFileA(
			nullptr,
			"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/AVX2.dll",
			tbb,
			0,
			nullptr
			);
	}
#undef CONTEXT_XSTATE

#if defined(_M_X64)
#define CONTEXT_XSTATE                      (0x00100040)
#else
#define CONTEXT_XSTATE                      (0x00010040)
#endif


#define XSTATE_MASK_AVX                     (XSTATE_MASK_GSSE)

	typedef DWORD64(WINAPI *PGETENABLEDXSTATEFEATURES)();
		HMODULE hm = GetModuleHandle(_T("kernel32.dll"));

		PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(hm, "GetEnabledXStateFeatures");
		DWORD64 FeatureMask = pfnGetEnabledXStateFeatures();
		// AVX
		if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != NULL)
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/AVX.dll",
				tbb,
				0,
				nullptr
				);
		}
		//SSE2
		if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/SSE2.dll",
				tbb,
				0,
				nullptr
				);
		}
		//SSE
		if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/SSE.dll",
				tbb,
				0,
				nullptr
				);
		}
		//Default
		else
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Default.dll",
				tbb,
				0,
				nullptr
				);
		}
		DeleteFileA("RADS\solutions\lol_game_client_sln\releases\0.0.1.62\deploy\tbb.dll:Zone.Identifier");

		return 1;
}