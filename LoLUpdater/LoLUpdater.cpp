// Minimum supported processor = Intel Pentium 4 (Or anything with at least SSE2)
// Supported Windows Versions: Windows XP SP3 -> Windows 10
// Windows Server is currently not supported
// Wine support is unknown

#include <Windows.h>
#include <wininet.h>
#include <thread>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <resource.h>
#include <Tlhelp32.h>
#include <sstream>
#define IDM_FIRSTCHILD 900001

struct Version
{
	int major, minor, revision, build;

	explicit Version(const std::wstring& version)
	{
		swscanf_s(version.c_str(), L"%d.%d.%d.%d", &major, &minor, &revision, &build, sizeof(wchar_t));
		if (major < 0) major = 0;
		if (minor < 0) minor = 0;
		if (revision < 0) revision = 0;
		if (build < 0) build = 0;
	}

	bool operator <(const Version& other)
	{
		if (major < other.major)
			return true;
		if (minor < other.minor)
			return true;
		if (revision < other.revision)
			return true;
		if (build < other.build)
			return true;
		return false;
	}

	friend std::wostream& operator <<(std::wostream& stream, const Version& ver)
	{
		stream << ver.major;
		stream << '.';
		stream << ver.minor;
		stream << '.';
		stream << ver.revision;
		stream << '.';
		stream << ver.build;
		return stream;
	}
};

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

std::wstring file2bremove;
std::wstring file2bupdate;
bool finished = false;
wchar_t loldir[MAX_PATH + 1];
wchar_t gameclient[MAX_PATH + 1] = { 0 };
wchar_t patchclient[MAX_PATH + 1] = { 0 };
wchar_t runair[MAX_PATH + 1] = { 0 };
const std::wstring tbbfile = L"tbb.dll";
wchar_t* cwd(_wgetcwd(nullptr, 0));
wchar_t currentdir[MAX_PATH + 1] = { 0 };
wchar_t majortxt[MAX_PATH + 1] = { 0 };
wchar_t minortxt[MAX_PATH + 1] = { 0 };
wchar_t revisiontxt[MAX_PATH + 1] = { 0 };
wchar_t buildtxt[MAX_PATH + 1] = { 0 };
const std::wstring major = L"major.txt";
const std::wstring minor = L"minor.txt";
const std::wstring revision = L"revision.txt";
const std::wstring build = L"build.txt";
wchar_t currentdir2[MAX_PATH] = { 0 };
const std::wstring ftp = L"http://lol.jdhpro.com/";
// Check if there are updates for this one every now and then http://labs.adobe.com/downloads/air.html
const std::wstring airsetup = L"air17_win.exe";
const std::wstring unblocktag = L":Zone.Identifier";
wchar_t tbbname[INTERNET_MAX_URL_LENGTH] = { 0 };
const std::wstring p120 = L"msvcp120.dll";
const std::wstring r120 = L"msvcr120.dll";
wchar_t airdest[MAX_PATH + 1] = { 0 };
wchar_t airlatest[MAX_PATH + 1] = { 0 };
wchar_t flash[MAX_PATH + 1] = { L"Resources" };
wchar_t flashdest[MAX_PATH + 1] = { 0 };
wchar_t flashlatest[MAX_PATH + 1] = { 0 };
wchar_t cgdest[MAX_PATH + 1] = { 0 };
wchar_t cggldest[MAX_PATH + 1] = { 0 };
wchar_t cgd3d9dest[MAX_PATH + 1] = { 0 };
wchar_t fullpath[MAX_PATH + 1] = { 0 };
const std::wstring air = L"Adobe AIR.dll";
const std::wstring cg = L"cg.dll";
const std::wstring cggl = L"cgGL.dll";
const std::wstring cgd3d9 = L"cgD3D9.dll";
HWND hwnd;
HWND hwndButton;
HWND hwndButton2;
HWND hwnd2;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc2(HWND, UINT, WPARAM, LPARAM);

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

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	if (URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr) != S_OK)
		throw std::runtime_error("failed to initialize download");
}

void UrlComb(PCTSTR pszRelative)
{
	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = {0};
	DWORD dwLength = sizeof(finalurl);

	if (UrlCombine(ftp.c_str(), pszRelative, finalurl, &dwLength, 0) != S_OK)
		throw std::runtime_error("failed to combine Url");

	downloadFile(finalurl, pszRelative);
}


