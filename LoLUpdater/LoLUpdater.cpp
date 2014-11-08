#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#define VC_EXTRALEAN
#include <Windows.h>
#include <Shlwapi.h>

// Todo: find c++ way
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

bool done = false;
// 0 = intermediate combiner
// 1 = currentdir
// 2 = adobe install dir
std::wstringstream pathcontainer[3];
const std::wstring constants[3] = { L":Zone.Identifier", L"deploy", L"Air\\Adobe AIR\\Versions\\1.0" };
std::wstring *tbb = nullptr;
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };
bool garena = std::wifstream(L"lol.exe").good();
std::wstring *gameclientfinal = nullptr;
std::wstring *airclientfinal = nullptr;

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
	pathcontainer[0] << (pathcontainer[1].str() + reinterpret_cast<LPWSTR>(unblocker) + constants[0]);
	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring(nullptr));
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = dest.c_str();
	ShExecInfo.lpParameters = args.c_str();
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void tbbdownload(const std::wstring file)
{
	std::wstring ftp(L"http://lol.jdhpro.com/");
	wcsncat_s(
		&ftp[0],
		MAX_PATH + 1,
		file.c_str(),
		_TRUNCATE
		);
	URLDownloadToFile(
		nullptr,
		ftp.c_str(),
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

std::wstring gamedir()
{
	if (garena)
	{
		return L"Game";
	}
	std::wstring *game1 = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(game1), reinterpret_cast<LPWSTR>(gameclientfinal), L"0.0.1.64");
	std::wstring *game2 = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(game2), reinterpret_cast<LPWSTR>(game1), constants[1].c_str());
	return reinterpret_cast<LPWSTR>(game2);
}

std::wstring airdir()
{
	if (garena)
	{
		return constants[2];
	}
	std::wstring *air1 = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(air1), reinterpret_cast<LPWSTR>(airclientfinal), L"0.0.1.117");
	std::wstring *air2 = nullptr;
	PathCombine(reinterpret_cast<LPWSTR>(air2), reinterpret_cast<LPWSTR>(air1), constants[2].c_str());
	return reinterpret_cast<LPWSTR>(air2);
}

std::wstring pathcombine(std::wstring path1, std::wstring path2)
{
	std::wstring *out = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(out),
		path1.c_str(),
		path2.c_str()
		);
	return reinterpret_cast<LPWSTR>(out);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
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
	const std::wstring rads(L"RADS");


	std::wstring *airclient = nullptr;

	PathCombine(
		reinterpret_cast<LPWSTR>(airclient),
		rads.c_str(),
		pathcombine(L"projects", L"lol_air_client").c_str()
		);


	std::wstring *gameclient = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(gameclient),
		rads.c_str(),
		pathcombine(L"solutions", L"lol_game_client_sln").c_str()
		);

	const std::wstring rel(L"releases");

	PathCombine(
		reinterpret_cast<LPWSTR>(gameclientfinal),
		reinterpret_cast<LPWSTR>(gameclient),
		rel.c_str()
		);

	PathCombine(
		reinterpret_cast<LPWSTR>(airclientfinal),
		reinterpret_cast<LPWSTR>(airclient),
		rel.c_str()
		);

	std::vector<std::wstring> currentdirectorybuffer(MAX_PATH + 1, nullptr);
	GetModuleFileName(nullptr, reinterpret_cast<LPWSTR>(&currentdirectorybuffer[0]), MAX_PATH + 1);
	pathcontainer[1] << currentdirectorybuffer[0].substr(0, static_cast<size_t>(currentdirectorybuffer[0].find_last_of(L"\\/"))) + std::wstring(L"\\");
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE = compact");
	std::vector<std::wstring> cgbinpath(MAX_PATH + 1, nullptr);
	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
	                       MAX_PATH + 1);
	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH + 1,
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


	pathcontainer[2] << pathcombine(L"Common Files", L"Adobe ");

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

	std::wstring *gameclientfull = nullptr;


	PathCombine(
		reinterpret_cast<LPWSTR>(gameclientfull),
		pathcontainer[1].str().c_str(),
		gamedir().c_str()
		);

	std::wstring *airclientfull = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airclientfull),
		pathcontainer[1].str().c_str(),
		airdir().c_str()
		);


	PathCombine(
		reinterpret_cast<LPWSTR>(tbb),
		reinterpret_cast<LPWSTR>(gameclientfull),
		L"tbb.dll"
		);

	const std::wstring air(L"Adobe AIR.dll");
	std::wstring *airdest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airdest),
		reinterpret_cast<LPWSTR>(airclientfull),
		air.c_str()
		);

	std::wstring *airlatest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airlatest),
		pathcontainer[2].str().c_str(),
		air.c_str()
		);


	std::wstring *flash = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(flash),
		L"Resources",
		L"NPSWF32.dll"
		);

	std::wstring *flashdest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(flashdest),
		reinterpret_cast<LPWSTR>(airclientfinal),
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
		reinterpret_cast<LPWSTR>(gameclientfinal),
		cg.c_str()
		);

	std::wstring *cggldest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cggldest),
		reinterpret_cast<LPWSTR>(gameclientfinal),
		cggl.c_str()
		);

	std::wstring *cgd3d9dest = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(cgd3d9dest),
		reinterpret_cast<LPWSTR>(gameclientfinal),
		cgd3d9.c_str()
		);


	std::wstring *airunblock = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(airunblock),
		reinterpret_cast<LPWSTR>(airdest),
		constants[0].c_str()
		);

	std::wstring *flashunblock = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(flashunblock),
		reinterpret_cast<LPWSTR>(flashdest),
		constants[0].c_str()
		);

	std::wstring *tbbunblock = nullptr;
	PathCombine(
		reinterpret_cast<LPWSTR>(tbbunblock),
		reinterpret_cast<LPWSTR>(tbb),
		constants[0].c_str()
		);

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
		static int canuseavx2 = 1;
		uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
		uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);
		uint32_t xcr0(static_cast<uint32_t>(_xgetbv(0)));

		__cpuidex(abcd, 1, 0);
		if (((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask) || (!(xcr0 & 6) == 6))
		{
			canuseavx2 = -1;
		}

		__cpuidex(abcd, 7, 0);
		if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		{
			canuseavx2 = -1;
		}

		__cpuidex(abcd, 0x80000001, 0);
		if ((abcd[2] & (1 << 5)) == 0)
		{
			canuseavx2 = -1;
		}
		if (canuseavx2 > 0)
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
	const std::wstring unblocks[3] = { reinterpret_cast<LPWSTR>(tbbunblock), reinterpret_cast<LPWSTR>(airunblock), reinterpret_cast<LPWSTR>(flashunblock) };
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