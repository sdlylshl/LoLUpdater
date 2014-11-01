// LoLUpdater.cpp : Defines the entry point for the console application.
//

// Include all the stuff that didn't make it into the main method
#include "LoLUpdater.h"

// Buffers
// holds the adobe air installation directory
wstringstream buff_c[MAX_PATH + 1];
// holds the path to where tbb.dll will be downloaded
wstringstream tbb0[MAX_PATH + 1];
// holds the path to the latest adobe air.dll
wstringstream airfile[MAX_PATH + 1];
// holds the path to where adobe air.dll will be copied to
wstringstream airdir[MAX_PATH + 1];
// holds the path to the latest "flash" dll.
wstringstream flashfile[MAX_PATH + 1];
// holds the path to where the flash dll will be copied to
wstringstream flashdir[MAX_PATH + 1];
// holds the path to where the updated cg.dll is.
wstringstream cgbin[MAX_PATH + 1];
// holds the path to the cginstaller that is downloaded together with the unblock tag
wstringstream cginstunblock[MAX_PATH + 1];
// holds the path to the adobe air installer that is downloaded together with the unblock tag
wstringstream airinstunblock[MAX_PATH + 1];
// holds the path together with the unblock tag to where the adobe air.dll is in the LoL installation.
wstringstream airunblock[MAX_PATH + 1];
// holds the path to where the updated cgd3d9.dll is.
wstringstream cgd3d9bin[MAX_PATH + 1];
// holds the path to where the updated cggl.dll is.
wstringstream cgglbin[MAX_PATH + 1];
// holds the path to the final destination of the updated cg.dll
wstringstream cgpath[MAX_PATH + 1];
// holds the path to the final destination of the updated cggl.dll
wstringstream cgglpath[MAX_PATH + 1];
// holds the path to the tbb dll together with the unblock tag
wstringstream tbbunblock[MAX_PATH + 1];
// holds the path to the "flash" dll together with the unblock tag
wstringstream flashunblock[MAX_PATH + 1];
// holds the path to the final destination of the updated cgd3d9.dll
wstringstream cgd3d9path[MAX_PATH + 1];
// holds the path to the final destination of the updated cggl.dll together with the unblock tag
wstringstream cgglunblock[MAX_PATH + 1];
// holds the path to the final destination of the updated cg.dll together with the unblock tag
wstringstream cgunblock[MAX_PATH + 1];
// holds the path to the final destination of the updated cgd3d9.dll together with the unblock tag
wstringstream cgd3d9unblock[MAX_PATH + 1];
// holds the full path to where all adobe files will be copied to.
wstringstream airpath_f[MAX_PATH + 1];
// holds the full path to where all game files will be copied to.
wstringstream slnpath_f[MAX_PATH + 1];
// holds the path to the current working directory (where the executable was ran from)
wstringstream cwd[MAX_PATH + 1];
// holds the environmental variable for CG_BIN_PATH (todo: use wstring instead of wchar_t)
vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);

// Constants
// unblock tag
const wstring unblock(L":Zone.Identifier");
// name of the adobe air dll
const wstring air(L"Adobe AIR.dll");
// relative path to the flash dll from where the adobe air dll is
const wstring flash(L"Resources\\NPSWF32.dll");
// cg dll name
const wstring cgfile(L"Cg.dll");
// cggl dll name
const wstring cgglfile(L"CgGL.dll");
// cgd3d9 dll name
const wstring cgd3d9file(L"CgD3D9.dll");
// name of the downloaded cg installer
const wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
// tbb dll name
const wstring tbbfile(L"tbb.dll");
// name of the downloaded adobe air installer
const wstring airwin(L"air15_win.exe");
// collection of all files that will be unblocked at the end
const wstring unblockfiles[] = { cgunblock[0].str(), cgglunblock[0].str(), cgd3d9unblock[0].str(), tbbunblock[0].str(), airunblock[0].str(), flashunblock[0].str() };

