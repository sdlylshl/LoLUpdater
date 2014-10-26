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
		strcpy(cginstunblock, workingdirbuffer);
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

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
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
	std::cout << "gets here";
	strcpy(airfile, buff_c);
	strcpy(airfile, adobepath);
	strcat(airfile, air);
	std::cout << "gets here";
	strcpy(flashfile, buff_c);
	strcpy(flashfile, adobepath);
	strcat(flashfile, flash);
	std::cout << "gets here";
	// String-combining logic
	
	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);
	std::cout << "gets here";
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);
	std::cout << "gets here";
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);
	std::cout << "gets here";
	strcpy(cgpath, workingdirbuffer);
	strcpy(cgpath, slnpath);
	strcat(cgpath, cgfile);
	std::cout << "gets here";
	strcpy(cgglpath, workingdirbuffer);
	strcpy(cgglpath, slnpath);
	strcat(cgglpath, cgglfile);
	std::cout << "gets here";
	strcpy(cgd3d9path, workingdirbuffer);
	strcpy(cgd3d9path, slnpath);
	strcat(cgd3d9path, cgd3d9file);
	std::cout << "gets here";

	strcpy(tbb, workingdirbuffer);
	strcpy(tbb, slnpath);
	strcat(tbb, tbbfile);

	strcpy(airdir, workingdirbuffer);
	strcpy(airdir, airpath);
	strcat(airdir, air);
	std::cout << "gets here";
	strcpy(flashdir, workingdirbuffer);
	strcpy(flashdir, airpath);
	strcat(flashdir, flash);
	std::cout << "gets here";

	strcpy(cgglunblock, cgglpath);
	strcat(cgglunblock, unblock);
	std::cout << "gets here";
	strcpy(cgunblock, cgpath);
	strcat(cgunblock, unblock);
	std::cout << "gets here";
	strcpy(cgd3d9unblock, cgd3d9path);
	strcat(cgd3d9unblock, unblock); 
	std::cout << "gets here";

	std::cout << "gets here";
	strcpy(strair, workingdirbuffer);
	strcpy(strair, airwin);
	strcat(strair, unblock);
	std::cout << "gets here";
	strcpy(airunblock, airdir);
	strcat(airunblock, unblock);
	std::cout << "gets here";
	strcpy(airunblock, flashdir);
	strcat(airunblock, unblock);
	std::cout << "gets here";
	strcpy(tbbunblock, tbb);
	strcat(tbbunblock, unblock);
	std::cout << "gets here";
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

		strcpy(tbb, workingdirbuffer);
		strcpy(tbb, game);
		strcat(tbb, tbbfile);

		strcpy(cgpath, workingdirbuffer);
		strcpy(cgpath, game);
		strcat(cgpath, cgd3d9file);

		strcpy(cgglpath, workingdirbuffer);
		strcpy(cgglpath, game);
		strcat(cgglpath, cgglfile);

		strcpy(cgd3d9path, workingdirbuffer);
		strcpy(cgd3d9path, game);
		strcat(cgd3d9path, cgd3d9file);

		strcpy(cgglunblock, cgglpath);
		strcat(cgglunblock, unblock);

		strcpy(cgglunblock, cgpath);
		strcat(cgglunblock, unblock);

		strcpy(cgglunblock, cgd3d9path);
		strcat(cgglunblock, unblock);

	}
	URLDownloadToFile(
		nullptr,
		"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airwin,
		0,
		nullptr
		);
	DeleteFile(strair);
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

	CopyFile(
		cgbin,
		cgpath,
		false
		);
	CopyFile(
		cgglbin,
		cgglpath,
		false
		);
	CopyFile(
		cgd3d9bin,
		cgd3d9path,
		false
		);
	CopyFile(
		airfile,
		airdir,
		false
		);
	CopyFile(
		flashfile,
		flashdir,
		false
		);

	// Unblocks all files
	for each (char* i in unblockfiles) {
		DeleteFile(i);
		std::cout << i;
	}

	return 0;
}