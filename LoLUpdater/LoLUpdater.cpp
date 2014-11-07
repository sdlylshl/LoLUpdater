#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#define VC_EXTRALEAN
#include <Windows.h>

int avx2()
{
	int abcd[4];
	static int canuseavx2;
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
	if (canuseavx2 < 0)
	{
		return canuseavx2;
	}

	return 1;
}
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

// Keep magical numbers
// 0 = adobe air installation directory
// 1 = path to where tbb.dll will be downloaded
// 2 = path to the latest adobe air.dll
// 3 = path to where adobe air.dll will be copied to
// 4 = path to the latest "flash" dll
// 5 = path to where the flash dll will be copied to
// 6 = path to where the updated cg.dll is.
// 7 = path to the cginstaller that is downloaded together with the unblock tag
// 8 = path to the adobe air installer that is downloaded together with the unblock tag
// 9 = path together with the unblock tag to where the adobe air.dll is in the LoL installation.
// 10 = path to where the updated cgd3d9.dll is.
// 11 = path to where the updated cggl.dll is.
// 12 = path to the final destination of the updated cg.dll
// 13 = path to the final destination of the updated cggl.dll
// 14 = path to the tbb dll together with the unblock tag
// 15 = path to the "flash" dll together with the unblock tag
// 16 = path to the final destination of the updated cgd3d9.dll
// 17 = full path to where all adobe files will be copied to.
// 18 = full path to where all game files will be copied to.
// 19 = path to the current working directory (where the executable was ran from)
// 20 = path to air client (RADS)
// 21 = path to game client (RADS)
// 22 = 20 + releases
// 23 = 21 + releases
std::wstringstream pathcontainer[24];

void Copy(int from, int to)
{
	CopyFile(
		pathcontainer[from].str().c_str(),
		pathcontainer[to].str().c_str(),
		false
		);
}

void charreduction(int dest, int path1, const std::wstring path2)
{
	pathcontainer[dest] << (pathcontainer[path1].str().c_str() + path2);
}

std::vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);
std::vector<wchar_t> currentdirectorybuffer(MAX_PATH + 1, 0);
const std::wstring unblock(L":Zone.Identifier");
const std::wstring air(L"Adobe AIR.dll");
const std::wstring flash(L"Resources\\NPSWF32.dll");
const std::wstring cgfile(L"Cg.dll");
const std::wstring cgglfile(L"CgGL.dll");
const std::wstring cgd3d9file(L"CgD3D9.dll");
const std::wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
const std::wstring tbbfile(L"tbb.dll");
const std::wstring airwin(L"air15_win.exe");
const std::wstring deploy(L"\\deploy\\");
const std::wstring airpart(L"Air\\Adobe AIR\\Versions\\1.0\\");
const std::wstring ftp(L"http://lol.jdhpro.com/");
const std::wstring rads(L"RADS\\");
const std::wstring progfiles(L":\\Program Files");
const std::wstring rel(L"\\releases\\");
int bit = sizeof(void*);
bool done = false;
bool garena = std::wifstream(L"lol.exe").good();


void download(const std::wstring fromurl, const std::wstring topath, int pathcont, int frompathcont, const std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		fromurl.c_str(),
		topath.c_str(),
		0,
		nullptr
		);
	pathcontainer[pathcont] << (pathcontainer[frompathcont].str() + topath + &unblock[0]);
	DeleteFile(pathcontainer[pathcont].str().c_str());
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = topath.c_str();
	ShExecInfo.lpParameters = args.c_str();
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void tbbdownload(const std::wstring url)
{
	URLDownloadToFile(
		nullptr,
		url.c_str(),
		pathcontainer[1].str().c_str(),
		0,
		nullptr
		);
}

HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

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

std::vector<std::wstring> MyVect;
std::wstring airver;
std::wstring gamever;

