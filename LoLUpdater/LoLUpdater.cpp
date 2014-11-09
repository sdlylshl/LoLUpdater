#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>

bool done = false;
// 0 = adobe install dir
// 1 = unblock air
// 2 = unblock flash
// 3 = unblock tbb
std::wstringstream pathcontainer[4];

const std::wstring constants[3] = { std::wstring(L":Zone.Identifier"), std::wstring(L"Adobe AIR\\Versions\\1.0"), std::wstring(L"AIR\\") };
wchar_t* tbb;

std::wstring cwd(_wgetcwd(
	nullptr,
	0
	));
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

void download(const std::wstring& url, const std::wstring& file, const std::wstring& args)
{
	URLDownloadToFile(
		nullptr,
		url.c_str(),
		file.c_str(),
		0,
		nullptr
		);

	wchar_t unblocker[MAX_PATH] = L"";
	wchar_t* unblocker1;
	unblocker1 = unblocker;

	wchar_t unblocker2[MAX_PATH] = L"";

	wcsncat_s(
		unblocker2,
		MAX_PATH,
		reinterpret_cast<LPWSTR>(&cwd[0]),
		_TRUNCATE
		);

	wchar_t* unblockerq;
	unblockerq = unblocker2;

	wchar_t unblocker3[MAX_PATH] = L"";

	wcsncat_s(
		unblocker3,
		MAX_PATH,
		file.c_str(),
		_TRUNCATE
		);

	wchar_t* unblockerqq;
	unblockerqq = unblocker3;

	PathCombine(
		unblocker1,
		unblockerq,
		unblockerqq
		);

	pathcontainer[0] << (std::wstring(unblocker1) + constants[0]);

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
	std::wstring* ftp[MAX_PATH];

	UrlCombine(
		L"http://lol.jdhpro.com/",
		file.c_str(),
		reinterpret_cast<LPWSTR>(ftp),
		reinterpret_cast<LPDWORD>(ftp),
		0
		);

	URLDownloadToFile(
		nullptr,
		reinterpret_cast<LPWSTR>(ftp),
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
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

	// base
	wchar_t airclient1[MAX_PATH] = L"";
	wcsncat_s(
		airclient1,
		MAX_PATH,
		reinterpret_cast<LPCWSTR>(cwd[0]),
		_TRUNCATE
		);

	wchar_t* airclient;
	airclient = airclient1;
	wchar_t gameclient1[MAX_PATH] = L"";
	wcsncat_s(
		gameclient1,
		MAX_PATH,
		reinterpret_cast<LPCWSTR>(cwd[0]),
		_TRUNCATE
		);
	wchar_t* gameclient;
	gameclient = gameclient1;

	// Constants
	wchar_t* rel = L"releases";
	wchar_t* rads = L"RADS";

	// air
	wchar_t proj1[MAX_PATH] = L"projects";
	wchar_t* proj;
	proj = proj1;
	wchar_t lolair1[] = L"lol_air_client";
	wchar_t* lolair;
	lolair = lolair1;
	PathAppend(airclient, rads);
	PathAppend(airclient, proj);
	PathAppend(airclient, lolair);
	PathAppend(airclient, rel);
	// deallocate lolair1

	//game
	wchar_t sol1[MAX_PATH] = L"solutions";
	wchar_t* sol;
	sol = sol1;
	wchar_t lolgame1[] = L"lol_game_client_sln";
	wchar_t* lolgame;
	lolgame = lolgame1;
	PathAppend(airclient, rads);
	PathAppend(airclient, sol);
	PathAppend(airclient, lolgame);
	PathAppend(airclient, rel);
	// deallocate lolgame1

	download(std::wstring(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"), std::wstring(L"Cg-3.1_April2012_Setup.exe"), std::wstring(L"/verysilent /TYPE = compact"));
	std::vector<std::wstring> cgbinpath(MAX_PATH, std::wstring());
	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH);

	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH,
		L"\\",
		_TRUNCATE
		);

	pathcontainer[0] << (std::wstring(reinterpret_cast<LPCWSTR>(cwd[0])) + std::wstring(L":Program Files"));

	if (sizeof(void*) == 4)
	{
		pathcontainer[0] << std::wstring(L" (x86)");
	}

	download(std::wstring(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"), std::wstring(L"air15_win.exe"), std::wstring(L"-silent"));

	pathcontainer[0] << std::wstring(L"\\Common Files\\");

	wchar_t adobepath1[MAX_PATH] = L"";
	wchar_t* adobepath;
	adobepath = adobepath1;

	wchar_t asd[MAX_PATH] = L"";
	wcsncat_s(
		asd,
		MAX_PATH,
		pathcontainer[0].str().c_str(),
		_TRUNCATE
		);
	wchar_t* asd1;
	asd1 = asd;

	wchar_t asd2[MAX_PATH] = L"";
	wcsncat_s(
		asd2,
		MAX_PATH,
		constants[1].c_str(),
		_TRUNCATE
		);
	wchar_t* asd21;
	asd21 = asd2;

	PathCombine(
		adobepath,
		asd1,
		asd21
		);

	wchar_t cgbin1[MAX_PATH] = L"";
	wchar_t* cgbin;
	cgbin = cgbin1;

	wchar_t asd233[MAX_PATH] = L"";

	wcsncat_s(
		asd233,
		MAX_PATH,
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		_TRUNCATE
		);

	wchar_t* asd12;
	asd12 = asd233;
	wchar_t* cg = L"Cg.dll";

	PathCombine(
		cgbin,
		asd12,
		cg
		);

	wchar_t cgglbin1[MAX_PATH] = L"";
	wchar_t* cgglbin;
	cgglbin = cgglbin1;
	wchar_t* cggl = L"CgGL.dll";

	PathCombine(
		cgglbin,
		asd12,
		cggl
		);

	wchar_t* cgd3d9 = L"CgD3D9.dll";
	wchar_t cgd3d9bin1[MAX_PATH] = L"";
	wchar_t* cgd3d9bin;
	cgd3d9bin = cgd3d9bin1;

	PathCombine(
		cgd3d9bin,
		asd12,
		cgd3d9
		);

	wchar_t* gameversion = L"0.0.1.64";
	PathAppend(gameclient, gameversion);
	wchar_t* dep = L"deploy";
	PathAppend(gameclient, dep);

	wchar_t* airversion = L"0.0.1.117";
	PathAppend(airclient, airversion);
	PathAppend(airclient, dep);
	PathAppend(airclient, asd21);

	std::wifstream garena(L"lol.exe");

	if (garena.good())
	{
		airclient = L"";

		gameclient = L"";

		gameclient = L"Game";

		wchar_t airclient1[MAX_PATH] = L"";
		wcsncat_s(
			airclient1,
			MAX_PATH,
			reinterpret_cast<LPCWSTR>(cwd[0]),
			_TRUNCATE
			);

		wchar_t* airclient;
		airclient = airclient1;
		wchar_t gameclient1[MAX_PATH] = L"";
		wcsncat_s(
			gameclient1,
			MAX_PATH,
			reinterpret_cast<LPCWSTR>(cwd[0]),
			_TRUNCATE
			);
		wchar_t* gameclient;
		gameclient = gameclient1;

		wchar_t asd22[MAX_PATH] = L"";
		wcsncat_s(
			asd22,
			MAX_PATH,
			constants[2].c_str(),
			_TRUNCATE
			);
		wchar_t* asd212;
		asd212 = asd22;

		wcsncat_s(
			airclient1,
			MAX_PATH,
			reinterpret_cast<LPCWSTR>(cwd[0]),
			_TRUNCATE
			);

		PathAppend(airclient, asd212);
	}

	wchar_t tbb1[MAX_PATH] = L"";
	tbb = tbb1;
	wchar_t* tbbfile = L"tbb.dll";

	PathCombine(
		tbb,
		gameclient,
		tbbfile
		);

	wchar_t airdest1[MAX_PATH] = L"";
	wchar_t* airdest;
	airdest = airdest1;

	wchar_t* air = L"Adobe AIR.dll";

	PathCombine(
		airdest,
		airclient,
		air
		);

	wchar_t airlatest[MAX_PATH] = L"";
	wchar_t* airlatest1;
	airlatest1 = airlatest;

	PathCombine(
		airlatest,
		adobepath,
		air
		);

	wchar_t buffer_11[MAX_PATH] = L"Resources";
	wchar_t* flash;
	flash = buffer_11;

	wchar_t buffer_11a[MAX_PATH] = L"NPSWF32.dll";
	wchar_t* flasha;
	flasha = buffer_11a;

	PathAppend(flash, flasha);

	wchar_t flashdest1[MAX_PATH] = L"";
	wchar_t* flashdest;
	flashdest = flashdest1;

	PathCombine(
		flashdest,
		airclient,
		flash
		);

	wchar_t flashlatest1[MAX_PATH] = L"";
	wchar_t* flashlatest;
	flashlatest = flashlatest1;

	PathCombine(
		flashlatest,
		adobepath,
		flash
		);

	wchar_t cgdest1[MAX_PATH] = L"";
	wchar_t* cgdest;
	cgdest = cgdest1;

	PathCombine(
		cgdest,
		gameclient,
		cg
		);

	wchar_t cggldest1[MAX_PATH] = L"";
	wchar_t* cggldest;
	cggldest = cggldest1;

	PathCombine(
		cggldest,
		gameclient,
		cggl
		);

	wchar_t cgd3d9dest1[MAX_PATH] = L"";
	wchar_t* cgd3d9dest;
	cgd3d9dest = cgd3d9dest1;

	PathCombine(
		cgd3d9dest,
		gameclient,
		cgd3d9
		);

	pathcontainer[1] << (std::wstring(airdest1) + constants[0]);
	pathcontainer[2] << (std::wstring(flashdest1) + constants[0]);
	pathcontainer[3] << (std::wstring(tbb1) + constants[0]);

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

	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);
	std::wstring unblocks[3] = { std::wstring(pathcontainer[1].str().c_str()), std::wstring(pathcontainer[2].str().c_str()), std::wstring(pathcontainer[3].str().c_str()) };
	for (std::wstring i : unblocks)
	{
		DeleteFile(&i[0]);
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