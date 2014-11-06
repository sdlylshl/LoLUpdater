#define VC_EXTRALEAN

#include <tchar.h>
#include <direct.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <ShlObj.h>
#include <VersionHelpers.h>

#include <stdint.h>
#include <intrin.h>

#include <d3dx9core.h>
#include <d3d9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>

int s_width = 800;
int s_height = 600;
#define CENTERX (GetSystemMetrics(SM_CXSCREEN)/2)-(s_width/2)
#define CENTERY (GetSystemMetrics(SM_CYSCREEN)/2)-(s_height/2)
LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;
HWND hWnd;
const MARGINS margin = {0, 0, s_width, s_height};
LPD3DXFONT pFont;

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
	static int the_4th_gen_features_available = -1;
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();
	return the_4th_gen_features_available;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#ifdef _WIN64
#define ENVIRONMENT64
#elif defined _WIN32
#define ENVIRONMENT32
#endif

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

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
std::wstringstream pathcontainer[20];

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

std::vector<std::wstring> cgbinpath(MAX_PATH + 1, nullptr);
std::vector<std::wstring> cwd0(MAX_PATH + 1, nullptr);

const std::wstring unblock(L":Zone.Identifier");
const std::wstring air(L"Adobe AIR.dll");
const std::wstring flash(L"Resources\\NPSWF32.dll");
const std::wstring cgfile(L"Cg.dll");
const std::wstring cgglfile(L"CgGL.dll");
const std::wstring cgd3d9file(L"CgD3D9.dll");
const std::wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
const std::wstring tbbfile(L"tbb.dll");
const std::wstring airwin(L"air15_win.exe");

bool garena = std::wifstream(L"lol.exe").good();

std::wstring airdir()
{
	if (garena)
	{
		return L"Air\\Adobe AIR\\Versions\\1.0\\";
	}
	return L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\";
}

std::wstring gamedir()
{
	if (garena)
	{
		return L"Game\\";
	}
	return L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\";
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
	pathcontainer[pathcont] << (pathcontainer[frompathcont].str() + topath + &unblock[0]);
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

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp)); // clear out the struct for use
	d3dpp.Windowed = TRUE; // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames
	d3dpp.hDeviceWindow = hWnd; // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = s_width; // set the width of the buffer
	d3dpp.BackBufferHeight = s_height; // set the height of the buffer

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
	                                    D3DDEVTYPE_HAL,
	                                    hWnd,
	                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
	                                    &d3dpp,
	                                    &d3ddev);

	D3DXCreateFont(d3ddev, 50, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &pFont);
}

void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, std::wstring fmt)
{
	RECT FontPos = {x, y, x + 120, y + 16};
	std::wstring buf[1024] = {'\0'};
	va_list va_alist;
	
	va_start(va_alist, fmt);
	wprintf_s(std::wstring(buf[0].begin(), buf[0].end()).c_str(), fmt, va_alist);
	va_end(va_alist);
	g_pFont->DrawText(NULL, std::wstring(buf[0].begin(), buf[0].end()).c_str(), -1, &FontPos, DT_NOCLIP, color);
}

void render()
{
	// clear the window alpha
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene(); // begins the 3D scene

	DrawString(10, 50, D3DCOLOR_ARGB(255, 255, 0, 0), pFont, L"Patching...");

	d3ddev->EndScene(); // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL); // displays the created frame on the screen
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	std::wstring value = L"LoLUpdater";


	RECT rc;


	HWND newhwnd = FindWindow(nullptr, value.c_str());
	if (newhwnd != NULL)
	{
		GetWindowRect(newhwnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
	}
	else
	{
		ExitProcess(0);
	}
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(RGB(0, 0, 0));
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(0,
	                      L"WindowClass",
	                      L"",
	                      WS_EX_TOPMOST | WS_POPUP,
	                      rc.left, rc.top,
	                      s_width, s_height,
	                      nullptr,
	                      nullptr,
	                      hInstance,
	                      nullptr);

	SetWindowLong(hWnd, GWL_EXSTYLE, static_cast<int>(GetWindowLong(hWnd, GWL_EXSTYLE)) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);

	ShowWindow(hWnd, nCmdShow);


	initD3D(hWnd);
	MSG msg;
	SetWindowPos(FindWindow(nullptr, value.c_str()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	while (TRUE)
	{
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		if (!FindWindow(nullptr, value.c_str()))
			ExitProcess(0);
		render();

		GetModuleFileName(nullptr, const_cast< wchar_t* >(std::wstring(cwd0[0].begin(), cwd0[0].end()).c_str()), MAX_PATH + 1);
		pathcontainer[19] << (std::wstring(const_cast< wchar_t* >(std::wstring(cwd0[0].begin(), cwd0[0].end()).c_str())).substr(0, std::wstring(const_cast< wchar_t* >(std::wstring(cwd0[0].begin(), cwd0[0].end()).c_str())).find_last_of(L"\\/")) + L"\\");
		download(L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe", cginstaller.c_str(), 7, 19, L"/verysilent /TYPE = compact");
		GetEnvironmentVariableW(L"CG_BIN_PATH",
			const_cast< wchar_t* >(std::wstring(cgbinpath[0].begin(), cgbinpath[0].end()).c_str()),
		                        MAX_PATH + 1);
		wcsncat_s(
			const_cast< wchar_t* >(std::wstring(cgbinpath[0].begin(), cgbinpath[0].end()).c_str()),
			MAX_PATH + 1,
			L"\\",
			_TRUNCATE
		);
		pathcontainer[0] << pathcontainer[19].str().c_str()[0];
#ifdef ENVIRONMENT64
		pathcontainer[0] << ":\\Program Files (x86)";
#else
		pathcontainer[0] << ":\\Program Files";
#endif
		download(L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe", airwin.c_str(), 8, 19, L"-silent");
		pathcontainer[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";
		pathcontainer[6] << (const_cast< wchar_t* >(std::wstring(cgbinpath[0].begin(), cgbinpath[0].end()).c_str()) + cgfile);
		pathcontainer[11] << (const_cast< wchar_t* >(std::wstring(cgbinpath[0].begin(), cgbinpath[0].end()).c_str()) + cgglfile);
		pathcontainer[10] << (const_cast< wchar_t* >(std::wstring(cgbinpath[0].begin(), cgbinpath[0].end()).c_str()) + cgd3d9file);
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

		if (IsWindowsXPSP3OrGreater() && !IsWindowsVistaOrGreater())
		{
			tbbdownload(L"http://lol.jdhpro.com/Xp.dll");
		}
		if (can_use_intel_core_4th_gen_features())
		{
			tbbdownload(L"http://lol.jdhpro.com/Avx2.dll");
		}
		else
		{
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			if ((cpuInfo[2] & (1 << 27) || false) && (cpuInfo[2] & (1 << 28) || false) && ((_xgetbv(_XCR_XFEATURE_ENABLED_MASK) & 0x6) || false))
			{
				tbbdownload(L"http://lol.jdhpro.com/Avx.dll");
			}
			else
			{
				if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
				{
					tbbdownload(L"http://lol.jdhpro.com/Sse2.dll");
				}
				else
				{
					if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
					{
						tbbdownload(L"http://lol.jdhpro.com/Sse.dll");
					}
					else
					{
						tbbdownload(L"http://lol.jdhpro.com/Default.dll");
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
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);
	}
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{
			DwmExtendFrameIntoClientArea(hWnd, &margin);
		}
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}