#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <Shlwapi.h>
#include <direct.h>
#include <Shlobj.h>
bool done = false;
std::wstring cwd(_wgetcwd(nullptr, 0));
wchar_t unblocker1[MAX_PATH + 1] = L"";
wchar_t* unblocker = unblocker1;

void unblockFile(std::wstring const& path)
{
	*unblocker = '\0';
	PathCombine(unblocker, cwd.c_str(), path.c_str());
	wcsncat_s(
		unblocker,
		MAX_PATH + 1,
		L":Zone.Identifier",
		_TRUNCATE
		);
	DeleteFile(unblocker);
}

void runAndWait(std::wstring const& file, std::wstring const& args)
{
	unblockFile(file);
	*unblocker = '\0';
	PathCombine(unblocker, cwd.c_str(), file.c_str());
	SHELLEXECUTEINFO ei = {};
	ei.cbSize = sizeof(SHELLEXECUTEINFO);
	ei.fMask = SEE_MASK_NOCLOSEPROCESS;
	ei.lpVerb = L"runas";
	ei.lpFile = unblocker;
	ei.lpParameters = args.c_str();
	ei.nShow = SW_SHOW;
}

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr);
}

void downloadAndRunFile(std::wstring const& url, std::wstring const& file, std::wstring const& args)
{
	downloadFile(url, file);
	runAndWait(file, args);
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
		if (done)
		{
			DrawText(hdc, L"Finished!, Enjoy a better League", -1, &end, DT_SINGLELINE | DT_NOCLIP);
		}
		EndPaint(hwnd, &ps);
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
	WNDCLASSEX wc;
	const std::wstring g_szClassName(L"mainwindow1");
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
 
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName.c_str(),
		L"LoLUpdater",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 407, 134,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, nCmdShow);
	HRSRC hRes = FindResource(nullptr, MAKEINTRESOURCE(1), RT_RCDATA);
	FILE* f;
	const std::wstring cgsetup = L"Cg-3.1_April2012_Setup.exe";
	_wfopen_s(&f, cgsetup.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);
	const std::wstring airsetup = L"air16_win.exe";
	downloadAndRunFile(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air16_win.exe", airsetup, L"-silent");
	runAndWait(cgsetup, L"/verysilent /TYPE = compact");
	DeleteFile((cwd + std::wstring(L"\\") + cgsetup).c_str());
	DeleteFile((cwd + std::wstring(L"\\") + airsetup).c_str());
	wchar_t progdrive[MAX_PATH + 1];
	wchar_t cgbinpath[MAX_PATH + 1];
	GetEnvironmentVariable(L"CG_BIN_PATH",
		cgbinpath,
		MAX_PATH + 1);
	wcsncat_s(
		cgbinpath,
		MAX_PATH + 1,
		L"\\",
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
	SHGetFolderPath(nullptr,
		CSIDL_PROGRAM_FILES_COMMON,
		nullptr,
		0,
		progdrive);
	wchar_t commonfiles1[MAX_PATH + 1] = L"";
	wchar_t* commonfiles = commonfiles1;
	wcsncat_s(
		commonfiles1,
		MAX_PATH + 1,
		progdrive,
		_TRUNCATE
		);
	wchar_t adobedir1[MAX_PATH + 1] = L"";
	wchar_t* adobedir = adobedir1;
	const std::wstring adobeairpath = { std::wstring(L"Adobe AIR\\Versions\\1.0") };
	wcsncat_s(
		adobedir,
		MAX_PATH + 1,
		adobeairpath.c_str(),
		_TRUNCATE
		);
	wchar_t adobepath1[MAX_PATH + 1] = L"";
	wchar_t* adobepath = adobepath1;
	PathCombine(
		adobepath,
		commonfiles,
		adobedir
		);
	wchar_t* airclient = airclient1;
	wchar_t* gameclient = gameclient1;
	if (std::wifstream(L"lol.exe").good() && std::wifstream(L"lol.launcher.exe").fail())
	{
		PathAppend(gameclient, L"Game");
		wchar_t garenaair1[MAX_PATH + 1] = L"";
		wchar_t* garenaair = garenaair1;
		wcsncat_s(
			garenaair1,
			MAX_PATH + 1,
			L"AIR\\",
			_TRUNCATE
			);
		wchar_t garenaair2[MAX_PATH + 1] = L"";
		wchar_t* garenaair20 = garenaair2;
		wcsncat_s(
			garenaair2,
			MAX_PATH + 1,
			adobeairpath.c_str(),
			_TRUNCATE
			);
		PathAppend(airclient, garenaair);
		PathAppend(airclient, garenaair20);
	}

	if (std::wifstream(L"lol.launcher.exe").good() && std::wifstream(L"lol.exe").fail())
	{
		const wchar_t* rads = L"RADS";
		PathAppend(airclient, rads);
		PathAppend(airclient, L"projects");
		PathAppend(airclient, L"lol_air_client");
		const wchar_t* rel = L"releases";
		PathAppend(airclient, rel);
		PathAppend(gameclient, rads);
		PathAppend(gameclient, L"solutions");
		PathAppend(gameclient, L"lol_game_client_sln");
		PathAppend(gameclient, rel);
		PathAppend(gameclient, L"0.0.1.71");
		const wchar_t* dep = L"deploy";
		PathAppend(gameclient, dep);
		PathAppend(airclient, L"0.0.1.124");
		PathAppend(airclient, dep);
		PathAppend(airclient, adobedir);
	}

	wchar_t cgbasepath1[MAX_PATH + 1] = L"";
	wchar_t* cgbasepath = cgbasepath1;
	wcsncat_s(
		cgbasepath1,
		MAX_PATH + 1,
		cgbinpath,
		_TRUNCATE
		);
	wchar_t cgbin1[MAX_PATH + 1] = L"";
	wchar_t* cgbin = cgbin1;
	const wchar_t* cg = L"Cg.dll";
	PathCombine(
		cgbin,
		cgbasepath,
		cg
		);
	wchar_t cgglbin1[MAX_PATH + 1] = L"";
	wchar_t* cgglbin = cgglbin1;
	const wchar_t* cggl = L"CgGL.dll";
	PathCombine(
		cgglbin,
		cgbasepath,
		cggl
		);
	wchar_t cgd3d9bin1[MAX_PATH + 1] = L"";
	wchar_t* cgd3d9bin = cgd3d9bin1;
	const wchar_t* cgd3d9 = L"CgD3D9.dll";
	PathCombine(
		cgd3d9bin,
		cgbasepath,
		cgd3d9
		);
	wchar_t tbb1[MAX_PATH + 1] = L"";
	wchar_t* tbb = tbb1;
	PathCombine(tbb, gameclient, L"tbb.dll");
	wchar_t airdest1[MAX_PATH + 1] = L"";
	wchar_t* airdest = airdest1;
	const wchar_t* air = L"Adobe AIR.dll";
	PathCombine(
		airdest,
		airclient,
		air
		);
	wchar_t airlatest1[MAX_PATH + 1] = L"";
	wchar_t* airlatest = airlatest1;
	PathCombine(
		airlatest,
		adobepath,
		air
		);
	wchar_t res[MAX_PATH + 1] = L"Resources";
	wchar_t* flash = res;
	PathAppend(flash, L"NPSWF32.dll");
	wchar_t flashdest1[MAX_PATH + 1] = L"";
	wchar_t* flashdest = flashdest1;
	PathCombine(
		flashdest,
		airclient,
		flash
		);
	wchar_t flashlatest1[MAX_PATH + 1] = L"";
	wchar_t* flashlatest = flashlatest1;
	PathCombine(
		flashlatest,
		adobepath,
		flash
		);
	wchar_t cgdest1[MAX_PATH + 1] = L"";
	wchar_t* cgdest = cgdest1;
	PathCombine(
		cgdest,
		gameclient,
		cg
		);
	wchar_t cggldest1[MAX_PATH + 1] = L"";
	wchar_t* cggldest = cggldest1;
	PathCombine(
		cggldest,
		gameclient,
		cggl
		);
	wchar_t cgd3d9dest1[MAX_PATH + 1] = L"";
	wchar_t* cgd3d9dest = cgd3d9dest1;
	PathCombine(
		cgd3d9dest,
		gameclient,
		cgd3d9
		);
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	wchar_t finalurl[2048] = L"";
	DWORD dwLength = sizeof(finalurl);
	wchar_t tbbname[2048] = L"";
	if ((osvi.dwMajorVersion == 5) & (osvi.dwMinorVersion == 1))
	{
		wcsncat_s(
			tbbname,
			2048,
			L"XP.dll",
			_TRUNCATE
			);
	}
	else
	{
		int avx2 = 0;
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
			wcsncat_s(
				tbbname,
				2048,
				L"AVX2.dll",
				_TRUNCATE
				);
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if (((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false)) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) == 6))
			{
				wcsncat_s(
					tbbname,
					2048,
					L"AVX.dll",
					_TRUNCATE
					);
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					wcsncat_s(
						tbbname,
						2048,
						L"SSE2.dll",
						_TRUNCATE
						);
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						wcsncat_s(
							tbbname,
							2048,
							L"SSE.dll",
							_TRUNCATE
							);
					}
					else
					{
						wcsncat_s(
							tbbname,
							2048,
							L"Default.dll",
							_TRUNCATE
							);
					}
				}
			}
		}
	}

	UrlCombine(
		L"http://lol.jdhpro.com/",
		tbbname,
		finalurl,
		&dwLength,
		0
		);
	downloadFile(std::wstring(finalurl), std::wstring(tbb));
	CopyFile(cgbin, cgdest, false);
	CopyFile(cgglbin, cggldest, false);
	CopyFile(cgd3d9bin, cgd3d9dest, false);
	CopyFile(airlatest, airdest, false);
	CopyFile(flashlatest, flashdest, false);
	unblockFile(tbb);
	unblockFile(airdest);
	unblockFile(flashdest);
	done = true;
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}