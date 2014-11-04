// A custom C++ installer (For Windows XP and up) for copying files to the League of Legends directory (Standard LoL, PBE or Garena) which replaces outdated DLL files with "newer" (and/or modified) DLL files in an attempt to increase FPS and the Pvp.net client (pre-game client) responsiveness.
// Custom files are downloaded from an FTP of choice.
// Supports detection of various SMID-instructions (AVX2, AVX, SSE2 and SSE)
// Removes the "Zone.Identifier" tag that comes with files downloaded from the internet through a Windows OS to prevent corruption and intended functionality (Normally refered as "Unblocking" files).

// Website: http://lolupdater.com
// Source-code (Github): https://github.com/Loggan08/LoLUpdater
// IRC: #LoLUpdater @ Quakenet , IRCWebchat: http://webchat.quakenet.org/?channels=LoLUpdater

// Copyright 2014: Elias Sollander
// Contributor(s):
// David Knaack

// Special thanks:
// ##C++ @ Freenode http://webchat.freenode.net/?channels=##C++
// #C++ @ Quakenet http://webchat.quakenet.org/?channels=#C++

// Other credits are mentioned in the code

// LoLUpdater.cpp : Defines the entry point for the console application.
//

#ifdef _WINDOWS
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif
#endif
// Contains AVX2 check from intel described below, some defines as well as the includes for this project.
#include "LoLUpdater.h"
#include <tchar.h>
#include "ShlObj.h"
#include <direct.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
// used to get the working directory without the app.exe extension
EXTERN_C IMAGE_DOS_HEADER __ImageBase;


typedef std::basic_string<TCHAR> tstring;


// Just for reference (Todo: make the "magic numbers" less magical (for now))
// 0 = adobe air installation directory
// 1 = path to where tbb.dll will be downloaded
// 2 = path to the latest adobe air.dll
// 3 = path to where adobe air.dll will be copied to
// 4 = path to the latest "flash" dll
// 5 = path to where the flash dll will be copied to
// 6 = path to where the updated cg.dll is.
// 7 = path to the cginstaller that is downloaded together with the unblock tag
// 8 = path to the adobe air installer that is downloaded together with the unblock tag
// 9 = path together with the unblock tag to where the adobe air.dll is in the LoL installation.
// 10 = path to where the updated cgd3d9.dll is.
// 11 = path to where the updated cggl.dll is.
// 12 = path to the final destination of the updated cg.dll
// 13 = path to the final destination of the updated cggl.dll
// 14 = path to the tbb dll together with the unblock tag
// 15 = path to the "flash" dll together with the unblock tag
// 16 = path to the final destination of the updated cgd3d9.dll
// 17 = full path to where all adobe files will be copied to.
// 18 = full path to where all game files will be copied to.
// 19 = path to the current working directory (where the executable was ran from)
std::wstringstream pathcontainer[20];

// function to reduce amount of lines in source-code, improves readability (questionable)
void Copy(int from, int to)
{
	CopyFile(
		pathcontainer[from].str().c_str(),
		pathcontainer[to].str().c_str(),
		false
		);
}

// function to reduce length of lines, improves readability (questionable)
void charreduction(int dest, int path1, const tstring path2)
{
	pathcontainer[dest] << (pathcontainer[path1].str().c_str() + path2);
}

// environmental variable for CG_BIN_PATH (todo: make into wstringstream)
std::vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);
// full path  (incl file.ext) to the program (todo: make into wstringstream)
std::vector<wchar_t> cwd0(MAX_PATH + 1, 0);

// Unblock tag
tstring unblock(_T(":Zone.Identifier"));
// Full name of the adobe air dll
tstring air(_T("Adobe AIR.dl"));
// relative path to the flash dll from where the adobe air dll is
tstring flash(_T("Resources\\NPSWF32.dl"));
//  Full cg dll name
tstring cgfile(_T("Cg.dl"));
//  Full cggl dll name
tstring cgglfile(_T("CgGL.dl"));
//  Full cgd3d9 dll name
tstring cgd3d9file(_T("CgD3D9.dl"));
//  Full name of the downloaded cg installer
tstring cginstaller(_T("Cg-3.1_April2012_Setup.exe"));
//  Full tbb dll name
tstring tbbfile(_T("tbb.dl"));
//  Full name of the downloaded adobe air installer
tstring airwin(_T("air15_win.exe"));
// garena stream
bool garena = std::wofstream(_T("lol.exe")).good();

// Game version test
// Todo: Automatically get "version" (x.x.x.x) folder as a wstring
// returns installation path depending on game version (Regular or Garena)
tstring aair()
{
	if (garena)
	{
		return _T("Air\\Adobe AIR\\Versions\\1.0\\");
	}
	return _T("RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");
}