void killProcessByName(const wchar_t* filename)
{
	auto hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	auto hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (wcscmp(pEntry.szExeFile, filename) == 0)
		{
			auto hProcess = OpenProcess(PROCESS_TERMINATE, 0, static_cast<DWORD>(pEntry.th32ProcessID));
			if (hProcess != nullptr)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

bool IsProcessRunning(const wchar_t* processName)
{
	auto exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}

void DLUpdate()
{
	downloadFile(L"http://lol.jdhpro.com/LoLUpdater.exe", fullpath);
}

void DLstuff()
{
	UrlComb(major.c_str());
	UrlComb(minor.c_str());
	UrlComb(revision.c_str());
	UrlComb(build.c_str());
}

void Updater()
{
	std::thread DLInfo{ DLstuff };
	DLInfo.join();

	PCombine(majortxt, cwd, major.c_str());
	PCombine(minortxt, cwd, minor.c_str());
	PCombine(revisiontxt, cwd, revision.c_str());
	PCombine(buildtxt, cwd, build.c_str());

	std::wifstream t0(majortxt);
	std::wstringstream buffer0;
	buffer0 << t0.rdbuf();
	t0.close();
	DeleteFile(majortxt);

	std::wifstream t1(minortxt);
	std::wstringstream buffer1;
	buffer1 << t1.rdbuf();
	t1.close();
	DeleteFile(minortxt);

	std::wifstream t2(revisiontxt);
	std::wstringstream buffer2;
	buffer2 << t2.rdbuf();
	t2.close();
	DeleteFile(revisiontxt);

	std::wifstream t3(buildtxt);
	std::wstringstream buffer3;
	buffer3 << t3.rdbuf();
	t3.close();
	DeleteFile(buildtxt);

	DWORD handle = 0;
	DWORD size;
	size = GetFileVersionInfoSize(currentdir2, &handle);
	if (size == NULL)
		throw std::runtime_error("failed to get file version infosize");

	auto versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(currentdir2, handle, size, versionInfo))
		throw std::runtime_error("failed to get file version info");

	UINT32 len = 0;
	VS_FIXEDFILEINFO* vsfi = nullptr;
	if (VerQueryValue(versionInfo, L"\\", reinterpret_cast<void**>(&vsfi), &len) == NULL)
		throw std::runtime_error("failed to retrive fileversion details");

	delete[] versionInfo;

	if (Version(std::wstring(std::to_wstring(HIWORD(vsfi->dwFileVersionMS)) + L"." + std::to_wstring(LOWORD(vsfi->dwFileVersionMS)) + L"." + std::to_wstring(HIWORD(vsfi->dwFileVersionLS)) + L"." + std::to_wstring(LOWORD(vsfi->dwFileVersionLS)))) < Version(std::wstring(buffer0.str() + L"." + buffer1.str() + L"." + buffer2.str() + L"." + buffer3.str())))
	{
		PCombine(fullpath, cwd, file2bupdate.c_str());
		std::thread superman{ DLUpdate};
		superman.join();
	}
	else
	{
		return;
	}

	SHELLEXECUTEINFO ei0 = {};
	ei0.cbSize = sizeof(SHELLEXECUTEINFO);
	ei0.fMask = SEE_MASK_NOCLOSEPROCESS ;
	ei0.lpVerb = L"runas";
	ei0.lpFile = currentdir;
	ei0.nShow = SW_SHOW;

	if (!ShellExecuteEx(&ei0))
		throw std::runtime_error("failed to execute updated LoLUpdater");

	killProcessByName(file2bremove.c_str());
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

	DeleteFile(std::wstring(loldir + filename + L":Zone.Identifier").c_str());
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
		if (!FindClose(hFind))
			throw std::runtime_error("failed to close file handle");
	}
	else
		throw std::runtime_error("failed to find file/directory");

	return data;
}

void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
	__cpuidex(abcd, eax, ecx);
}

