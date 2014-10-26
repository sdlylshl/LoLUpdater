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
	if (cgbinpath == NULL)
	{
		strcpy(cginstunblock, cwd);
		strcpy(cginstunblock, cginstaller);
		strcpy(cginstunblock, unblock);
		URLDownloadToFile(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller,
			0,
			nullptr
		);
		DeleteFile(cginstunblock);
		free(cginstunblock);
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
		free(cginstaller);
	}

#if defined(ENVIRONMENT64)
	SHGetFolderPath(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		NULL,
		buff_c
	);
#elif defined (ENVIRONMENT32)
	SHGetFolderPath(
		nullptr,
		CSIDL_PROGRAM_FILES,
		nullptr,
		NULL,
		buff_c
		);
#endif

	// Check for Garena
	if (is_file_exist("lol.exe"))
	{
		// Separate string-combining logic for Garena
		strcpy(airfile, buff_c);
		strcpy(airfile, gair);
		strcat(airfile, air);

		strcpy(flashfile, buff_c);
		strcpy(flashfile, gair);
		strcat(flashfile, flash);

		strcpy(tbb, cwd);
		strcpy(tbb, game);
		strcat(tbb, tbbfile);

		strcpy(cgpath, cwd);
		strcpy(cgpath, game);
		strcat(cgpath, cgd3d9file);

		strcpy(cgglpath, cwd);
		strcpy(cgglpath, game);
		strcat(cgglpath, cgglfile);

		strcpy(cgd3d9path, cwd);
		strcpy(cgd3d9path, game);
		strcat(cgd3d9path, cgd3d9file);

		strcpy(cgglunblock, cgglpath);
		strcat(cgglunblock, unblock);

		strcpy(cgglunblock, cgpath);
		strcat(cgglunblock, unblock);

		strcpy(cgglunblock, cgd3d9path);
		strcat(cgglunblock, unblock);
	}

	strcpy(airfile, buff_c);
	strcpy(airfile, adobepath);
	strcat(airfile, air);

	strcpy(flashfile, buff_c);
	strcpy(flashfile, adobepath);
	strcat(flashfile, flash);
	free(buff_c);
	free(adobepath);
	free(air);
	free(flash);
	// String-combining logic

	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);
	free(cgfile);
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);
	free(cgd3d9file);
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);
	free(cgglfile);
	free(cgbinpath);
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


	strcpy(cgglunblock, cgglpath);
	strcat(cgglunblock, unblock);

	strcpy(cgunblock, cgpath);
	strcat(cgunblock, unblock);

	strcpy(cgd3d9unblock, cgd3d9path);
	strcat(cgd3d9unblock, unblock);


	strcpy(strair, cwd);
	strcpy(strair, airwin);
	strcat(strair, unblock);

	strcpy(airunblock, airdir);
	strcat(airunblock, unblock);

	strcpy(airunblock, flashdir);
	strcat(airunblock, unblock);

	strcpy(tbbunblock, cwd);
	strcpy(tbbunblock, tbb);
	strcat(tbbunblock, unblock);
	free(cwd);
	free(unblock);


	if (!is_file_exist(airwin))
	{
		URLDownloadToFile(
			nullptr,
			"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			airwin,
			0,
			nullptr
			);

		strcpy(airinst_buff, cwd);
		strcpy(airinst_buff, airwin);
		strcpy(airinst_buff, unblock);

		DeleteFile(airinst_buff);
		free(airinst_buff);
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
		free(airwin);
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
	free(tbb);
	CopyFile(
		cgbin,
		cgpath,
		false
	);
	free(cgbin);
	free(cgpath);
	CopyFile(
		cgglbin,
		cgglpath,
		false
	);
	free(cgglbin);
	free(cgglpath);
	CopyFile(
		cgd3d9bin,
		cgd3d9path,
		false
	);
	free(cgd3d9bin);
	free(cgd3d9path);
	CopyFile(
		airfile,
		airdir,
		false
	);
	free(airfile);
	free(airdir);
	CopyFile(
		flashfile,
		flashdir,
		false
	);
	free(flashfile);
	free(flashdir);

	// Unblocks all files
	for each (char* i in unblockfiles)
	{
		DeleteFile(i);
		std::cout << i;
	}
	free(unblockfiles);

	return 0;
}