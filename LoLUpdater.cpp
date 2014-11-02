// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include "LoLUpdater.h"

using namespace std;
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
stringstream pathkeeper[23];

// convert string to "Windows unicode" string
const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

// Function to reduce amount of lines
void Copy(int x, int y)
{
	CopyFileW(
		GetWC(pathkeeper[x].str().c_str()),
		GetWC(pathkeeper[y].str().c_str()),
		false
		);
}


// holds the environmental variable for CG_BIN_PATH (todo make into stringstream)
vector<char> cgbinpath(MAX_PATH + 1, 0);
// holds the full path  (incl file.ext) to the program (todo make into stringstream)
vector<char> cwd0(MAX_PATH + 1, 0);

// Unblock tag
const string unblock(":Zone.Identifier");
// Full name of the adobe air dll
const string air("Adobe AIR.dll");
// relative path to the flash dll from where the adobe air dll is
const string flash("Resources\\NPSWF32.dll");
//  Full cg dll name
const string cgfile("Cg.dll");
//  Full cggl dll name
const string cgglfile("CgGL.dll");
//  Full cgd3d9 dll name
const string cgd3d9file("CgD3D9.dll");
//  Full name of the downloaded cg installer
const string cginstaller("Cg-3.1_April2012_Setup.exe");
//  Full tbb dll name
const string tbbfile("tbb.dll");
//  Full name of the downloaded adobe air installer
const string airwin("air15_win.exe");
// collection of all files that will be unblocked at the end
const string unblockfiles[] = { pathkeeper[9].str(), pathkeeper[14].str(), pathkeeper[15].str(), pathkeeper[17].str(), pathkeeper[18].str(), pathkeeper[19].str() };
// Garena executable
const string garena("lol.exe");


// Game version test
string aair()
{
	if (ifstream(garena).good())
	{
		return "Air\\Adobe AIR\\Versions\\1.0\\";
	}
	return "RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\";
}

// Game version test
string game()
{
	if (ifstream(garena).good())
	{
		return "Game\\";
	}
	return "RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\";
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Version + progress indication
	cout << "LoLUpdater Alpha 1 Build 18";
	cout << endl;
	cout << "Patching...";
	cout << endl;


	// gets working directory with app.ext
	GetModuleFileNameW(nullptr, const_cast<wchar_t *>(GetWC(&cwd0[0])), MAX_PATH + 1);

	// remove app.ext and append backslash to the working-dir buffer.
	pathkeeper[22] << (string(&cwd0[0]).substr(0, string(&cwd0[0]).find_last_of("\\/")) + "\\");


	// string-builder for the cginstaller
	pathkeeper[7] << (pathkeeper[22].str().c_str() + cginstaller + unblock);
	// Downloads Nvidia-CG
	URLDownloadToFileW(
		nullptr,
		GetWC("http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"),
		GetWC(cginstaller.c_str()),
		0,
		nullptr
		);

	// Unblocks the cg-installer
	DeleteFileW(GetWC(pathkeeper[7].str().c_str()));

	// Starts the cg-executable
	SHELLEXECUTEINFOW ShExecInfocg = { 0 };
	ShExecInfocg.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfocg.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfocg.hwnd = nullptr;
	ShExecInfocg.lpVerb = nullptr;
	ShExecInfocg.lpFile = GetWC(cginstaller.c_str());
	// silent and minimal install
	ShExecInfocg.lpParameters = GetWC("/verysilent /TYPE=compact");
	ShExecInfocg.lpDirectory = nullptr;
	ShExecInfocg.nShow = SW_SHOW;
	ShExecInfocg.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfocg);
	// Wait for process to finish before continuing.
	WaitForSingleObject(ShExecInfocg.hProcess, INFINITE);

	// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
	GetEnvironmentVariableW(GetWC("CG_BIN_PATH"),
		const_cast<wchar_t *>(GetWC(&cgbinpath[0])),
		MAX_PATH + 1);

	// appends a backslash to the path for later processing.
	strncat(&cgbinpath[0], "\\", MAX_PATH + 1);

	// add drive letter to the variable
	pathkeeper[0] << pathkeeper[22].str().c_str()[0];

	// different paths depending if it is a 64 or 32bit system
