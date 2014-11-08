#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#define VC_EXTRALEAN
#include <Windows.h>
#include <Shlwapi.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
bool done = false;
// 0 = intermediate combiner
// 1 = currentdir
// 2 = adobe install dir
std::wstringstream pathcontainer[3];
const std::wstring constants[3] = { L":Zone.Identifier", L"deploy", L"Air\\Adobe AIR\\Versions\\1.0" };

void download(const std::wstring fromurl, const std::wstring dest, const std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		fromurl.c_str(),
		dest.c_str(),
		0,
		nullptr
	);
	wchar_t *unblocker = nullptr;
	PathCombine(unblocker, pathcontainer[1].str().c_str(), dest.c_str());
	pathcontainer[0] << (pathcontainer[1].str() + unblocker + constants[0]);
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
wchar_t *tbb = nullptr;
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
		tbb,
		0,
		nullptr
	);
}

HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = {0, 0, 100, 20};
RECT end = {0, 100, 100, 120};
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

bool garena = std::wifstream(L"lol.exe").good();

wchar_t *gameclientfinal = nullptr;
wchar_t *airclientfinal = nullptr;
std::wstring gamedir()
{
	if (garena)
	{
		return L"Game";
	}
	wchar_t *game1 = nullptr;
	PathCombine(game1, gameclientfinal, L"0.0.1.64");
	wchar_t *game2 = nullptr;
	PathCombine(game2, game1, constants[1].c_str());
	return game2;
}

std::wstring airdir()
{
	if (garena)
	{
		return constants[2];
	}
	wchar_t *air1 = nullptr;
	PathCombine(air1, airclientfinal, L"0.0.1.117");
	wchar_t *air2 = nullptr;
	PathCombine(air2, air1, constants[2].c_str());
	return air2;
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
	wchar_t *airclient = nullptr;
	PathCombine(
		airclient,
		rads.c_str(),
		L"projects\\lol_air_client"
		);
	wchar_t *gameclient = nullptr;
	PathCombine(
		gameclient,
		rads.c_str(),
		L"solutions\\lol_game_client_sln"
		);

	const std::wstring rel(L"releases");

	PathCombine(
		gameclientfinal,
		gameclient,
		rel.c_str()
		);

	PathCombine(
		airclientfinal,
		airclient,
		rel.c_str()
		);

	std::vector<wchar_t> currentdirectorybuffer(MAX_PATH + 1, 0);
	GetModuleFileName(nullptr, &currentdirectorybuffer[0], MAX_PATH + 1);
	pathcontainer[1] << (std::wstring(&currentdirectorybuffer[0]).substr(0, std::wstring(&currentdirectorybuffer[0]).find_last_of(L"\\/")) + L"\\");
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", L"Cg-3.1_April2012_Setup.exe", L"/verysilent /TYPE = compact");
	std::vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);
	GetEnvironmentVariable(L"CG_BIN_PATH",
	                       &cgbinpath[0],
	                       MAX_PATH + 1);
	wcsncat_s(
		&cgbinpath[0],
		MAX_PATH + 1,
		L"\\",
		_TRUNCATE
	);
	pathcontainer[2] << pathcontainer[1].str().c_str()[0];
	int bit = sizeof(void*);
	const std::wstring progfiles(L":\\Program Files");
	if (bit == 8)
	{
		pathcontainer[2] << (progfiles + L" (x86)");
	}
	else
	{
		pathcontainer[2] << (progfiles);
	}
	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", L"air15_win.exe", L"-silent");
	pathcontainer[2] << (L"\\Common Files\\Adobe " + constants[2]);

	const std::wstring cg(L"Cg.dll");
	wchar_t *cgbin = nullptr;
	PathCombine(
		cgbin,
		&cgbinpath[0],
		cg.c_str()
		);
	const std::wstring cggl(L"CgGL.dll");
	wchar_t *cgglbin = nullptr;
	PathCombine(
		cgglbin,
		&cgbinpath[0],
		cggl.c_str()
		);
	const std::wstring cgd3d9(L"CgD3D9.dll");
	wchar_t *cgd3d9bin = nullptr;
	PathCombine(
		cgd3d9bin,
		&cgbinpath[0],
		cgd3d9.c_str()
		);

	wchar_t *gameclientfull = nullptr;
	wchar_t *airclientfull = nullptr;

	PathCombine(
		gameclientfull,
		pathcontainer[1].str().c_str(),
		gamedir().c_str()
		);

	PathCombine(
		airclientfull,
		pathcontainer[1].str().c_str(),
		airdir().c_str()
		);


	PathCombine(
		tbb,
		gameclientfull,
		L"tbb.dll"
		);

	const std::wstring air(L"Adobe AIR.dll");
	wchar_t *airdest= nullptr;
	PathCombine(
		airdest,
		airclientfull,
		air.c_str()
		);

	wchar_t *airlatest = nullptr;
	PathCombine(
		airlatest,
		pathcontainer[2].str().c_str(),
		air.c_str()
		);


	const std::wstring flash(L"Resources\\NPSWF32.dll");
	wchar_t *flashdest = nullptr;
	PathCombine(
		flashdest,
		airclientfinal,
		flash.c_str()
		);

	wchar_t *flashlatest = nullptr;
	PathCombine(
		flashlatest,
		pathcontainer[2].str().c_str(),
		flash.c_str()
		);


	wchar_t *cgdest = nullptr;
	PathCombine(
		cgdest,
		gameclientfinal,
		cg.c_str()
		);

	wchar_t *cggldest = nullptr;
	PathCombine(
		cggldest,
		gameclientfinal,
		cggl.c_str()
		);

	wchar_t *cgd3d9dest = nullptr;
	PathCombine(
		cgd3d9dest,
		gameclientfinal,
		cgd3d9.c_str()
		);


	wchar_t *airunblock = nullptr;
	PathCombine(
		airunblock,
		airdest,
		constants[0].c_str()
		);

	wchar_t *flashunblock = nullptr;
	PathCombine(
		flashunblock,
		flashdest,
		constants[0].c_str()
		);

	wchar_t *tbbunblock = nullptr;
	PathCombine(
		tbbunblock,
		tbb,
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


	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest ,false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);
	const std::wstring unblocks[3] = {tbbunblock, airunblock, flashunblock};
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