int _tmain(int argc, _TCHAR* argv[])
{
	// basic command line output
	wcout << L"LoLUpdater Alpha 1 Build 14";
	wcout << endl;
	wcout << L"Patching...";
	wcout << endl;

	// append backslash to the working-dir buffer.
	cwd[0] << cwd1().c_str();
	cwd[0] << L"\\";


		// string-builder for the cginstaller
		cginstunblock[0] << cwd[0].str().c_str();
		cginstunblock[0] << cginstaller;
		cginstunblock[0] << &unblock[0];

		// Downloads Nvidia-CG
		URLDownloadToFileW(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller.c_str(),
			0,
			nullptr
			);
		// Unblocks the installer
		DeleteFileW(cginstunblock[0].str().c_str());

		// Starts the executable with some arguments
		SHELLEXECUTEINFOW ShExecInfocg = { 0 };
		ShExecInfocg.cbSize = sizeof(SHELLEXECUTEINFOW);
		ShExecInfocg.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfocg.hwnd = nullptr;
		ShExecInfocg.lpVerb = nullptr;
		ShExecInfocg.lpFile = cginstaller.c_str();
		// silent and minimal install
		ShExecInfocg.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfocg.lpDirectory = nullptr;
		ShExecInfocg.nShow = SW_SHOW;
		ShExecInfocg.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfocg);
		// Wait for process to finish before continuing.
		WaitForSingleObject(ShExecInfocg.hProcess, INFINITE);

		// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
		GetEnvironmentVariableW(L"CG_BIN_PATH",
			&cgbinpath[0],
			MAX_PATH + 1);

	// appends a backslash to the path for later processing.
	wcsncat(&cgbinpath[0], L"\\", MAX_PATH + 1);

	// add drive letter to the variable
	buff_c[0] << cwd[0].str().c_str()[0];

	// basic stringbuilding
	// different paths depending if it is a 64 or 32bit system
#if defined(ENVIRONMENT64)
	buff_c[0] << L":\\Program Files (x86)";
#elif defined(ENVIRONMENT32)
	buff_c[0] << L":\\Program Files";
