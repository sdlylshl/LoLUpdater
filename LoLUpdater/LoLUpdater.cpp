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
// 1 = currentdir
// 2 = adobe install dir
// 3 = gameclientfull
// 4 = airclientfull
// 5 = unblock air
// 6 = unblock flash
// 7 = unblock tbb

std::wstringstream pathcontainer[8];
const std::wstring constants[3] = { L":Zone.Identifier", L"deploy", L"Air\\Adobe AIR\\Versions\\1.0" };
std::wstring *tbb = nullptr;
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

void download(const std::wstring fromurl, const std::wstring dest, const std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		fromurl.c_str(),
		dest.c_str(),
		0,
		nullptr
		);
	std::wstring *unblocker = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(unblocker), pathcontainer[1].str().c_str(), dest.c_str());
	pathcontainer[0] << (reinterpret_cast<LPWSTR>(&unblocker) + constants[0]);
	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring());
	pathcontainer[0].clear();
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = L"runas";
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


	std::wstring buffer_1[MAX_PATH] = { L"projects" };
	std::wstring *lpStr1;
	lpStr1 = buffer_1;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr1), L"lol_air_client");

	const std::wstring rads(L"RADS");

	std::wstring buffer_2[MAX_PATH] = { rads };
	std::wstring *lpStr2;
	lpStr2 = buffer_2;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr2), reinterpret_cast<LPWSTR>(lpStr1));

	std::wstring buffer_3[MAX_PATH] = { L"solutions" };
	std::wstring *lpStr3;
	lpStr3 = buffer_3;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr3), L"lol_game_client_sln");

	std::wstring buffer_4[MAX_PATH] = { rads };
	std::wstring *lpStr4;
	lpStr4 = buffer_4;

	PathAppend(reinterpret_cast<LPWSTR>(lpStr4), reinterpret_cast<LPWSTR>(lpStr3));


	const std::wstring rel(L"releases");



	std::wstring buffer_5[MAX_PATH] = { reinterpret_cast<LPWSTR>(lpStr4) };
	std::wstring *gameclient;
	gameclient = buffer_5;

	PathAppend(reinterpret_cast<LPWSTR>(gameclient), rel.c_str());

	std::wstring buffer_6[MAX_PATH] = { reinterpret_cast<LPWSTR>(lpStr2) };
	std::wstring *airclient;
	airclient = buffer_6;

	PathAppend(reinterpret_cast<LPWSTR>(airclient), rel.c_str());


	std::vector<std::wstring> currentdirectorybuffer(MAX_PATH, std::wstring());
	GetModuleFileName(nullptr, reinterpret_cast<LPWSTR>(&currentdirectorybuffer[0]), MAX_PATH);


	// pathcontainer[1] << (cwdfinal.substr(0, cwdfinal.find_last_of(L"\\/")) + L"\\");
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE=compact");
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
	pathcontainer[2] << pathcontainer[1].str().c_str()[0];
	int bit = sizeof(void*);

	std::wstring progfiles = std::wstring(L":") + std::wstring(L"Program Files");


	if (bit == 8)
	{
		pathcontainer[2] << progfiles + std::wstring(L" (x86)");
	}
	else
	{
		pathcontainer[2] << progfiles;
	}
	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", L"air15_win.exe", L"-silent");

	std::wstring *common = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(common),
		L"Common Files",
		L"Adobe "
		);
	pathcontainer[2] << common;

	const std::wstring cg(L"Cg.dll");
	std::wstring *cgbin = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cg.c_str()
		);
	const std::wstring cggl(L"CgGL.dll");
	std::wstring *cgglbin = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgglbin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cggl.c_str()
		);
	const std::wstring cgd3d9(L"CgD3D9.dll");
	std::wstring *cgd3d9bin = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9bin),
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		cgd3d9.c_str()
		);



	std::wstring buffer_7[MAX_PATH] = { reinterpret_cast<LPWSTR>(gameclient) };
	std::wstring *random7;
	random7 = buffer_7;

	PathAppend(reinterpret_cast<LPWSTR>(random7), L"0.0.1.64");

	std::wstring buffer_8[MAX_PATH] = { reinterpret_cast<LPWSTR>(gameclient) };
	std::wstring *version;
	version = buffer_8;

	PathAppend(reinterpret_cast<LPWSTR>(random7), constants[1].c_str());


	std::wstring buffer_9[MAX_PATH] = { reinterpret_cast<LPWSTR>(airclient) };
	std::wstring *random9;
	random9 = buffer_9;

	PathAppend(reinterpret_cast<LPWSTR>(random9), L"0.0.1.117");

	std::wstring buffer_10[MAX_PATH] = { reinterpret_cast<LPWSTR>(random9) };
	std::wstring *airversion;
	airversion = buffer_10;

	PathAppend(reinterpret_cast<LPWSTR>(airversion), constants[1].c_str());


	std::wifstream garena(L"lol.exe");

	if (garena.good())
	{
		version[0] = L"Game";
		airversion[0] = constants[2];
	}

	pathcontainer[3] << (pathcontainer[1].str() + reinterpret_cast<LPWSTR>(version));
	pathcontainer[4] << (pathcontainer[1].str() + reinterpret_cast<LPWSTR>(airversion));


	PathCombine(
		reinterpret_cast<LPWSTR>(tbb),
		pathcontainer[3].str().c_str(),
		L"tbb.dll"
		);

	const std::wstring air(L"Adobe AIR.dll");
	std::wstring *airdest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airdest),
		pathcontainer[4].str().c_str(),
		air.c_str()
		);

	std::wstring *airlatest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		pathcontainer[2].str().c_str(),
		air.c_str()
		);



	std::wstring buffer_11[MAX_PATH] = { reinterpret_cast<LPWSTR>(L"Resources") };
	std::wstring *flash;
	flash = buffer_11;

	PathAppend(reinterpret_cast<LPWSTR>(flash), L"NPSWF32.dll");


	std::wstring *flashdest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(flashdest),
		pathcontainer[4].str().c_str(),
		reinterpret_cast<LPWSTR>(flash)
		);

	std::wstring *flashlatest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(flashlatest),
		pathcontainer[2].str().c_str(),
		reinterpret_cast<LPWSTR>(flash)
		);


	std::wstring *cgdest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgdest),
		pathcontainer[3].str().c_str(),
		cg.c_str()
		);

	std::wstring *cggldest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cggldest),
		pathcontainer[3].str().c_str(),
		cggl.c_str()
		);

	std::wstring *cgd3d9dest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9dest),
		pathcontainer[3].str().c_str(),
		cgd3d9.c_str()
		);

	pathcontainer[5] << (reinterpret_cast<LPWSTR>(airdest)+constants[0]);


	pathcontainer[6] << (reinterpret_cast<LPWSTR>(flashdest)+constants[0]);

	pathcontainer[7] << (reinterpret_cast<LPWSTR>(tbb)+constants[0]);

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	{
		tbbdownload(L"Xp.dll");
	}
	else
	{
		int abcd[4];
		uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
		uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);
		// /* breaks in debug */ uint32_t xcr0(static_cast<uint32_t>(_xgetbv(0)));

		__cpuidex(abcd, 1, 0);
		__cpuidex(abcd, 7, 0);
		__cpuidex(abcd, 0x80000001, 0);
		if (((abcd[2] & fma_movbe_osxsave_mask) == fma_movbe_osxsave_mask) || /* ((xcr0 & 6) == 6) || */ ((abcd[1] & avx2_bmi12_mask) == avx2_bmi12_mask) || ((abcd[2] & (1 << 5)) != 0))
		{
			tbbdownload(L"Avx2.dll");
		}
		else
		{
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
	}

	CopyFile(reinterpret_cast<LPWSTR>(airlatest), reinterpret_cast<LPWSTR>(airdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(flashlatest), reinterpret_cast<LPWSTR>(flashdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgbin), reinterpret_cast<LPWSTR>(cgdest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgglbin), reinterpret_cast<LPWSTR>(cggldest), false);
	CopyFile(reinterpret_cast<LPWSTR>(cgd3d9bin), reinterpret_cast<LPWSTR>(cgd3d9dest), false);
	std::wstring unblocks[3] = { pathcontainer[5].str(), pathcontainer[6].str(), pathcontainer[7].str() };
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