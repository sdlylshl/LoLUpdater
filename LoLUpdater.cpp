// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 7" << std::endl << "Patching..." << std::endl;

	// Path builder


	// Too lazy to calculate string-length myself
#if defined(ENVIRONMENT64)
	memcpy(&buff_c, drive.c_str(), wcslen(drive.c_str()+1));
	memcpy(&buff_c, progx86.c_str(), wcslen(prog.c_str()+1));
	memcpy(&buff_c, adobepath.c_str(), wcslen(adobepath.c_str()+1));

#elif defined (ENVIRONMENT32)
	memcpy(&buff_c, drive.c_str(), wcslen(drive.c_str()+1));
	memcpy(&buff_c, prog.c_str(), wcslen(prog.c_str() + 1));
	memcpy(&buff_c, adobepath.c_str(), wcslen(adobepath.c_str() + 1));
#endif
	memcpy(&slnpath_f, cwd.c_str(), wcslen(cwd.c_str() + 1));
	memcpy(&slnpath_f, slnpath.c_str(), wcslen(cwd.c_str() + 1));

	memcpy(&airpath_f, cwd.c_str(), wcslen(cwd.c_str() + 1));
	memcpy(&airpath_f, airpath.c_str(), wcslen(cwd.c_str() + 1));

	memcpy(&cginst, cwd.c_str(), wcslen(cwd.c_str() + 1));
	memcpy(&cginst, cginstaller.c_str(), wcslen(cwd.c_str() + 1));

	memcpy(&airinst, cwd.c_str(), wcslen(cwd.c_str() + 1));
	memcpy(&airinst, airwin.c_str(), wcslen(cwd.c_str() + 1));

	// End Path builder

	if (cgbinpath.empty())
	{

		memcpy(&cginstunblock, cginst[0].c_str(), wcslen(cginst[0].c_str() + 1));
		memcpy(&cginstunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

		URLDownloadToFileW(
			nullptr,
			L"https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginst[0].c_str(),
			0,
			nullptr
			);

		DeleteFileW(cginstunblock[0].c_str());
		SHELLEXECUTEINFOW ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = cginst[0].c_str();
		ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	// Common Files

	// Gets location of latest adobeair dll
	memcpy(&airfile, buff_c[0].c_str(), wcslen(buff_c[0].c_str() + 1));
	memcpy(&airfile, air.c_str(), wcslen(air.c_str() + 1));

	// Gets location of latest flash dll
	memcpy(&flashfile, buff_c[0].c_str(), wcslen(buff_c[0].c_str() + 1));
	memcpy(&flashfile, flash.c_str(), wcslen(flash.c_str() + 1));


	// CG_BIN_PATH

	// Gets location of latest cg dll
	memcpy(&cgbin, cgbinpath.c_str(), wcslen(cgbinpath.c_str() + 1));
	memcpy(&cgbin, cgfile.c_str(), wcslen(cgfile.c_str() + 1));

	// Gets location of latest cgd3d9 dll
	memcpy(&cgd3d9bin, cgbinpath.c_str(), wcslen(cgbinpath.c_str() + 1));
	memcpy(&cgd3d9bin, cgd3d9file.c_str(), wcslen(cgd3d9file.c_str() + 1));

	// Gets location of latest cggl dll
	memcpy(&cgglbin, cgbinpath.c_str(), wcslen(cgbinpath.c_str() + 1));
	memcpy(&cgglbin, cgglfile.c_str(), wcslen(cgglfile.c_str() + 1));



	// Working Directory
	memcpy(&cgpath, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
	memcpy(&cgpath, cgfile.c_str(), wcslen(cgfile.c_str() + 1));

	memcpy(&cgglpath, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
	memcpy(&cgglpath, cgglfile.c_str(), wcslen(cgglfile.c_str() + 1));

	memcpy(&cgd3d9path, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
	memcpy(&cgd3d9path, cgd3d9file.c_str(), wcslen(cgd3d9file.c_str() + 1));

	memcpy(&tbb, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
	memcpy(&tbb, tbbfile.c_str(), wcslen(tbbfile.c_str() + 1));

	memcpy(&airdir, airpath_f[0].c_str(), wcslen(airpath_f[0].c_str() + 1));
	memcpy(&airdir, air.c_str(), wcslen(air.c_str() + 1));

	memcpy(&flashdir, airpath_f[0].c_str(), wcslen(airpath_f[0].c_str() + 1));
	memcpy(&flashdir, flash.c_str(), wcslen(flash.c_str() + 1));


		URLDownloadToFileW(
			nullptr,
			L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			airinst[0].c_str(),
			0,
			nullptr
		);

		DeleteFileW(airinstunblock[0].c_str());
	SHELLEXECUTEINFOW ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airinst[0].c_str();
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Check for Garena
	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"\\Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"\\Game");
		// Separate string-combining logic for Garena

		// Working Directory
		memcpy(&cgpath, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
		memcpy(&cgpath, cgfile.c_str(), wcslen(cgfile.c_str() + 1));

		memcpy(&cgglpath, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
		memcpy(&cgglpath, cgglfile.c_str(), wcslen(cgglfile.c_str() + 1));

		memcpy(&cgd3d9path, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
		memcpy(&cgd3d9path, cgd3d9file.c_str(), wcslen(cgd3d9file.c_str() + 1));

		memcpy(&tbb, slnpath_f[0].c_str(), wcslen(slnpath_f[0].c_str() + 1));
		memcpy(&tbb, tbbfile.c_str(), wcslen(tbbfile.c_str() + 1));

		memcpy(&airdir, airpath_f[0].c_str(), wcslen(airpath_f[0].c_str() + 1));
		memcpy(&airdir, air.c_str(), wcslen(air.c_str() + 1));

		memcpy(&flashdir, airpath_f[0].c_str(), wcslen(airpath_f[0].c_str() + 1));
		memcpy(&flashdir, flash.c_str(), wcslen(flash.c_str() + 1));
	}

	// CG_BIN_PATH UNBLOCK
	memcpy(&cgglunblock, cgglpath[0].c_str(), wcslen(cgglpath[0].c_str() + 1));
	memcpy(&cgglunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	memcpy(&cgunblock, cgpath[0].c_str(), wcslen(cgpath[0].c_str() + 1));
	memcpy(&cgunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	memcpy(&cgd3d9unblock, cgd3d9path[0].c_str(), wcslen(cgd3d9path[0].c_str() + 1));
	memcpy(&cgd3d9unblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	// Common Files UNBLOCK
	memcpy(&airunblock, airdir[0].c_str(), wcslen(airdir[0].c_str() + 1));
	memcpy(&airunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	memcpy(&flashunblock, flashdir[0].c_str(), wcslen(flashdir[0].c_str() + 1));
	memcpy(&flashunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	// Misc UNBLOCK
	memcpy(&tbbunblock, tbb[0].c_str(), wcslen(tbb[0].c_str() + 1));
	memcpy(&tbbunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));

	memcpy(&airinstunblock, airinst[0].c_str(), wcslen(airinst[0].c_str() + 1));
	memcpy(&airinstunblock, unblock.c_str(), wcslen(unblock.c_str() + 1));
#if _XP
	// XP
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Xp.dll",
			tbb[0].c_str(),
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
			tbb[0].c_str(),
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
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
				tbb[0].c_str(),
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
					tbb[0].c_str(),
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
						tbb[0].c_str(),
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
						tbb[0].c_str(),
						0,
						nullptr
					);
				}
			}
		}
	}

#endif
	CopyFileW(
		cgbin[0].c_str(),
		cgpath[0].c_str(),
		false
	);
	CopyFileW(
		cgglbin[0].c_str(),
		cgglpath[0].c_str(),
		false
	);
	CopyFileW(
		cgd3d9bin[0].c_str(),
		cgd3d9path[0].c_str(),
		false
	);
	CopyFileW(
		airfile[0].c_str(),
		airdir[0].c_str(),
		false
	);
	CopyFileW(
		flashfile[0].c_str(),
		flashdir[0].c_str(),
		false
	);
	for each (std::wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}

	std::cout << "LoLUpdater finished!" << std::endl << "";
	system("pause");

	return 0;
}