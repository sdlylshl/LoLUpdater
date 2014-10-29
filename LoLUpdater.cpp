// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 7" << std::endl << "Patching..." << std::endl;

	// Path builder


	// Too lazy to calculate string-length myself
#if defined(ENVIRONMENT64)
	copy(drive.begin(), drive.end(), &buff_c);
	copy(progx86.begin(), progx86.end(), &buff_c);
	copy(adobepath.begin(), adobepath.end(), &buff_c);

#elif defined (ENVIRONMENT32)
	copy(drive.begin(), drive.end(), &buff_c);
	copy(prog.begin(), prog.end(), &buff_c);
	copy(slnpath.begin(), slnpath.end(), &buff_c);
#endif
	copy(cwd.begin(), cwd.end(), &slnpath_f);
	copy(slnpath.begin(), slnpath.end(), &slnpath_f);

	copy(cwd.begin(), cwd.end(), &slnpath_f);
	copy(airpath.begin(), airpath.end(), &slnpath_f);

	copy(cwd.begin(), cwd.end(), &cginst);
	copy(cginstaller.begin(), cginstaller.end(), &cginst);

	copy(cwd.begin(), cwd.end(), &airinst);
	copy(airwin.begin(), airwin.end(), &airinst);


	// End Path builder

	if (cgbinpath.empty())
	{
		copy(cwd.begin(), cwd.end(), &cginstunblock);
		copy(cginst[0].begin(), cginst[0].end(), &cginstunblock);

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

	// CG_BIN_PATH


	
	// Gets location of latest cg dll
	copy(cgbinpath.begin(), cgbinpath.end(), &cgbin);
	copy(cgbinpath.begin(), cgbinpath.end(), &cgfile);

	// Gets location of latest cgd3d9 dll
	copy(cgbinpath.begin(), cgbinpath.end(), &cgd3d9bin);
	copy(cgbinpath.begin(), cgbinpath.end(), &cgd3d9file);

	// Gets location of latest cggl dll
	copy(cgbinpath.begin(), cgbinpath.end(), &cgglbin);
	copy(cgbinpath.begin(), cgbinpath.end(), &cgglfile);



	// Working Directory
	copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgpath);
	copy(cgfile.begin(), cgfile.end(), &cgpath);

	copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgglpath);
	copy(cgglfile.begin(), cgglfile.end(), &cgglpath);

	copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgd3d9path);
	copy(cgd3d9file.begin(), cgd3d9file.end(), &cgd3d9path);

	copy(slnpath_f[0].begin(), slnpath_f[0].end(), &tbb);
	copy(tbbfile.begin(), tbbfile.end(), &tbb);

	copy(airinst[0].begin(), airinst[0].end(), &airinstunblock);
	copy(unblock.begin(), unblock.end(), &airinstunblock);

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


	// Gets location of latest adobeair dll
	copy(buff_c[0].begin(), buff_c[0].end(), &airfile);
	copy(air.begin(), air.end(), &airfile);

	copy(buff_c[0].begin(), buff_c[0].end(), &flashfile);
	copy(flash.begin(), flash.end(), &flashfile);


	// Check for Garena
	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"\\Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"\\Game");
		// Separate string-combining logic for Garena

		// Gets location of latest cg dll
		copy(cgbinpath.begin(), cgbinpath.end(), &cgbin);
		copy(cgbinpath.begin(), cgbinpath.end(), &cgfile);

		// Gets location of latest cgd3d9 dll
		copy(cgbinpath.begin(), cgbinpath.end(), &cgd3d9bin);
		copy(cgbinpath.begin(), cgbinpath.end(), &cgd3d9file);

		// Gets location of latest cggl dll
		copy(cgbinpath.begin(), cgbinpath.end(), &cgglbin);
		copy(cgbinpath.begin(), cgbinpath.end(), &cgglfile);



		// Working Directory
		copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgpath);
		copy(cgfile.begin(), cgfile.end(), &cgpath);

		copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgglpath);
		copy(cgglfile.begin(), cgglfile.end(), &cgglpath);

		copy(slnpath_f[0].begin(), slnpath_f[0].end(), &cgd3d9path);
		copy(cgd3d9file.begin(), cgd3d9file.end(), &cgd3d9path);

		copy(slnpath_f[0].begin(), slnpath_f[0].end(), &tbb);
		copy(tbbfile.begin(), tbbfile.end(), &tbb);

		// Gets location of latest adobeair dll
		copy(buff_c[0].begin(), buff_c[0].end(), &airfile);
		copy(air.begin(), air.end(), &airfile);

		copy(buff_c[0].begin(), buff_c[0].end(), &flashfile);
		copy(flash.begin(), flash.end(), &flashfile);
	}

	copy(cgpath[0].begin(), cgpath[0].end(), &cgunblock);
	copy(unblock.begin(), unblock.end(), &cgunblock);

	copy(cgglpath[0].begin(), cgglpath[0].end(), &cgglunblock);
	copy(unblock.begin(), unblock.end(), &cgglunblock);

	copy(cgd3d9path[0].begin(), cgd3d9path[0].end(), &cgd3d9unblock);
	copy(unblock.begin(), unblock.end(), &cgd3d9unblock);

	copy(airdir[0].begin(), airdir[0].end(), &airunblock);
	copy(unblock.begin(), unblock.end(), &airunblock);

	copy(flashdir[0].begin(), flashdir[0].end(), &flashunblock);
	copy(unblock.begin(), unblock.end(), &flashunblock);

	copy(tbb[0].begin(), tbb[0].end(), &tbbunblock);
	copy(unblock.begin(), unblock.end(), &tbbunblock);



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