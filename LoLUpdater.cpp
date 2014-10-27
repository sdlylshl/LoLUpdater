// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 6" << std::endl << "Patching..." << std::endl;

	if (cgbinpath == NULL)
	{
		strcpy(cginstunblock, cginstaller);
		strncat(cginstunblock, unblock, MAX_PATH+1);
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller,
			0,
			nullptr
		);
		DeleteFileA(cginstunblock);
		SHELLEXECUTEINFOA ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = cginstaller;
		ShExecInfo.lpParameters = "/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExA(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	// Path builder
#if defined(ENVIRONMENT64)
	strcpy(buff_c, &drive);
	strncat(buff_c, ":\\Program Files (x86)", MAX_PATH+1);
	strncat(buff_c, adobepath, MAX_PATH+1);

#elif defined (ENVIRONMENT32)
	strcpy(buff_c, &drive);
	strncat(buff_c, ":\\Program Files", MAX_PATH+1);
	strncat(buff_c, adobepath, MAX_PATH+1);
#endif
	strcpy(slnpath_f, cwd);
	strncat(slnpath_f, slnpath, MAX_PATH+1);
	strcpy(airpath_f, cwd);
	strncat(airpath_f, airpath, MAX_PATH+1);

	// End Path builder

	// Common Files

	// Gets location of latest adobeair dll
	strcpy(airfile, buff_c);
	strncat(airfile, air, MAX_PATH+1);

	// Gets location of latest flash dll
	strcpy(flashfile, buff_c);
	strncat(flashfile, flash, MAX_PATH+1);

	// Common Files UNBLOCK
	strcpy(airunblock, airdir);
	strncat(airunblock, unblock, MAX_PATH+1);

	strcpy(flashunblock, flashdir);
	strncat(flashunblock, unblock, MAX_PATH+1);

	// CG_BIN_PATH

	// Gets location of latest cg dll
	strcpy(cgbin, cgbinpath);
	strncat(cgbin, cgfile, MAX_PATH+1);

	// Gets location of latest cgd3d9 dll
	strcpy(cgd3d9bin, cgbinpath);
	strncat(cgd3d9bin, cgd3d9file, MAX_PATH+1);

	// Gets location of latest cggl dll
	strcpy(cgglbin, cgbinpath);
	strncat(cgglbin, cgglfile, MAX_PATH+1);

	// CG_BIN_PATH UNBLOCK
	strcpy(cgglunblock, cgglpath);
	strncat(cgglunblock, unblock, MAX_PATH+1);

	strcpy(cgunblock, cgpath);
	strncat(cgunblock, unblock, MAX_PATH+1);

	strcpy(cgd3d9unblock, cgd3d9path);
	strncat(cgd3d9unblock, unblock, MAX_PATH+1);

	// Working Directory
	strcpy(cgpath, slnpath_f);
	strncat(cgpath, cgfile, MAX_PATH+1);

	strcpy(cgglpath, slnpath_f);
	strncat(cgglpath, cgglfile, MAX_PATH+1);

	strcpy(cgd3d9path, slnpath_f);
	strncat(cgd3d9path, cgd3d9file, MAX_PATH+1);

	strcpy(tbb, slnpath_f);
	strncat(tbb, tbbfile, MAX_PATH+1);

	strcpy(airdir, airpath_f);
	strncat(airdir, air, MAX_PATH+1);

	strcpy(flashdir, airpath_f);
	strncat(flashdir, flash, MAX_PATH+1);

	// Misc UNBLOCK
	strcpy(tbbunblock, tbb);
	strncat(tbbunblock, unblock, MAX_PATH+1);

	strcpy(airinstunblock, airwin);
	strncat(airinstunblock, unblock, MAX_PATH+1);

		URLDownloadToFileA(
			nullptr,
			"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			airwin,
			0,
			nullptr
		);

	DeleteFileA(airinstunblock);
	SHELLEXECUTEINFOA ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airwin;
	ShExecInfo.lpParameters = "-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExA(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Check for Garena
	if (is_file_exist("lol.exe"))
	{
		char* airpath = "\\Air\\Adobe AIR\\Versions\\1.0\\";
		char* slnpath = "\\Game";
		// Separate string-combining logic for Garena
		strcpy(airdir, buff_c);
		strcpy(airdir, airpath);
		strncat(airdir, air, MAX_PATH+1);

		strcpy(flashdir, buff_c);
		strcpy(flashdir, airpath);
		strncat(flashdir, flash, MAX_PATH+1);

		strcpy(tbb, cwd);
		strcpy(tbb, slnpath);
		strncat(tbb, tbbfile, MAX_PATH+1);

		strcpy(cgd3d9path, cwd);
		strcpy(cgd3d9path, slnpath);
		strncat(cgd3d9path, cgd3d9file, MAX_PATH+1);

		strcpy(cgglpath, cwd);
		strcpy(cgglpath, slnpath);
		strncat(cgglpath, cgglfile, MAX_PATH+1);

		strcpy(cgpath, cwd);
		strcpy(cgpath, slnpath);
		strncat(cgpath, cgfile, MAX_PATH+1);

		strcpy(cgglunblock, cgglpath);
		strncat(cgglunblock, unblock, MAX_PATH+1);

		strcpy(cgunblock, cgpath);
		strncat(cgunblock, unblock, MAX_PATH+1);

		strcpy(cgd3d9unblock, cgd3d9path);
		strncat(cgd3d9unblock, unblock, MAX_PATH+1);
	}
#if _XP
	// XP
		URLDownloadToFileA(
			nullptr,
			"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Xp.dll",
			tbb,
			0,
			nullptr
			);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2
		URLDownloadToFileA(
			nullptr,
			"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx2.dll",
			tbb,
			0,
			nullptr
		);
	}
	else
	{
		// AVX
		if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != 0)
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
				tbb,
				0,
				nullptr
			);
		}
		else
		{
			//SSE2
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				URLDownloadToFileA(
					nullptr,
					"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse2.dll",
					tbb,
					0,
					nullptr
				);
			}
			else
			{
				//SSE
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					URLDownloadToFileA(
						nullptr,
						"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse.dll",
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
			}
		}
	}

#endif
	CopyFileA(
		cgbin,
		cgpath,
		false
	);
	CopyFileA(
		cgglbin,
		cgglpath,
		false
	);
	CopyFileA(
		cgd3d9bin,
		cgd3d9path,
		false
	);
	CopyFileA(
		airfile,
		airdir,
		false
	);
	CopyFileA(
		flashfile,
		flashdir,
		false
	);
	for each (char* i in unblockfiles)
	{
		DeleteFileA(i);
	}

	std::cout << "LoLUpdater finished!" << std::endl << "";
	system("pause");

	return 0;
}