// Game version test
// Todo: Automatically get "version" (x.x.x.x) folder as a wstring
// returns installation path depending on game version (Regular or Garena)
tstring game()
{
	if (garena)
	{
		return _T("Game\\");
	}
	return _T("RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
}

// Todo: Make files download simultaneously to decrease "patching" time (does my logic make sence?)
void download(tstring fromurl, tstring topath, int pathcont, int frompathcont, tstring args)
{
	// Downloads file
	URLDownloadToFile(
		nullptr,
		_T(fromurl.c_str()),
		_T(topath.c_str()),
		0,
		nullptr
		);

	// Unblocks the installer
	pathcontainer[pathcont] << (pathcontainer[frompathcont].str() + topath + unblock);
	DeleteFileW(pathcontainer[pathcont].str().c_str());

	// Starts the executable
	SHELLEXECUTEINFO ShExecInfocg = { 0 };
	ShExecInfocg.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfocg.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfocg.hwnd = nullptr;
	ShExecInfocg.lpVerb = nullptr;
	ShExecInfocg.lpFile = topath.c_str();

	// arguments
	ShExecInfocg.lpParameters = args.c_str();
	ShExecInfocg.lpDirectory = nullptr;
	ShExecInfocg.nShow = SW_SHOW;
	ShExecInfocg.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfocg);
	// Wait for process to finish before continuing.
	WaitForSingleObject(ShExecInfocg.hProcess, INFINITE);
}

// Download the intel threading building blocks dll (as a function due to multiple statement checks)
void tbbdownload(const tstring url)
{
	URLDownloadToFile(
		nullptr,
		url.c_str(),
		pathcontainer[1].str().c_str(),
		0,
		nullptr
		);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Version + progress indication
	std::wcout << "LoLUpdater Alpha 1 Build 21";
	std::wcout << std::endl;
	std::wcout << "Patching...";
	std::wcout << std::endl;

	// gets working directory with app.ext
	GetModuleFileName(nullptr, &cwd0[0], MAX_PATH + 1);

	// remove app.ext and append backslash to the working-dir buffer.
	pathcontainer[19] << (tstring(&cwd0[0]).substr(0, tstring(&cwd0[0]).find_last_of(_T(\\/")) + _T(\\");

	download(_T(http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", cginstaller.c_str(), 7, 19, _T(/verysilent /TYPE=compact");

	// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
	GetEnvironmentVariable(_T(CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH + 1);

	// appends a backslash to the path for later processing.
	wcsncat(&cgbinpath[0], _T(\\", MAX_PATH + 1);

	// add drive letter to the variable
	pathcontainer[0] << pathcontainer[19].str().c_str()[0];

	// different paths depending if it is a 64 or 32bit system
#ifdef ENVIRONMENT64
	pathcontainer[0] << _T(":\\Program Files (x86)");
#else
	pathcontainer[0] << _T(:\\Program Files";
#endif

	download(_T(https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", airwin.c_str(), 8, 19, _T(-silent");

	// Todo: use vectors and foreach here to compress it some more.
	// wstring building
	// finish with the default install directory from %Programfiles%
	pathcontainer[0] << _T("\\Common Files\\Adobe AIR\\Versions\\1.0\\");

	pathcontainer[6] << (&cgbinpath[0] + cgfile);
	pathcontainer[11] << (&cgbinpath[0] + cgglfile);
	pathcontainer[10] << (&cgbinpath[0] + cgd3d9file);

	// *Not a good way to do this
	charreduction(18, 19, game());
	charreduction(17, 19, aair());
	charreduction(1, 18, tbbfile);
	charreduction(2, 0, air);
	charreduction(4, 0, flash);
	charreduction(12, 18, cgfile);
	charreduction(13, 18, cgglfile);
	charreduction(16, 18, cgd3d9file);
	charreduction(3, 17, air);
	charreduction(5, 17, flash);
	charreduction(9, 3, unblock);
	charreduction(15, 5, unblock);
	charreduction(14, 1, unblock);

	// Each variant of tbb is built with support for certain SMID instructions (or none)
#ifdef XP
	// Is built without any support for any SMID instructions.
	tbbdownload(_T(http://lol.jdhpro.com/Xp.dl");
#else
	// Test for AVX2 (code in header file taken from: https://software.intel.com/en-us/articles/how-to-detect-new-instruction-support-in-the-4th-generation-intel-core-processor-family)
	if (can_use_intel_core_4th_gen_features())
	{
		tbbdownload(_T(http://lol.jdhpro.com/Avx2.dl");
	}
#if (_MSC_FULL_VER >= 160040219)
	else
	{
		int cpuInfo[4];
		__cpuid(cpuInfo, 1);

		// Test for AVX (condensed function from: http://insufficientlycomplicated.wordpress.com/2011/11/07/detecting-intel-advanced-vector-extensions-avx-in-visual-studio/)

		if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
		{
			tbbdownload(_T(http://lol.jdhpro.com/Avx.dl");
		}
#endif
		else
		{
			//SSE2  tbb download
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				tbbdownload(_T(http://lol.jdhpro.com/Sse2.dl");
			}
			else
			{
				//SSE  tbb download
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(_T(http://lol.jdhpro.com/Sse.dl");
				}
				//download tbb without any extra SMID instructions if SSE is not supported.
				else
				{
					tbbdownload(_T(http://lol.jdhpro.com/Default.dl");
				}
			}
		}
	}
	// Unblocks the downloaded tbb file.
	DeleteFileW(pathcontainer[14].str().c_str());

#endif
	// Todo: use vectors and a for (c++11 loop) here
	// Copy all files
	Copy(6, 12);
	Copy(11, 13);
	Copy(10, 16);
	Copy(2, 3);
	Copy(4, 5);

	// progress-indicator end
	std::cout << "LoLUpdater finished!";
	std::cout << std::endl;
	std::string dummy;
	std::cout << "Enter to continue...";
	std::cout << std::endl;
	getline(std::cin, dummy);

	// exit program
	return 0;
}