std::wstring getlatestfolder(const std::wstring path)
{
	std::wstringstream versionfolders[1];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	std::wstring sPath;
	std::wstring max;
	hFind = FindFirstFile(path.c_str(), &FindFileData);
	do
	{
		if (FindFileData.dwFileAttributes == 16)
		{
			MyVect.push_back(FindFileData.cFileName);
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	for (int i = 0; i < MyVect.size(); i++)
	{
		versionfolders[0] << MyVect.at(i).data();
	}

	std::wstring finalDirectory = L"";
	std::wstring version = L"";
	uint32_t versionCompare = 0;
	for (std::wstring x : versionfolders[0].str)
	{
		std::wstring compare1 = x.substr(x.find_last_of(L"\\/") + 1);

			
		std::wstring versionParts = compare1.Split /* Todo: String.Split but in C++ */ (L".");
		if (!compare1.find(L".") || sizeof(versionParts) != 4)
			continue;
		uint32_t CompareVersion;
		CompareVersion = static_cast<uint32_t>(versionParts[0]) << 24 | static_cast<uint32_t>(versionParts[1]) << 16 | static_cast<uint32_t>(versionParts[2]) << 8 | static_cast<uint32_t>(versionParts[3]);

		if (CompareVersion > versionCompare)
		{
			versionCompare = CompareVersion;
			version = x.replace(path.begin(), path.end, L"\\", L"");
			finalDirectory = x;
		}
	}

	return max;
}

std::wstring gamedir()
{
	if (garena)
	{
		return L"Game\\";
	}
	return (pathcontainer[22].str().c_str() + getlatestfolder(pathcontainer[20].str().c_str()) + deploy);
}

std::wstring airdir()
{
	if (garena)
	{
		return airpart;
	}
	return (pathcontainer[23].str().c_str() + getlatestfolder(pathcontainer[21].str().c_str()) + deploy + L"Adobe " + airpart);
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
	pathcontainer[20] <<(rads + L"projects\\lol_air_client");
	pathcontainer[21] << (rads + L"solutions\\lol_game_client_sln");
	charreduction(22, 20, rel);
	charreduction(23, 21, rel);
	GetModuleFileName(nullptr, &currentdirectorybuffer[0], MAX_PATH + 1);
	pathcontainer[19] << (std::wstring(&currentdirectorybuffer[0]).substr(0, std::wstring(&currentdirectorybuffer[0]).find_last_of(L"\\/")) + L"\\");
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", cginstaller.c_str(), 7, 19, L"/verysilent /TYPE = compact");
	GetEnvironmentVariable(L"CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH + 1);
	wcsncat_s(
		&cgbinpath[0],
		MAX_PATH + 1,
		L"\\",
		_TRUNCATE
		);
	pathcontainer[0] << pathcontainer[19].str().c_str()[0];
	if (bit == 8)
	{
		pathcontainer[0] << (progfiles + L" (x86)");
	}
	else
	{
		pathcontainer[0] << progfiles;
	}
	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", airwin.c_str(), 8, 19, L"-silent");
	pathcontainer[0] << (L"\\Common Files\\Adobe " + airpart);
	pathcontainer[6] << (&cgbinpath[0] + cgfile);
	pathcontainer[11] << (&cgbinpath[0] + cgglfile);
	pathcontainer[10] << (&cgbinpath[0] + cgd3d9file);
	charreduction(18, 19, gamedir());
	charreduction(17, 19, airdir());
	charreduction(1, 18, tbbfile);
	charreduction(2, 0, air);
	charreduction(4, 0, flash);
	charreduction(12, 18, cgfile);
	charreduction(13, 18, cgglfile);
	charreduction(16, 18, cgd3d9file);
	charreduction(3, 17, air);
	charreduction(5, 17, flash);
	charreduction(9, 3, unblock);
	charreduction(15, 5, unblock);
	charreduction(14, 1, unblock);
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	{
		tbbdownload(ftp + L"Xp.dll");
	}
	else
	{
		if (avx2())
		{
			tbbdownload(ftp + L"Avx2.dll");
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
			{
				tbbdownload(ftp + L"Avx.dll");
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(ftp + L"Sse2.dll");
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						tbbdownload(ftp + L"Sse.dll");
					}
					else
					{
						tbbdownload(ftp + L"Default.dll");
					}
				}
			}
		}
	}

	DeleteFile(pathcontainer[14].str().c_str());
	Copy(6, 12);
	Copy(11, 13);
	Copy(10, 16);
	Copy(2, 3);
	Copy(4, 5);
	done = true;
	InvalidateRect(hwnd, &end, false);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}