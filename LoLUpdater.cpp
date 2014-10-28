// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 7" << std::endl << "Patching..." << std::endl;

	if (cgbinpath == L"")
	{
		memcpy(cginstunblock, &cginstaller[0], PATH_MAX);
		memcpy(cginstunblock, &unblock[0], PATH_MAX);
		URLDownloadToFileW(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			&cginstaller[0],
			0,
			nullptr
		);
		DeleteFileW(cginstunblock[0].c_str());
		SHELLEXECUTEINFOW ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = &cginstaller[0];
		ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	// Path builder
#if defined(ENVIRONMENT64)
	memcpy(buff_c, &drive);
	memcpy(buff_c, L":\\Program Files (x86)", PATH_MAX);
	memcpy(buff_c, adobepath, PATH_MAX);

#elif defined (ENVIRONMENT32)
	memcpy(buff_c, drive.c_str(), PATH_MAX);
	memcpy(buff_c, L":\\Program Files", PATH_MAX);
	memcpy(buff_c, &adobepath[0], PATH_MAX);
#endif
	memcpy(slnpath_f, &cwd[0], PATH_MAX);
	memcpy(slnpath_f, &slnpath[0], PATH_MAX);
	memcpy(airpath_f, &cwd[0], PATH_MAX);
	memcpy(airpath_f, &airpath[0], PATH_MAX);

	// End Path builder

	// Common Files

	// Gets location of latest adobeair dll
	memcpy(airfile, buff_c[0].c_str(), PATH_MAX);
	memcpy(airfile, &air[0], PATH_MAX);

	// Gets location of latest flash dll
	memcpy(flashfile, buff_c[0].c_str(), PATH_MAX);
	memcpy(flashfile, &flash[0], PATH_MAX);

	// Common Files UNBLOCK
	memcpy(airunblock, airdir[0].c_str(), PATH_MAX);
	memcpy(airunblock, &unblock[0], PATH_MAX);

	memcpy(flashunblock, flashdir[0].c_str(), PATH_MAX);
	memcpy(flashunblock, &unblock[0], PATH_MAX);

	// CG_BIN_PATH

	// Gets location of latest cg dll
	memcpy(cgbin, &cgbinpath[0], PATH_MAX);
	memcpy(cgbin, &cgfile[0], PATH_MAX);

	// Gets location of latest cgd3d9 dll
	memcpy(cgd3d9bin, &cgbinpath[0], PATH_MAX);
	memcpy(cgd3d9bin, &cgd3d9file[0], PATH_MAX);

	// Gets location of latest cggl dll
	memcpy(cgglbin, &cgbinpath[0], PATH_MAX);
	memcpy(cgglbin, &cgglfile[0], PATH_MAX);

	// CG_BIN_PATH UNBLOCK
	memcpy(cgglunblock, cgglpath[0].c_str(), PATH_MAX);
	memcpy(cgglunblock, &unblock[0], PATH_MAX);

	memcpy(cgunblock, cgpath[0].c_str(), PATH_MAX);
	memcpy(cgunblock, &unblock[0], PATH_MAX);

	memcpy(cgd3d9unblock, cgd3d9path[0].c_str(), PATH_MAX);
	memcpy(cgd3d9unblock, &unblock[0], PATH_MAX);

	// Working Directory
	memcpy(cgpath, slnpath_f[0].c_str(), PATH_MAX);
	memcpy(cgpath, &cgfile[0], PATH_MAX);

	memcpy(cgglpath, slnpath_f[0].c_str(), PATH_MAX);
	memcpy(cgglpath, &cgglfile[0], PATH_MAX);

	memcpy(cgd3d9path, slnpath_f[0].c_str(), PATH_MAX);
	memcpy(cgd3d9path, &cgd3d9file[0], PATH_MAX);

	memcpy(tbb, slnpath_f[0].c_str(), PATH_MAX);
	memcpy(tbb, &tbbfile[0], PATH_MAX);

	memcpy(airdir, airpath_f[0].c_str(), PATH_MAX);
	memcpy(airdir, &air[0], PATH_MAX);

	memcpy(flashdir, airpath_f[0].c_str(), PATH_MAX);
	memcpy(flashdir, &flash[0], PATH_MAX);

	// Misc UNBLOCK
	memcpy(tbbunblock, tbb[0].c_str(), PATH_MAX);
	memcpy(tbbunblock, &unblock[0], PATH_MAX);

	memcpy(airinstunblock, &airwin[0], PATH_MAX);
	memcpy(airinstunblock, &unblock[0], PATH_MAX);

		URLDownloadToFileW(
			nullptr,
			L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
			&airwin[0],
			0,
			nullptr
		);

		DeleteFileW(airinstunblock[0].c_str());
	SHELLEXECUTEINFOW ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = &airwin[0];
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
		memcpy(airdir, buff_c[0].c_str(), PATH_MAX);
		memcpy(airdir, &airpath[0], PATH_MAX);
		memcpy(airdir, &air[0], PATH_MAX);

		memcpy(flashdir, buff_c[0].c_str(), PATH_MAX);
		memcpy(flashdir, &airpath[0], PATH_MAX);
		memcpy(flashdir, &flash[0], PATH_MAX);

		memcpy(tbb, &cwd[0], PATH_MAX);
		memcpy(tbb, &slnpath[0], PATH_MAX);
		memcpy(tbb, &tbbfile[0], PATH_MAX);

		memcpy(cgd3d9path, &cwd[0], PATH_MAX);
		memcpy(cgd3d9path, &slnpath[0], PATH_MAX);
		memcpy(cgd3d9path, &cgd3d9file[0], PATH_MAX);

		memcpy(cgglpath, &cwd[0], PATH_MAX);
		memcpy(cgglpath, &slnpath[0], PATH_MAX);
		memcpy(cgglpath, &cgglfile[0], PATH_MAX);

		memcpy(cgpath, &cwd[0], PATH_MAX);
		memcpy(cgpath, &slnpath[0], PATH_MAX);
		memcpy(cgpath, &cgfile[0], PATH_MAX);

		memcpy(cgglunblock, cgglpath[0].c_str(), PATH_MAX);
		memcpy(cgglunblock, &unblock[0], PATH_MAX);

		memcpy(cgunblock, cgpath[0].c_str(), PATH_MAX);
		memcpy(cgunblock, &unblock[0], PATH_MAX);

		memcpy(cgd3d9unblock, cgd3d9path[0].c_str(), PATH_MAX);
		memcpy(cgd3d9unblock, &unblock[0], PATH_MAX);
	}
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