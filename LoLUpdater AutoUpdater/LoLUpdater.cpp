#include <Windows.h>
#include <cwchar>
#include <string>
#include <fstream>
#include <sstream>

class CLimitSingleInstance
{
protected:
	DWORD m_dLastError;
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

CLimitSingleInstance g_SingleInstanceObj(L"Global\\{101UPD473R4U70UPD473R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCH}");

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

	bool operator ==(const Version& other)
	{
		return major == other.major
			&& minor == other.minor
			&& revision == other.revision
			&& build == other.build;
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

bool noupdate = false;
bool update = false;

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	if (URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr) != S_OK)
		throw std::runtime_error("failed to initialize download");
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

		if (DrawText(hdc, L"Check for updates, Please Wait!", -1, &start, DT_SINGLELINE | DT_NOCLIP) == NULL)
			throw std::runtime_error("failed to draw text");

		if (update)
		{
			if (DrawText(hdc, L"Updated!", -1, &end, DT_SINGLELINE | DT_NOCLIP) == NULL)
				throw std::runtime_error("failed to draw text");
		}
		if (noupdate)
		{
			if (DrawText(hdc, L"No update found", -1, &end, DT_SINGLELINE | DT_NOCLIP) == NULL)
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

	auto hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName.c_str(), L"LoLUpdater AutoUpdater", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 260, 100, nullptr, nullptr, hInstance, nullptr);

	if (hwnd == nullptr)
	{
		throw std::runtime_error("failed to create window");
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	const std::wstring majortxt = L"major.txt";
	const std::wstring minortxt = L"minor.txt";
	const std::wstring revisiontxt = L"revision.txt";
	const std::wstring buildtxt = L"build.txt";
	downloadFile(L"http://lol.jdhpro.com/major.txt", majortxt);
	downloadFile(L"http://lol.jdhpro.com/minor.txt", minortxt);
	downloadFile(L"http://lol.jdhpro.com/revision.txt", revisiontxt);
	downloadFile(L"http://lol.jdhpro.com/build.txt", buildtxt);

	std::wifstream t0(majortxt);
	std::wstringstream buffer0;
	buffer0 << t0.rdbuf();

	std::wifstream t1(minortxt);
	std::wstringstream buffer1;
	buffer1 << t1.rdbuf();

	std::wifstream t2(revisiontxt);
	std::wstringstream buffer2;
	buffer2 << t2.rdbuf();

	std::wifstream t3(buildtxt);
	std::wstringstream buffer3;
	buffer3 << t3.rdbuf();

	const auto major = buffer0.str();
	const auto minor = buffer1.str();
	const auto revision = buffer2.str();
	const auto build = buffer3.str();

	wchar_t fileName[MAX_PATH] = {L"LoLUpdater.exe"};
	auto size = GetModuleFileName(nullptr, fileName, _MAX_PATH);
	fileName[size] = NULL;
	DWORD handle = 0;
	size = GetFileVersionInfoSize(fileName, &handle);
	auto versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(fileName, handle, size, versionInfo))
	{
		delete[] versionInfo;
		return;
	}
	// we have version information
	UINT32 len = 0;
	int aVersion[4];
	VS_FIXEDFILEINFO* vsfi = nullptr;
	VerQueryValue(versionInfo, L"\\", reinterpret_cast<void**>(&vsfi), &len);
	aVersion[0] = HIWORD(vsfi->dwFileVersionMS);
	aVersion[1] = LOWORD(vsfi->dwFileVersionMS);
	aVersion[2] = HIWORD(vsfi->dwFileVersionLS);
	aVersion[3] = LOWORD(vsfi->dwFileVersionLS);
	delete[] versionInfo;

	if (Version(std::wstring(aVersion[0] + L"." + aVersion[1] + std::wstring(L"." + aVersion[2]) + std::wstring(L"." + aVersion[3]))) < Version(std::wstring(major + L"." + minor + L"." + revision + L"." + build)))
	{
		downloadFile(L"http://www.smoothdev.org/mirrors/user/Loggan/LoLUpdater.exe", L"LoLUpdater.exe");
		update = true;
	}
	else
	{
		noupdate = true;
	}

	DeleteFile(majortxt.c_str());
	DeleteFile(minortxt.c_str());
	DeleteFile(revisiontxt.c_str());
	DeleteFile(buildtxt.c_str());

	RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
