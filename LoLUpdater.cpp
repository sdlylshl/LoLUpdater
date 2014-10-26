// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"
#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>
#if !_XP
#include <VersionHelpers.h>
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	char* unblock = ":Zone.Identifier";
	char* workingdirbuffer = nullptr;
	char* workingdir = _getcwd(
		workingdirbuffer,
		MAX_PATH
		);
	char* cgbinpath = getenv("CG_BIN_PATH");
	if (cgbinpath == NULL)
	{
		char* cginstaller = "Cg-3.1_April2012_Setup.exe";
		URLDownloadToFile(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller,
			0,
			nullptr
			);
		char cginstunblock[MAX_PATH];
		strcpy(cginstunblock, workingdir);
		strcpy(cginstunblock, cginstaller);
		strcpy(cginstunblock, unblock);

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

	// Todo: convert to foreach/Parallelforeacg
	char* cgfile = "\\Cg.dll";
	char* cgglfile = "\\CgGL.dll";
	char* cgd3d9file = "\\CgD3D9.dll";
	char cgbin[MAX_PATH];
	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);

	char cgd3d9bin[MAX_PATH];
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);


	char cgglbin[MAX_PATH];
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);

	char* slnpath = "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy";
	char cgpath[MAX_PATH];
	strcpy(cgpath, workingdir);
	strcpy(cgpath, slnpath);
	strcat(cgpath, cgd3d9file);


	char cgglpath[MAX_PATH];
	strcpy(cgglpath, workingdir);
	strcpy(cgglpath, slnpath);
	strcat(cgglpath, cgglfile);

	char cgd3d9path[MAX_PATH];
	strcpy(cgd3d9path, workingdir);
	strcpy(cgd3d9path, slnpath);
	strcat(cgd3d9path, cgd3d9file);

	char cgglunblock[MAX_PATH];
	strcpy(cgglunblock, cgglpath);
	strcat(cgglunblock, unblock);

	char cgunblock[MAX_PATH];
	strcpy(cgglunblock, cgpath);
	strcat(cgglunblock, unblock);

	char cgd3d9unblock[MAX_PATH];
	strcpy(cgglunblock, cgd3d9path);
	strcat(cgglunblock, unblock);

	char* game = "\\Game";
	char* tbbfile = "\\tbb.dll";
	char tbb[MAX_PATH];
	strcpy(tbb, workingdir);
	strcpy(tbb, slnpath);
	strcpy(tbb, tbbfile);

	if (is_file_exist("lol.exe"))
	{

		char tbb[MAX_PATH];
		strcpy(tbb, workingdir);
		strcpy(tbb, game);
		strcpy(tbb, tbbfile);

		char cgpath[MAX_PATH];
		strcpy(cgpath, workingdir);
		strcpy(cgpath, game);
		strcat(cgpath, cgd3d9file);

		char cgglpath[MAX_PATH];
		strcpy(cgglpath, workingdir);
		strcpy(cgglpath, game);
		strcat(cgglpath, cgglfile);

		char cgd3d9path[MAX_PATH];
		strcpy(cgd3d9path, workingdir);
		strcpy(cgd3d9path, game);
		strcat(cgd3d9path, cgd3d9file);

		char cgglunblock[MAX_PATH];
		strcpy(cgglunblock, cgglpath);
		strcat(cgglunblock, unblock);

		char cgunblock[MAX_PATH];
		strcpy(cgglunblock, cgpath);
		strcat(cgglunblock, unblock);

		char cgd3d9unblock[MAX_PATH];
		strcpy(cgglunblock, cgd3d9path);
		strcat(cgglunblock, unblock);
	}

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

	DeleteFile(cgunblock);
	DeleteFile(cgglunblock);
	DeleteFile(cgd3d9unblock);

	char* airwin = "air15_win.exe";
	char strair[MAX_PATH];
	strcpy(strair, workingdir);
	strcpy(strair, airwin);
	strcat(strair, unblock);
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

	char wide[MAX_PATH];


#if defined(ENVIRONMENT64)
	SHGetFolderPath(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		NULL,
		wide
		);
#elif defined (ENVIRONMENT32)
	SHGetFolderPath(
		nullptr,
		CSIDL_PROGRAM_FILES,
		nullptr,
		NULL,
		wide
		);
#endif

	char* adobepath = "\\Common Files\\Adobe AIR\\Versions\\1.0\\";
	char* airpath = "\\RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\";
	char* air = "Adobe AIR.dll";
	char airfile[MAX_PATH];
	strcpy(airfile, wide);
	strcpy(airfile, adobepath);
	strcat(airfile, air);

	char airdir[MAX_PATH];
	strcpy(airdir, workingdir);
	strcpy(airdir, airpath);
	strcat(airdir, air);

	char* flash = "Resources\\NPSWF32.dll";

	char flashfile[MAX_PATH];
	strcpy(flashfile, wide);
	strcpy(flashfile, adobepath);
	strcat(flashfile, flash);

	char flashdir[MAX_PATH];
	strcpy(flashdir, workingdir);
	strcpy(flashdir, airpath);
	strcat(flashdir, flash);


	if (is_file_exist("lol.exe"))
	{
		char* airpath = "\\AIR\\Adobe AIR\\Versions\\1.0\\";
		char airdir[MAX_PATH];
		strcpy(airdir, workingdir);
		strcpy(airdir, airpath);
		strcat(airdir, air);

		char flashdir[MAX_PATH];
		strcpy(flashdir, workingdir);
		strcpy(flashdir, airpath);
		strcat(flashdir, flash);
	}

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

	char airunblock[MAX_PATH];
	strcpy(airunblock, airdir);
	strcat(airunblock, unblock);

	char flashunblock[MAX_PATH];
	strcpy(airunblock, flashdir);
	strcat(airunblock, unblock);
	DeleteFile(airunblock);
	DeleteFile(flashunblock);


#if _XP

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

	char tbbunblock[MAX_PATH];
	strcpy(tbbunblock, tbb);
	strcat(tbbunblock, unblock);
	DeleteFile(tbbunblock);

	return 0;
}