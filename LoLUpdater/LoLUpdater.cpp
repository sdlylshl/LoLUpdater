#include <sstream>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <thread>
#include <wininet.h>

class CLimitSingleInstance
{
protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;

public:
	CLimitSingleInstance(std::wstring const& strMutexName)
	{
		m_hMutex = CreateMutex(nullptr, 0, strMutexName.c_str());
		m_dwLastError = GetLastError();
	}

	~CLimitSingleInstance()
	{
		if (m_hMutex)
		{
			CloseHandle(m_hMutex);
			m_hMutex = nullptr;
		}
	}

	BOOL IsAnotherInstanceRunning()
	{
		return (ERROR_ALREADY_EXISTS == m_dwLastError);
	}
};

CLimitSingleInstance g_SingleInstanceObj(L"Global\\{101UPD473R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCH}");

bool finished = false;
wchar_t loldir[MAX_PATH + 1];
wchar_t unblocker[MAX_PATH + 1] = { 0 };
wchar_t* cwd(_wgetcwd(nullptr, 0));
const std::wstring unblocktag = L":Zone.Identifier";
const std::wstring airsetup = L"air16_win.exe";
const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";
wchar_t gameclient[MAX_PATH + 1] = { 0 };

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	if (!URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr) == S_OK)
	{
		throw std::runtime_error("failed to initialize download");
	}
}

