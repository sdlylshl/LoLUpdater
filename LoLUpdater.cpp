// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"
#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 5" << std::endl << "Patching...";
	std::cout << "";
	if (cgbinpath == NULL)
	{
		strcpy(cginst, cwd);
		strcat(cginst, "\\Cg-3.1_April2012_Setup.exe");

		strcpy(cginstunblock, cginst);
		strcat(cginstunblock, unblock);
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginst,
			0,
			nullptr
		);
		DeleteFileA(cginstunblock);
		SHELLEXECUTEINFOA ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = cginst;
		ShExecInfo.lpParameters = "/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExA(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

#if defined(ENVIRONMENT64)
	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		0,
		buff_c
	);
#elif defined (ENVIRONMENT32)
	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILES,
		nullptr,
		0,
		buff_c
		);
#endif
	// String-combining logic

	strcpy(airfile, buff_c);
	strcpy(airfile, adobepath);
	strcat(airfile, air);
	std::cout << airfile;
	strcpy(flashfile, buff_c);
	strcpy(flashfile, adobepath);
	strcat(flashfile, flash);
	std::cout << flashfile;
	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);
	std::cout << cgbin;
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);
	std::cout << cgd3d9bin;
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);
	std::cout << cgglbin;
	strcpy(cgpath, cwd);
	strcpy(cgpath, slnpath);
	strcat(cgpath, cgfile);
	std::cout << cgpath;
	strcpy(cgglpath, cwd);
	strcpy(cgglpath, slnpath);
	strcat(cgglpath, cgglfile);
	std::cout << cgglpath;
	strcpy(cgd3d9path, cwd);
	strcpy(cgd3d9path, slnpath);
	strcat(cgd3d9path, cgd3d9file);
	std::cout << cgd3d9path;
	strcpy(tbb, cwd);
	strcpy(tbb, slnpath);
	strcat(tbb, tbbfile);
	std::cout << tbb;
	strcpy(airdir, cwd);
	strcpy(airdir, airpath);
	strcat(airdir, air);
	std::cout << airdir;
	strcpy(flashdir, cwd);
	strcpy(flashdir, airpath);
	strcat(flashdir, flash);
	std::cout << flashdir;

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


	if (!is_file_exist(airwin))
	{
		strcpy(airinst, cwd);
		strcpy(airinst, "\\");
		strcpy(airinst, airwin);

		strcpy(airinstunblock, airinst);
		strcpy(airinstunblock, unblock);

		URLDownloadToFileA(
			nullptr,
			"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			airwin,
			0,
			nullptr
			);

		DeleteFileA(airinstunblock);
		SHELLEXECUTEINFOA ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = airwin;
		ShExecInfo.lpParameters = "-silent";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExA(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}
	// Check for Garena
	if (is_file_exist("lol.exe"))
	{
		// Separate string-combining logic for Garena
		strcpy(airdir, buff_c);
		strcpy(airdir, gair);
		strcat(airdir, air);

		strcpy(flashdir, buff_c);
		strcpy(flashdir, gair);
		strcat(flashdir, flash);

		strcpy(tbb, cwd);
		strcpy(tbb, game);
		strcat(tbb, tbbfile);

		strcpy(cgd3d9path, cwd);
		strcpy(cgd3d9path, game);
		strcat(cgd3d9path, cgd3d9file);

		strcpy(cgglpath, cwd);
		strcpy(cgglpath, game);
		strcat(cgglpath, cgglfile);

		strcpy(cgpath, cwd);
		strcpy(cgpath, game);
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

	// Unblocks all files
	for each (char* i in unblockfiles)
	{
		DeleteFileA(i);
	}
	std::cout << "LoLUpdater finished!";
	system("pause");

	return 0;
}