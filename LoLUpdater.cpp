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
	char* zone = ":Zone.Identifier";
	char* buffer12 = nullptr;
	char* start = _getcwd(
		buffer12,
		200
		);
	char* cgbinpath = getenv("CG_BIN_PATH");
	if (cgbinpath == NULL)
	{
		char* cginstaller = "Cg-3.1_April2012_Setup.exe";
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller,
			0,
			nullptr
			);
		char strcg[MAX_PATH];
		strcpy(strcg, start);
		strcpy(strcg, cginstaller);
		strcpy(strcg, zone);

		DeleteFileA(strcg);

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
	char* cg = "\\Cg.dll";
	char* cggl = "\\CgGL.dll";
	char* cgd3d9 = "\\CgD3D9.dll";
	char str11[MAX_PATH];
	strcpy(str11, cgbinpath);
	strcat(str11, "\\Cg.dll");

	char str22[MAX_PATH];
	strcpy(str22, cgbinpath);
	strcat(str22, cg);

	char str33[MAX_PATH];
	strcpy(str33, cgbinpath);
	strcat(str33, cgd3d9);

	char* slnpath = "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy";
	char str1[MAX_PATH];
	strcpy(str1, start);
	strcat(str1, cggl);

	char str2[MAX_PATH];
	strcpy(str2, start);
	strcpy(str2, slnpath);
	strcat(str2, cggl);

	char str3[MAX_PATH];
	strcpy(str3, start);
	strcpy(str2, slnpath);
	strcat(str2, cgd3d9);

	char str0[MAX_PATH];
	strcpy(str0, start);
	strcpy(str0, str1);
	strcat(str0, zone);
	char str01[MAX_PATH];
	strcpy(str0, start);
	strcpy(str0, str2);
	strcat(str0, zone);
	char str02[MAX_PATH];
	strcpy(str0, start);
	strcpy(str0, str3);
	strcat(str0, zone);
	char* game = "//Game";
	char* tbbfile = "\\tbb.dll";
	char tbb[MAX_PATH];
	strcpy(tbb, start);
	strcpy(tbb, slnpath);
	strcpy(tbb, tbbfile);

	if (is_file_exist("lol.exe"))
	{

		char tbb[MAX_PATH];
		strcpy(tbb, start);
		strcpy(tbb, game);
		strcpy(tbb, tbbfile);

		char str1[MAX_PATH];
		strcpy(str1, start);
		strcpy(str1, game);
		strcpy(str1, cg);

		char str2[MAX_PATH];
		strcpy(str2, start);
		strcpy(str1, game);
		strcpy(str1, cggl);

		char str3[MAX_PATH];
		strcpy(str3, start);
		strcpy(str1, game);
		strcpy(str1, cgd3d9);

		char str0[MAX_PATH];
		strcpy(str0, start);
		strcpy(str0, str1);
		strcat(str0, zone);
		char str01[MAX_PATH];
		strcpy(str0, start);
		strcpy(str0, str2);
		strcat(str0, zone);
		char str02[MAX_PATH];
		strcpy(str0, start);
		strcpy(str0, str3);
		strcat(str0, zone);
	}

	CopyFileA(
		str11,
		str1,
		false
		);
	CopyFileA(
		str22,
		str2,
		false
		);
	CopyFileA(
		str33,
		str3,
		false
		);

	DeleteFileA(str0);
	DeleteFileA(str01);
	DeleteFileA(str02);

	char* airwin = "air15_win.exe";
	char strair[MAX_PATH];
	strcpy(strair, start);
	strcpy(strair, airwin);
	strcat(strair, zone);
	URLDownloadToFileA(
		nullptr,
		"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airwin,
		0,
		nullptr
		);
	DeleteFileA(strair);


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
	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		NULL,
		wide
		);
#elif defined (ENVIRONMENT32)
	SHGetFolderPathA(
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
	char str00[MAX_PATH];
	strcpy(str00, wide);
	strcpy(str00, adobepath);
	strcat(str00, air);
	char str000[MAX_PATH];
	strcpy(str000, start);
	strcpy(str000, airpath);
	strcat(str000, air);

	char* flash = "Resources\\NPSWF32.dll";
	char str001[MAX_PATH];
	strcpy(str001, wide);
	strcpy(str00, adobepath);
	strcat(str001, flash);
	char str0001[MAX_PATH];
	strcpy(str0001, start);
	strcpy(str000, airpath);
	strcat(str0001, flash);


	if (is_file_exist("lol.exe"))
	{
		char* airpath = "\\AIR\\Adobe AIR\\Versions\\1.0\\";
		char str000[MAX_PATH];
		strcpy(str000, start);
		strcpy(str000, airpath);
		strcat(str000, air);

		char str0001[MAX_PATH];
		strcpy(str0001, start);
		strcpy(str000, airpath);
		strcat(str000, flash);
	}

	CopyFileA(
		str00,
		str000,
		false
		);
	CopyFileA(
		str001,
		str0001,
		false
		);

	char str0000[MAX_PATH];
	strcpy(str0000, str000);
	strcat(str0000, zone);

	char str00001[MAX_PATH];
	strcpy(str00001, str0001);
	strcat(str00001, zone);
	DeleteFileA(str0000);
	DeleteFileA(str00001);


#if _XP

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

	char strtbb_c[MAX_PATH];
	strcpy(strtbb_c, tbb);
	strcat(strtbb_c, zone);
	DeleteFileA(strtbb_c);

	return 0;
}