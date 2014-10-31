// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 9";
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
#elif defined(ENVIRONMENT32)
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
else
		{
		 // AVX
	if (isAvxSupported() == 1)
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