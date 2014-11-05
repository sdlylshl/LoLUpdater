#ifdef _WIN64
#define ENVIRONMENT64
#define _WINDOWS
#elif defined _WIN32
#define ENVIRONMENT32
#define _WINDOWS
#elif defined __APPLE__
#include <TargetConditionals.h>
#elif defined TARGET_OS_OSX
#define _MAC
#elif defined __LP64__
#define ENVIRONMENT64
#elif defined __i386__
#define ENVIRONMENT32
#endif

// Use windows-unicode just to skip the W defines on the winapi functions.
#ifdef _WINDOWS
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

// used to get the working directory without the app.exe extension
#include <ShlObj.h>
#include "LoLUpdater.h"
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

#include "LoLUpdater.h"
#include <tchar.h>
#include <direct.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

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
void charreduction(int dest, int path1, const std::wstring path2)
{
    pathcontainer[dest] << (pathcontainer[path1].str().c_str() + path2);
}

// environmental variable for CG_BIN_PATH (todo: make into std::wstringstream)
std::vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);
// full path  (incl file.ext) to the program (todo: make into std::wstringstream)
std::vector<wchar_t> cwd0(MAX_PATH + 1, 0);

// Unblock tag
const std::wstring unblock(L":Zone.Identifier");
// Full name of the adobe air dll
const std::wstring air(L"Adobe AIR.dll");
// relative path to the flash dll from where the adobe air dll is
const std::wstring flash(L"Resources\\NPSWF32.dll");
//  Full cg dll name
const std::wstring cgfile(L"Cg.dll");
//  Full cggl dll name
const std::wstring cgglfile(L"CgGL.dll");
//  Full cgd3d9 dll name
const std::wstring cgd3d9file(L"CgD3D9.dll");
//  Full name of the downloaded cg installer
const std::wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
//  Full tbb dll name
const std::wstring tbbfile(L"tbb.dll");
//  Full name of the downloaded adobe air installer
const std::wstring airwin(L"air15_win.exe");
// garena stream
bool garena = std::wifstream(L"lol.exe").good();

// Game version test
// Todo: Automatically get "version" (x.x.x.x) folder as a std::wstring
// returns installation path depending on game version (Regular or Garena)
std::wstring aair()
{
    if (garena)
    {
        return L"Air\\Adobe AIR\\Versions\\1.0\\";
}
return L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\";
}

// Game version test
// Todo: Automatically get "version" (x.x.x.x) folder as a std::wstring
// returns installation path depending on game version (Regular or Garena)
std::wstring game()
{
    if (garena)
    {
        return L"Game\\";
}
return L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\";
}

// Todo: Make files download simultaneously to decrease "patching" time (does my logic make sence?)
void download(const std::wstring fromurl, const std::wstring topath, int pathcont, int frompathcont, const std::wstring args)
{
    // Downloads file
    URLDownloadToFile(
        nullptr,
        fromurl.c_str(),
    topath.c_str(),
    0,
    nullptr
    );

    // Unblocks the installer
    pathcontainer[pathcont] << (pathcontainer[frompathcont].str() + topath + &unblock[0]);
    DeleteFile(pathcontainer[pathcont].str().c_str());

    // Starts the executable
    SHELLEXECUTEINFO ShExecInfo = { 0 };
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
ShExecInfo.hwnd = nullptr;
ShExecInfo.lpVerb = nullptr;
ShExecInfo.lpFile = topath.c_str();

// arguments
ShExecInfo.lpParameters = args.c_str();
ShExecInfo.lpDirectory = nullptr;
ShExecInfo.nShow = SW_SHOW;
ShExecInfo.hInstApp = nullptr;
ShellExecuteEx(&ShExecInfo);
// Wait for process to finish before continuing.
WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

// Download the intel threading building blocks dll (as a function due to multiple statement checks)
void tbbdownload(const std::wstring url)
{
    URLDownloadToFile(
        nullptr,
        url.c_str(),
    pathcontainer[1].str().c_str(),
    0,
    nullptr
    );
}

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"LoLUpdater",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		500,    // width of the window
		400,    // height of the window
		nullptr,    // we have no parent window, NULL
		nullptr,    // we aren't using menus, NULL
		hInstance,    // application handle
		nullptr);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}

	// gets working directory with app.ext
	GetModuleFileName(nullptr, &cwd0[0], MAX_PATH + 1);

	// remove app.ext and append backslash to the working-dir buffer.
	pathcontainer[19] << (std::wstring(&cwd0[0]).substr(0, std::wstring(&cwd0[0]).find_last_of(L"\\/")) + L"\\");

	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", cginstaller.c_str(), 7, 19, L"/verysilent /TYPE = compact");

	// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
	GetEnvironmentVariable(L"CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH + 1);

	// appends a backslash to the path for later processing.
	wcsncat_s(
		&cgbinpath[0],
		MAX_PATH + 1,
		L"\\",
		_TRUNCATE
		);

	// add drive letter to the variable
	pathcontainer[0] << pathcontainer[19].str().c_str()[0];

	// different paths depending if it is a 64 or 32bit system
#ifdef ENVIRONMENT64
	pathcontainer[0] << ":\\Program Files (x86)";
#else
	pathcontainer[0] << ":\\Program Files";
#endif

	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", airwin.c_str(), 8, 19, L"-silent");

	// Todo: use vectors and foreach here to compress it some more.
	// std::wstring building
	// finish with the default install directory from %Programfiles%
	pathcontainer[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

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
#ifdef _XP
	// Is built without any support for any SMID instructions.
	tbbdownload(L"http://lol.jdhpro.com/Xp.dll");
#else
	// Test for AVX2 (code in header file taken from: https://software.intel.com/en-us/articles/how-to-detect-new-instruction-support-in-the-4th-generation-intel-core-processor-family)
	if (can_use_intel_core_4th_gen_features())
	{
		tbbdownload(L"http://lol.jdhpro.com/Avx2.dll");
	}
	else
	{
		int cpuInfo[4];
		__cpuid(cpuInfo, 1);

		// Test for AVX (condensed function from: http://insufficientlycomplicated.wordpress.com/2011/11/07/detecting-intel-advanced-vector-extensions-avx-in-visual-studio/)

		if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
		{
			tbbdownload(L"http://lol.jdhpro.com/Avx.dll");
		}
		else
		{
			//SSE2  tbb download
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				tbbdownload(L"http://lol.jdhpro.com/Sse2.dll");
			}
			else
			{
				//SSE  tbb download
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(L"http://lol.jdhpro.com/Sse.dll");
				}
				//download tbb without any extra SMID instructions if SSE is not supported.
				else
				{
					tbbdownload(L"http://lol.jdhpro.com/Default.dll");
				}
			}
		}
	}
	// Unblocks the downloaded tbb file.
	DeleteFile(pathcontainer[14].str().c_str());

#endif
	// Todo: use vectors and a for (c++11 loop) here
	// Copy all files
	Copy(6, 12);
	Copy(11, 13);
	Copy(10, 16);
	Copy(2, 3);
	Copy(4, 5);


	// return this part of the WM_QUIT message to Windows
	return msg.wParam;


	// Currently you will have to restart the app to patch again.
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}