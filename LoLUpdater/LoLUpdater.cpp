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
// 1 = game
// 2 = pvp.net
// 3 = unblock air
// 4 = unblock flash
// 5 = unblock tbb

std::wstringstream pathcontainer[6];
const std::wstring constants[3] = { L":Zone.Identifier", L"Adobe AIR\\Versions\\1.0", L"AIR\\" };
std::wstring *tbb = nullptr;
std::wstring cwd(_wgetcwd(
	nullptr,
	0
	));
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

void download(const std::wstring url, const std::wstring file, const std::wstring args)
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
		std::wstring *unblocker = nullptr;
		PathCombine(reinterpret_cast<LPWSTR>(unblocker), reinterpret_cast<LPWSTR>(&cwd[0]), file.c_str());
		pathcontainer[0] << (std::wstring(reinterpret_cast<LPWSTR>(&unblocker)) + constants[0]);
		

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

void tbbdownload(const std::wstring file)
{
	std::wstring *ftp[MAX_PATH+1];

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
	std::wstring buffer_1[2] = { L"projects" };
	std::wstring *lpStr1;
	lpStr1 = buffer_1;

	std::wstring buffer_1a[2] = { L"lol_air_client" };
	std::wstring *lpStr1a;
	lpStr1a = buffer_1a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr1), reinterpret_cast<LPWSTR>(lpStr1a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr1));
#endif

	std::wstring buffer_3[2] = { L"solutions" };
	std::wstring *lpStr3;
	lpStr3 = buffer_3;

	std::wstring buffer_3a[2] = { L"lol_game_client_sln" };
	std::wstring *lpStr3a;
	lpStr3a = buffer_3a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr3), reinterpret_cast<LPWSTR>(lpStr3a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr3));
#endif

	const std::wstring rads(L"RADS");
	// air
	std::wstring buffer_2[2] = { rads };
	std::wstring *lpStr2;
	lpStr2 = buffer_2;

	std::wstring buffer_2a[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr1)) };
	std::wstring *lpStr2a;
	lpStr2a = buffer_2a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr2), reinterpret_cast<LPWSTR>(lpStr2));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr2));
#endif
	// game
	std::wstring buffer_4[2] = { rads };
	std::wstring *lpStr4;
	lpStr4 = buffer_4;

	std::wstring buffer_4a[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr3)) };
	std::wstring *lpStr4a;
	lpStr4a = buffer_4a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr4), reinterpret_cast<LPWSTR>(lpStr4a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_4));
#endif

	const std::wstring rel(L"releases");
	std::wstring buffer_5[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr4)) };
	std::wstring *gameclient;
	gameclient = buffer_5;

	std::wstring buffer_5a[2] = { std::wstring(rel.c_str()) };
	std::wstring *lpStr5a;
	lpStr5a = buffer_5a;

	PathAppend(reinterpret_cast<LPWSTR>(gameclient), reinterpret_cast<LPWSTR>(lpStr5a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_5));
#endif
	std::wstring buffer_6[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr2)) };
	std::wstring *airclient;
	airclient = buffer_6;
	PathAppend(reinterpret_cast<LPWSTR>(airclient), reinterpret_cast<LPWSTR>(lpStr5a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airclient));
#endif


	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE = compact");
	std::vector<std::wstring> cgbinpath(MAX_PATH + 1, std::wstring());
	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH + 1);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&cgbinpath[0]));
#endif
	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH + 1,
		L"\\",
		_TRUNCATE
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&cgbinpath[0]));
#endif

	std::wstring progfiles = std::wstring(L":") + std::wstring(L"Program Files");
	pathcontainer[0] << std::wstring(reinterpret_cast<LPWSTR>(&cwd[0]))[0] + progfiles;
#ifdef DEBUG
	wprintf(pathcontainer[0].str().c_str());
#endif
	if (sizeof(void*) == 8)
	{
		pathcontainer[0] << std::wstring(L" (x86)");
#ifdef DEBUG
		wprintf(pathcontainer[0].str().c_str());
#endif
	}

	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", L"air15_win.exe", L"-silent");

	pathcontainer[0] << (std::wstring(L"\\Common Files") + std::wstring(L"\\Adobe ") + constants[2]);
#ifdef DEBUG
	wprintf(pathcontainer[0].str().c_str());
#endif
	std::wstring adobepath[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(adobepath),
		pathcontainer[0].str().c_str(),
		constants[1].c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(adobepath));
#endif
	

	const std::wstring cg(L"Cg.dll");
	std::wstring cgbin[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cg.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgbin));
