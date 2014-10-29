// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "LoLUpdater Alpha 1 Build 7" << std::endl << "Patching..." << std::endl;
	std::locale::global(std::locale(""));
	// Path builder

	cwd.push_back(_wgetcwd(
		nullptr,
		0
		));
	drive.push_back(cwd[0]);
	cgbinpath.push_back(_wgetenv(L"CG_BIN_PATH"));
	unblock.push_back(L":Zone.Identifier");
	air.push_back(L"Adobe AIR.dll");
	flash.push_back(L"Resources\\NPSWF32.dll");
	cgfile.push_back(L"\\Cg.dll");
	cgglfile.push_back(L"\\CgGL.dll");
	cgd3d9file.push_back(L"\\CgD3D9.dll");
	prog.push_back(L":\\Program Files (x86)");
	progx86.push_back(L":\\Program Files");
	cginstaller.push_back(L"\\Cg-3.1_April2012_Setup.exe");
	tbbfile.push_back(L"\\tbb.dll");
	adobepath.push_back(L"\\Common Files\\Adobe AIR\\Versions\\1.0\\");
	slnpath.push_back(L"\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy");
	airpath.push_back(L"\\RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");
	airwin.push_back(L"\\air15_win.exe");

	copy(drive.begin(), drive.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(buff_c[0], L","));
	buff_c[0] << drive.back();

#if defined(ENVIRONMENT64)

	copy(progx86.begin(), progx86.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(buff_c[0], L","));
	buff_c[0] << progx86.back();


#elif defined (ENVIRONMENT32)

	copy(prog.begin(), prog.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(buff_c[0], L","));
	buff_c[0] << prog.back();
