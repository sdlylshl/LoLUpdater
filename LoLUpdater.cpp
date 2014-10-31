// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 8";
	std::cout << std::endl;
	std::cout << "Patching..." << std::endl;


	GetCurrentDirectoryW(MAX_PATH+1, &cwd[0]);
GetEnvironmentVariableW(L"CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH);

	// Path builder
	buff_c[0] << cwd[0];

#if defined(ENVIRONMENT64)
	buff_c[0] << progx86;
#elif defined (ENVIRONMENT32)
	buff_c[0] << prog;
#endif
	buff_c[0] << adobepath;


	// Check for Garena
	if (file_exists(L"lol.launcher.exe"))
	{
		std::wstring slnpath(L"\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
		std::wstring airpath(L"\\RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

		slnpath_f[0] << &cwd;
		slnpath_f[0] << slnpath;

		airpath_f[0] << &cwd;
		airpath_f[0] << airpath;

	}
	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"\\Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"\\Game\\");

		slnpath_f[0] << &cwd;
		slnpath_f[0] << slnpath;

		airpath_f[0] << &cwd;
		airpath_f[0] << airpath;
	}


	airinstunblock[0] << &cwd;
	airinstunblock[0] << L"\\";
	airinstunblock[0] << airwin;
	airinstunblock[0] << unblock;

	// End Path builder

	cginstunblock[0] << &cwd;
		cginstunblock[0] << L"\\";
		cginstunblock[0] << cginstaller;
		cginstunblock[0] << unblock;
		if (&cgbinpath[0] == L"")
		{
			URLDownloadToFileW(
				nullptr,
				L"https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
				&cginstaller[0],
				0,
				nullptr
				);

			DeleteFileW(&cginstaller[0]);
			SHELLEXECUTEINFOW ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = nullptr;
			ShExecInfo.lpVerb = nullptr;
			ShExecInfo.lpFile = &cginstaller[0];
			ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
			ShExecInfo.lpDirectory = nullptr;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = nullptr;
			ShellExecuteExW(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		}
		

	
	// Gets location of latest cg dll
		cgbin[0] << &cgbinpath;
	cgbin[0] << cgfile;

	// Gets location of latest cggl dll
	cgglbin[0] << &cgbinpath;
	cgglbin[0] << cgglfile;

	// Gets location of latest cgd3d9 dll
	cgd3d9bin[0] << &cgbinpath;
	cgd3d9bin[0] << cgd3d9file;


	airfile[0] << buff_c[0].str().c_str();
	airfile[0] << air;

	flashfile[0] << buff_c[0].str().c_str();
	flashfile[0] << flash;


	// Working Directory
	


		URLDownloadToFileW(
			nullptr,
			L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			&airwin[0],
			0,
			nullptr
		);

		DeleteFileW(airinstunblock[0].str().c_str());
	SHELLEXECUTEINFOW ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = &airwin[0];
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);


	cgpath[0] << slnpath_f[0].str();
	cgpath[0] << cgfile;

	cgglpath[0] << slnpath_f[0].str();
	cgglpath[0] << cgglfile;




	cgd3d9path[0] << slnpath_f[0].str();
	cgd3d9path[0] << cgd3d9file;

	tbb0[0] << slnpath_f[0].str();
	tbb0[0] << tbbfile;

	airdir[0] << airpath_f[0].str();
	airdir[0] << air;

	flashdir[0] << airpath_f[0].str();
	flashdir[0] << flash;

	cgunblock[0] << cgpath[0].str();
	cgunblock[0] << unblock;

	cgglunblock[0] << cgglpath[0].str();
	cgglunblock[0] << unblock;

	cgd3d9unblock[0] << cgd3d9path[0].str();
	cgd3d9unblock[0] << unblock;



	airunblock[0] << airdir[0].str();
	airunblock[0] << unblock;

	flashunblock[0] << flashdir[0].str();
	flashunblock[0] << unblock;

	tbbunblock[0] << cwd[0];
	tbbunblock[0] << tbb0[0].str();
	tbbunblock[0] << unblock;





#if _XP
	// XP
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Xp.dll",
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
			L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Avx2.dll",
			tbb0[0].str().c_str(),
			0,
			nullptr
		);
	}
	else
	{
		// AVX
		if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != 0)
		{
			URLDownloadToFileW(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Avx.dll",
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
					L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Sse2.dll",
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
						L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Sse.dll",
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
						L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb/Default.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
					);
				}
			}
		}
	}

#endif
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
	for each (std::wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}
	std::wcout << "LoLUpdater finished!";
	std::wcout << std::endl;
	std::wcout << cgbin[0].str().c_str();
	std::wcout << cgpath[0].str().c_str();
	std::wcout << airfile[0].str().c_str();
	std::wcout << flashdir[0].str().c_str();
	system("pause");

	return 0;
}