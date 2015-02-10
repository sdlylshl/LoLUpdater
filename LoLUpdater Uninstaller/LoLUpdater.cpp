#include <Windows.h>
#include <string>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>

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

CLimitSingleInstance g_SingleInstanceObj(L"Global\\{101UPD473RUN1NS74113R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCH}");

bool finished = false;
wchar_t loldir[MAX_PATH + 1];
wchar_t gameclient[MAX_PATH + 1] = { 0 };
wchar_t patchclient[MAX_PATH + 1] = { 0 };
wchar_t runair[MAX_PATH + 1] = { 0 };

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
	std::wstring search = { folder + L"\\*" };
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
		data = +newest.info.cFileName;
		if (!FindClose(hFind))
			throw std::runtime_error("failed to close file handle");
	}
	else
		throw std::runtime_error("failed to find file/directory");

	return data;
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
		if (hdc == nullptr)
			throw std::runtime_error("Nothing to render in");

		if (DrawText(hdc, L"Uninstalling, Please Wait!", -1, &start, DT_SINGLELINE | DT_NOCLIP) == NULL)
			throw std::runtime_error("failed to draw text");

		if (finished)
		{
			if (DrawText(hdc, L"Finished!, thank you for testing LoLUpdater!", -1, &end, DT_SINGLELINE | DT_NOCLIP) == NULL)
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

	MSG Msg = { 0 };
	WNDCLASSEX wc = { 0 };
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

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select your (League of Legends)/GarenaLoL/LoLQQ installation directory:";
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

	wchar_t instdir[MAX_PATH + 1] = { 0 };
	PCombine(instdir, loldir, L"lol.launcher.exe");

	wchar_t instdirCN[MAX_PATH + 1] = { 0 };
	PCombine(instdirCN, loldir, L"lol.launcher_tencent.exe");

	wchar_t instdirGarena[MAX_PATH + 1] = { 0 };
	PCombine(instdirGarena, loldir, L"lol.exe");

	wchar_t airclient[MAX_PATH + 1] = { 0 };

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

	wchar_t airdest[MAX_PATH + 1] = { 0 };
	const std::wstring air = L"Adobe AIR.dll";
	PCombine(airdest, airclient, air.c_str());

	wchar_t flash[MAX_PATH + 1] = { L"Resources" };
	PAppend(flash, L"NPSWF32.dll");

	wchar_t flashdest[MAX_PATH + 1] = { 0 };
	PCombine(flashdest, airclient, flash);

	wchar_t cgdest[MAX_PATH + 1] = { 0 };
	PCombine(cgdest, gameclient, L"cg.dll");

	wchar_t cggldest[MAX_PATH + 1] = { 0 };
	PCombine(cggldest, gameclient, L"cgGL.dll");

	wchar_t cgd3d9dest[MAX_PATH + 1] = { 0 };
	PCombine(cgd3d9dest, gameclient, L"cgd3d9.dll");

	wchar_t msvcpdest[MAX_PATH + 1] = { 0 };
	PCombine(msvcpdest, patchclient, L"msvcp120.dll");

	wchar_t msvcrdest[MAX_PATH + 1] = { 0 };
	PCombine(msvcrdest, patchclient, L"msvcr120.dll");

	wchar_t tbbdest[MAX_PATH + 1] = { 0 };
	PCombine(tbbdest, gameclient, L"tbb.dll");

	ExtractResource(1, airdest);
	ExtractResource(2, cgdest);
	ExtractResource(3, cggldest);
	ExtractResource(4, cgd3d9dest);
	ExtractResource(5, msvcpdest);
	ExtractResource(6, msvcrdest);
	ExtractResource(7, flashdest);
	ExtractResource(8, tbbdest);

	finished = true;

	RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}