// LoLUpdater.cpp : Defines the entry point for the console application.
//

// Include all the stuff that didn't make it into the main method
#include "LoLUpdater.h"
using namespace std;
// Buffer
// 0 = holds the adobe air installation directory
// 1 = holds the path to where tbb.dll will be downloaded
// 2 = holds the path to the latest adobe air.dll
// 3 = holds the path to where adobe air.dll will be copied to
// 4 = holds the path to the latest "flash" dll
// 5 = holds the path to where the flash dll will be copied to
// 6 = holds the path to where the updated cg.dll is.
// 7 = holds the path to the cginstaller that is downloaded together with the unblock tag
// 8 = holds the path to the adobe air installer that is downloaded together with the unblock tag
// 9 = holds the path together with the unblock tag to where the adobe air.dll is in the LoL installation.
// 10 = holds the path to where the updated cgd3d9.dll is.
// 11 = holds the path to where the updated cggl.dll is.
// 12 = holds the path to the final destination of the updated cg.dll
// 13 = holds the path to the final destination of the updated cggl.dll
// 14 = holds the path to the tbb dll together with the unblock tag
// 15 = holds the path to the "flash" dll together with the unblock tag
// 16 = holds the path to the final destination of the updated cgd3d9.dll
// 17 = holds the path to the final destination of the updated cggl.dll together with the unblock tag
// 18 = holds the path to the final destination of the updated cg.dll together with the unblock tag
// 19 = holds the path to the final destination of the updated cgd3d9.dll together with the unblock tag
// 20 = holds the full path to where all adobe files will be copied to.
// 21 = holds the full path to where all game files will be copied to.
// 22 = holds the path to the current working directory (where the executable was ran from)
wstringstream wbuffer[23];

// holds the environmental variable for CG_BIN_PATH (todo make into wstringstream)
vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);
// holds the full path  (incl file.ext) to the program (todo make into wstringstream)
vector<wchar_t> cwd0(MAX_PATH + 1, 0);

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
const wstring unblockfiles[] = { wbuffer[9].str(), wbuffer[14].str(), wbuffer[15].str(), wbuffer[17].str(), wbuffer[18].str(), wbuffer[19].str()};

int _tmain(int argc, _TCHAR* argv[])
{
	// basic command line output
	wcout << L"LoLUpdater Alpha 1 Build 16";
	wcout << endl;
	wcout << L"Patching...";
	wcout << endl;

	// get working directory
	GetModuleFileNameW(nullptr, &cwd0[0], MAX_PATH + 1);

	// append backslash to the working-dir buffer.
	wbuffer[22] << wstring(&cwd0[0]).substr(0, wstring(&cwd0[0]).find_last_of(L"\\/"));
	wbuffer[22] << L"\\";


		// string-builder for the cginstaller
	wbuffer[7] << wbuffer[22].str().c_str();
	wbuffer[7] << cginstaller;
	wbuffer[7] << unblock;

		// Downloads Nvidia-CG
		URLDownloadToFileW(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginstaller.c_str(),
			0,
			nullptr
			);
		// Unblocks the installer
		DeleteFileW(wbuffer[7].str().c_str());

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
		wbuffer[0] << wbuffer[22].str().c_str()[0];

	// basic stringbuilding
	// different paths depending if it is a 64 or 32bit system
#ifdef ENVIRONMENT64
		wbuffer[0] << L":\\Program Files (x86)";
#else
		wbuffer[0] << L":\\Program Files";
#endif
		wbuffer[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	// paths to where files should be copied
	const wstring slnpath(L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
	const wstring airpath(L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

	ifstream infile(L"lol.exe");
	// garena installations have lol.exe in the main directory
	if (infile.good())
	{
		// Overload if it is Garena
		const wstring airpath(L"Air\\Adobe AIR\\Versions\\1.0\\");
		const wstring slnpath(L"Game\\");
	}

	// finalize variables for use in later stringbuilding
	wbuffer[21] << wbuffer[22].str().c_str();
	wbuffer[21] << slnpath;

	wbuffer[20] << wbuffer[22].str().c_str();
	wbuffer[20] << airpath;

	wbuffer[1] << wbuffer[21].str();
	wbuffer[1] << tbbfile;

	// Gets location of latest cg dll
	wbuffer[6] << &cgbinpath[0];
	wbuffer[6] << cgfile;

	// Gets location of latest cggl dll
	wbuffer[11] << &cgbinpath[0];
	wbuffer[11] << cgglfile;

	// Gets location of latest cgd3d9 dll
	wbuffer[10] << &cgbinpath[0];
	wbuffer[10] << cgd3d9file;

	// Gets location of latest adobe air dll
	wbuffer[2] << wbuffer[0].str().c_str();
	wbuffer[2] << air;

	// Gets location of latest "flash" dll
	wbuffer[4] << wbuffer[0].str().c_str();
	wbuffer[4] << flash;

	// string-builder for adobe air installer
	wbuffer[8] << wbuffer[22].str().c_str();
	wbuffer[8] << airwin;
	wbuffer[8] << unblock;

	// Downloads adobe-air
	URLDownloadToFileW(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airwin.c_str(),
		0,
		nullptr
		);
	// Unblocks the adobe air installer
	DeleteFileW(wbuffer[8].str().c_str());

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
	wbuffer[12] << wbuffer[21].str();
	wbuffer[12] << cgfile;

	wbuffer[13] << wbuffer[21].str();
	wbuffer[13] << cgglfile;

	wbuffer[16] << wbuffer[21].str();
	wbuffer[16] << cgd3d9file;

	wbuffer[3] << wbuffer[20].str();
	wbuffer[3] << air;

	wbuffer[5] << wbuffer[20].str();
	wbuffer[5] << flash;

	// Finalize paths with the unblock tag
	wbuffer[18] << wbuffer[12].str();
	wbuffer[18] << unblock;

	wbuffer[17] << wbuffer[13].str();
	wbuffer[17] << unblock;

	wbuffer[19] << wbuffer[16].str();
	wbuffer[19] << unblock;

	wbuffer[9] << wbuffer[3].str();
	wbuffer[9] << unblock;

	wbuffer[15] << wbuffer[5].str();
	wbuffer[15] << unblock;

	wbuffer[14] << wbuffer[1].str();
	wbuffer[14] << unblock;

#if XP
	// XP tbb download
	URLDownloadToFileW(
		nullptr,
		L" http://lol.jdhpro.com/Xp.dll",
		wbuffer[1].str().c_str(),
		0,
		nullptr
		);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2  tbb download
		URLDownloadToFileW(
			nullptr,
			L"Avx2.dll",
			wbuffer[1].str().c_str(),
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
				wbuffer[1].str().c_str(),
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
					wbuffer[1].str().c_str(),
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
						wbuffer[1].str().c_str(),
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
						wbuffer[1].str().c_str(),
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
		wbuffer[6].str().c_str(),
		wbuffer[12].str().c_str(),
		false
		);
	CopyFileW(
		wbuffer[11].str().c_str(),
		wbuffer[13].str().c_str(),
		false
		);
	CopyFileW(
		wbuffer[10].str().c_str(),
		wbuffer[16].str().c_str(),
		false
		);
	CopyFileW(
		wbuffer[2].str().c_str(),
		wbuffer[3].str().c_str(),
		false
		);
	CopyFileW(
		wbuffer[4].str().c_str(),
		wbuffer[5].str().c_str(),
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