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

// Windows 7 SP1 is the first version of Windows to support the AVX API.

// The value for CONTEXT_XSTATE has changed between Windows 7 and
// Windows 7 SP1 and greater.
// While the value will be correct for future SDK headers, we need to set 
// this value manually when building with a Windows 7 SDK for running on 
// Windows 7 SPI OS bits.


int _tmain(int argc, _TCHAR* argv[])
{
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

	DWORD bufferSize = 65535;
	std::wstring buff;
	buff.resize(bufferSize);


	DWORD cgbinpath = GetEnvironmentVariableW(L"CG_BIN_PATH", &buff[0], bufferSize);

	if (cgbinpath == NULL)
	{
		URLDownloadToFileA(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			L"Cg-3.1_April2012_Setup.exe",
			0,
			nullptr
			);
		DeleteFile(L"Cg - 3.1_April2012_Setup.exe:Zone.Identifier");

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = L"Cg - 3.1_April2012_Setup.exe";
		ShExecInfo.lpParameters = L"/verysilent";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		std::string input = std::to_string(cgbinpath);
		std::wstring output;
		std::wstring output1;
		std::wstring output2;
		output = std::wstring(input.begin(), input.end());
		output = output + std::wstring(L"\cg.dll");
		output1 = output + std::wstring(L"\cgGL.dll");
		output2 = output + std::wstring(L"\cgD3D9.dll");

		CopyFile(
			output.c_str(),
			L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/Cg.dll",
			false
			);

		CopyFile(
			output1.c_str(),
			L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/CgGL.dll",
			false
			);
		CopyFile(
			output2.c_str(),
			L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/CgD3D9.dll",
			false
			);
		DeleteFile(L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/CgD3D9.dll:Zone.Identifier");
		DeleteFile(L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/Cg.dll:Zone.Identifier");
		DeleteFile(L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/CgGL.dll:Zone.Identifier");
	}
	URLDownloadToFileA(
		nullptr,
		"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		"air15_win.exe",
		0,
		nullptr
		);
	DeleteFile(L"air15_win.exe:Zone.Identifier");
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


	wchar_t* szPath = nullptr;

	SHGetFolderPath(nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		0,
		szPath);

	std::wstring input = szPath;
	std::wstring output3;
	std::wstring output4;
	output3 = std::wstring(input.begin(), input.end());
	output3 = output3 + std::wstring(L"\Common Files\Adobe AIR\Versions\1.0\Adobe AIR.dll");
	output4 = output3 + std::wstring(L"\Common Files\Adobe AIR\Versions\1.0\Resources\NPSWF32.dll");


	CopyFile(

		output3.c_str(),
		L"RADS\projects\lol_air_client\releases\0.0.1.114\deploy\Adobe AIR\versions\1.0\Adobe AIR.dll",
		false
		);
	CopyFile(
		output4.c_str(),
		L"RADS\projects\lol_air_client\releases\0.0.1.114\deploy\Adobe AIR\versions\1.0\Resources\NPSWF32.dll",
		false
		);


	if (can_use_intel_core_4th_gen_features())
	{
		URLDownloadToFileA(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/AVX2.dll",
			L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll",
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

	// Since the AVX API is not declared in the Windows 7 SDK headers and 
	// since we don't have the proper libs to work with, we will declare 
	// the API as function pointers and get them with GetProcAddress calls 
	// from kernel32.dll.  We also need to set some #defines.

#define XSTATE_AVX                          (XSTATE_GSSE)
#define XSTATE_MASK_AVX                     (XSTATE_MASK_GSSE)

	typedef DWORD64(WINAPI *PGETENABLEDXSTATEFEATURES)();
	PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = NULL;

	typedef BOOL(WINAPI *PINITIALIZECONTEXT)(PVOID Buffer, DWORD ContextFlags, PCONTEXT* Context, PDWORD ContextLength);
	PINITIALIZECONTEXT pfnInitializeContext = NULL;

	typedef BOOL(WINAPI *PGETXSTATEFEATURESMASK)(PCONTEXT Context, PDWORD64 FeatureMask);
	PGETXSTATEFEATURESMASK pfnGetXStateFeaturesMask = NULL;

	typedef PVOID(WINAPI *LOCATEXSTATEFEATURE)(PCONTEXT Context, DWORD FeatureId, PDWORD Length);
	LOCATEXSTATEFEATURE pfnLocateXStateFeature = NULL;

	typedef BOOL(WINAPI *SETXSTATEFEATURESMASK)(PCONTEXT Context, DWORD64 FeatureMask);
	SETXSTATEFEATURESMASK pfnSetXStateFeaturesMask = NULL;


	PVOID Buffer;
	PCONTEXT Context;
	DWORD ContextSize;
	DWORD64 FeatureMask;
	DWORD FeatureLength;
	ULONG Index;
	BOOL Success;
	PM128A Xmm;
	PM128A Ymm;



	// Get the addresses of the AVX XState functions.
	if (pfnGetEnabledXStateFeatures == NULL)
	{
		HMODULE hm = GetModuleHandle(_T("kernel32.dll"));

		pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(hm, "GetEnabledXStateFeatures");
		pfnInitializeContext = (PINITIALIZECONTEXT)GetProcAddress(hm, "InitializeContext");
		pfnGetXStateFeaturesMask = (PGETXSTATEFEATURESMASK)GetProcAddress(hm, "GetXStateFeaturesMask");
		pfnLocateXStateFeature = (LOCATEXSTATEFEATURE)GetProcAddress(hm, "LocateXStateFeature");
		pfnSetXStateFeaturesMask = (SETXSTATEFEATURESMASK)GetProcAddress(hm, "SetXStateFeaturesMask");


		if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != 0)
		{
			URLDownloadToFileA(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/AVX.dll",
				L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll",
				0,
				nullptr
				);
		}
		if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		{
			URLDownloadToFileA(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/SSE2.dll",
				L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll",
				0,
				nullptr
				);
		}
		if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
		{
			URLDownloadToFileA(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/SSE.dll",
				L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll",
				0,
				nullptr
				);
		}
		else
		{
			URLDownloadToFileA(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Default.dll",
				L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll",
				0,
				nullptr
				);
		}
		DeleteFile(L"RADS/solutions/lol_game_client_sln/releases/0.0.1.62/deploy/tbb.dll:Zone.Identifier");

		return 0;
	}
}