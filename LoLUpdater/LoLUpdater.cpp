// Minimum supported processor = Intel Pentium 4 (Or anything with at least SSE2)
// Supported Windows Versions: Windows XP SP3 -> Windows 10 Preview (Build 9926)
// Windows Server is currently not supported
// Also unsupported on Wine (Will fix this)

#include <sstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <thread>
#include <wininet.h>
#include <fstream>
#include <stdint.h>
#include <intrin.h>

class CLimitSingleInstance
{
protected:
	DWORD m_dwLastError;
	HANDLE m_hMutex;

public:
	explicit CLimitSingleInstance(std::wstring const& strMutexName)
	{
		m_hMutex = CreateMutex(nullptr, 0, strMutexName.c_str());
		if (m_hMutex == nullptr)
			throw std::runtime_error("failed to create mutex");

		m_dwLastError = GetLastError();
	}

	~CLimitSingleInstance()
	{
		if (m_hMutex)
		{
			if (CloseHandle(m_hMutex) == NULL)
				throw std::runtime_error("failed to close handle");

			m_hMutex = nullptr;
		}
	}

	BOOL IsAnotherInstanceRunning()
	{
		return (ERROR_ALREADY_EXISTS == m_dwLastError);
	}
};

CLimitSingleInstance g_SingleInstanceObj(L"Global\\{101UPD473R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCH}");

// Check if there are updates for this one every now and then http://labs.adobe.com/downloads/air.html
const std::wstring airsetup = L"air17_win.exe";

bool finished = false;
wchar_t loldir[MAX_PATH + 1];
const std::wstring unblocktag = L":Zone.Identifier";
wchar_t gameclient[MAX_PATH + 1] = {0};
wchar_t tbbname[INTERNET_MAX_URL_LENGTH] = {0};

void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
	__cpuidex(abcd, eax, ecx);
}

int check_xcr0_ymm()
{
	uint32_t xcr0;
	xcr0 = static_cast<uint32_t>(_xgetbv(0));
	return ((xcr0 & 6) == 6);
}

int check_4th_gen_intel_core_features()
{
	int abcd[4];
	uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
	uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);

	run_cpuid(1, 0, abcd);
	if ((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask)
		return 0;

	if (!check_xcr0_ymm())
		return 0;

	run_cpuid(7, 0, abcd);
	if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		return 0;

	run_cpuid(0x80000001, 0, abcd);
	if ((abcd[2] & (1 << 5)) == 0)
		return 0;

	return 1;
}

static int can_use_intel_core_4th_gen_features()
{
	static auto the_4th_gen_features_available = -1;
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();

	return the_4th_gen_features_available;
}

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	if (!URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr) == S_OK)
		throw std::runtime_error("failed to initialize download");
}

void AdobeAirDL()
{
	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = {0};
	DWORD dwLength = sizeof(finalurl);

	if (UrlCombine(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/", airsetup.c_str(), finalurl, &dwLength, 0) != S_OK)
		throw std::runtime_error("failed to combine Url");

	downloadFile(finalurl, airsetup.c_str());
}

void errorcheck(BOOL res)
{
	if (res == NULL)
		throw std::runtime_error("failed to copy/unblock file");
}

void UnblockFile(std::wstring const& filename)
{
	DeleteFile(std::wstring(loldir + filename + unblocktag).c_str());
}

void ExtractResource(int RCDATAID, std::wstring const& filename)
{
	FILE* f;
	auto hRes = FindResource(nullptr, MAKEINTRESOURCE(RCDATAID), RT_RCDATA);

	if (hRes == nullptr)
		throw std::runtime_error("failed to find resource");

	if (_wfopen_s(&f, filename.c_str(), L"wb") != NULL)
		throw std::runtime_error("failed to open resource");

	if (fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f) == NULL)
		throw std::runtime_error("failed to write resource");

	if (fclose(f) != NULL)
		throw std::runtime_error("failed to close resource");
}

void PCombine(LPTSTR pszPathOut, LPCTSTR pszPathIn, LPCTSTR pszMore)
{
	if (PathCombine(pszPathOut, pszPathIn, pszMore) == nullptr)
		throw std::runtime_error("failed to combine path");
}

void PAppend(LPTSTR pszPath, LPCTSTR pszMore)
{
	if (PathAppend(pszPath, pszMore) == NULL)
		throw std::runtime_error("failed to append path");
}

void AVXSSE2detect(std::wstring const& AVXname, std::wstring const& SSE2name)
{
	int cpuInfo[4];
	__cpuid(cpuInfo, 1);
	if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && check_xcr0_ymm())
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, AVXname.c_str(), _TRUNCATE);
	}
	else
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, SSE2name.c_str(), _TRUNCATE);
	}
}