#endif
	buff_c[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	// paths to where files should be copied
	const wstring slnpath(L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
	const wstring airpath(L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

	// garena installations have lol.exe in the main directory
	if (file_exists(L"lol.exe"))
	{
		// Overload if it is Garena
		const wstring airpath(L"Air\\Adobe AIR\\Versions\\1.0\\");
		const wstring slnpath(L"Game\\");
	}

	// finalize variables for use in later stringbuilding
	slnpath_f[0] << cwd[0].str().c_str();
	slnpath_f[0] << &slnpath[0];

	airpath_f[0] << cwd[0].str().c_str();
	airpath_f[0] << &airpath[0];

	tbb0[0] << slnpath_f[0].str();
	tbb0[0] << tbbfile;

	// Gets location of latest cg dll
	cgbin[0] << &cgbinpath[0];
	cgbin[0] << cgfile;

	// Gets location of latest cggl dll
	cgglbin[0] << &cgbinpath[0];
	cgglbin[0] << cgglfile;

	// Gets location of latest cgd3d9 dll
	cgd3d9bin[0] << &cgbinpath[0];
	cgd3d9bin[0] << cgd3d9file;

	// Gets location of latest adobe air dll
	airfile[0] << buff_c[0].str().c_str();
	airfile[0] << air;

	// Gets location of latest "flash" dll
	flashfile[0] << buff_c[0].str().c_str();
	flashfile[0] << flash;

	// string-builder for adobe air installer
	airinstunblock[0] << cwd[0].str().c_str();
	airinstunblock[0] << airwin;
	airinstunblock[0] << &unblock[0];

	// Downloads adobe-air
	URLDownloadToFileW(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airwin.c_str(),
		0,
		nullptr
		);
	// Unblocks the adobe air installer
	DeleteFileW(airinstunblock[0].str().c_str());

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airwin.c_str();
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	// Waits for program to finish
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Finalize full copy paths
	cgpath[0] << slnpath_f[0].str();
	cgpath[0] << cgfile;

	cgglpath[0] << slnpath_f[0].str();
	cgglpath[0] << cgglfile;

	cgd3d9path[0] << slnpath_f[0].str();
	cgd3d9path[0] << cgd3d9file;

	airdir[0] << airpath_f[0].str();
	airdir[0] << air;

	flashdir[0] << airpath_f[0].str();
	flashdir[0] << flash;

	// Finalize paths with the unblock tag
	cgunblock[0] << cgpath[0].str();
	cgunblock[0] << &unblock[0];

	cgglunblock[0] << cgglpath[0].str();
	cgglunblock[0] << &unblock[0];

	cgd3d9unblock[0] << cgd3d9path[0].str();
	cgd3d9unblock[0] << &unblock[0];

	airunblock[0] << airdir[0].str();
	airunblock[0] << &unblock[0];

	flashunblock[0] << flashdir[0].str();
	flashunblock[0] << &unblock[0];

	tbbunblock[0] << tbb0[0].str();
	tbbunblock[0] << &unblock[0];

#if XP
	// XP tbb download
	URLDownloadToFileW(
		nullptr,
		L" http://lol.jdhpro.com/Xp.dll",
		tbb0[0].str().c_str(),
		0,
		nullptr
		);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2  tbb download
		URLDownloadToFileW(
			nullptr,
			L"http://lol.jdhpro.com/Avx2.dll",
			tbb0[0].str().c_str(),
			0,
			nullptr
			);
	}
#if (_MSC_FULL_VER >= 160040219)
	else
	{
		// If Visual Studio 2010 SP1 or later

		// Checking for AVX requires 3 things:
		// 1) CPUID indicates that the OS uses XSAVE and XRSTORE
		//     instructions (allowing saving YMM registers on context
		//     switch)
		// 2) CPUID indicates support for AVX
		// 3) XGETBV indicates the AVX registers will be saved and
		//     restored on context switch
		//
		// Note that XGETBV is only available on 686 or later CPUs, so
		// the instruction needs to be conditionally run.
		int cpuInfo[4];
		__cpuid(cpuInfo, 1);

		bool osUsesXSAVE_XRSTORE = cpuInfo[2] & (1 << 27) || false;
		bool cpuAVXSuport = cpuInfo[2] & (1 << 28) || false;



		if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
		{
			// Check if the OS will save the YMM registers (basically final check for AVX support)
			unsigned long long xcrFeatureMask = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
			avxSupported = (xcrFeatureMask & 0x6) || false;
		}

		// AVX  tbb download
		if (avxSupported)
		{
			URLDownloadToFileW(
				nullptr,
				L"http://lol.jdhpro.com/Avx.dll",
				tbb0[0].str().c_str(),
				0,
				nullptr
				);
		}
#endif
		else
		{
			//SSE2  tbb download
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				URLDownloadToFileW(
					nullptr,
					L"http://lol.jdhpro.com/Sse2.dll",
					tbb0[0].str().c_str(),
					0,
					nullptr
					);
			}
			else
			{
				//SSE  tbb download
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					URLDownloadToFileW(
						nullptr,
						L"http://lol.jdhpro.com/Sse.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
				//download tbb without any extra SMID instructions if SSE is not supported.
				else
				{
					URLDownloadToFileW(
						nullptr,
						L"http://lol.jdhpro.com/Default.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
			}
		}
	}

#endif

	// Copy all files
	CopyFileW(
		cgbin[0].str().c_str(),
		cgpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgglbin[0].str().c_str(),
		cgglpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgd3d9bin[0].str().c_str(),
		cgd3d9path[0].str().c_str(),
		false
		);
	CopyFileW(
		airfile[0].str().c_str(),
		airdir[0].str().c_str(),
		false
		);
	CopyFileW(
		flashfile[0].str().c_str(),
		flashdir[0].str().c_str(),
		false
		);
	// End copy all files

	// Unblock _all_ files just to be safe
	for each (wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}

	wcout << "LoLUpdater finished!";
	wcout << endl;
	wstring dummy;
	wcout << "Enter to continue..." << endl;
	getline(wcin, dummy);

	return 0;
}