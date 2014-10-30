// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 8" << std::endl << "Patching..." << std::endl;
	// Path builder

	buff_c[0] << drive;

#if defined(ENVIRONMENT64)
	buff_c[0] << progx86;
#elif defined (ENVIRONMENT32)
	buff_c[0] << prog;
#endif
	buff_c[0] << adobepath;
	// Check for Garena

	slnpath_f[0] << cwd;
	slnpath_f[0] << slnpath;

	airpath_f[0] << cwd;
	airpath_f[0] << airpath;

	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"\\Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"\\Game");

		slnpath_f[0] << cwd;
		slnpath_f[0] << slnpath;

		airpath_f[0] << cwd;
		airpath_f[0] << airpath;

	}

	cginst[0] << cwd;
	cginst[0] << cginstaller;

	airinst[0] << cwd;
	airinst[0] << airwin;

	airinstunblock[0] << airwin;
	airinst[0] << unblock;

	// End Path builder

		cginst[0] << cwd;
		cginst[0] << cginstaller;

		cginstunblock[0] << cginstaller;
		cginstunblock[0] << unblock;
		if (cgbinpath == 0)
		{
			URLDownloadToFileW(
				nullptr,
				L"https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
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
		}
		

	
	// Gets location of latest cg dll
	cgbin[0] << cgbinpath;
	cgbin[0] << cgfile;

	// Gets location of latest cggl dll
	cgglbin[0] << cgbinpath;
	cgglbin[0] << cgglfile;

	// Gets location of latest cgd3d9 dll
	cgd3d9bin[0] << cgbinpath;
	cgd3d9bin[0] << cgd3d9file;


	airfile[0] << buff_c;
	airfile[0] << air;

	flashfile[0] << buff_c;
	flashfile[0] << flash;


	// Working Directory
	


		URLDownloadToFileW(
			nullptr,
			L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			airinst[0].str().c_str(),
			0,
			nullptr
		);

		DeleteFileW(airinstunblock[0].str().c_str());
	SHELLEXECUTEINFOW ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airinst[0].str().c_str();
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

	tbb0[0] << slnpath[0];
	tbb0[0] << tbbfile;

	airdir[0] << airpath_f[0].str();
	airdir[0] << air;

	flashdir[0] << airpath_f[0].str();
	flashdir[0] << flash;

	cgunblock[0] << cgpath[0].str();
	cgunblock[0] << unblock;


	cgd3d9unblock[0] << cgd3d9path[0].str();
	cgd3d9unblock[0] << unblock;

	cgglunblock[0] << cgglpath[0].str();
	cgglunblock[0] << unblock;

	airunblock[0] << airdir[0].str();
	airunblock[0] << unblock;

	flashunblock[0] << airdir[0].str();
	flashunblock[0] << unblock;

	tbbunblock[0] << slnpath_f[0].str();
	tbbunblock[0] << tbbfile[0];
	tbbunblock[0] << unblock;





#if _XP
	// XP
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Xp.dll",
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
			L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Avx2.dll",
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
				L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Avx.dll",
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
					L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Sse2.dll",
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
						L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Sse.dll",
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
						L"https://github.com/Loggan08/LoLUpdater/raw/master/tbb0/Default.dll",
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
	std::wcout << buff_c[0].str().c_str();
	std::wcout << cwd.c_str();
	std::cout << "LoLUpdater finished!" << std::endl << "";
	system("pause");

	return 0;
}