void threadingbuildingblocks()
{
	wchar_t tbb[MAX_PATH + 1] = {0};
	PCombine(tbb, gameclient, L"tbb.dll");

	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = {0};
	DWORD dwLength = sizeof(finalurl);

	OSVERSIONINFO osvi{};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	errorcheck(GetVersionEx(&osvi));

	// Unknown how this will do on Wine
	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"XP.dll", _TRUNCATE);
	}
	if ((osvi.dwMajorVersion == 6) & (osvi.dwMinorVersion == 0))
	{
		wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"Vista.dll", _TRUNCATE);
	}
	if ((osvi.dwMajorVersion == 6) & (osvi.dwMinorVersion == 1))
	{
		if (can_use_intel_core_4th_gen_features())
		{
			wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"AVX2-Win7.dll", _TRUNCATE);
		}
		else
		{
			AVXSSE2detect(L"AVX-Win7.dll", L"SSE2-Win7.dll");
		}
	}
	else
	{
		if (can_use_intel_core_4th_gen_features())
		{
			wcsncat_s(tbbname, INTERNET_MAX_URL_LENGTH, L"AVX2.dll", _TRUNCATE);
		}
		else
		{
			AVXSSE2detect(L"AVX.dll", L"SSE2.dll");
		}
	}

	if (UrlCombine(L"http://lol.jdhpro.com/", tbbname, finalurl, &dwLength, 0) != S_OK)
	{
		throw std::runtime_error("failed to combine Url");
	}
	downloadFile(finalurl, tbb);
	UnblockFile(tbb);
}

