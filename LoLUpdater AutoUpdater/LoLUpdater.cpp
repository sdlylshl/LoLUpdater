#include <Windows.h>

void downloadFile(std::wstring const& url, std::wstring const& file)
{
	if (URLDownloadToFile(nullptr, url.c_str(), file.c_str(), 0, nullptr) != S_OK)
		throw std::runtime_error("failed to initialize download");
}

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

CLimitSingleInstance g_SingleInstanceObj(L"Global\\{101UPD473R4U70UPD473R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCH}");


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR, int nCmdShow)
{
	const std::wstring latestversion = L"latestversion.txt";
	downloadFile(L"http://lol.jdhpro.com/latestversion.txt", latestversion.c_str());
	std::ifstream t("file.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
}
