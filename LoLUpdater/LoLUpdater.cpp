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
std::wstring* tbb[MAX_PATH + 1];
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
	HRESULT hRez = URLDownloadToFile(
		nullptr,
		url.c_str(),
		file.c_str(),
		0,
		nullptr
		);

	if (hRez == 0)
	{


		std::wstring unblocker[MAX_PATH];
		std::wstring *unblocker1;
		unblocker1 = unblocker;

		// String for balance of path name.
		std::wstring unblocker2[] = { std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) };
		std::wstring *unblockerq;
		unblockerq = unblocker2;

		// String for directory name.
		std::wstring unblocker3[] = { std::wstring(file.c_str()) };
		std::wstring *unblockerqq;
		unblockerqq = unblocker3;


		PathCombine(
			reinterpret_cast<LPWSTR>(unblocker1),
			reinterpret_cast<LPWSTR>(unblockerq),
			reinterpret_cast<LPWSTR>(unblockerqq)
			);


		pathcontainer[0] << (std::wstring(unblocker[0].c_str()) + std::wstring(constants[0]));

		DeleteFile(pathcontainer[0].str().c_str());
		pathcontainer[0].str(std::wstring());
		pathcontainer[0].clear();

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = file.c_str();
		ShExecInfo.lpParameters = args.c_str();
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(
			ShExecInfo.hProcess,
			INFINITE
			);
	}
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
	std::wstring buffer_1[MAX_PATH] = { std::wstring(L"projects") };
	std::wstring *lpStr1;
	lpStr1 = buffer_1;

	std::wstring buffer_1a[] = { std::wstring(L"lol_air_client") };
	std::wstring *lpStr1a;
	lpStr1a = buffer_1a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr1), reinterpret_cast<LPWSTR>(lpStr1a));

#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&lpStr1));
#endif

	std::wstring buffer_3[MAX_PATH] = { std::wstring(L"solutions") };
	std::wstring* lpStr3;
	lpStr3 = buffer_3;

	std::wstring buffer_3a[] = { std::wstring(L"lol_game_client_sln") };
	std::wstring* lpStr3a;
	lpStr3a = buffer_3a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr3), reinterpret_cast<LPWSTR>(lpStr3a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr3));
#endif

	const std::wstring rads(L"RADS");
	// air
	std::wstring buffer_2[MAX_PATH] = { rads };
	std::wstring* lpStr2;
	lpStr2 = buffer_2;

	std::wstring buffer_2a[] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr1)) };
	std::wstring* lpStr2a;
	lpStr2a = buffer_2a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr2), reinterpret_cast<LPWSTR>(lpStr2a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr2));
#endif
	// game
	std::wstring buffer_4[MAX_PATH] = { rads };
	std::wstring* lpStr4;
	lpStr4 = buffer_4;

	std::wstring buffer_4a[] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr3)) };
	std::wstring* lpStr4a;
	lpStr4a = buffer_4a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr4), reinterpret_cast<LPWSTR>(lpStr4a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr4));
#endif

	std::wstring buffer_5[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr4)) };
	std::wstring* gameclient;
	gameclient = buffer_5;

	std::wstring buffer_5a[] = { std::wstring(L"releases") };
	std::wstring* lpStr5a;
	lpStr5a = buffer_5a;

	PathAppend(reinterpret_cast<LPWSTR>(gameclient), reinterpret_cast<LPWSTR>(lpStr5a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(gameclient));
#endif
	std::wstring buffer_6[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr2)) };
	std::wstring* airclient;
	airclient = buffer_6;
	PathAppend(reinterpret_cast<LPWSTR>(airclient), reinterpret_cast<LPWSTR>(lpStr5a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airclient));
