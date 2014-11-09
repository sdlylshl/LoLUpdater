#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>

// Todo: find c++ way
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

bool done = false;
// 0 = intermediate combiner
// 1 = adobe install dir
// 2 = game
// 3 = pvp.net
// 4 = unblock air
// 5 = unblock flash
// 6 = unblock tbb

std::wstringstream pathcontainer[7];
const std::wstring constants[3] = { L":Zone.Identifier", L"Adobe AIR\\Versions\\1.0", L"AIR\\" };
std::wstring *tbb = nullptr;
std::vector<std::wstring> cwd(MAX_PATH, std::wstring());
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

void download(std::wstring fromurl, std::wstring dest, std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		fromurl.c_str(),
		dest.c_str(),
		0,
		nullptr
		);
	std::wstring *unblocker = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(&unblocker), reinterpret_cast<LPWSTR>(&cwd[0]), dest.c_str());
	pathcontainer[0] << (reinterpret_cast<LPWSTR>(&unblocker) + constants[0]);
	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring());
	pathcontainer[0].clear();
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = dest.c_str();
	ShExecInfo.lpParameters = args.c_str();
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(
		ShExecInfo.hProcess,
		INFINITE
		);
}

void tbbdownload(const std::wstring file)
{
	const std::wstring ftp(L"http://lol.jdhpro.com/");
	pathcontainer[0] << (ftp + file.c_str());

	URLDownloadToFile(
		nullptr,
		pathcontainer[0].str().c_str(),
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
	std::wstring buffer_1[MAX_PATH] = { L"projects" };
	std::wstring *lpStr1;
	lpStr1 = buffer_1;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr1), L"lol_air_client");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_1));
#endif

	std::wstring buffer_3[MAX_PATH] = { L"solutions" };
	std::wstring *lpStr3;
	lpStr3 = buffer_3;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr3), L"lol_game_client_sln");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_3));
#endif

	const std::wstring rads(L"RADS");
	// air
	std::wstring buffer_2[MAX_PATH] = { rads };
	std::wstring *lpStr2;
	lpStr2 = buffer_2;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr2), reinterpret_cast<LPWSTR>(buffer_1));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_2));
#endif
	// game
	std::wstring buffer_4[MAX_PATH] = { rads };
	std::wstring *lpStr4;
	lpStr4 = buffer_4;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr4), reinterpret_cast<LPWSTR>(lpStr3));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_4));
#endif

	const std::wstring rel(L"releases");
	std::wstring buffer_5[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(buffer_4)) };
	std::wstring *gameclient;
	gameclient = buffer_5;
	PathAppend(reinterpret_cast<LPWSTR>(gameclient), rel.c_str());
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_5));
#endif
	std::wstring buffer_6[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(lpStr2)) };
	std::wstring *airclient;
	airclient = buffer_6;
	PathAppend(reinterpret_cast<LPWSTR>(airclient), rel.c_str());
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_6));
#endif
	GetModuleFileName(nullptr, reinterpret_cast<LPWSTR>(&cwd[0]), MAX_PATH);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&cwd[0]));
#endif
	// todo fix this
	// /* fix this tomorrow */ cwd.resize(cwd[0].length() - 14);
#ifdef DEBUG
	// wprintf(const_cast<LPWSTR>(cwd[0].c_str()));
#endif
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE = compact");
	std::vector<std::wstring> cgbinpath(MAX_PATH, std::wstring());
	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&cgbinpath[0]));
#endif
	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH,
		L"\\",
		_TRUNCATE
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(&cgbinpath[0]));
#endif

	std::wstring progfiles = std::wstring(L":") + std::wstring(L"Program Files");
	pathcontainer[1] << std::wstring(reinterpret_cast<LPWSTR>(&cwd[0]))[0] + progfiles;
#ifdef DEBUG
	wprintf(pathcontainer[1].str().c_str());
#endif
	if (sizeof(void*) == 4)
	{
		pathcontainer[1] << std::wstring(L" (x86)");
#ifdef DEBUG
		wprintf(pathcontainer[1].str().c_str());
#endif
	}

	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", L"air15_win.exe", L"-silent");

	pathcontainer[1] << std::wstring(L"Common Files") + std::wstring(L"Adobe ") + constants[2];
#ifdef DEBUG
	wprintf(pathcontainer[1].str().c_str());
#endif
	std::wstring adobepath[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(adobepath),
		pathcontainer[1].str().c_str(),
		constants[1].c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(adobepath));
#endif
	

	const std::wstring cg(L"Cg.dll");
	std::wstring cgbin[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cg.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgbin));
#endif
	const std::wstring cggl(L"CgGL.dll");
	std::wstring cgglbin[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgglbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cggl.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgglbin));
