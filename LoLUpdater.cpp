// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"


#if XP == FALSE
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <winerror.h>

// Windows 7 SP1 is the first version of Windows to support the AVX API.

// The value for CONTEXT_XSTATE has changed between Windows 7 and
// Windows 7 SP1 and greater.
// While the value will be correct for future SDK headers, we need to set 
// this value manually when building with a Windows 7 SDK for running on 
// Windows 7 SPI OS bits.

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
PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = nullptr;

typedef BOOL(WINAPI *PINITIALIZECONTEXT)(PVOID Buffer, DWORD ContextFlags, PCONTEXT* Context, PDWORD ContextLength);
PINITIALIZECONTEXT pfnInitializeContext = nullptr;

typedef BOOL(WINAPI *PGETXSTATEFEATURESMASK)(PCONTEXT Context, PDWORD64 FeatureMask);
PGETXSTATEFEATURESMASK pfnGetXStateFeaturesMask = nullptr;

typedef PVOID(WINAPI *LOCATEXSTATEFEATURE)(PCONTEXT Context, DWORD FeatureId, PDWORD Length);
LOCATEXSTATEFEATURE pfnLocateXStateFeature = nullptr;

typedef BOOL(WINAPI *SETXSTATEFEATURESMASK)(PCONTEXT Context, DWORD64 FeatureMask);
SETXSTATEFEATURESMASK pfnSetXStateFeaturesMask = nullptr;

VOID
PrintThreadAvxState(
__in HANDLE hThread
)
{
	PVOID Buffer;
	PCONTEXT Context;
	DWORD ContextSize;
	DWORD64 FeatureMask;
	DWORD FeatureLength;
	ULONG Index;
	BOOL Success;
	PM128A Xmm;
	PM128A Ymm;

	// If this function was called before and we were not running on 
	// at least Windws 7 SP1, then bail.
	if (pfnGetEnabledXStateFeatures == (PGETENABLEDXSTATEFEATURES)-1)
	{
		_tprintf(_T("This needs to run on Windows 7 SP1 or greater.\n"));
		return;
	}

	// Get the addresses of the AVX XState functions.
	if (pfnGetEnabledXStateFeatures == NULL)
	{
		HMODULE hm = GetModuleHandle(_T("kernel32.dll"));
		if (hm == NULL)
		{
			pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)-1;
			_tprintf(_T("GetModuleHandle failed (error == %d).\n"), GetLastError());
			return;
		}

		pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(hm, "GetEnabledXStateFeatures");
		pfnInitializeContext = (PINITIALIZECONTEXT)GetProcAddress(hm, "InitializeContext");
		pfnGetXStateFeaturesMask = (PGETXSTATEFEATURESMASK)GetProcAddress(hm, "GetXStateFeaturesMask");
		pfnLocateXStateFeature = (LOCATEXSTATEFEATURE)GetProcAddress(hm, "LocateXStateFeature");
		pfnSetXStateFeaturesMask = (SETXSTATEFEATURESMASK)GetProcAddress(hm, "SetXStateFeaturesMask");

		if (pfnGetEnabledXStateFeatures == NULL
			|| pfnInitializeContext == NULL
			|| pfnGetXStateFeaturesMask == NULL
			|| pfnLocateXStateFeature == NULL
			|| pfnSetXStateFeaturesMask == NULL)
		{
			pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)-1;
			_tprintf(_T("This needs to run on Windows 7 SP1 or greater.\n"));
			return;
		}
	}

	FeatureMask = pfnGetEnabledXStateFeatures();
	if ((FeatureMask & XSTATE_MASK_AVX) == 0)
	{
		_tprintf(_T("The AVX feature is not enabled.\n"));
		return;
	}

	ContextSize = 0;
	Success = pfnInitializeContext(nullptr,
		CONTEXT_ALL | CONTEXT_XSTATE,
		nullptr,
		&ContextSize);

	if ((Success == TRUE) || (GetLastError() != ERROR_INSUFFICIENT_BUFFER))
	{
		_tprintf(_T("Unexpected result from InitializeContext (success == %d, error == %d).\n"),
			Success,
			GetLastError());
	}

	Buffer = malloc(ContextSize);
	if (Buffer == NULL)
	{
		_tprintf(_T("Out of memory.\n"));
		return;
	}

	Success = pfnInitializeContext(Buffer,
		CONTEXT_ALL | CONTEXT_XSTATE,
		&Context,
		&ContextSize);

	if (Success == FALSE)
	{
		_tprintf(_T("InitializeContext failed (error == %d).\n"), GetLastError());
		goto Cleanup;
	}

	Success = pfnSetXStateFeaturesMask(Context, XSTATE_MASK_AVX);
	if (Success == FALSE)
	{
		_tprintf(_T("SetXStateFeaturesMask failed (error == %d).\n"), GetLastError());
		goto Cleanup;
	}

	Success = GetThreadContext(hThread, Context);
	// Note: Thread state may not be accurate. For best results, suspend 
	// the thread and use the CONTEXT_EXCEPTION_REQUEST flags prior to 
	// calling GetThreadContext.
	if (Success == FALSE)
	{
		_tprintf(_T("GetThreadContext failed (error == %d).\n"), GetLastError());
		goto Cleanup;
	}

	Success = pfnGetXStateFeaturesMask(Context, &FeatureMask);
	if (Success == FALSE)
	{
		_tprintf(_T("GetXStateFeatureMask failed (error == %d).\n"), GetLastError());
		goto Cleanup;
	}

	//
	// The AVX feature consists of 8 (x86) or 16 (x64) 256-bit Ymm registers.
	// The lower 128 bits share storage with the corresponding Xmm (SSE)
	// registers and the high 128 bits are denoted by Ymm_H0 - Ymm_Hn.
	//

	if ((FeatureMask & XSTATE_MASK_AVX) == 0)
	{
		_tprintf(_T("AVX is in the INIT state (YMM_H registers are all zero).\n"));
		goto Cleanup;
	}

	Xmm = (PM128A)pfnLocateXStateFeature(Context, XSTATE_LEGACY_SSE, &FeatureLength);
	Ymm = (PM128A)pfnLocateXStateFeature(Context, XSTATE_AVX, nullptr);

	//
	// Print values in the YMM registers.
	//

	for (Index = 0; Index < FeatureLength / sizeof(Xmm[0]); Index += 1)
	{
		_tprintf(_T("Ymm%d: %I64d %I64d %I64d %I64d\n"),
			Index,
			Xmm[Index].Low,
			Xmm[Index].High,
			Ymm[Index].Low,
			Ymm[Index].High);
	}