#endif


	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE = compact");
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

	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", L"air15_win.exe", L"-silent");

	pathcontainer[0] << std::wstring(L"\\Common Files\\");

	std::wstring adobepath[MAX_PATH];
	std::wstring *adobepath1;
	adobepath1 = adobepath;

	// String for balance of path name.
	std::wstring asd[] = { pathcontainer[0].str() };
	std::wstring *asd1;
	asd1 = asd;

	// String for directory name.
	std::wstring asd2[] = { constants[1] };
	std::wstring *asd21;
	asd21 = asd2;


	PathCombine(
		reinterpret_cast<LPWSTR>(adobepath),
		reinterpret_cast<LPWSTR>(asd1),
		reinterpret_cast<LPWSTR>(asd21)
		);

#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(adobepath));
#endif


	const std::wstring cg(L"Cg.dll");



	std::wstring cgbin[MAX_PATH];
	std::wstring *cgbin1;
	cgbin1 = cgbin;

	// String for balance of path name.
	std::wstring asd233[] = { std::wstring(reinterpret_cast<LPWSTR>(&cgbinpath[0])) };
	std::wstring *asd12;
	asd12 = asd233;

	// String for directory name.
	std::wstring asd23[] = { cg };
	std::wstring *asd213;
	asd213 = asd23;


	PathCombine(
		reinterpret_cast<LPWSTR>(cgbin),
		reinterpret_cast<LPWSTR>(asd12),
		reinterpret_cast<LPWSTR>(asd213)
		);






	const std::wstring cggl(L"CgGL.dll");

	std::wstring cgglbin[MAX_PATH];
	std::wstring *cgglbin1;
	cgglbin1 = cgglbin;


	// String for directory name.
	std::wstring asd232[] = { cggl };
	std::wstring *asd2132;
	asd2132 = asd232;


	PathCombine(
		reinterpret_cast<LPWSTR>(cgglbin),
		reinterpret_cast<LPWSTR>(asd12),
		reinterpret_cast<LPWSTR>(asd2132)
		);




	const std::wstring cgd3d9(L"CgD3D9.dll");
	std::wstring cgd3d9bin[MAX_PATH];
	std::wstring *cgd3d9bin1;
	cgd3d9bin1 = cgd3d9bin;


	// String for directory name.
	std::wstring asd2322[] = { cgd3d9 };
	std::wstring *asd21321;
	asd21321 = asd2322;


	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9bin),
		reinterpret_cast<LPWSTR>(asd12),
		reinterpret_cast<LPWSTR>(asd21321)
		);

	std::wstring buffer_7[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(gameclient)) };
	std::wstring* random7;
	random7 = buffer_7;

	std::wstring buffer_7a[] = { std::wstring(L"0.0.1.64") };
	std::wstring* lpStr7a;
	lpStr7a = buffer_7a;


	PathAppend(reinterpret_cast<LPWSTR>(random7), reinterpret_cast<LPWSTR>(lpStr7a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(random7));
#endif
	std::wstring buffer_12[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(random7)) };
	std::wstring* version;
	version = buffer_12;


	std::wstring buffer_12a[] = { std::wstring(L"deploy") };
	std::wstring* lpStr12a;
	lpStr12a = buffer_12a;

	PathAppend(reinterpret_cast<LPWSTR>(version), reinterpret_cast<LPWSTR>(lpStr12a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(version));
#endif


	std::wstring buffer_14[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(airclient)) };
	std::wstring* lpStr14;
	lpStr14 = buffer_14;

	std::wstring buffer_14a[] = { std::wstring(L"0.0.1.117") };
	std::wstring* lpStr14a;
	lpStr14a = buffer_14a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr14), reinterpret_cast<LPWSTR>(lpStr14a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr14));
#endif
	std::wstring buffer_13[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr14)) };
	std::wstring* lpStr13;
	lpStr13 = buffer_13;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr13), reinterpret_cast<LPWSTR>(lpStr12a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr13));
#endif


	std::wstring buffer_15[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr13)) };
	std::wstring* airversion;
	airversion = buffer_15;

	std::wstring buffer_15a[] = { constants[1] };
	std::wstring* lpStr15a;
	lpStr15a = buffer_15a;

	PathAppend(reinterpret_cast<LPWSTR>(airversion), reinterpret_cast<LPWSTR>(lpStr15a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airversion));