int check_xcr0_ymm()
{
	uint32_t xcr0;
	xcr0 = static_cast<uint32_t>(_xgetbv(0));
	return (xcr0 & 6) == 6;
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


void AdobeAirDL()
{
	wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = {0};
	DWORD dwLength = sizeof(finalurl);

	if (UrlCombine(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/", airsetup.c_str(), finalurl, &dwLength, 0) != S_OK)
		throw std::runtime_error("failed to combine Url");

	downloadFile(finalurl, runair);
}

void UnblockFile(std::wstring const& filename)
{
	DeleteFile(std::wstring(loldir + filename + unblocktag).c_str());
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

void msvc(std::wstring const& DEST, int RESID, std::wstring const& RESIDNAME)
{
	wchar_t svc[MAX_PATH + 1] = {0};
	PCombine(svc, DEST.c_str(), RESIDNAME.c_str());
	ExtractResource(RESID, svc);
	UnblockFile(svc);
}

void CpFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	if (!CopyFile(lpExistingFileName, lpNewFileName, false))
		throw std::runtime_error("failed to copy file");
}


WNDPROC OldButtonProc;
WNDPROC OldButtonProc2;

LRESULT CALLBACK ButtonProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(hwndButton, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Patching..."));
		EnableWindow(hwndButton, FALSE);
		MSG Msg = { 0 };
		wchar_t tbb[MAX_PATH + 1] = { 0 };
		PCombine(tbb, gameclient, tbbfile.c_str());

		wchar_t finalurl[INTERNET_MAX_URL_LENGTH] = { 0 };
		DWORD dwLength = sizeof(finalurl);

		OSVERSIONINFO osvi{};
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(&osvi))
			throw std::runtime_error("failed to get version info");


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
		if ((osvi.dwMajorVersion == 6) & (osvi.dwMinorVersion == 2))
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
			throw std::runtime_error("failed to combine Url");

		downloadFile(finalurl, tbb);
		UnblockFile(tbb);

		PCombine(runair, cwd, airsetup.c_str());
		std::thread t{ AdobeAirDL };
		t.join();

		DeleteFile(std::wstring(runair + unblocktag).c_str());

		SHELLEXECUTEINFO ei = {};
		ei.cbSize = sizeof(SHELLEXECUTEINFO);
		ei.fMask = SEE_MASK_NOCLOSEPROCESS;
		ei.lpVerb = L"runas";
		ei.lpParameters = L"-silent";
		ei.lpFile = runair;
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

		DeleteFile(runair);

		wchar_t runcg[MAX_PATH + 1] = { 0 };
		const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";
		PCombine(runcg, cwd, cgsetup.c_str());
		ExtractResource(1, runcg);
		DeleteFile(std::wstring(runcg + unblocktag).c_str());

		SHELLEXECUTEINFO ei1 = {};
		ei1.cbSize = sizeof(SHELLEXECUTEINFO);
		ei1.fMask = SEE_MASK_NOCLOSEPROCESS;
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

		DeleteFile(runcg);

		wchar_t cgbinpath[MAX_PATH + 1];
		if (GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, MAX_PATH + 1) == NULL)
			throw std::runtime_error("failed to get environmental variable path");

		wchar_t cgbin[MAX_PATH + 1] = { 0 };

		PCombine(cgbin, cgbinpath, cg.c_str());

		wchar_t cgglbin[MAX_PATH + 1] = { 0 };

		PCombine(cgglbin, cgbinpath, cggl.c_str());

		wchar_t cgd3d9bin[MAX_PATH + 1] = { 0 };

		PCombine(cgd3d9bin, cgbinpath, cgd3d9.c_str());

		wchar_t progdrive[MAX_PATH + 1];
		if (SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive) != S_OK)
			throw std::runtime_error("Unable to get folder path");

		const std::wstring adobedir = L"Adobe AIR\\Versions\\1.0";
		wchar_t adobepath[MAX_PATH + 1] = { 0 };
		PCombine(adobepath, progdrive, adobedir.c_str());

		PCombine(airlatest, adobepath, air.c_str());
		PCombine(flashlatest, adobepath, flash);

		CpFile(cgbin, cgdest);
		CpFile(cgglbin, cggldest);
		CpFile(cgd3d9bin, cgd3d9dest);
		CpFile(airlatest, airdest);
		CpFile(flashlatest, flashdest);

		msvc(gameclient, 2, p120.c_str());
		msvc(gameclient, 3, r120.c_str());

		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		
		SendMessage(hwndButton, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Finished!"));
		return 0;
	}
	return CallWindowProc(OldButtonProc, hwndButton, msg, wp, lp);
}