Cleanup:
	free(Buffer);
	return;
}
#endif




int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 8";
	std::cout << std::endl;
	std::cout << "Patching..." << std::endl;

	GetCurrentDirectoryW(MAX_PATH + 1, &cwd[0]);
	wcsncat(&cwd[0], L"\\", MAX_PATH + 1);
	GetEnvironmentVariableW(L"CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH + 1);
	wcsncat(&cgbinpath[0], L"\\", MAX_PATH + 1);

	// Pathbuilder
	buff_c[0] << cwd[0];
#if defined(ENVIRONMENT64)
	buff_c[0] << L":\\Program Files (x86)";
#elif defined (ENVIRONMENT32)
	buff_c[0] << L":\\Program Files";
#endif
	buff_c[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	std::wstring slnpath(L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
	std::wstring airpath(L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"Game\\");
	}

	slnpath_f[0] << &cwd[0];
	slnpath_f[0] << &slnpath[0];

	airpath_f[0] << &cwd[0];
	airpath_f[0] << &airpath[0];

	tbb0[0] << slnpath_f[0].str();
	tbb0[0] << tbbfile;

	airinst[0] << &cwd[0];
	airinst[0] << airwin;

	cginst[0] << &cwd[0];
	cginst[0] << cginstaller;

	airinstunblock[0] << &airinst[0];
	airinstunblock[0] << &unblock[0];

	cginstunblock[0] << &cginst[0];
	cginstunblock[0] << &unblock[0];

	// End Pathbuilder

	URLDownloadToFileW(
		nullptr,
		L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
		cginst[0].str().c_str(),
		0,
		nullptr
		);


	DeleteFileW(cginstunblock[0].str().c_str());
	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = cginst[0].str().c_str();
	ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Gets location of latest cg dll
	cgbin[0] << &cgbinpath[0];
	cgbin[0] << cgfile;

	// Gets location of latest cggl dll
	cgglbin[0] << &cgbinpath[0];
	cgglbin[0] << cgglfile;

	// Gets location of latest cgd3d9 dll
	cgd3d9bin[0] << &cgbinpath[0];
	cgd3d9bin[0] << cgd3d9file;

	// Gets location of latest adobe air dll
	airfile[0] << buff_c[0].str().c_str();
	airfile[0] << air;

	// Gets location of latest "flash" dll
	flashfile[0] << buff_c[0].str().c_str();
	flashfile[0] << flash;

	URLDownloadToFileW(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airinst[0].str().c_str(),
		0,
		nullptr
		);

	DeleteFileW(airinstunblock[0].str().c_str());
	SHELLEXECUTEINFOW ShExecInfo1 = { 0 };
	ShExecInfo1.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo1.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo1.hwnd = nullptr;
	ShExecInfo1.lpVerb = nullptr;
	ShExecInfo1.lpFile = airinst[0].str().c_str();
	ShExecInfo1.lpParameters = L"-silent";
	ShExecInfo1.lpDirectory = nullptr;
	ShExecInfo1.nShow = SW_SHOW;
	ShExecInfo1.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo1);
	WaitForSingleObject(ShExecInfo1.hProcess, INFINITE);

	// Working Directory
	cgpath[0] << slnpath_f[0].str();
	cgpath[0] << cgfile;

	cgglpath[0] << slnpath_f[0].str();
	cgglpath[0] << cgglfile;

	cgd3d9path[0] << slnpath_f[0].str();
	cgd3d9path[0] << cgd3d9file;

	airdir[0] << airpath_f[0].str();
	airdir[0] << air;

	flashdir[0] << airpath_f[0].str();
	flashdir[0] << flash;

	cgunblock[0] << cgpath[0].str();
	cgunblock[0] << &unblock[0];

	cgglunblock[0] << cgglpath[0].str();
	cgglunblock[0] << &unblock[0];

	cgd3d9unblock[0] << cgd3d9path[0].str();
	cgd3d9unblock[0] << &unblock[0];

	airunblock[0] << airdir[0].str();
	airunblock[0] << &unblock[0];

	flashunblock[0] << flashdir[0].str();
	flashunblock[0] << &unblock[0];

	tbbunblock[0] << tbb0[0].str();
	tbbunblock[0] << &unblock[0];
	// Working Directory End

#if XP
	// XP
	URLDownloadToFileW(
		nullptr,
		L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Xp.dll",
		tbb0[0].str().c_str(),
		0,
		nullptr
		);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx2.dll",
			tbb0[0].str().c_str(),
			0,
			nullptr
			);
	}
	// temp-disabled
	// else
		// {
		// AVX
		// if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != 0)
		// {
		// URLDownloadToFileW(
		// 	nullptr,
		// 	L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
		// 	tbb0[0].str().c_str(),
		// 	0,
		// 	nullptr
		// );
		// }
		else
		{
			//SSE2
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				URLDownloadToFileW(
					nullptr,
					L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse2.dll",
					tbb0[0].str().c_str(),
					0,
					nullptr
					);
			}
			else
			{
				//SSE
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					URLDownloadToFileW(
						nullptr,
						L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
				//Default
				else
				{
					URLDownloadToFileW(
						nullptr,
						L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Default.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
			}
		}
	// Temp disabled}

#endif

	// Copy all files
	CopyFileW(
		cgbin[0].str().c_str(),
		cgpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgglbin[0].str().c_str(),
		cgglpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgd3d9bin[0].str().c_str(),
		cgd3d9path[0].str().c_str(),
		false
		);
	CopyFileW(
		airfile[0].str().c_str(),
		airdir[0].str().c_str(),
		false
		);
	CopyFileW(
		flashfile[0].str().c_str(),
		flashdir[0].str().c_str(),
		false
		);
	// End copy all files

	// Unblock _all_ files just to be safe
	for each (std::wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}
	// End unblock

	std::wcout << "LoLUpdater finished!";
	std::wcout << std::endl;
	system("pause");

	return 0;
}