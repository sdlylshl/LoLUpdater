#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>

bool done = false;
// 0 = adobe install dir
// 1 = unblock air
// 2 = unblock flash
// 3 = unblock tbb
std::wstringstream pathcontainer[4];

const std::wstring constants[3] = { std::wstring(L":Zone.Identifier"), std::wstring(L"Adobe AIR\\Versions\\1.0"), std::wstring(L"AIR\\") };
wchar_t* tbb;

std::wstring cwd(_wgetcwd(
	nullptr,
	0
	));
HWND hwnd;
const std::wstring g_szClassName(L"mainwindow1");
RECT start = { 0, 0, 100, 20 };
RECT end = { 0, 100, 100, 120 };

inline void download(const std::wstring& url, const std::wstring& file, const std::wstring& args)
{
	URLDownloadToFile(
		nullptr,
		url.c_str(),
		file.c_str(),
		0,
		nullptr
		);

	wchar_t unblocker[MAX_PATH] = L"";
	wchar_t* unblocker1;
	unblocker1 = unblocker;

	wchar_t unblocker2[MAX_PATH] = L"";

	wcsncat_s(
		unblocker2,
		MAX_PATH,
		reinterpret_cast<LPWSTR>(&cwd[0]),
		_TRUNCATE
		);

	wchar_t* unblockerq;
	unblockerq = unblocker2;

	wchar_t unblocker3[MAX_PATH] = L"";

	wcsncat_s(
		unblocker3,
		MAX_PATH,
		file.c_str(),
		_TRUNCATE
		);

	wchar_t* unblockerqq;
	unblockerqq = unblocker3;

	PathCombine(
		unblocker1,
		unblockerq,
		unblockerqq
		);

	pathcontainer[0] << (std::wstring(unblocker1) + constants[0]);

	DeleteFile(pathcontainer[0].str().c_str());
	pathcontainer[0].str(std::wstring());
	pathcontainer[0].clear();

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = L"runas";
	ShExecInfo.lpFile = file.c_str();
	ShExecInfo.lpParameters = args.c_str();
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

inline void tbbdownload(const std::wstring& file)
{
	std::wstring* ftp[MAX_PATH];

	UrlCombine(
		L"http://lol.jdhpro.com/",
		file.c_str(),
		reinterpret_cast<LPWSTR>(ftp),
		reinterpret_cast<LPDWORD>(ftp),
		0
		);

	URLDownloadToFile(
		nullptr,
		reinterpret_cast<LPWSTR>(ftp),
		tbb,
		0,
		nullptr
		);
}

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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