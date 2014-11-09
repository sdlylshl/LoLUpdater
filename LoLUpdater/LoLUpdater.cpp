#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

bool done = false;
// 0 = adobe install dir
// 1 = game
// 2 = pvp.net
// 3 = unblock air
// 4 = unblock flash
// 5 = unblock tbb
std::wstringstream pathcontainer[6];

const std::wstring constants[3] = { std::wstring(L":Zone.Identifier"), std::wstring(L"Adobe AIR\\Versions\\1.0"), std::wstring(L"AIR\\") };
wchar_t tbb[MAX_PATH];

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

		wchar_t unblocker[MAX_PATH];
		wchar_t *unblocker1;
		unblocker1 = unblocker;

		std::wstring unblocker2[] =  { std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) };
		std::wstring *unblockerq;
		unblockerq = unblocker2;

		std::wstring unblocker3[] = { std::wstring(file.c_str()) };
		std::wstring *unblockerqq;
		unblockerqq = unblocker3;

		PathCombine(
			reinterpret_cast<LPWSTR>(unblocker1),
			reinterpret_cast<LPWSTR>(unblockerq),
			reinterpret_cast<LPWSTR>(unblockerqq)
			);

		pathcontainer[0] << (std::wstring(reinterpret_cast<LPWSTR>(unblocker1)) + constants[0]);

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
		reinterpret_cast<LPWSTR>(tbb),
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
#ifdef DEBUG
	SetConsoleOutputCP(CP_UTF8);
	AllocConsole();
	_wfreopen(L"CONOUT$", L"w", stdout);
#endif
	wchar_t buffer_1[MAX_PATH] = L"projects";
	wchar_t *lpStr1;
	lpStr1 = buffer_1;

	wchar_t buffer_1a[] = L"lol_air_client";
	wchar_t *lpStr1a;
	lpStr1a = buffer_1a;

	PathAppend(lpStr1, lpStr1a);

#ifdef DEBUG
	wprintf(lpStr1);
#endif

	wchar_t buffer_3[MAX_PATH] = L"solutions";
	wchar_t* lpStr3;
	lpStr3 = buffer_3;

	wchar_t buffer_3a[] = L"lol_game_client_sln";
	wchar_t* lpStr3a;
	lpStr3a = buffer_3a;

	PathAppend(lpStr3, lpStr3a);
#ifdef DEBUG
	wprintf(lpStr3);
#endif


	wchar_t buffer_2[MAX_PATH] = L"RADS";
	wchar_t* lpStr2;
	lpStr2 = buffer_2;

	wchar_t buffer_2a[MAX_PATH] = (wchar_t)std::wstring(lpStr1);
	wchar_t* lpStr2a;
	lpStr2a = buffer_2a;

	PathAppend(lpStr2, lpStr2a);
#ifdef DEBUG
	wprintf(lpStr2);
#endif
	// game
	wchar_t buffer_4[MAX_PATH] = L"RADS";
	wchar_t* lpStr4;
	lpStr4 = buffer_4;

	wchar_t buffer_4a[MAX_PATH] = lpStr3;
	wchar_t* lpStr4a;
	lpStr4a = buffer_4a;

	PathAppend(lpStr4, lpStr4a);
#ifdef DEBUG
	wprintf(lpStr4);
#endif

	wchar_t buffer_5[MAX_PATH] = lpStr4;
	wchar_t* gameclient;
	gameclient = buffer_5;

	wchar_t buffer_5a[MAX_PATH] = L"releases";
	wchar_t* lpStr5a;
	lpStr5a = buffer_5a;

	PathAppend(gameclient, lpStr5a);
#ifdef DEBUG
	wprintf(gameclient);
#endif
	wchar_t buffer_6[MAX_PATH] = lpStr2;
	wchar_t* airclient;
	airclient = buffer_6;
	PathAppend(airclient, lpStr5a);
#ifdef DEBUG
	wprintf(airclient);
