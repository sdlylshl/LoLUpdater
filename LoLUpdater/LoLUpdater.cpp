#include <sstream>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <thread>
#include <wininet.h>

wchar_t path[MAX_PATH+1];
bool finished = false;
wchar_t unblocker1[MAX_PATH+1] = L"";
auto unblocker = unblocker1;

void RunAndWait(std::wstring const& file, std::wstring const& args)
{
	SHELLEXECUTEINFO ei = {};
	ei.cbSize = sizeof(SHELLEXECUTEINFO);
	ei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ei.lpVerb = L"runas";
	ei.lpFile = file.c_str();
	ei.lpParameters = args.c_str();
	ei.nShow = SW_SHOW;
	ShellExecuteEx(&ei);
	WaitForSingleObject(ei.hProcess, INFINITE);
}

void UnblockFile(std::wstring const& path1)
{
	*unblocker = '\0';
	wcsncat_s(unblocker, MAX_PATH+1, path, _TRUNCATE);
	wcsncat_s(unblocker, MAX_PATH+1, path1.c_str(), _TRUNCATE);
	wcsncat_s(unblocker, MAX_PATH+1, L":Zone.Identifier", _TRUNCATE);
	DeleteFile(unblocker);
}

void patch()
{
	const std::wstring airsetup = L"air16_win.exe";
	const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";

	URLDownloadToFile(nullptr, L"http://labsdownload.adobe.com/pub/labs/flashruntimes/air/air16_win.exe", airsetup.c_str(), 0, nullptr);

	FILE* f;
	auto hRes = FindResource(nullptr, MAKEINTRESOURCE(1), RT_RCDATA);
	_wfopen_s(&f, cgsetup.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);

	wchar_t gameclient1[MAX_PATH+1] = L"";
	auto gameclient = gameclient1;
	wcsncat_s(gameclient, MAX_PATH+1, path, _TRUNCATE);

	wchar_t airclient1[MAX_PATH+1] = L"";
	auto airclient = airclient1;
	wcsncat_s(airclient, MAX_PATH+1, path, _TRUNCATE);

	UnblockFile(airsetup);
	RunAndWait(airsetup, L"-silent");
	DeleteFile(airsetup.c_str());

	wchar_t progdrive[MAX_PATH+1];
	SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive);

	wchar_t adobedir[MAX_PATH+1] = L"Adobe AIR\\Versions\\1.0";

	wchar_t adobepath1[MAX_PATH+1] = L"";
	auto adobepath = adobepath1;
	PathCombine(adobepath, progdrive, adobedir);

	wchar_t instdir1[MAX_PATH+1] = L"";
	auto instdir = instdir1;
	PathCombine(instdir, path, L"lol.exe");

	if (std::wifstream(instdir).good())
	{
		PathAppend(gameclient, L"Game");
		wchar_t garenaair1[MAX_PATH+1] = L"Air\\";
		auto garenaair = garenaair1;
		wcsncat_s(garenaair, MAX_PATH+1, adobedir, _TRUNCATE);
		PathAppend(airclient, garenaair);
	}
	else
	{
		auto rads = L"RADS";
		PathAppend(airclient, rads);
		PathAppend(airclient, L"projects");
		PathAppend(airclient, L"lol_air_client");
		auto rel = L"releases";
		PathAppend(airclient, rel);
		PathAppend(gameclient, rads);
		PathAppend(gameclient, L"solutions");
		PathAppend(gameclient, L"lol_game_client_sln");
		PathAppend(gameclient, rel);
		PathAppend(gameclient, L"0.0.1.73");
		auto dep = L"deploy";
		PathAppend(gameclient, dep);
		PathAppend(airclient, L"0.0.1.127");
		PathAppend(airclient, dep);
		PathAppend(airclient, adobedir);
	}

	wchar_t cgbinpath[MAX_PATH+1];
	UnblockFile(cgsetup);
	RunAndWait(cgsetup, L"/verysilent /TYPE = compact");
	DeleteFile(cgsetup.c_str());

	GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, MAX_PATH+1);

	wchar_t cgbin1[MAX_PATH+1] = L"";
	auto cgbin = cgbin1;
	auto cg = L"\\Cg.dll";
	PathCombine(cgbin, cgbinpath, cg);

	wchar_t cgglbin1[MAX_PATH+1] = L"";
	auto cgglbin = cgglbin1;
	auto cggl = L"\\CgGL.dll";
	PathCombine(cgglbin, cgbinpath, cggl);

	wchar_t cgd3d9bin1[MAX_PATH+1] = L"";
	auto cgd3d9bin = cgd3d9bin1;
	auto cgd3d9 = L"\\CgD3D9.dll";
	PathCombine(cgd3d9bin, cgbinpath, cgd3d9);

	wchar_t airdest1[MAX_PATH+1] = L"";
	auto airdest = airdest1;
	auto air = L"Adobe AIR.dll";
	PathCombine(airdest, airclient, air);

	wchar_t airlatest1[MAX_PATH+1] = L"";
	auto airlatest = airlatest1;
	PathCombine(airlatest, adobepath, air);

	wchar_t res[MAX_PATH+1] = L"Resources";
	auto flash = res;
	PathAppend(flash, L"NPSWF32.dll");

	wchar_t flashdest1[MAX_PATH+1] = L"";
	auto flashdest = flashdest1;
	PathCombine(flashdest, airclient, flash);

	wchar_t flashlatest1[MAX_PATH+1] = L"";
	auto flashlatest = flashlatest1;
	PathCombine(flashlatest, adobepath, flash);

	wchar_t cgdest1[MAX_PATH+1] = L"";
	auto cgdest = cgdest1;
	PathCombine(cgdest, gameclient, cg);

	wchar_t cggldest1[MAX_PATH+1] = L"";
	auto cggldest = cggldest1;
	PathCombine(cggldest, gameclient, cggl);

	wchar_t cgd3d9dest1[MAX_PATH+1] = L"";
	auto cgd3d9dest = cgd3d9dest1;
	PathCombine(cgd3d9dest, gameclient, cgd3d9);

	wchar_t tbb1[MAX_PATH+1] = L"";
	auto tbb = tbb1;
	PathCombine(tbb, gameclient, L"tbb.dll");

	OSVERSIONINFO osvi{};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = L"";
	DWORD dwLength = sizeof(finalurl);
	wchar_t tbbname[INTERNET_MAX_URL_LENGTH] = L"";

	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"XP.dll", _TRUNCATE);
	}
	else
	{
		auto avx2 = 0;
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
			wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"AVX2.dll", _TRUNCATE);
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if (((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false)) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) == 6))
			{
				wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"AVX.dll", _TRUNCATE);
			}
			else
			{
				wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"SSE2.dll", _TRUNCATE);
			}
		}
	}
	UrlCombine(L"http://lol.jdhpro.com/", tbbname, finalurl, &dwLength, 0);
	URLDownloadToFile(nullptr, finalurl, tbb, 0, nullptr);
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);
	UnblockFile(tbb);
	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);

	FILE* f1;
	auto hRes1 = FindResource(nullptr, MAKEINTRESOURCE(2), RT_RCDATA);
	const std::wstring ccp = L"msvcp120.dll";
	_wfopen_s(&f1, ccp.c_str(), L"wb");
	wchar_t cp1[MAX_PATH+1] = L"";
	auto cp = cp1;
	PathCombine(cp, gameclient, ccp.c_str());
	fwrite(LockResource(LoadResource(nullptr, hRes1)), SizeofResource(nullptr, hRes1), 1, f1);
	fclose(f1);
	CopyFile(ccp.c_str(), cp, false);
	DeleteFile(ccp.c_str());
	UnblockFile(cp);

	FILE* f2;
	auto hRes2 = FindResource(nullptr, MAKEINTRESOURCE(3), RT_RCDATA);
	const std::wstring ccr = L"msvcr120.dll";
	_wfopen_s(&f2, ccr.c_str(), L"wb");
	wchar_t cr1[MAX_PATH+1] = L"";
	auto cr = cr1;
	PathCombine(cr, gameclient, ccr.c_str());
	fwrite(LockResource(LoadResource(nullptr, hRes2)), SizeofResource(nullptr, hRes2), 1, f2);
	fclose(f2);
	CopyFile(ccr.c_str(), cr, false);
	DeleteFile(ccr.c_str());
	UnblockFile(cr);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT start = { 2, 0, 0, 0 };
	RECT end = { 2, 20, 0, 0 };
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, L"In Progress...", -1, &start, DT_SINGLELINE | DT_NOCLIP);
		if (finished == true)
		{
			DrawText(hdc, L"Finished!, Enjoy a better League!", -1, &end, DT_SINGLELINE | DT_NOCLIP);
		}
		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR, int nCmdShow)
{
	MSG Msg = { 0 };
	WNDCLASSEX wc = { 0 };
	const std::wstring g_szClassName(L"mainwindow");
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	RegisterClassEx(&wc);

	HWND hwnd;
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 260, 100, nullptr, nullptr, hInstance, nullptr);
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select your League of Legends/GarenaLoL installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidl, path);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	std::thread t{patch};
	t.join();
	
	finished = true;
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}