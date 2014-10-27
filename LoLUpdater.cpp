// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 5" << std::endl << "Patching...";
	std::cout << "";
	if (cgbinpath == NULL)
	{


		strcpy(cginstunblock, cginstaller);
		strcat(cginstunblock, unblock);
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller,
			0,
			nullptr
		);
		DeleteFileA(cginstunblock);
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = cginstaller;
		ShExecInfo.lpParameters = "/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

#if defined(ENVIRONMENT64)
	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		NULL,
		buff_c
		);
#elif defined (ENVIRONMENT32)
	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILES,
		nullptr,
		NULL,
		buff_c
		);
#endif
	// String-combining logic

	strcpy(airfile, buff_c);
	strcpy(airfile, adobepath);
	strcat(airfile, air);
	strcpy(flashfile, buff_c);
	strcpy(flashfile, adobepath);
	strcat(flashfile, flash);
	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);
	// Problem starts here
	strcpy(cgpath, cwd);
	strcpy(cgpath, slnpath);
	strcat(cgpath, cgfile);
	strcpy(cgglpath, cwd);
	strcpy(cgglpath, slnpath);
	strcat(cgglpath, cgglfile);
	strcpy(cgd3d9path, cwd);
	strcpy(cgd3d9path, slnpath);
	strcat(cgd3d9path, cgd3d9file);
	strcpy(tbb, cwd);
	strcpy(tbb, slnpath);
	strcat(tbb, tbbfile);
	strcpy(airdir, cwd);
	strcpy(airdir, airpath);
	strcat(airdir, air);
	strcpy(flashdir, cwd);
	strcpy(flashdir, airpath);
	strcat(flashdir, flash);
	// problem ends here
	strcpy(cgglunblock, cgglpath);
	strcat(cgglunblock, unblock);
	strcpy(cgunblock, cgpath);
	strcat(cgunblock, unblock);
	strcpy(cgd3d9unblock, cgd3d9path);
	strcat(cgd3d9unblock, unblock);

	strcpy(airunblock, airdir);
	strcat(airunblock, unblock);

	strcpy(flashunblock, flashdir);
	strcat(flashunblock, unblock);

	strcpy(tbbunblock, cwd);
	strcpy(tbbunblock, tbb);
	strcat(tbbunblock, unblock);


		strcpy(airinstunblock, airwin);
		strcpy(airinstunblock, unblock);
		if (is_file_exist(airwin))
		{
			URLDownloadToFileA(
				nullptr,
				"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
				airwin,
				0,
				nullptr
				);
		}
		
		DeleteFileA(airinstunblock);
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = airwin;
		ShExecInfo.lpParameters = "-silent";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Check for Garena
	if (is_file_exist("lol.exe"))
	{
		char* airpath = "\\Air\\Adobe AIR\\Versions\\1.0\\";
		char* slnpath = "\\Game";
		// Separate string-combining logic for Garena
		strcpy(airdir, buff_c);
		strcpy(airdir, airpath);
		strcat(airdir, air);

		strcpy(flashdir, buff_c);
		strcpy(flashdir, airpath);
		strcat(flashdir, flash);

		strcpy(tbb, cwd);
		strcpy(tbb, slnpath);
		strcat(tbb, tbbfile);

		strcpy(cgd3d9path, cwd);
		strcpy(cgd3d9path, slnpath);
		strcat(cgd3d9path, cgd3d9file);

		strcpy(cgglpath, cwd);
		strcpy(cgglpath, slnpath);
		strcat(cgglpath, cgglfile);

		strcpy(cgpath, cwd);
		strcpy(cgpath, slnpath );
		strcat(cgpath, cgfile);

		strcpy(cgglunblock, cgglpath);
		strcat(cgglunblock, unblock);

		strcpy(cgunblock, cgpath);
		strcat(cgunblock, unblock);

		strcpy(cgd3d9unblock, cgd3d9path);
		strcat(cgd3d9unblock, unblock);
	}
#if _XP
	// XP
		URLDownloadToFile(
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
		URLDownloadToFile(
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
			URLDownloadToFile(
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
				URLDownloadToFile(
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
					URLDownloadToFile(
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
					URLDownloadToFile(
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

	// Unblocks all files
	for each (char* i in unblockfiles)
	{
		DeleteFileA(i);
	}
	std::cout << "LoLUpdater finished!" << std::endl << "";
	std::cout << flashdir;
	std::cout << cgglpath;
	system("pause");

	return 0;
}