#endif


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

	pathcontainer[0] << (std::wstring(reinterpret_cast<LPWSTR>(&cwd[0]))[0] + std::wstring(L":Program Files"));

	if (sizeof(void*) == 4)
	{
		pathcontainer[0] << std::wstring(L" (x86)");
	}

	download(std::wstring(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"), std::wstring(L"air15_win.exe"), std::wstring(L"-silent"));

	pathcontainer[0] << std::wstring(L"\\Common Files\\");

	wchar_t adobepath[MAX_PATH];
	wchar_t *adobepath1;
	adobepath1 = adobepath;

	wchar_t asd[MAX_PATH] = pathcontainer[0].str().c_str();
	wchar_t *asd1;
	asd1 = asd;

	wchar_t asd2[MAX_PATH] = constants[1].c_str();
	wchar_t *asd21;
	asd21 = asd2;


	PathCombine(
		adobepath,
		asd1,
		asd21
		);

#ifdef DEBUG
	wprintf(adobepath);
#endif


	wchar_t cgbin[MAX_PATH];
	wchar_t *cgbin1;
	cgbin1 = cgbin;

	wchar_t asd233[MAX_PATH] = &cgbinpath[0];
	wchar_t *asd12;
	asd12 = asd233;

	wchar_t asd23[MAX_PATH] = L"Cg.dll";
	wchar_t *asd213;
	asd213 = asd23;


	PathCombine(
	cgbin,
	asd12,
	asd213
		);







	wchar_t cgglbin[MAX_PATH];
	wchar_t *cgglbin1;
	cgglbin1 = cgglbin;

	wchar_t asd232[MAX_PATH] = L"CgGL.dll";
	wchar_t *asd2132;
	asd2132 = asd232;


	PathCombine(
		cgglbin1,
		asd12,
		asd2132
		);


	wchar_t cgd3d9bin[MAX_PATH];
	wchar_t *cgd3d9bin1;
	cgd3d9bin1 = cgd3d9bin;

	wchar_t asd2322[MAX_PATH] = L"CgD3D9.dll";
	wchar_t *asd21321;
	asd21321 = asd2322;


	PathCombine(
		cgd3d9bin1,
		asd12,
		asd21321
		);

	wchar_t buffer_7[MAX_PATH] = gameclient;
	wchar_t* random7;
	random7 = buffer_7;

	wchar_t buffer_7a[MAX_PATH] = L"0.0.1.64";
	wchar_t* lpStr7a;
	lpStr7a = buffer_7a;


	PathAppend(random7, lpStr7a);
#ifdef DEBUG
	wprintf(random7);
#endif
	wchar_t buffer_12[MAX_PATH] = random7;
	wchar_t* version;
	version = buffer_12;


	wchar_t buffer_12a[MAX_PATH] = L"deploy";
	wchar_t* lpStr12a;
	lpStr12a = buffer_12a;

	PathAppend(version, lpStr12a);
#ifdef DEBUG
	wprintf(version);
#endif


	wchar_t buffer_14[MAX_PATH] = airclient;
	wchar_t* lpStr14;
	lpStr14 = buffer_14;

	wchar_t buffer_14a[MAX_PATH] = L"0.0.1.117";
	wchar_t* lpStr14a;
	lpStr14a = buffer_14a;

	PathAppend(lpStr14, lpStr14a);
#ifdef DEBUG
	wprintf(lpStr14);
#endif
	wchar_t buffer_13[MAX_PATH] = lpStr14;
	wchar_t* lpStr13;
	lpStr13 = buffer_13;
	PathAppend(lpStr13, lpStr12a);
#ifdef DEBUG
	wprintf(lpStr13);
#endif


	wchar_t buffer_15[MAX_PATH] = lpStr13;
	wchar_t* airversion;
	airversion = buffer_15;

	wchar_t buffer_15a[MAX_PATH] = constants[1];
	wchar_t* lpStr15a;
	lpStr15a = buffer_15a;

	PathAppend(airversion, lpStr15a);
#ifdef DEBUG
	wprintf(airversion);
#endif


	std::wifstream garena(L"lol.exe");

	if (garena.good())
	{
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(garena.good()));
#endif
		version[0] = wchar_t();
#ifdef DEBUG
		wprintf(version);
#endif
		airversion[0] = wchar_t();
#ifdef DEBUG
		wprintf(airversion);
#endif
		version = L"Game";
#ifdef DEBUG
		wprintf(version);
#endif
		wchar_t buffer_17[MAX_PATH] = constants[1];
		wchar_t* airversion;
		airversion = buffer_17;

		wchar_t buffer_17a[MAX_PATH] = constants[2];
		wchar_t* lpStr17a;
		lpStr17a = buffer_17a;

		PathAppend(airversion1, pStr17a);
#ifdef DEBUG
		wprintf(airversion);
#endif
	}

	pathcontainer[1] << (std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) + std::wstring(reinterpret_cast<LPWSTR>(version)));