#endif


	std::wifstream garena(L"lol.exe");

	if (garena.good())
	{
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(garena.good()));
#endif
		version[0] = std::wstring();
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(version));
#endif
		airversion[0] = std::wstring();
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(airversion));
#endif
		version[0] = L"Game";
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(version));
#endif
		std::wstring buffer_17[MAX_PATH] = { constants[1] };
		std::wstring* airversion;
		airversion = buffer_17;

		std::wstring buffer_17a[] = { constants[2] };
		std::wstring* lpStr17a;
		lpStr17a = buffer_17a;

		PathAppend(reinterpret_cast<LPWSTR>(airversion), reinterpret_cast<LPWSTR>(lpStr17a));
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(airversion));
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




	// String for balance of path name.
	std::wstring asdaa[] = { pathcontainer[1].str() };
	std::wstring *asd1aa;
	asd1aa = asdaa;

	// String for directory name.
	std::wstring asd2bb[] = { std::wstring(L"tbb.dll") };
	std::wstring *asd21bb;
	asd21bb = asd2bb;


	PathCombine(
		reinterpret_cast<LPWSTR>(tbb),
		reinterpret_cast<LPWSTR>(asd1aa),
		reinterpret_cast<LPWSTR>(asd21bb)
		);




#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(tbb));
#endif
	const std::wstring air(L"Adobe AIR.dll");
	std::wstring airdest[MAX_PATH];
	std::wstring *airdest1;
	airdest1 = airdest;

	// String for balance of path name.
	std::wstring asdcc[] = { pathcontainer[2].str() };
	std::wstring *asd1cc;
	asd1cc = asdcc;

	// String for directory name.
	std::wstring asd2cc[] = { air };
	std::wstring *asd21cc;
	asd21cc = asd2cc;


	PathCombine(
		reinterpret_cast<LPWSTR>(airdest),
		reinterpret_cast<LPWSTR>(asdcc),
		reinterpret_cast<LPWSTR>(asd21cc)
		);


#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airdest));
#endif
	std::wstring  airlatest[MAX_PATH];
	std::wstring * airlatest1;
	airlatest1 = airlatest;

	// String for balance of path name.
	std::wstring asdccdd[] = { std::wstring(reinterpret_cast<LPWSTR>(adobepath)) };
	std::wstring *asd1ccdd;
	asd1ccdd = asdccdd;

	// String for directory name.
	std::wstring asd2ccqq[] = { air };
	std::wstring *asd21ccqq;
	asd21ccqq = asd2ccqq;


	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		reinterpret_cast<LPWSTR>(asd1ccdd),
		reinterpret_cast<LPWSTR>(asd21ccqq)
		);


#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airlatest));
#endif

	std::wstring buffer_11[MAX_PATH] = { std::wstring(L"Resources") };
	std::wstring* flash;
	flash = buffer_11;

	std::wstring buffer_11a[] = { std::wstring(L"NPSWF32.dll") };
	std::wstring* flasha;
	flasha = buffer_11a;

	PathAppend(reinterpret_cast<LPWSTR>(flash), reinterpret_cast<LPWSTR>(flasha));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flash));
#endif

	std::wstring  flashdest[MAX_PATH];
	std::wstring * flashdest1;
	flashdest1 = flashdest;

	// String for balance of path name.
	std::wstring asdccddqq[] = { pathcontainer[2].str() };
	std::wstring *asd1ccddqq;
	asd1ccddqq = asdccddqq;

	// String for directory name.
	std::wstring asd2ccqqqq[] = { std::wstring(reinterpret_cast<LPWSTR>(flash)) };
	std::wstring *asd21ccqqqq;
	asd21ccqqqq = asd2ccqqqq;


	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		reinterpret_cast<LPWSTR>(asd1ccddqq),
		reinterpret_cast<LPWSTR>(asd21ccqqqq)
		);

