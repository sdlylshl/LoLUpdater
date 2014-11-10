#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>
#include <Shlobj.h>
#include <wininet.h>

bool done = false;
// used to keep filepaths together with the unblock tag (constants[0])
// 0 = download buffer (used in the download function to store nvidiacg/adobeair installer, cleared after each use)
// 1 = adobe air.dll destination (incl working directory)
// 2 = npswf32.dll destination (incl working directory)
// 3 = tbb.dll destination (incl working directory)
std::wstringstream pathcontainer[4];

const std::wstring constants[3] = { std::wstring(L":Zone.Identifier"), std::wstring(L"Adobe AIR\\Versions\\1.0"), std::wstring(L"AIR\\") };
wchar_t* tbb;

wchar_t cwd[MAX_PATH];
wchar_t drive = *cwd;

std::vector<std::wstring> cgbinpath(MAX_PATH, std::wstring());
wchar_t progdrive[MAX_PATH];
wchar_t airclient1[MAX_PATH] = L"";
wchar_t* airclient;
wchar_t gameclient1[MAX_PATH] = L"";
wchar_t* gameclient;



HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

void download(std::wstring url, std::wstring file, std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		url.c_str(),
		file.c_str(),
		0,
		nullptr
		);

	wchar_t unblocker1[MAX_PATH] = L"";
	wchar_t* unblocker;
	wchar_t unblocker2[] = L"";
	wchar_t* unblockerq;
	wchar_t unblocker3[] = L"";
	wchar_t* unblockerqq;
	unblocker = unblocker1;

	wcsncat_s(
		unblocker2,
		MAX_PATH,
		&drive,
		_TRUNCATE
		);

	unblockerq = unblocker2;

	wcsncat_s(
		unblocker3,
		MAX_PATH,
		file.c_str(),
		_TRUNCATE
		);

	unblockerqq = unblocker3;

	PathCombine(
		unblocker,
		unblockerq,
		unblockerqq
		);

	pathcontainer[0] << std::wstring(unblocker1 + constants[0]);

	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring());
	pathcontainer[0].clear();

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = L"runas";
	ShExecInfo.lpFile = file.c_str();
	ShExecInfo.lpParameters = args.c_str();
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void tbbdownload(const std::wstring& file)
{
	wchar_t finalurl[INTERNET_MAX_URL_LENGTH];
	DWORD dwLength = sizeof(finalurl);
	UrlCombine(
		L"http://lol.jdhpro.com/",
		file.c_str(),
		finalurl,
		&dwLength,
		0
		);

	URLDownloadToFile(
		nullptr,
		finalurl,
		tbb,
		0,
		nullptr
		);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, L"Patching..", -1, &start, DT_CENTER);
		if (done == true)
		{
			DrawText(hdc, L"Done!", -1, &end, DT_CENTER);
			EndPaint(hwnd, &ps);
		}
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void initbasepaths()
{
	wcsncat_s(
		airclient1,
		MAX_PATH,
		&cwd[0],
		_TRUNCATE
		);
	airclient = airclient1;

	wcsncat_s(
		gameclient1,
		MAX_PATH,
		&cwd[0],
		_TRUNCATE
		);
	gameclient = gameclient1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#ifdef DEBUG
	SetConsoleOutputCP(CP_UTF8);
	AllocConsole();
	_wfreopen(L"CONOUT$", L"w", stdout);
#endif

	MSG Msg;
	WNDCLASSEXW wc;

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName.c_str(),
		L"LoLUpdater",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 240,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, nCmdShow);
	_wgetcwd(cwd, MAX_PATH);

	SHGetFolderPath(nullptr,
		CSIDL_PROGRAM_FILES_COMMON,
		nullptr,
		0,
		progdrive);

	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH);
	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH,
		L"\\",
		_TRUNCATE
		);

	initbasepaths();

	wchar_t* rel = L"releases";
	wchar_t* rads = L"RADS";

	wchar_t lolgame1[] = L"lol_game_client_sln";
	wchar_t* lolgame;
	wchar_t adobepath1[MAX_PATH] = L"";
	wchar_t* adobepath;
	wchar_t asd[MAX_PATH] = L"";

	wchar_t lolair1[] = L"lol_air_client";
	wchar_t* lolair;
	wchar_t cgbin1[MAX_PATH] = L"";
	wchar_t cgbasepath1[MAX_PATH] = L"";
	wchar_t* cgbasepath;
	wchar_t* cgbin;
	wchar_t cgglbin1[MAX_PATH] = L"";
	wchar_t* cgglbin;
	wchar_t* cg = L"Cg.dll";
	wchar_t* cggl = L"CgGL.dll";
	wchar_t* cgd3d9 = L"CgD3D9.dll";
	wchar_t cgd3d9bin1[MAX_PATH] = L"";
	wchar_t* cgd3d9bin;
	wchar_t* gameversion = L"0.0.1.64";
	wchar_t* dep = L"deploy";
	wchar_t* airversion = L"0.0.1.117";
	wchar_t tbb1[MAX_PATH] = L"";
	wchar_t* tbbfile = L"tbb.dll";
	wchar_t airdest1[MAX_PATH] = L"";
	wchar_t* airdest;
	wchar_t airlatest[MAX_PATH] = L"";
	wchar_t* airlatest1;
	wchar_t buffer_11[MAX_PATH] = L"Resources";
	wchar_t* flash;
	wchar_t* air = L"Adobe AIR.dll";
	wchar_t buffer_11a[MAX_PATH] = L"NPSWF32.dll";
	wchar_t* flasha;
	wchar_t flashdest1[MAX_PATH] = L"";
	wchar_t* flashdest;
	wchar_t flashlatest1[MAX_PATH] = L"";
	wchar_t* flashlatest;
	wchar_t cgdest1[MAX_PATH] = L"";
	wchar_t* cgdest;
	wchar_t cggldest1[MAX_PATH] = L"";
	wchar_t* cggldest;
	wchar_t cgd3d9dest1[MAX_PATH] = L"";
	wchar_t* cgd3d9dest;

	wchar_t* commonfiles;
	wchar_t adobedir1[MAX_PATH] = L"";
	wchar_t* adobedir;
	wchar_t commonfiles1[MAX_PATH] = L"";
	wchar_t* asd212;


	wchar_t proj1[MAX_PATH] = L"projects";
	wchar_t* proj;
	proj = proj1;

	lolair = lolair1;
	PathAppend(airclient, rads);
	PathAppend(airclient, proj);
	PathAppend(airclient, lolair);
	PathAppend(airclient, rel);

	wchar_t sol1[MAX_PATH] = L"solutions";
	wchar_t* sol;
	sol = sol1;

	lolgame = lolgame1;
	PathAppend(gameclient, rads);
	PathAppend(gameclient, sol);
	PathAppend(gameclient, lolgame);
	PathAppend(gameclient, rel);
	adobepath = adobepath1;
	wcsncat_s(
		commonfiles1,
		MAX_PATH,
		progdrive,
		_TRUNCATE
		);


	commonfiles = commonfiles1;

	adobedir = adobedir1;

	wcsncat_s(
		adobedir,
		MAX_PATH,
		constants[1].c_str(),
		_TRUNCATE
		);

	PathCombine(
		adobepath,
		commonfiles,
		adobedir
		);

	cgbin = cgbin1;
	wcsncat_s(
		cgbasepath1,
		MAX_PATH,
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		_TRUNCATE
		);

	cgbasepath = cgbasepath1;

	PathCombine(
		cgbin,
		cgbasepath,
		cg
		);

	cgglbin = cgglbin1;

	PathCombine(
		cgglbin,
		cgbasepath,
		cggl
		);

	cgd3d9bin = cgd3d9bin1;

	PathCombine(
		cgd3d9bin,
		cgbasepath,
		cgd3d9
		);

	PathAppend(gameclient, gameversion);
	PathAppend(gameclient, dep);
	PathAppend(airclient, airversion);
	PathAppend(airclient, dep);
	PathAppend(airclient, adobedir);

	std::wifstream garena(L"lol.exe");

	if (garena.good())
	{
		airclient = L"";

		gameclient = L"";

		wchar_t *garenagame = L"Game";

		initbasepaths();

		PathAppend(gameclient, garenagame);

		wcsncat_s(
			asd22,
			MAX_PATH,
			constants[2].c_str(),
			_TRUNCATE
			);

		asd212 = asd22;

		PathAppend(airclient, asd212);
	}

	tbb = tbb1;

	PathCombine(tbb, gameclient, tbbfile);

	airdest = airdest1;
	PathCombine(
		airdest,
		airclient,
		air
		);

	airlatest1 = airlatest;
	PathCombine(
		airlatest,
		adobepath,
		air
		);

	flash = buffer_11;
	flasha = buffer_11a;
	PathAppend(flash, flasha);

	flashdest = flashdest1;
	PathCombine(
		flashdest,
		airclient,
		flash
		);

	flashlatest = flashlatest1;
	PathCombine(
		flashlatest,
		adobepath,
		flash
		);

	cgdest = cgdest1;
	PathCombine(
		cgdest,
		gameclient,
		cg
		);

	cggldest = cggldest1;
	PathCombine(
		cggldest,
		gameclient,
		cggl
		);

	cgd3d9dest = cgd3d9dest1;
	PathCombine(
		cgd3d9dest,
		gameclient,
		cgd3d9
		);

	pathcontainer[1] << (std::wstring(airdest) + constants[0]);
	pathcontainer[2] << (std::wstring(flashdest) + constants[0]);
	pathcontainer[3] << (std::wstring(tbb) + constants[0]);

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		tbbdownload(L"Xp.dll");
	}
	else
	{
		// Todo: add AVX2 support
		int cpuInfo[4];
		__cpuid(cpuInfo, 1);
		if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
		{
			tbbdownload(L"Avx.dll");
		}
		else
		{
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				tbbdownload(L"Sse2.dll");
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(L"Sse.dll");
				}
				else
				{
					tbbdownload(L"Default.dll");
				}
			}
		}
	}
#ifdef DEBUG
	wprintf(airlatest);
	wprintf(airdest);
	wprintf(flashlatest);
	wprintf(flashdest);
	wprintf(cgbin);
	wprintf(cgdest);
	wprintf(cgglbin);
	wprintf(cggldest);
	wprintf(cgd3d9bin);
	wprintf(cgd3d9dest);
#endif
	download(std::wstring(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"), std::wstring(L"air15_win.exe"), std::wstring(L"-silent"));
	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	download(std::wstring(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"), std::wstring(L"Cg-3.1_April2012_Setup.exe"), std::wstring(L"/verysilent /TYPE = compact"));
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);
	std::wstring unblocks[3] = { pathcontainer[1].str(), pathcontainer[2].str(), pathcontainer[3].str() };

	for (std::wstring& e : unblocks)
	{
		DeleteFile(e.c_str());
	}
	done = true;
	InvalidateRect(hwnd, &end, false);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}