// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"
#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>

int _tmain(int argc, _TCHAR* argv[])
{
	
	if (cgbinpath == NULL)
	{
		strcpy(cginstunblock, workingdir);
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

#if XP
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
#else
#if defined(ENVIRONMENT64)
	SHGetKnownFolderPath(
		FOLDERID_ProgramFilesCommonX64,
		0,
		nullptr,
		buff_w
		);
#elif defined (ENVIRONMENT32)
	SHGetKnownFolderPath(
		FOLDERID_ProgramFilesCommon,
		0,
		nullptr,
		buff_w
		);
#endif
#endif

	// String-combining logic
#if XP
	strcpy(airdir, buff_c);
	strcpy(airdir, adobepathXP);
	strcat(airdir, air);

	strcpy(flashfile, buff_c);
	strcpy(flashfile, adobepathXP);
	strcat(flashfile, flash);
#else
	std::wstring wstrValue;
	wstrValue[0] = (wchar_t)buff_w;
	std::string strValue;
	strValue.assign(wstrValue.begin(), wstrValue.end());  // convert wstring to string


	std::string sym(1, strValue[0]);

	const char* char_value = sym.c_str();
	strcpy(airdir, char_value);
	strcpy(airdir, adobepath);
	strcat(airdir, air);
	printf("gets here");
	strcpy(flashdir, (const char*)char_value);
	strcpy(flashdir, adobepath);
	strcat(flashdir, flash);

	printf("gets here");

#endif
	// Temp test to check if correct value is returned
	printf(airdir);
	printf(flashdir);
	// end test
	strcpy(airfile, workingdir);
	strcpy(airfile, airpath);
	strcat(airfile, air);

	strcpy(flashdir, workingdir);
	strcpy(flashdir, airpath);
	strcat(flashdir, flash);

	strcpy(cgbin, cgbinpath);
	strcat(cgbin, cgfile);
	strcpy(cgd3d9bin, cgbinpath);
	strcat(cgd3d9bin, cgd3d9file);
	strcpy(cgglbin, cgbinpath);
	strcat(cgglbin, cgglfile);
	strcpy(cgpath, workingdir);
	strcpy(cgpath, slnpath);
	strcat(cgpath, cgd3d9file);
	strcpy(cgglpath, workingdir);
	strcpy(cgglpath, slnpath);
	strcat(cgglpath, cgglfile);
	strcpy(cgd3d9path, workingdir);
	strcpy(cgd3d9path, slnpath);
	strcat(cgd3d9path, cgd3d9file);
	strcpy(cgglunblock, cgglpath);
	strcat(cgglunblock, unblock);
	strcpy(cgglunblock, cgpath);
	strcat(cgglunblock, unblock);
	strcpy(cgglunblock, cgd3d9path);
	strcat(cgglunblock, unblock);
	strcpy(tbb, workingdir);
	strcpy(tbb, slnpath);
	strcpy(tbb, tbbfile);
	strcpy(strair, workingdir);
	strcpy(strair, airwin);
	strcat(strair, unblock);
	strcpy(airunblock, airdir);
	strcat(airunblock, unblock);
	strcpy(airunblock, flashdir);
	strcat(airunblock, unblock);
	strcpy(tbbunblock, tbb);
	strcat(tbbunblock, unblock);

	// Check for Garena
	if (is_file_exist("lol.exe"))
	{
		char* airpath = "\\AIR\\Adobe AIR\\Versions\\1.0\\";

		// Separate string´-combining logic for Garena
		strcpy(tbb, workingdir);
		strcpy(tbb, game);
		strcpy(tbb, tbbfile);
		strcpy(cgpath, workingdir);
		strcpy(cgpath, game);
		strcat(cgpath, cgd3d9file);
		strcpy(cgglpath, workingdir);
		strcpy(cgglpath, game);
		strcat(cgglpath, cgglfile);
		strcpy(cgd3d9path, workingdir);
		strcpy(cgd3d9path, game);
		strcat(cgd3d9path, cgd3d9file);
		strcpy(cgglunblock, cgglpath);
		strcat(cgglunblock, unblock);
		strcpy(cgglunblock, cgpath);
		strcat(cgglunblock, unblock);
		strcpy(cgglunblock, cgd3d9path);
		strcat(cgglunblock, unblock);
#if XP
		strcpy(airdir, buff_c);
		strcpy(airdir, airpath);
		strcat(airdir, air);

		strcpy(flashfile, buff_c);
		strcpy(flashfile, adobepath);
		strcat(flashfile, flash);
#else

		std::wstring wstrValue;
		wstrValue[0] = (wchar_t)buff_w;

		std::string strValue;
		strValue.assign(wstrValue.begin(), wstrValue.end());  // convert wstring to string

		char char_value = strValue[0];

		strcpy(airdir, (const char*)char_value);
		strcpy(airdir, airpath);
		strcat(airdir, air);

		strcpy(flashdir, (const char*)char_value);
		strcpy(flashdir, airpath);
		strcat(flashdir, flash);



#endif

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

	// Unblocks all files
	for each (char* i in unblockfiles) {
		DeleteFile(i);
	}
	return 0;
}