#ifdef DEBUG
	wprintf(pathcontainer[1].str().c_str());
#endif
	pathcontainer[2] << (std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) + std::wstring(reinterpret_cast<LPWSTR>(airversion)));
#ifdef DEBUG
	wprintf(pathcontainer[2].str().c_str());
#endif



	wchar_t *tbb1;
	tbb1 = tbb;

	// String for balance of path name.
	wchar_t asdaa[MAX_PATH] = pathcontainer[1].str().c_str();
	wchar_t *asd1aa;
	asd1aa = asdaa;

	// String for directory name.
	wchar_t asd2bb[MAX_PATH] = L"tbb.dll";
	wchar_t *asd21bb;
	asd21bb = asd2bb;


	PathCombine(
		tbb1,
		asd1aa,
		asd21bb
		);




#ifdef DEBUG
	wprintf(tbb1);
#endif
	wchar_t airdest[MAX_PATH];
	wchar_t *airdest1;
	airdest1 = airdest;

	wchar_t asdcc[MAX_PATH] = pathcontainer[2].str().c_str();
	wchar_t *asd1cc;
	asd1cc = asdcc;

	wchar_t asd2cc[MAX_PATH] = L"Adobe AIR.dll";
	wchar_t *asd21cc;
	asd21cc = asd2cc;


	PathCombine(
		airdest1,
		asd1cc,
		asd21cc
		);


#ifdef DEBUG
	wprintf(airdest);
#endif
	wchar_t  airlatest[MAX_PATH];
	wchar_t * airlatest1;
	airlatest1 = airlatest;

	wchar_t asdccdd[MAX_PATH] = adobepath;
	wchar_t *asd1ccdd;
	asd1ccdd = asdccdd;

	wchar_t asd2ccqq[MAX_PATH] = L"Adobe AIR.dll";
	wchar_t *asd21ccqq;
	asd21ccqq = asd2ccqq;


	PathCombine(
		airlatest,
		asd1ccdd,
		asd21ccqq
		);


#ifdef DEBUG
	wprintf(airlatest);
#endif

	wchar_t buffer_11[MAX_PATH] = L"Resources";
	wchar_t* flash;
	flash = buffer_11;

	wchar_t buffer_11a[MAX_PATH] = L"NPSWF32.dll";
	wchar_t* flasha;
	flasha = buffer_11a;

	PathAppend(flash, flasha);
#ifdef DEBUG
	wprintf(flash);
#endif

	wchar_t  flashdest[MAX_PATH];
	wchar_t * flashdest1;
	flashdest1 = flashdest;

	// String for balance of path name.
	wchar_t asdccddqq[MAX_PATH] = pathcontainer[2].str();
	wchar_t *asd1ccddqq;
	asd1ccddqq = asdccddqq;

	// String for directory name.
	wchar_t asd2ccqqqq[MAX_PATH] = flash;
	wchar_t *asd21ccqqqq;
	asd21ccqqqq = asd2ccqqqq;


	PathCombine(
		flashdest1,
		asd1ccddqq,
		asd21ccqqqq
		);

#ifdef DEBUG
	wprintf(flashdest);