#endif
	const std::wstring cggl(L"CgGL.dll");
	std::wstring cgglbin[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgglbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cggl.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgglbin));
#endif
	const std::wstring cgd3d9(L"CgD3D9.dll");
	std::wstring cgd3d9bin[2];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9bin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cgd3d9.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgd3d9bin));
#endif

	std::wstring buffer_7[2] = { std::wstring(reinterpret_cast<LPWSTR>(gameclient)) };
	std::wstring *random7;
	random7 = buffer_7;

	std::wstring buffer_7a[2] = { std::wstring(L"0.0.1.64") };
	std::wstring *lpStr7a;
	lpStr7a = buffer_7a;


	PathAppend(reinterpret_cast<LPWSTR>(random7), reinterpret_cast<LPWSTR>(lpStr7a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(random7));
#endif
	const std::wstring dep(L"deploy");
	std::wstring buffer_12[MAX_PATH + 1] = { std::wstring(reinterpret_cast<LPWSTR>(random7)) };
	std::wstring *version;
	version = buffer_12;


	std::wstring buffer_12a[2] = { std::wstring(dep.c_str()) };
	std::wstring *lpStr12a;
	lpStr12a = buffer_12a;

	PathAppend(reinterpret_cast<LPWSTR>(version), reinterpret_cast<LPWSTR>(lpStr12a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(version));
#endif



	std::wstring buffer_14[2] = { std::wstring(reinterpret_cast<LPWSTR>(airclient)) };
	std::wstring*lpStr14;
	lpStr14 = buffer_14;

	std::wstring buffer_14a[2] = { std::wstring(L"0.0.1.117") };
	std::wstring *lpStr14a;
	lpStr14a = buffer_14a;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr14), reinterpret_cast<LPWSTR>(lpStr14a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr14));
#endif
	std::wstring buffer_13[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr14)) };
	std::wstring *lpStr13;
	lpStr13 = buffer_13;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr13), reinterpret_cast<LPWSTR>(lpStr12a));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(lpStr13));
#endif


	std::wstring buffer_15[2] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr13)) };
	std::wstring *airversion;
	airversion = buffer_15;

	std::wstring buffer_15a[2] = { std::wstring(constants[1].c_str()) };
	std::wstring *lpStr15a;
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
		version[0].clear();
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(version));
#endif
		airversion[0].clear();
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(airversion));
#endif
		version[0] = L"Game";
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(version));
#endif
		std::wstring buffer_17[MAX_PATH + 1] = { constants[1].c_str() };
		std::wstring *airversion;
		airversion = buffer_17;
		PathAppend(reinterpret_cast<LPWSTR>(airversion), constants[2].c_str());
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

	PathCombine(
		reinterpret_cast<LPWSTR>(tbb),
		pathcontainer[1].str().c_str(),
		L"tbb.dll"
		);
#ifdef DEBUG
	wprintf(L"tbb path", reinterpret_cast<LPWSTR>(tbb));
#endif
	const std::wstring air(L"Adobe AIR.dll");
	std::wstring airdest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(airdest),
		pathcontainer[2].str().c_str(),
		air.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airdest));
#endif
	std::wstring airlatest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		reinterpret_cast<LPWSTR>(adobepath),
		air.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airlatest));
#endif

	std::wstring buffer_11[MAX_PATH + 1] = { L"Resources" };
	std::wstring *flash;
	flash = buffer_11;

	PathAppend(reinterpret_cast<LPWSTR>(flash), L"NPSWF32.dll");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flash));
#endif

	std::wstring flashdest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(flashdest),
		pathcontainer[2].str().c_str(),
		reinterpret_cast<LPWSTR>(flash)
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashdest));
#endif

	std::wstring flashlatest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(flashlatest),
		reinterpret_cast<LPWSTR>(adobepath),
		reinterpret_cast<LPWSTR>(flash)
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashlatest));
#endif

	std::wstring cgdest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgdest),
		pathcontainer[1].str().c_str(),
		cg.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgdest));
#endif

	std::wstring cggldest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(cggldest),
		pathcontainer[1].str().c_str(),
		cggl.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cggldest));
#endif
	std::wstring cgd3d9dest[MAX_PATH + 1];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9dest),
		pathcontainer[1].str().c_str(),
		cgd3d9.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgd3d9dest));
#endif
	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(airdest))+constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[3].str().c_str());
#endif

	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(flashdest))+constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[4].str().c_str());
#endif
	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(&tbb)) + constants[0]);
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
	std::wstring unblocks[3] = { pathcontainer[3].str(), pathcontainer[4].str(), pathcontainer[5].str() };
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