std::wstring findlatest(std::wstring const& folder)
{
	std::wstring data;
	std::wstring search = {folder + L"\\*"};
	HANDLE hFind;
	WIN32_FIND_DATA data2;

	hFind = FindFirstFile(search.c_str(), &data2);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		struct FileInfo
		{
			HANDLE h;
			WIN32_FIND_DATA info;
		} newest;
		newest.h = hFind;
		newest.info = data2;
		while (FindNextFile(hFind, &data2))
		{
			if (CompareFileTime(&data2.ftLastWriteTime, &newest.info.ftLastWriteTime) > 0)
			{
				newest.h = hFind;
				newest.info = data2;
			}
		}
		data =+ newest.info.cFileName;
		errorcheck(FindClose(hFind));
	}
	else
		throw std::runtime_error("failed to find file/directory");

	return data;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT start = {2, 0, 0, 0};
	RECT end = {2, 20, 0, 0};
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (hdc == nullptr)
			throw std::runtime_error("Nothing to render in");

		if (DrawText(hdc, L"In Progress, Please Wait!", -1, &start, DT_SINGLELINE | DT_NOCLIP) == NULL)
			throw std::runtime_error("failed to draw text");

		if (finished)
		{
			if (DrawText(hdc, L"Finished!, Enjoy a better League!", -1, &end, DT_SINGLELINE | DT_NOCLIP) == NULL)
				throw std::runtime_error("failed to draw text");
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

	MSG Msg = {0};
	WNDCLASSEX wc = {0};
	const std::wstring g_szClassName(L"mainwindow");
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	if (wc.hIcon == nullptr)
		throw std::runtime_error("failed to load icon");

	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));

	if (wc.hIconSm == nullptr)
		throw std::runtime_error("failed to load icon");

	if (RegisterClassEx(&wc) == NULL)
	{
		throw std::runtime_error("failed to register windowclass");
	}

	auto hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 260, 100, nullptr, nullptr, hInstance, nullptr);

	if (hwnd == nullptr)
	{
		throw std::runtime_error("failed to create window");
	}

	BROWSEINFO bi = {0};
	bi.lpszTitle = L"Select your (League of Legends)/GarenaLoL installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	if (pidl == nullptr)
	{
		return 0;
	}
	if (SHGetPathFromIDList(pidl, loldir) == NULL)
	{
		throw std::runtime_error("failed to get browse path");
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	std::thread t{AdobeAirDL};
	t.join();

	auto cwd(_wgetcwd(nullptr, 0));

	wchar_t runair[MAX_PATH + 1] = {0};
	PCombine(runair, cwd, airsetup.c_str());
	DeleteFile(std::wstring(runair + unblocktag).c_str());

	SHELLEXECUTEINFO ei = {};
	ei.cbSize = sizeof(SHELLEXECUTEINFO);
	ei.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ei.lpVerb = L"runas";
	ei.lpFile = runair;
	ei.lpParameters = L"-silent";
	ei.nShow = SW_SHOW;

	if (!ShellExecuteEx(&ei))
		throw std::runtime_error("failed to execute the Adobe AIR Installer");

	while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &ei.hProcess, FALSE, INFINITE, QS_ALLINPUT))
	{
		while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Msg);
		}
	}

	errorcheck(DeleteFile(runair));
	wchar_t progdrive[MAX_PATH + 1];
	if (SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive) != S_OK)
	{
		throw std::runtime_error("failed to get path");
	}

	const std::wstring adobedir = L"Adobe AIR\\Versions\\1.0";
	wchar_t adobepath[MAX_PATH + 1] = {0};
	PCombine(adobepath, progdrive, adobedir.c_str());

	const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";
	ExtractResource(1, cgsetup.c_str());
	wchar_t runcg[MAX_PATH + 1] = {0};
	PCombine(runcg, cwd, cgsetup.c_str());
	DeleteFile(std::wstring(runcg + unblocktag).c_str());

	SHELLEXECUTEINFO ei1 = {};
	ei1.cbSize = sizeof(SHELLEXECUTEINFO);
	ei1.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ei1.lpVerb = L"runas";
	ei1.lpFile = runcg;
	ei1.lpParameters = L"/verysilent /TYPE = compact";
	ei1.nShow = SW_SHOW;

	if (!ShellExecuteEx(&ei1))
		throw std::runtime_error("failed to execute the NvidiaCG Installer");

	while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &ei1.hProcess, FALSE, INFINITE, QS_ALLINPUT))
	{
		while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Msg);
		}
	}

	errorcheck(DeleteFile(runcg));
	wchar_t cgbinpath[MAX_PATH + 1];
	if (GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, MAX_PATH + 1) == NULL)
	{
		throw std::runtime_error("failed to get environmental variable path");
	}

	wchar_t cgbin[MAX_PATH + 1] = { 0 };
	auto cg = L"cg.dll";
	PCombine(cgbin, cgbinpath, cg);

	wchar_t cgglbin[MAX_PATH + 1] = { 0 };
	auto cggl = L"cgGL.dll";
	PCombine(cgglbin, cgbinpath, cggl);

	wchar_t cgd3d9bin[MAX_PATH + 1] = { 0 };
	auto cgd3d9 = L"cgD3D9.dll";
	PCombine(cgd3d9bin, cgbinpath, cgd3d9);

	wchar_t instdirGarena[MAX_PATH + 1] = {0};
	PCombine(instdirGarena, loldir, L"lol.exe");

	wchar_t instdirCN[MAX_PATH + 1] = {0};
	PCombine(instdirCN, loldir, L"lol.launcher_tencent.exe");

	wchar_t instdir[MAX_PATH + 1] = {0};
	PCombine(instdir, loldir, L"lol.launcher.exe");

	const std::wstring cpp = L"msvcp120.dll";
	const std::wstring cpr = L"msvcr120.dll";

	wchar_t dep[MAX_PATH + 1] = L"deploy";

	wchar_t airclient[MAX_PATH + 1] = {0};
	wchar_t patchclient[MAX_PATH + 1] = {0};

	wchar_t cp[MAX_PATH + 1] = {0};
	wchar_t cr[MAX_PATH + 1] = {0};

	if (std::wifstream(instdirGarena).fail() & std::wifstream(instdir).good() & std::wifstream(instdirCN).fail())
	{
		auto rads = L"RADS";
		PCombine(airclient, loldir, rads);
		auto proj = L"projects";
		PAppend(airclient, proj);
		PAppend(airclient, L"lol_air_client");
		auto rel = L"releases";
		PAppend(airclient, rel);

		PCombine(patchclient, loldir, rads);
		PAppend(patchclient, proj);
		PAppend(patchclient, L"lol_patcher");
		PAppend(patchclient, rel);

		PCombine(gameclient, loldir, rads);
		PAppend(gameclient, L"solutions");
		PAppend(gameclient, L"lol_game_client_sln");
		PAppend(gameclient, rel);

		PAppend(patchclient, findlatest(patchclient).c_str());
		PAppend(patchclient, dep);

		PCombine(cp, patchclient, cpp.c_str());
		ExtractResource(2, cp);
		UnblockFile(cp);

		PCombine(cr, patchclient, cpr.c_str());
		ExtractResource(3, cr);
		UnblockFile(cr);

		PAppend(airclient, findlatest(airclient).c_str());
		PAppend(airclient, dep);
		PAppend(airclient, adobedir.c_str());

		PAppend(gameclient, findlatest(gameclient).c_str());
		PAppend(gameclient, dep);
	}
	else
	{
		if (std::wifstream(instdirGarena).good() || std::wifstream(instdirCN).good())
		{
			PCombine(gameclient, loldir, L"Game");
			PCombine(airclient, loldir, std::wstring(L"Air\\" + adobedir).c_str());
		}
		else
			throw std::runtime_error("Unable to determine LoL version");
	}

	wchar_t airdest[MAX_PATH + 1] = {0};
	auto air = L"Adobe AIR.dll";
	PCombine(airdest, airclient, air);

	wchar_t airlatest[MAX_PATH + 1] = {0};
	PCombine(airlatest, adobepath, air);

	wchar_t flash[MAX_PATH + 1] = {L"Resources"};
	PAppend(flash, L"NPSWF32.dll");

	wchar_t flashdest[MAX_PATH + 1] = {0};
	PCombine(flashdest, airclient, flash);

	wchar_t flashlatest[MAX_PATH + 1] = {0};
	PCombine(flashlatest, adobepath, flash);

	wchar_t cgdest[MAX_PATH + 1] = {0};
	PCombine(cgdest, gameclient, cg);

	wchar_t cggldest[MAX_PATH + 1] = {0};
	PCombine(cggldest, gameclient, cggl);

	wchar_t cgd3d9dest[MAX_PATH + 1] = {0};
	PCombine(cgd3d9dest, gameclient, cgd3d9);

	std::thread t1{threadingbuildingblocks};
	t1.join();

	errorcheck(CopyFile(cgbin, cgdest, false));
	errorcheck(CopyFile(cgglbin, cggldest, false));
	errorcheck(CopyFile(cgd3d9bin, cgd3d9dest, false));
	errorcheck(CopyFile(airlatest, airdest, false));
	errorcheck(CopyFile(flashlatest, flashdest, false));

	*cp = '\0';
	PCombine(cp, gameclient, cpp.c_str());
	ExtractResource(2, cp);
	UnblockFile(cp);

	*cr = '\0';
	PCombine(cr, gameclient, cpr.c_str());
	ExtractResource(3, cr);
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
