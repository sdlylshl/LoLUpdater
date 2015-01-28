#include <sstream>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <thread>
#define MAX_URL_LENGTH 33
#define PATH 261

auto done = false;
wchar_t path[PATH];
wchar_t unblocker1[PATH] = L"";
auto unblocker = unblocker1;
const std::wstring airsetup = L"air16_win.exe";
const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";

void runAndWait(std::wstring const& file, std::wstring const& args)
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


void unblockFile(std::wstring const& path1)
{
	*unblocker = '\0';
	wcsncat_s(unblocker, PATH, path, _TRUNCATE);
	wcsncat_s(unblocker, PATH, path1.c_str(), _TRUNCATE);
	wcsncat_s(unblocker, PATH, L":Zone.Identifier", _TRUNCATE);
	DeleteFile(unblocker);
}

void patch()
{
	wchar_t gameclient1[PATH] = L"";
	auto gameclient = gameclient1;
	wcsncat_s(gameclient, PATH, path, _TRUNCATE);

	wchar_t airclient1[PATH] = L"";
	auto airclient = airclient1;
	wcsncat_s(airclient, PATH, path, _TRUNCATE);

	wchar_t progdrive[PATH];
	SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive);

	wchar_t commonfiles1[PATH] = L"";
	auto commonfiles = commonfiles1;
	wcsncat_s(commonfiles, PATH, progdrive, _TRUNCATE);

	wchar_t adobedir1[PATH] = L"";
	auto adobedir = adobedir1;
	const std::wstring adobeairpath = { std::wstring(L"Adobe AIR\\Versions\\1.0") };
	wcsncat_s(adobedir, PATH, adobeairpath.c_str(), _TRUNCATE);

	wchar_t adobepath1[PATH] = L"";
	auto adobepath = adobepath1;
	PathCombine(adobepath, commonfiles, adobedir);

	wchar_t instdir1[PATH] = L"";
	auto instdir = instdir1;
	PathCombine(instdir, path, L"lol.exe");

	if (std::wifstream(instdir).good())
	{
		PathAppend(gameclient, L"Game");

		wchar_t garenaair1[PATH] = L"";
		auto garenaair = garenaair1;
		wcsncat_s(garenaair, PATH, L"Air\\", _TRUNCATE);
		wcsncat_s(garenaair, PATH, adobeairpath.c_str(), _TRUNCATE);

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

	wchar_t cgbinpath[PATH];
	runAndWait(cgsetup, L"/verysilent /TYPE = compact");

	GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, PATH);
	wcsncat_s(cgbinpath, PATH, L"\\", _TRUNCATE);

	wchar_t cgbin1[PATH] = L"";
	auto cgbin = cgbin1;
	auto cg = L"Cg.dll";
	PathCombine(cgbin, cgbinpath, cg);

	wchar_t cgglbin1[PATH] = L"";
	auto cgglbin = cgglbin1;
	auto cggl = L"CgGL.dll";
	PathCombine(cgglbin, cgbinpath, cggl);

	wchar_t cgd3d9bin1[PATH] = L"";
	auto cgd3d9bin = cgd3d9bin1;
	auto cgd3d9 = L"CgD3D9.dll";
	PathCombine(cgd3d9bin, cgbinpath, cgd3d9);

	wchar_t airdest1[PATH] = L"";
	auto airdest = airdest1;
	auto air = L"Adobe AIR.dll";
	PathCombine(airdest, airclient, air);

	wchar_t airlatest1[PATH] = L"";
	auto airlatest = airlatest1;
	PathCombine(airlatest, adobepath, air);

	wchar_t res[PATH] = L"Resources";
	auto flash = res;
	PathAppend(flash, L"NPSWF32.dll");

	wchar_t flashdest1[PATH] = L"";
	auto flashdest = flashdest1;
	PathCombine(flashdest, airclient, flash);

	wchar_t flashlatest1[PATH] = L"";
	auto flashlatest = flashlatest1;
	PathCombine(flashlatest, adobepath, flash);

	wchar_t cgdest1[PATH] = L"";
	auto cgdest = cgdest1;
	PathCombine(cgdest, gameclient, cg);

	wchar_t cggldest1[PATH] = L"";
	auto cggldest = cggldest1;
	PathCombine(cggldest, gameclient, cggl);

	wchar_t cgd3d9dest1[PATH] = L"";
	auto cgd3d9dest = cgd3d9dest1;
	PathCombine(cgd3d9dest, gameclient, cgd3d9);

	wchar_t tbb1[PATH] = L"";
	auto tbb = tbb1;
	PathCombine(tbb, gameclient, L"tbb.dll");

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	wchar_t finalurl[MAX_URL_LENGTH] = L"";
	DWORD dwLength = sizeof(finalurl);
	wchar_t tbbname[MAX_URL_LENGTH] = L"";

	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		wcsncat_s(tbbname, MAX_URL_LENGTH, L"XP.dll", _TRUNCATE);
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
			wcsncat_s(tbbname, MAX_URL_LENGTH, L"AVX2.dll", _TRUNCATE);
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if (((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false)) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) == 6))
			{
				wcsncat_s(tbbname, MAX_URL_LENGTH, L"AVX.dll", _TRUNCATE);
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					wcsncat_s(tbbname, MAX_URL_LENGTH, L"SSE2.dll", _TRUNCATE);
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						wcsncat_s(tbbname, MAX_URL_LENGTH, L"SSE.dll", _TRUNCATE);
					}
					else
					{
						wcsncat_s(tbbname, MAX_URL_LENGTH, L"Default.dll", _TRUNCATE);
					}
				}
			}
		}
	}

	UrlCombine(L"http://lol.jdhpro.com/", tbbname, finalurl, &dwLength, 0);
	URLDownloadToFile(nullptr, finalurl, tbb, 0, nullptr);
	unblockFile(tbb);

	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);

	runAndWait(airsetup, L"-silent");

	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
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
		if (done == true)
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
	MSG Msg;
	WNDCLASSEX wc;
	const std::wstring g_szClassName(L"mainwindow");
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	RegisterClassEx(&wc);

	HWND hwnd;
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 260, 100, nullptr, nullptr, hInstance, nullptr);
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select your League of Legends/GarenaLoL installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	URLDownloadToFile(nullptr, L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air16_win.exe", airsetup.c_str(), 0, nullptr);
	unblockFile(airsetup);
	SHGetPathFromIDList(pidl, path);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	FILE* f;
	auto hRes = FindResource(nullptr, MAKEINTRESOURCE(1), RT_RCDATA);
	_wfopen_s(&f, cgsetup.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);
	

	std::thread t{patch};
	t.join();

	done = true;
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}