#endif
	const std::wstring cgd3d9(L"CgD3D9.dll");
	std::wstring cgd3d9bin[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9bin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cgd3d9.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgd3d9bin));
#endif

	std::wstring buffer_7[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(buffer_5)) };
	std::wstring *random7;
	random7 = buffer_7;
	PathAppend(reinterpret_cast<LPWSTR>(random7), L"0.0.1.64");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(random7));
#endif
	const std::wstring dep(L"deploy");
	std::wstring buffer_12[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(random7)) };
	std::wstring *version;
	version = buffer_12;
	PathAppend(reinterpret_cast<LPWSTR>(version), dep.c_str());
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(version));
#endif



	std::wstring buffer_14[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(buffer_6)) };
	std::wstring*lpStr14;
	lpStr14 = buffer_14;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr14), L"0.0.1.117");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_14));
#endif
	std::wstring buffer_13[MAX_PATH] = { std::wstring(reinterpret_cast<LPWSTR>(buffer_14)) };
	std::wstring *lpStr13;
	lpStr13 = buffer_13;
	PathAppend(reinterpret_cast<LPWSTR>(lpStr13), dep.c_str());
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_13));
#endif


	std::wstring buffer_15[MAX_PATH] = { constants[1].c_str() };
	std::wstring *airversion;
	airversion = buffer_15;
	PathAppend(reinterpret_cast<LPWSTR>(airversion), reinterpret_cast<LPWSTR>(lpStr13));
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(buffer_15));
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
		std::wstring buffer_17[MAX_PATH] = { constants[1].c_str() };
		std::wstring *airversion;
		airversion = buffer_17;
		PathAppend(reinterpret_cast<LPWSTR>(airversion), constants[2].c_str());
#ifdef DEBUG
		wprintf(reinterpret_cast<LPWSTR>(airversion));
#endif
	}

	pathcontainer[2] << (std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) + std::wstring(reinterpret_cast<LPWSTR>(buffer_12)));
#ifdef DEBUG
	wprintf(pathcontainer[2].str().c_str());
#endif
	pathcontainer[3] << (std::wstring(reinterpret_cast<LPWSTR>(&cwd[0])) + std::wstring(reinterpret_cast<LPWSTR>(buffer_15)));
#ifdef DEBUG
	wprintf(pathcontainer[3].str().c_str());
#endif

	PathCombine(
		reinterpret_cast<LPWSTR>(tbb),
		pathcontainer[2].str().c_str(),
		L"tbb.dll"
		);
#ifdef DEBUG
	wprintf(L"tbb path", reinterpret_cast<LPWSTR>(tbb));
#endif
	const std::wstring air(L"Adobe AIR.dll");
	std::wstring airdest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(airdest),
		pathcontainer[3].str().c_str(),
		air.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airdest));
#endif
	std::wstring airlatest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		reinterpret_cast<LPWSTR>(adobepath),
		air.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(airlatest));
#endif

	std::wstring buffer_11[MAX_PATH] = { L"Resources" };
	std::wstring *flash;
	flash = buffer_11;

	PathAppend(reinterpret_cast<LPWSTR>(flash), L"NPSWF32.dll");
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flash));
#endif

	std::wstring flashdest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(flashdest),
		pathcontainer[3].str().c_str(),
		reinterpret_cast<LPWSTR>(flash)
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashdest));
#endif

	std::wstring flashlatest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(flashlatest),
		reinterpret_cast<LPWSTR>(adobepath),
		reinterpret_cast<LPWSTR>(flash)
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(flashlatest));
#endif

	std::wstring cgdest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgdest),
		pathcontainer[2].str().c_str(),
		cg.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgdest));
#endif

	std::wstring cggldest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cggldest),
		pathcontainer[2].str().c_str(),
		cggl.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cggldest));
#endif
	std::wstring cgd3d9dest[MAX_PATH];
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9dest),
		pathcontainer[2].str().c_str(),
		cgd3d9.c_str()
		);
#ifdef DEBUG
	wprintf(reinterpret_cast<LPWSTR>(cgd3d9dest));
#endif
	pathcontainer[4] << (std::wstring(reinterpret_cast<LPWSTR>(airdest))+constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[4].str().c_str());
#endif

	pathcontainer[5] << (std::wstring(reinterpret_cast<LPWSTR>(flashdest))+constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[5].str().c_str());
#endif
	pathcontainer[6] << (std::wstring(reinterpret_cast<LPWSTR>(&tbb)) + constants[0]);
#ifdef DEBUG
	wprintf(pathcontainer[6].str().c_str());
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
	std::wstring unblocks[3] = { pathcontainer[4].str(), pathcontainer[5].str(), pathcontainer[6].str() };
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