#ifdef ENVIRONMENT64
	pathkeeper[0] << ":\\Program Files (x86)";
#else
	pathkeeper[0] << ":\\Program Files";
#endif
	// finish with the default install directory from %Programfiles%
	pathkeeper[0] << "\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	// Gets location of latest cg dll
	pathkeeper[6] << (&cgbinpath[0] + cgfile);

	// Gets location of latest cggl dll
	pathkeeper[11] << (&cgbinpath[0] + cgglfile);

	// Gets location of latest cgd3d9 dll
	pathkeeper[10] << (&cgbinpath[0] + cgd3d9file);


	// string-builder for adobe air installer
	pathkeeper[8] << (pathkeeper[22].str().c_str() + airwin + unblock);

	// Downloads adobe-air
	URLDownloadToFileW(
		nullptr,
		GetWC("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"),
		GetWC(airwin.c_str()),
		0,
		nullptr
		);
	// Unblocks the adobe air installer
	DeleteFileW(GetWC(pathkeeper[8].str().c_str()));

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = GetWC(airwin.c_str());
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	// Waits for air-installer to finish
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Todo: use vectors and foreach here.
	// string building
	pathkeeper[21] << (pathkeeper[22].str().c_str() + game());
	pathkeeper[20] << (pathkeeper[22].str().c_str() + aair());
	pathkeeper[1] << (pathkeeper[21].str().c_str() + tbbfile);
	pathkeeper[2] << (pathkeeper[0].str().c_str() + air);
	pathkeeper[4] << (pathkeeper[0].str().c_str() + flash);
	pathkeeper[12] << (pathkeeper[21].str().c_str() + cgfile);
	pathkeeper[13] << (pathkeeper[21].str().c_str() + cgglfile);
	pathkeeper[16] << (pathkeeper[21].str().c_str() + cgd3d9file);
	pathkeeper[3] << (pathkeeper[20].str().c_str() + air);
	pathkeeper[5] << (pathkeeper[20].str().c_str() + flash);
	pathkeeper[18] << (pathkeeper[12].str().c_str() + unblock);
	pathkeeper[17] << (pathkeeper[13].str().c_str() + unblock);
	pathkeeper[19] << (pathkeeper[16].str().c_str() + unblock);
	pathkeeper[9] << (pathkeeper[3].str().c_str() + unblock);
	pathkeeper[15] << (pathkeeper[5].str().c_str() + unblock);
	pathkeeper[14] << (pathkeeper[1].str().c_str() + unblock);

#ifdef XP
	// XP tbb download
	URLDownloadToFileW(
		nullptr,
		GetWC(" http://lol.jdhpro.com/Xp.dll"),
		GetWC(pathkeeper[1].str().c_str()),
		0,
		nullptr
		);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2  tbb download
		URLDownloadToFileW(
			nullptr,
			GetWC("Avx2.dll"),
			GetWC(pathkeeper[1].str().c_str()),
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
				GetWC("http://lol.jdhpro.com/Avx.dll"),
				GetWC(pathkeeper[1].str().c_str()),
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
					GetWC("http://lol.jdhpro.com/Sse2.dll"),
					GetWC(pathkeeper[1].str().c_str()),
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
						GetWC("http://lol.jdhpro.com/Sse.dll"),
						GetWC(pathkeeper[1].str().c_str()),
						0,
						nullptr
						);
				}
				//download tbb without any extra SMID instructions if SSE is not supported.
				else
				{
					URLDownloadToFileW(
						nullptr,
						GetWC("http://lol.jdhpro.com/Default.dll"),
						GetWC(pathkeeper[1].str().c_str()),
						0,
						nullptr
						);
				}
			}
		}
	}

#endif
	// Todo: use vectors and a foreach here
	// Copy all files
	Copy(6, 12);
	Copy(11, 13);
	Copy(10, 16);
	Copy(2, 3);
	Copy(4, 5);
	// End copy all files

	// Unblock all patched files
	for (string i : unblockfiles)
	{
		DeleteFileW(GetWC(&i[0]));
	}

	// progress-indicator end
	cout << "LoLUpdater finished!";
	cout << endl;
	string dummy;
	cout << "Enter to continue..." << endl;
	getline(cin, dummy);

	// exit program
	return 0;
}