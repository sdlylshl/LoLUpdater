#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <Shlwapi.h>
#include <direct.h>
#include <Shlobj.h>
#include <wininet.h>

bool done = false;
std::wstringstream pathcontainer[4];
const std::wstring constants[3] = { std::wstring(L":Zone.Identifier"), std::wstring(L"Adobe AIR\\Versions\\1.0"), std::wstring(L"AIR\\") };
wchar_t* tbb;
wchar_t* cwd(_wgetcwd(nullptr, 0));
wchar_t unblocker1[MAX_PATH + 1] = L"";
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

	wchar_t* unblocker;
	unblocker = unblocker1;

	wchar_t* unblocker2;
	wchar_t unblocker21[MAX_PATH + 1] = L"";
	unblocker2 = unblocker21;

	wcsncat_s(
		unblocker2,
		MAX_PATH + 1,
		file.c_str(),
		_TRUNCATE
		);

	PathAppend(
		unblocker,
		unblocker2
		);

	pathcontainer[0] << (std::wstring(unblocker) + constants[0]);
	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring());
	pathcontainer[0].clear();
	*unblocker1 = '\0';
	*unblocker21 = '\0';
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

void downloadtbb(const std::wstring& file)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEXW wc;
	std::wstring g_szClassName(L"mainwindow1");
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
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName.c_str(),
		L"LoLUpdater",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 240,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, nCmdShow);
	download(std::wstring(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"), std::wstring(L"air15_win.exe"), std::wstring(L"-silent"));
	download(std::wstring(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"), std::wstring(L"Cg-3.1_April2012_Setup.exe"), std::wstring(L"/verysilent /TYPE = compact"));
	wchar_t progdrive[MAX_PATH + 1];
	SHGetFolderPath(nullptr,
		CSIDL_PROGRAM_FILES_COMMON,
		nullptr,
		0,
		progdrive);
	std::vector<std::wstring> cgbinpath(MAX_PATH + 1, std::wstring());
	GetEnvironmentVariable(L"CG_BIN_PATH",
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH + 1);
	wcsncat_s(
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		MAX_PATH + 1,
		L"\\",
		_TRUNCATE
		);
	wcsncat_s(
		unblocker1,
		MAX_PATH,
		&cwd[0],
		_TRUNCATE
		);
	wchar_t airclient1[MAX_PATH + 1] = L"";
	wcsncat_s(
		airclient1,
		MAX_PATH + 1,
		&cwd[0],
		_TRUNCATE
		);
	wchar_t gameclient1[MAX_PATH + 1] = L"";
	wcsncat_s(
		gameclient1,
		MAX_PATH + 1,
		&cwd[0],
		_TRUNCATE
		);
	wchar_t* airclient;
	wchar_t* gameclient;
	airclient = airclient1;
	gameclient = gameclient1;
	std::wifstream garena(L"lol.exe");
	if (garena.good())
	{
		PathAppend(gameclient, L"Game");
		wchar_t garenaair1[MAX_PATH + 1] = L"";
		wchar_t* garenaair;

		wcsncat_s(
			garenaair1,
			MAX_PATH + 1,
			constants[2].c_str(),
			_TRUNCATE
			);
		garenaair = garenaair1;
		PathAppend(airclient, garenaair);
	}
	wchar_t* rads = L"RADS";
	wchar_t* rel = L"releases";
	PathAppend(airclient, rads);
	PathAppend(airclient, L"projects");
	PathAppend(airclient, L"lol_air_client");
	PathAppend(airclient, rel);
	PathAppend(gameclient, rads);
	PathAppend(gameclient, L"solutions");
	PathAppend(gameclient, L"lol_game_client_sln");
	PathAppend(gameclient, rel);
	wchar_t* adobepath;
	wchar_t adobepath1[MAX_PATH + 1] = L"";
	adobepath = adobepath1;
	wchar_t* commonfiles;
	wchar_t commonfiles1[MAX_PATH + 1] = L"";
	wcsncat_s(
		commonfiles1,
		MAX_PATH + 1,
		progdrive,
		_TRUNCATE
		);

	commonfiles = commonfiles1;
	wchar_t* adobedir;
	wchar_t adobedir1[MAX_PATH + 1] = L"";
	adobedir = adobedir1;

	wcsncat_s(
		adobedir,
		MAX_PATH + 1,
		constants[1].c_str(),
		_TRUNCATE
		);

	PathCombine(
		adobepath,
		commonfiles,
		adobedir
		);

	wchar_t* cgbasepath;
	wchar_t cgbasepath1[MAX_PATH + 1] = L"";
	wcsncat_s(
		cgbasepath1,
		MAX_PATH + 1,
		reinterpret_cast<LPWSTR>(&cgbinpath[0]),
		_TRUNCATE
		);
	cgbasepath = cgbasepath1;

	wchar_t* cg = L"Cg.dll";
	wchar_t* cgbin;
	wchar_t cgbin1[MAX_PATH + 1] = L"";
	cgbin = cgbin1;
	PathCombine(
		cgbin,
		cgbasepath,
		cg
		);
	wchar_t* cgglbin;
	wchar_t cgglbin1[MAX_PATH + 1] = L"";
	cgglbin = cgglbin1;
	wchar_t* cggl = L"CgGL.dll";
	PathCombine(
		cgglbin,
		cgbasepath,
		cggl
		);
	wchar_t* cgd3d9bin;
	wchar_t cgd3d9bin1[MAX_PATH + 1] = L"";
	cgd3d9bin = cgd3d9bin1;
	wchar_t* cgd3d9 = L"CgD3D9.dll";
	PathCombine(
		cgd3d9bin,
		cgbasepath,
		cgd3d9
		);

	wchar_t* dep = L"deploy";
	PathAppend(gameclient, L"0.0.1.64");
	PathAppend(gameclient, dep);
	PathAppend(airclient, L"0.0.1.117");
	PathAppend(airclient, dep);
	PathAppend(airclient, adobedir);

	wchar_t tbb1[MAX_PATH + 1] = L"";
	tbb = tbb1;
	PathCombine(tbb, gameclient, L"tbb.dll");
	wchar_t* airdest;
	wchar_t airdest1[MAX_PATH + 1] = L"";
	airdest = airdest1;
	wchar_t* air = L"Adobe AIR.dll";
	PathCombine(
		airdest,
		airclient,
		air
		);
	wchar_t* airlatest1;
	wchar_t airlatest[MAX_PATH + 1] = L"";
	airlatest1 = airlatest;
	PathCombine(
		airlatest,
		adobepath,
		air
		);

	wchar_t res[MAX_PATH + 1] = L"Resources";
	wchar_t* flash;
	flash = res;
	PathAppend(flash, L"NPSWF32.dll");

	wchar_t* flashdest;
	wchar_t flashdest1[MAX_PATH + 1] = L"";
	flashdest = flashdest1;
	PathCombine(
		flashdest,
		airclient,
		flash
		);
	wchar_t* flashlatest;
	wchar_t flashlatest1[MAX_PATH + 1] = L"";
	flashlatest = flashlatest1;
	PathCombine(
		flashlatest,
		adobepath,
		flash
		);
	wchar_t* cgdest;
	wchar_t cgdest1[MAX_PATH + 1] = L"";
	cgdest = cgdest1;
	PathCombine(
		cgdest,
		gameclient,
		cg
		);
	wchar_t* cggldest;
	wchar_t cggldest1[MAX_PATH + 1] = L"";
	cggldest = cggldest1;
	PathCombine(
		cggldest,
		gameclient,
		cggl
		);

	wchar_t* cgd3d9dest;
	wchar_t cgd3d9dest1[MAX_PATH + 1] = L"";
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
		downloadtbb(L"Xp.dll");
	}
	else
	{
		int avx2;
		int abcd[4];
		uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
		uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);
		__cpuidex(abcd, 1, 0);
		if (((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask) || !((static_cast<uint32_t>(_xgetbv(0)) & 6) == 6))
		{
			avx2 = 0;
		}
		__cpuidex(abcd, 7, 0);
		if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		{
			avx2 = 0;
		}
		__cpuidex(abcd, 0x80000001, 0);
		if ((abcd[2] & (1 << 5)) == 0)
		{
			avx2 = 0;
		}
		if (avx2 != 0)
		{
			downloadtbb(L"Avx2.dll");
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if (((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false)) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) == 6))
			{
				downloadtbb(L"Avx.dll");
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					downloadtbb(L"Sse2.dll");
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						downloadtbb(L"Sse.dll");
					}
					else
					{
						downloadtbb(L"Default.dll");
					}
				}
			}
		}
	}
	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);

	std::wstring unblocks[3]{ pathcontainer[1].str(), pathcontainer[2].str(), pathcontainer[3].str() };
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