LRESULT CALLBACK ButtonProc2(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(hwndButton2, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Uninstalling..."));
		EnableWindow(hwndButton2, FALSE);
		wchar_t msvcpdest[MAX_PATH + 1] = { 0 };
		PCombine(msvcpdest, patchclient, p120.c_str());

		wchar_t msvcrdest[MAX_PATH + 1] = { 0 };
		PCombine(msvcrdest, patchclient, r120.c_str());

		wchar_t tbbdest[MAX_PATH + 1] = { 0 };
		PCombine(tbbdest, gameclient, tbbfile.c_str());

		ExtractResource(101, airdest);
		ExtractResource(201, cgdest);
		ExtractResource(301, cggldest);
		ExtractResource(401, cgd3d9dest);
		ExtractResource(501, msvcpdest);
		ExtractResource(601, msvcrdest);
		ExtractResource(701, flashdest);
		ExtractResource(801, tbbdest);
		SendMessage(hwndButton2, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Finished!"));
		break;
	}
	return CallWindowProc(OldButtonProc2, hwndButton2, msg, wp, lp);
}

void AboutBox()
{
	ShowWindow(hwnd2, SW_SHOW);
	UpdateWindow(hwnd2);
	UpdateWindow(hwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case ID_HELP_CHECKFORUPDATES:
				Updater();
				break;

			case ID_HELP_ABOUT:
				ShowWindow(hwnd2, SW_SHOW);
				AboutBox();
				break;
			}
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
                   LPSTR, int)
{
	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
		return 0;

	if (IsProcessRunning(L"LoLUpdater.exe"))
	{
		file2bupdate = L"LoLUpdater2.exe";
		file2bremove = L"LoLUpdater.exe";
	}
	else
	{
		file2bupdate = L"LoLUpdater.exe";
		file2bremove = L"LoLUpdater2.exe";
	}

	PCombine(currentdir2, cwd, file2bremove.c_str());
	PCombine(currentdir, cwd, file2bupdate.c_str());

	if (IsProcessRunning(L"LoLUpdater.exe"))
	{
		DeleteFile(file2bupdate.c_str());
	}
	else
	{
		DeleteFile(file2bremove.c_str());
	}

	MSG Msg = {0};
	WNDCLASSEX wc = {0};
	const std::wstring g_szClassName(L"mainwindow");
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
	if (wc.hIcon == nullptr)
		throw std::runtime_error("failed to load icon");

	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));

	if (wc.hIconSm == nullptr)
		throw std::runtime_error("failed to load icon");

	if (RegisterClassEx(&wc) == NULL)
	{
		throw std::runtime_error("failed to register windowclass");
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 130, nullptr, nullptr, hInstance, nullptr);

	if (hwnd == nullptr)
	{
		throw std::runtime_error("failed to create window");
	}
	CLIENTCREATESTRUCT MDIClientCreateStruct;

	hwndButton = CreateWindow(L"button", L"Install", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 50, hwnd, (HMENU)200, nullptr, nullptr);
	OldButtonProc = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc)));
	hwndButton2 = CreateWindow(L"button", L"Uninstall", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 50, hwnd, (HMENU)200, nullptr, nullptr);
	OldButtonProc2 = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton2, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc2)));

	MDIClientCreateStruct.idFirstChild = IDM_FIRSTCHILD;
	hwnd2 = CreateWindowEx(WS_EX_TOOLWINDOW, L"MDICLIENT", L"About LoLUpdater", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 250, 130, hwnd, nullptr, nullptr, (void*)&MDIClientCreateStruct);

	if (hwnd2 == nullptr)
	{
		throw std::runtime_error("failed to create window");
	}


	BROWSEINFO bi = {0};
	bi.lpszTitle = L"Select your GarenaLoL/League of Legends/LoLQQ installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	if (pidl == nullptr)
	{
		return NULL;
	}

	if (SHGetPathFromIDList(pidl, loldir) == NULL)
	{
		throw std::runtime_error("failed to get browse path");
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	wchar_t instdir[MAX_PATH + 1] = {0};
	PCombine(instdir, loldir, L"lol.launcher.exe");

	wchar_t instdirCN[MAX_PATH + 1] = {0};
	PCombine(instdirCN, loldir, L"lol.launcher_tencent.exe");

	wchar_t instdirGarena[MAX_PATH + 1] = {0};
	PCombine(instdirGarena, loldir, L"lol.exe");

	wchar_t airclient[MAX_PATH + 1] = {0};

	const std::wstring adobedir = L"Adobe AIR\\Versions\\1.0";

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

		wchar_t dep[MAX_PATH + 1] = L"deploy";
		PAppend(patchclient, findlatest(patchclient).c_str());
		PAppend(patchclient, dep);

		PAppend(airclient, findlatest(airclient).c_str());
		PAppend(airclient, dep);
		PAppend(airclient, adobedir.c_str());

		PAppend(gameclient, findlatest(gameclient).c_str());
		PAppend(gameclient, dep);
	}
	else
	{
		if (std::wifstream(instdirGarena).good() | std::wifstream(instdirCN).good())
		{
			PCombine(gameclient, loldir, L"Game");
			PCombine(airclient, loldir, std::wstring(L"Air\\" + adobedir).c_str());
		}
		else
			throw std::runtime_error("Unable to determine LoL version");
	}


	PCombine(airdest, airclient, air.c_str());

	PAppend(flash, L"NPSWF32.dll");
	PCombine(flashdest, airclient, flash);

	PCombine(cgdest, gameclient, cg.c_str());
	PCombine(cggldest, gameclient, cggl.c_str());
	PCombine(cgd3d9dest, gameclient, cgd3d9.c_str());

	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return static_cast<int>(Msg.wParam);
}