#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashdest));
#endif

	std::wstring flashlatest[MAX_PATH];
	std::wstring * flashlatest1;
	flashlatest1 = flashlatest;

	// String for balance of path name.
	std::wstring asdccddqqqq[] = { std::wstring(reinterpret_cast<LPWSTR>(adobepath)) };
	std::wstring *asd1ccddqqqq;
	asd1ccddqqqq = asdccddqqqq;

	// String for directory name.
	std::wstring asd2ccqqqqqq[] = { std::wstring(reinterpret_cast<LPWSTR>(flash)) };
	std::wstring *asd21ccqqqqqq;
	asd21ccqqqqqq = asd2ccqqqqqq;


	PathCombine(
		reinterpret_cast<LPWSTR>(flashlatest),
		reinterpret_cast<LPWSTR>(asd1ccddqqqq),
		reinterpret_cast<LPWSTR>(asd21ccqqqqqq)
		);



#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashlatest));
#endif

	std::wstring cgdest[MAX_PATH];
	std::wstring * cgdest1;
	cgdest1 = cgdest;

	// String for balance of path name.
	std::wstring asdccddqqqqqq[] = { pathcontainer[1].str() };
	std::wstring *asd1ccddqqqqqq;
	asd1ccddqqqqqq = asdccddqqqqqq;

	// String for directory name.
	std::wstring asd2ccqqqqqqqq[] = { cg };
	std::wstring *asd21ccqqqqqqqq;
	asd21ccqqqqqqqq = asd2ccqqqqqqqq;


	PathCombine(
		reinterpret_cast<LPWSTR>(cgdest1),
		reinterpret_cast<LPWSTR>(asd1ccddqqqqqq),
		reinterpret_cast<LPWSTR>(asd21ccqqqqqqqq)
		);

#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgdest));
#endif

	std::wstring cggldest[MAX_PATH];
	std::wstring *cggldest1;
	cggldest1 = cggldest;

	// String for balance of path name.
	std::wstring asdccddqqqqqqqq[] = { pathcontainer[1].str() };
	std::wstring *asd1ccddqqqqqqqq;
	asd1ccddqqqqqqqq = asdccddqqqqqqqq;

	// String for directory name.
	std::wstring asd2ccqqqqqqqqqq[] = { cggl };
	std::wstring *asd21ccqqqqqqqqqq;
	asd21ccqqqqqqqqqq = asd2ccqqqqqqqqqq;


	PathCombine(
		reinterpret_cast<LPWSTR>(cggldest1),
		reinterpret_cast<LPWSTR>(asd1ccddqqqqqqqq),
		reinterpret_cast<LPWSTR>(asd21ccqqqqqqqqqq)
		);

#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cggldest));
#endif
	std::wstring cgd3d9dest[MAX_PATH];
	std::wstring *cgd3d9dest1;
	cgd3d9dest1 = cgdest;

	// String for balance of path name.
	std::wstring asdccddqqqqqqqqw[] = { pathcontainer[1].str() };
	std::wstring *asd1ccddqqqqqqqqw;
	asd1ccddqqqqqqqqw = asdccddqqqqqqqqw;

	// String for directory name.
	std::wstring asd2ccqqqqqqqqqqa[] = { cgd3d9 };
	std::wstring *asd21ccqqqqqqqqqqa;
	asd21ccqqqqqqqqqqa = asd2ccqqqqqqqqqqa;


	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9dest1),
		reinterpret_cast<LPWSTR>(asd1ccddqqqqqqqqw),
		reinterpret_cast<LPWSTR>(asd21ccqqqqqqqqqqa)
		);


#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgd3d9dest1));
#endif
	pathcontainer[3] << (std::wstring(reinterpret_cast<LPWSTR>(airdest)) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[3].str().c_str());
#endif

	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(flashdest)) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[4].str().c_str());
#endif
	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(tbb)) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[5].str().c_str());
#endif
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1)));
#endif
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

	CopyFile(reinterpret_cast<LPWSTR>(airlatest), reinterpret_cast<LPWSTR>(airdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(flashlatest), reinterpret_cast<LPWSTR>(flashdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgbin), reinterpret_cast<LPWSTR>(cgdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgglbin), reinterpret_cast<LPWSTR>(cggldest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgd3d9bin), reinterpret_cast<LPWSTR>(cgd3d9dest), false);
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