#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#define VC_EXTRALEAN
#include <Windows.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
bool done = false;

std::wstringstream pathcontainer[24];
void Copy(int from, int to)
{
	CopyFile(
		pathcontainer[from].str().c_str(),
		pathcontainer[to].str().c_str(),
		false
	);
}

const std::wstring constants[15] = {L":Zone.Identifier", L"Adobe AIR.dll", L"Resources\\NPSWF32.dll", L"Cg.dll", L"CgGL.dll", L"CgD3D9.dll", L"Cg-3.1_April2012_Setup.exe", L"tbb.dll", L"air15_win.exe", L"\\deploy\\", L"Air\\Adobe AIR\\Versions\\1.0\\", L"http://lol.jdhpro.com/", L"RADS\\", L":\\Program Files", L"\\releases\\"};
void charreduction(int dest, int path1, int path2)
{
	pathcontainer[dest] << (pathcontainer[path1].str().c_str() + constants[path2]);
}

void download(const std::wstring fromurl, const std::wstring topath, int pathcont, int frompathcont, const std::wstring args)
{
	URLDownloadToFile(
		nullptr,
		fromurl.c_str(),
		topath.c_str(),
		0,
		nullptr
	);
	pathcontainer[pathcont] << (pathcontainer[frompathcont].str() + topath + constants[0]);
	DeleteFile(pathcontainer[pathcont].str().c_str());
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
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
std::wstring gamedir()
{
	if (garena)
	{
		return L"Game\\";
	}
	return (pathcontainer[22].str() + L"0.0.1.64" + constants[9]);
}

std::wstring airdir()
{
	if (garena)
	{
		return constants[10];
	}
	return (pathcontainer[23].str() + L"0.0.1.117" + constants[9] + L"Adobe " + constants[10]);
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
	pathcontainer[20] << (constants[12] + L"projects\\lol_air_client");
	pathcontainer[21] << (constants[12] + L"solutions\\lol_game_client_sln");
	charreduction(22, 20, 14);
	charreduction(23, 21, 14);
	std::vector<wchar_t> currentdirectorybuffer(MAX_PATH + 1, 0);
	GetModuleFileName(nullptr, &currentdirectorybuffer[0], MAX_PATH + 1);
	pathcontainer[19] << (std::wstring(&currentdirectorybuffer[0]).substr(0, std::wstring(&currentdirectorybuffer[0]).find_last_of(L"\\/")) + L"\\");
	download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", constants[6].c_str(), 7, 19, L"/verysilent /TYPE = compact");
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
	pathcontainer[0] << pathcontainer[19].str().c_str()[0];
	int bit = sizeof(void*);
	if (bit == 8)
	{
		pathcontainer[0] << (constants[13] + L" (x86)");
	}
	else
	{
		pathcontainer[0] << constants[13];
	}
	download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", constants[8].c_str(), 8, 19, L"-silent");
	pathcontainer[0] << (L"\\Common Files\\Adobe " + constants[10]);
	pathcontainer[6] << (&cgbinpath[0] + constants[3]);
	pathcontainer[11] << (&cgbinpath[0] + constants[4]);
	pathcontainer[10] << (&cgbinpath[0] + constants[5]);
	pathcontainer[18] << (pathcontainer[19].str() + gamedir());
	pathcontainer[17] << (pathcontainer[19].str() + airdir());

	charreduction(1, 18, 7);
	charreduction(2, 0, 1);
	charreduction(4, 0, 2);
	charreduction(12, 18, 3);
	charreduction(13, 18, 4);
	charreduction(16, 18, 5);
	charreduction(3, 17, 1);
	charreduction(5, 17, 2);
	charreduction(9, 3, 0);
	charreduction(15, 5, 0);
	charreduction(14, 1, 0);
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
	{
		tbbdownload(constants[11] + L"Xp.dll");
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
			tbbdownload(constants[11] + L"Avx2.dll");
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
			{
				tbbdownload(constants[11] + L"Avx.dll");
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(constants[11] + L"Sse2.dll");
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						tbbdownload(constants[11] + L"Sse.dll");
					}
					else
					{
						tbbdownload(constants[11] + L"Default.dll");
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