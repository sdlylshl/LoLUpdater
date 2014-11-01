// LoLUpdater.cpp : Defines the entry point for the console application.
//

// Include all the stuff that didn't make it into the main method
#include "LoLUpdater.h"



int _tmain(int argc, _TCHAR* argv[])
{
	wcout << L"LoLUpdater Alpha 1 Build 12";
	wcout << endl;
	wcout << L"Patching...";
	wcout << endl;

	cwd[0] << cwd1().c_str();
	cwd[0] << L"\\";


		// string-builder for the cginstaller
		cginstunblock[0] << cwd[0].str().c_str();
		cginstunblock[0] << cginstaller;
		cginstunblock[0] << &unblock[0];

		// Downloads Nvidia-CG
		URLDownloadToFileW(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller.c_str(),
			0,
			nullptr
			);
		// Unblocks the installer
		DeleteFileW(cginstunblock[0].str().c_str());

		// Starts the executable with some arguments
		SHELLEXECUTEINFOW ShExecInfocg = { 0 };
		ShExecInfocg.cbSize = sizeof(SHELLEXECUTEINFOW);
		ShExecInfocg.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfocg.hwnd = nullptr;
		ShExecInfocg.lpVerb = nullptr;
		ShExecInfocg.lpFile = cginstaller.c_str();
		// silent and minimal install
		ShExecInfocg.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfocg.lpDirectory = nullptr;
		ShExecInfocg.nShow = SW_SHOW;
		ShExecInfocg.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfocg);
		// Wait for process to finish before continuing.
		WaitForSingleObject(ShExecInfocg.hProcess, INFINITE);

		// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
		GetEnvironmentVariableW(L"CG_BIN_PATH",
			&cgbinpath[0],
			MAX_PATH + 1);

	// appends a backslash to the path for later processing.
	wcsncat(&cgbinpath[0], L"\\", MAX_PATH + 1);

	// add drive letter to the variable
	buff_c[0] << cwd[0].str().c_str()[0];

	// basic stringbuilding
#if defined(ENVIRONMENT64)
	buff_c[0] << L":\\Program Files (x86)";
#elif defined(ENVIRONMENT32)
	buff_c[0] << L":\\Program Files";
#endif
	buff_c[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	// paths to where files should be copied
	wstring slnpath(L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
	wstring airpath(L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

	// garena installations have lol.exe in the main directory
	if (file_exists(L"lol.exe"))
	{
		// Overload if it is Garena
		wstring airpath(L"Air\\Adobe AIR\\Versions\\1.0\\");
		wstring slnpath(L"Game\\");
	}

	// finalize variables for use in the copy functions
	slnpath_f[0] << cwd[0].str().c_str();
	slnpath_f[0] << &slnpath[0];

	airpath_f[0] << cwd[0].str().c_str();
	airpath_f[0] << &airpath[0];

	tbb0[0] << slnpath_f[0].str();
	tbb0[0] << tbbfile;

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

	// string-builder for adobe air installer
	airinstunblock[0] << cwd[0].str().c_str();
	airinstunblock[0] << airwin;
	airinstunblock[0] << &unblock[0];

	// Downloads adobe-air
	URLDownloadToFileW(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airwin.c_str(),
		0,
		nullptr
		);
	// Unblocks the adobe air installer
	DeleteFileW(airinstunblock[0].str().c_str());

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airwin.c_str();
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	// Waits for program to finish
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Finalize full copy paths (long list)
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

#if XP
	// XP tbb download
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
		// AVX2  tbb download
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx2.dll",
			tbb0[0].str().c_str(),
			0,
			nullptr
			);
	}
	else
	{
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
		int cpuInfo[4];
		__cpuid(cpuInfo, 1);

		bool osUsesXSAVE_XRSTORE = cpuInfo[2] & (1 << 27) || false;
		bool cpuAVXSuport = cpuInfo[2] & (1 << 28) || false;
#endif


		if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
		{
			// Check if the OS will save the YMM registers (basically final check for AVX support)
			unsigned long long xcrFeatureMask = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
			avxSupported = (xcrFeatureMask & 0x6) || false;
		}

		// AVX  tbb download
		if (avxSupported)
		{
			URLDownloadToFileW(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
				tbb0[0].str().c_str(),
				0,
				nullptr
				);
		}
		else
		{
			//SSE2  tbb download
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
				//SSE  tbb download
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
				//download tbb without any extra SMID instructions if SSE is not supported.
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
	}

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
	for each (wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}
	// End unblock

	wcout << "LoLUpdater finished!";
	wcout << endl;
	system("pause");

	return 0;
}