#endif

	copy(adobepath.begin(), adobepath.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(buff_c[0], L","));
	buff_c[0] << adobepath.back();

	copy(cwd.begin(), cwd.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(slnpath_f[0], L","));
	slnpath_f[0] << cwd.back();
	copy(slnpath.begin(), slnpath.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(slnpath_f[0], L","));
	slnpath_f[0] << cwd.back();

	copy(cwd.begin(), cwd.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(airpath_f[0], L","));
	airpath_f[0] << cwd.back();
	copy(airpath.begin(), airpath.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(airpath_f[0], L","));
	airpath_f[0] << airpath.back();

	copy(cwd.begin(), cwd.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(cginst[0], L","));
	cginst[0] << cwd.back();
	copy(cginstaller.begin(), cginstaller.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(cginst[0], L","));
	cginst[0] << cginstaller.back();

	copy(cwd.begin(), cwd.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(airinst[0], L","));
	airinst[0] << cwd.back();
	copy(airwin.begin(), airwin.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(airinst[0], L","));
	airinst[0] << airwin.back();


	// End Path builder

	if (cgbinpath.empty())
	{
		copy(cgbinpath.begin(), cgbinpath.end()-1, std::ostream_iterator<std::wstring, wchar_t>(cginstunblock[0], L","));
		cgbin[0] << cwd.back();
		copy(&cginst.begin(), &cginst[0].end()-1, std::ostream_iterator<std::wstring, wchar_t>(cginstunblock[0], L","));
		cgbin[0] << cginst[0].back();


		// Now add the last element with no delimiter
		oss << vec.back();

		URLDownloadToFileW(
			nullptr,
			L"https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			&cginst[0].c_str(),
			0,
			nullptr
			);

		DeleteFileW(cginstunblock.c_str());
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
	copy(cgbinpath.begin(), cgbinpath.end(), std::ostream_iterator<std::wstring, wchar_t>(cgbin[0], L","));
	cgbin[0] << cgbinpath.back();
	copy(cgfile.begin(), cgfile.end(), std::ostream_iterator<std::wstring, wchar_t>(cgbin[0], L","));
	cgbin[0] << cgfile.back();

	// Gets location of latest cgd3d9 dll
	copy(cgbinpath.begin(), cgbinpath.end(), std::ostream_iterator<std::wstring, wchar_t>(cgd3d9bin[0], L","));
	cgd3d9bin[0] << cgbinpath.back();
	copy(cgd3d9file.begin(), cgd3d9file.end(), std::ostream_iterator<std::wstring, wchar_t>(cgd3d9bin[0], L","));
	cgd3d9bin[0] << cgd3d9file.back();

	// Gets location of latest cggl dll
	copy(cgbinpath.begin(), cgbinpath.end(), std::ostream_iterator<std::wstring, wchar_t>(cgglbin[0], L","));
	cgglbin[0] << cgbinpath.back();
	copy(cgglfile.begin(), cgglfile.end(), std::ostream_iterator<std::wstring, wchar_t>(cgglbin[0], L","));
	cgglbin[0] << cgglfile.back();



	// Working Directory
	copy(&slnpath_f.begin(), slnpath_f[0].end(), std::ostream_iterator<std::wstring, wchar_t>(cgpath[0], L","));
	cgglbin[0] << cgbinpath.back();
	copy(cgglfile.begin(), cgglfile.end(), std::ostream_iterator<std::wstring, wchar_t>(cgpath[0], L","));
	cgglbin[0] << cgglfile.back();


	copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgpath[0].begin());
	copy(cgfile.begin(), cgfile.end() - 1, cgpath[0].begin());

	copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgglpath[0].begin());
	copy(cgglfile.begin(), cgglfile.end() - 1, cgglpath[0].begin());

	copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgd3d9path[0].begin());
	copy(cgd3d9file.begin(), cgd3d9file.end() - 1, cgd3d9path[0].begin());

	copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, tbb[0].begin());
	copy(tbbfile.begin(), tbbfile.end() - 1, tbb[0].begin());

	copy(airinst[0].begin(), airinst[0].end() - 1, airinstunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, airinstunblock[0].begin());

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
	copy(buff_c[0].begin(), buff_c[0].end() - 1, airfile[0].begin());
	copy(air.begin(), air.end() - 1, airfile[0].begin());

	copy(buff_c[0].begin(), buff_c[0].end() - 1, flashfile[0].begin());
	copy(flash.begin(), flash.end() - 1, flashfile[0].begin());


	// Check for Garena
	if (file_exists(L"lol.exe"))
	{
		std::wstring airpath(L"\\Air\\Adobe AIR\\Versions\\1.0\\");
		std::wstring slnpath(L"\\Game");
		// Separate string-combining logic for Garena

		// Gets location of latest cg dll
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgfile.begin());
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgfile.begin());

		// Gets location of latest cgd3d9 dll
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgd3d9file.begin());
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgd3d9file.begin());

		// Gets location of latest cggl dll
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgglfile.begin());
		copy(cgbinpath.begin(), cgbinpath.end() - 1, cgglfile.begin());



		// Working Directory
		copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgpath[0].begin());
		copy(cgfile.begin(), cgfile.end() - 1, cgpath[0].begin());

		copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgglpath[0].begin());
		copy(cgglfile.begin(), cgglfile.end() - 1, cgglpath[0].begin());

		copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, cgd3d9path[0].begin());
		copy(cgd3d9file.begin(), cgd3d9file.end() - 1, cgd3d9path[0].begin());

		copy(slnpath_f[0].begin(), slnpath_f[0].end() - 1, tbb[0].begin());
		copy(tbbfile.begin(), tbbfile.end() - 1, tbb[0].begin());

		// Gets location of latest adobeair dll
		copy(buff_c[0].begin(), buff_c[0].end() - 1, airfile[0].begin());
		copy(air.begin(), air.end() - 1, airfile[0].begin());

		copy(buff_c[0].begin(), buff_c[0].end() - 1, flashfile[0].begin());
		copy(flash.begin(), flash.end() - 1, flashfile[0].begin());
	}

	copy(cgpath[0].begin(), cgpath[0].end() - 1, cgunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, cgunblock[0].begin());

	copy(cgglpath[0].begin(), cgglpath[0].end() - 1, cgglunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, cgglunblock[0].begin());

	copy(cgd3d9path[0].begin(), cgd3d9path[0].end() - 1, cgd3d9unblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, cgd3d9unblock[0].begin());

	copy(airdir[0].begin(), airdir[0].end() - 1, airunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, airunblock[0].begin());

	copy(flashdir[0].begin(), flashdir[0].end() - 1, flashunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, flashunblock[0].begin());

	copy(tbb[0].begin(), tbb[0].end() - 1, tbbunblock[0].begin());
	copy(unblock.begin(), unblock.end() - 1, tbbunblock[0].begin());



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
		&cgbin[0].c_str(),
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