#endif

	wchar_t flashlatest[MAX_PATH];
	wchar_t * flashlatest1;
	flashlatest1 = flashlatest;

	wchar_t asdccddqqqq[MAX_PATH] = adobepath;
	wchar_t *asd1ccddqqqq;
	asd1ccddqqqq = asdccddqqqq;

	// String for directory name.
	wchar_t asd2ccqqqqqq[MAX_PATH] = flash.c_str();
	wchar_t *asd21ccqqqqqq;
	asd21ccqqqqqq = asd2ccqqqqqq;


	PathCombine(
		flashlatest1,
		asd1ccddqqqq,
		asd21ccqqqqqq
		);



#ifdef DEBUG
	wprintf(flashlatest);
#endif

	wchar_t cgdest[MAX_PATH];
	wchar_t * cgdest1;
	cgdest1 = cgdest;

	wchar_t asdccddqqqqqq[MAX_PATH] = pathcontainer[1].str().c_str();
	wchar_t *asd1ccddqqqqqq;
	asd1ccddqqqqqq = asdccddqqqqqq;

	wchar_t asd2ccqqqqqqqq[MAX_PATH] = L"Cg.dll";
	wchar_t *asd21ccqqqqqqqq;
	asd21ccqqqqqqqq = asd2ccqqqqqqqq;


	PathCombine(
		cgdest1,
		asd1ccddqqqqqq,
		asd21ccqqqqqqqq
		);

#ifdef DEBUG
	wprintf(cgdest);
#endif

	wchar_t cggldest[MAX_PATH];
	wchar_t *cggldest1;
	cggldest1 = cggldest;

	wchar_t asdccddqqqqqqqq[MAX_PATH] = pathcontainer[1].str().c_str();
	wchar_t *asd1ccddqqqqqqqq;
	asd1ccddqqqqqqqq = asdccddqqqqqqqq;

	wchar_t asd2ccqqqqqqqqqq[MAX_PATH] = L"CgGL.dll";
	wchar_t *asd21ccqqqqqqqqqq;
	asd21ccqqqqqqqqqq = asd2ccqqqqqqqqqq;


	PathCombine(
		cggldest1,
		asd1ccddqqqqqqqq,
		asd21ccqqqqqqqqqq
		);

#ifdef DEBUG
	wprintf(cggldest);
#endif
	wchar_t cgd3d9dest[MAX_PATH];
	wchar_t *cgd3d9dest1;
	cgd3d9dest1 = cgdest;

	wchar_t asdccddqqqqqqqqw[MAX_PATH] = pathcontainer[1].str().c_str();
	wchar_t *asd1ccddqqqqqqqqw;
	asd1ccddqqqqqqqqw = asdccddqqqqqqqqw;

	wchar_t asd2ccqqqqqqqqqqa[MAX_PATH] = L"CgD3D9.dll";
	wchar_t *asd21ccqqqqqqqqqqa;
	asd21ccqqqqqqqqqqa = asd2ccqqqqqqqqqqa;


	PathCombine(
	cgd3d9dest1,
	asd1ccddqqqqqqqqw,
	asd21ccqqqqqqqqqqa
		);


#ifdef DEBUG
	wprintf(cgd3d9dest1);
#endif
	pathcontainer[3] << (std::wstring(airdest1) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[3].str().c_str());
#endif

	pathcontainer[5] << (std::wstring(flashdest1) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[4].str().c_str());
#endif
	pathcontainer[5] << (std::wstring(tbb1) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[5].str().c_str());
#endif
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

	CopyFile(airlatest1, airdest1, false);
	CopyFile(flashlatest1, flashdest1, false);
	CopyFile(cgbin1, cgdest1, false);
	CopyFile(cgglbin1, cggldest1, false);
	CopyFile(cgd3d9bin1, cgd3d9dest1, false);
	std::wstring unblocks[3] = { std::wstring(pathcontainer[3].str().c_str()), std::wstring(pathcontainer[4].str().c_str()), std::wstring(pathcontainer[5].str().c_str()) };
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(unblocks));
#endif
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