void AdobeAirDL()
{
	downloadFile(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air16_win.exe", airsetup.c_str());
}

void copyerrorcheck(BOOL res)
{
	if (res = NULL)
		throw std::runtime_error("failed to copy file");
}

void UnblockFile(std::wstring const& filename)
{
	*unblocker = '\0';
	wcsncat_s(unblocker, MAX_PATH + 1, loldir, _TRUNCATE);
	wcsncat_s(unblocker, MAX_PATH + 1, filename.c_str(), _TRUNCATE);
	wcsncat_s(unblocker, MAX_PATH + 1, unblocktag.c_str(), _TRUNCATE);
	DeleteFile(unblocker);
}

void ExtractResource(int RCDATAID, std::wstring const& filename)
{
	FILE* f;
	auto hRes = FindResource(nullptr, MAKEINTRESOURCE(RCDATAID), RT_RCDATA);
	_wfopen_s(&f, filename.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);
}

void threadingbuildingblocks()
{
	wchar_t tbb[MAX_PATH + 1] = { 0 };
	PathCombine(tbb, gameclient, L"tbb.dll");

	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = L"";
	DWORD dwLength = sizeof(finalurl);
	wchar_t tbbname[INTERNET_MAX_URL_LENGTH] = L"";

	OSVERSIONINFO osvi{};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"XP.dll", _TRUNCATE);
	}
	else
	{
		int abcd[4];
		uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);

		__cpuidex(abcd, 7, 0);
		if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
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
			else
		{
			wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"AVX2.dll", _TRUNCATE);
		}

	}
	UrlCombine(L"http://lol.jdhpro.com/", tbbname, finalurl, &dwLength, 0);
	downloadFile(finalurl, tbb);
	UnblockFile(tbb);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT start = { 2, 0, 0, 0 };
	RECT end = { 2, 20, 0, 0 };
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, L"In Progress, Please Wait!", -1, &start, DT_SINGLELINE | DT_NOCLIP);
		if (finished)
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
	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
		return 0;

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

	HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 260, 100, nullptr, nullptr, hInstance, nullptr);
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select your (League of Legends)/GarenaLoL installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidl, loldir);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	std::thread t{ AdobeAirDL };
	t.join();

	wchar_t runair[MAX_PATH + 1] = { 0 };
	PathCombine(runair, cwd, airsetup.c_str());
	DeleteFile(std::wstring(runair + unblocktag).c_str());

	SHELLEXECUTEINFO ei = {};
	ei.cbSize = sizeof(SHELLEXECUTEINFO);
	ei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ei.lpVerb = L"runas";
	ei.lpFile = runair;
	ei.lpParameters = L"-silent";
	ei.nShow = SW_SHOW;

	if (!ShellExecuteEx(&ei))
		throw std::runtime_error("failed to execute the Adobe AIR Installer");

	while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1,
		&ei.hProcess,
		FALSE,
		INFINITE,
		QS_ALLINPUT
		)
		)
	{
		while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Msg);
		}
	}
	DeleteFile(airsetup.c_str());

	ExtractResource(1, cgsetup.c_str());
	wchar_t runcg[MAX_PATH + 1] = { 0 };
	DeleteFile(std::wstring(runcg + unblocktag).c_str());
	PathCombine(runcg, cwd, cgsetup.c_str());

	SHELLEXECUTEINFO ei1 = {};
	ei1.cbSize = sizeof(SHELLEXECUTEINFO);
	ei1.fMask = SEE_MASK_NOCLOSEPROCESS;
	ei1.lpVerb = L"runas";
	ei1.lpFile = runcg;
	ei1.lpParameters = L"/verysilent /TYPE = compact";
	ei1.nShow = SW_SHOW;

	if (!ShellExecuteEx(&ei1))
		throw std::runtime_error("failed to execute the NvidiaCG Installer");

	while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1,
		&ei1.hProcess,
		FALSE,
		INFINITE,
		QS_ALLINPUT
		)
		)
	{
		while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Msg);
		}
	}
	DeleteFile(cgsetup.c_str());

	wchar_t cgbinpath[MAX_PATH + 1];

	wchar_t progdrive[MAX_PATH + 1];
	SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive);

	wchar_t adobedir[MAX_PATH + 1] = L"Adobe AIR\\Versions\\1.0";

	wchar_t adobepath[MAX_PATH + 1] = { 0 };
	PathCombine(adobepath, progdrive, adobedir);

	wcsncat_s(gameclient, MAX_PATH + 1, loldir, _TRUNCATE);

	wchar_t airclient[MAX_PATH + 1] = { 0 };
	wcsncat_s(airclient, MAX_PATH + 1, loldir, _TRUNCATE);

	wchar_t instdir[MAX_PATH + 1] = { 0 };
	PathCombine(instdir, loldir, L"lol.exe");

	if (std::wifstream(instdir).fail())
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
	else
	{
		PathAppend(gameclient, L"Game");
		wchar_t garenaair[MAX_PATH + 1] = L"Air\\";
		wcsncat_s(garenaair, MAX_PATH + 1, adobedir, _TRUNCATE);
		PathAppend(airclient, garenaair);
	}

	GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, MAX_PATH + 1);

	wchar_t cgbin[MAX_PATH + 1] = L"";
	auto cg = L"cg.dll";
	PathCombine(cgbin, cgbinpath, cg);

	wchar_t cgglbin[MAX_PATH + 1] = L"";
	auto cggl = L"cgGL.dll";
	PathCombine(cgglbin, cgbinpath, cggl);

	wchar_t cgd3d9bin[MAX_PATH + 1] = L"";
	auto cgd3d9 = L"cgD3D9.dll";
	PathCombine(cgd3d9bin, cgbinpath, cgd3d9);

	wchar_t airdest[MAX_PATH + 1] = L"";
	auto air = L"Adobe AIR.dll";
	PathCombine(airdest, airclient, air);

	wchar_t airlatest[MAX_PATH + 1] = L"";
	PathCombine(airlatest, adobepath, air);

	wchar_t res[MAX_PATH + 1] = L"Resources";
	auto flash = res;
	PathAppend(flash, L"NPSWF32.dll");

	wchar_t flashdest[MAX_PATH + 1] = L"";
	PathCombine(flashdest, airclient, flash);

	wchar_t flashlatest[MAX_PATH + 1] = L"";
	PathCombine(flashlatest, adobepath, flash);

	wchar_t cgdest1[MAX_PATH + 1] = L"";
	auto cgdest = cgdest1;
	PathCombine(cgdest, gameclient, cg);

	wchar_t cggldest1[MAX_PATH + 1] = L"";
	auto cggldest = cggldest1;
	PathCombine(cggldest, gameclient, cggl);

	wchar_t cgd3d9dest1[MAX_PATH + 1] = L"";
	auto cgd3d9dest = cgd3d9dest1;
	PathCombine(cgd3d9dest, gameclient, cgd3d9);

	std::thread t1{ threadingbuildingblocks };
	t1.join();

	copyerrorcheck(CopyFile(cgbin, cgdest, false));
	copyerrorcheck(CopyFile(cgglbin, cggldest, false));
	copyerrorcheck(CopyFile(cgd3d9bin, cgd3d9dest, false));
	copyerrorcheck(CopyFile(airlatest, airdest, false));
	copyerrorcheck(CopyFile(flashlatest, flashdest, false));

	const std::wstring ccp = L"msvcp120.dll";
	wchar_t cp[MAX_PATH + 1] = { 0 };
	PathCombine(cp, gameclient, ccp.c_str());
	ExtractResource(2, cp);
	copyerrorcheck(CopyFile(ccp.c_str(), ccp.c_str(), false));
	DeleteFile(ccp.c_str());
	UnblockFile(cp);

	const std::wstring ccr = L"msvcr120.dll";
	wchar_t cr[MAX_PATH + 1] = { 0 };
	PathCombine(cr, gameclient, ccr.c_str());
	ExtractResource(3, cr);
	copyerrorcheck(CopyFile(ccr.c_str(), cr, false));
	DeleteFile(ccr.c_str());
	UnblockFile(cr);

	finished = true;

	RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}