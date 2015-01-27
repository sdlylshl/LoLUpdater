#include <sstream>
#include <fstream>
#include <memory>
#include <Shlwapi.h>
#include <Shlobj.h>
#define MAX_URL_LENGTH 33
#define PATH 261

auto done = false;
wchar_t path[PATH];
wchar_t unblocker1[PATH] = L"";
auto unblocker = unblocker1;

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
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = g_szClassName.c_str();
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	RegisterClassEx(&wc);

	HWND hwnd;
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 410, 100, nullptr, nullptr, hInstance, nullptr);
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select your League of Legends/GarenaLoL installation directory:";
	auto pidl = SHBrowseForFolder(&bi);
	const std::wstring airsetup = L"air16_win.exe";
	URLDownloadToFile(nullptr, L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air16_win.exe", airsetup.c_str(), 0, nullptr);
	SHGetPathFromIDList(pidl, path);
	ShowWindow(hwnd, nCmdShow);
	
	EnableWindow(hwnd, FALSE);

	auto hRes = FindResource(nullptr, MAKEINTRESOURCE(1), RT_RCDATA);
	const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";
	FILE* f;
	_wfopen_s(&f, cgsetup.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);
	runAndWait(cgsetup, L"/verysilent /TYPE = compact");
	
	unblockFile(airsetup);
	runAndWait(airsetup, L"-silent");
	
	wchar_t cgbinpath[PATH];
	GetEnvironmentVariable(L"CG_BIN_PATH", cgbinpath, PATH);
	wcsncat_s(cgbinpath, PATH, L"\\", _TRUNCATE);
	
	wchar_t airclient1[PATH] = L"";
	wcsncat_s(airclient1, PATH, path, _TRUNCATE);
	
	wchar_t gameclient1[PATH] = L"";
	wcsncat_s(gameclient1, PATH, path, _TRUNCATE);
	
	wchar_t progdrive[PATH];
	SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES_COMMON, nullptr, 0, progdrive);
	
	wchar_t commonfiles1[PATH] = L"";
	auto commonfiles = commonfiles1;
	wcsncat_s(commonfiles1, PATH, progdrive, _TRUNCATE);
	
	wchar_t adobedir1[PATH] = L"";
	auto adobedir = adobedir1;
	const std::wstring adobeairpath = { std::wstring(L"Adobe AIR\\Versions\\1.0") };
	wcsncat_s(adobedir, PATH, adobeairpath.c_str(), _TRUNCATE);
	
	wchar_t adobepath1[PATH] = L"";
	auto adobepath = adobepath1;
	PathCombine(adobepath, commonfiles, adobedir);
	
	auto airclient = airclient1;
	auto gameclient = gameclient1;
	
	wchar_t instdir[PATH] = L"";
	auto instdir1 = instdir;
	PathCombine(instdir1, path, L"lol.exe");
	
	if (std::wifstream(instdir).good())
	{
		PathAppend(gameclient, L"Game");
		
		wchar_t garenaair1[PATH] = L"";
		auto garenaair = garenaair1;
		wcsncat_s(garenaair1, PATH, L"Air\\", _TRUNCATE);
		wcsncat_s(garenaair1, PATH, adobeairpath.c_str(), _TRUNCATE);
		
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
		PathAppend(gameclient, L"0.0.1.72");
		auto dep = L"deploy";
		PathAppend(gameclient, dep);
		
		PathAppend(airclient, L"0.0.1.126");
		PathAppend(airclient, dep);
		PathAppend(airclient, adobedir);
	}

	wchar_t cgbasepath1[PATH] = L"";
	auto cgbasepath = cgbasepath1;
	wcsncat_s(cgbasepath1, PATH, cgbinpath, _TRUNCATE);
	
	wchar_t cgbin1[PATH] = L"";
	auto cgbin = cgbin1;
	auto cg = L"Cg.dll";
	PathCombine(cgbin, cgbasepath, cg);
	
	wchar_t cgglbin1[PATH] = L"";
	auto cgglbin = cgglbin1;
	auto cggl = L"CgGL.dll";
	PathCombine(cgglbin, cgbasepath, cggl);
	
	wchar_t cgd3d9bin1[PATH] = L"";
	auto cgd3d9bin = cgd3d9bin1;
	auto cgd3d9 = L"CgD3D9.dll";
	PathCombine(cgd3d9bin, cgbasepath, cgd3d9);
	
	wchar_t tbb1[PATH] = L"";
	auto tbb = tbb1;
	PathCombine(tbb, gameclient, L"tbb.dll");
	
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
	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	done = true;
	EnableWindow(hwnd, TRUE);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}