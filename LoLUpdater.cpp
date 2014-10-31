// LoLUpdater.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <iostream>
#include <vector>
#include <tchar.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#if XP == FALSE
bool avxSupported = false;

int cpuInfo[4];
__cpuid(cpuInfo, 1);

bool osUsesXSAVE_XRSTORE = cpuInfo[2] & (1 << 27) || false;
bool cpuAVXSuport = cpuInfo[2] & (1 << 28) || false;

#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)

#include <immintrin.h>

int check_4th_gen_intel_core_features()
{
	const int the_4th_gen_features =
		(_FEATURE_AVX2 | _FEATURE_FMA | _FEATURE_BMI | _FEATURE_LZCNT | _FEATURE_MOVBE);
	return _may_i_use_cpu_feature(the_4th_gen_features);
}

#else /* non-Intel compiler */

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif

void run_cpuid(uint32_t eax, uint32_t ecx, uint32_t* abcd)
{
#if defined(_MSC_VER)
	__cpuidex(abcd, eax, ecx);
#else
	uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
	/* in case of PIC under 32-bit EBX cannot be clobbered */
	__asm__("movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
	__asm__("cpuid" : "+b" (ebx),
# endif
		"+a" (eax), "+c" (ecx), "=d" (edx));
	abcd[0] = eax; abcd[1] = ebx; abcd[2] = ecx; abcd[3] = edx;
#endif
}     

int check_xcr0_ymm() 
{
	uint32_t xcr0;
#if defined(_MSC_VER)
	xcr0 = (uint32_t)_xgetbv(0);  /* min VS2010 SP1 compiler is required */
#else
	__asm__ ("xgetbv" : "=a" (xcr0) : "c" (0) : "%edx" );
#endif
	return ((xcr0 & 6) == 6); /* checking if xmm and ymm state are enabled in XCR0 */
}


int check_4th_gen_intel_core_features()
{
	uint32_t abcd[4];
	uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
	uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);

	/* CPUID.(EAX=01H, ECX=0H):ECX.FMA[bit 12]==1   &&
	CPUID.(EAX=01H, ECX=0H):ECX.MOVBE[bit 22]==1 &&
	CPUID.(EAX=01H, ECX=0H):ECX.OSXSAVE[bit 27]==1 */
	run_cpuid(1, 0, abcd);
	if ((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask)
		return 0;

	if (!check_xcr0_ymm())
		return 0;

	/*  CPUID.(EAX=07H, ECX=0H):EBX.AVX2[bit 5]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI1[bit 3]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI2[bit 8]==1  */
	run_cpuid(7, 0, abcd);
	if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		return 0;

	/* CPUID.(EAX=80000001H):ECX.LZCNT[bit 5]==1 */
	run_cpuid(0x80000001, 0, abcd);
	if ((abcd[2] & (1 << 5)) == 0)
		return 0;

	return 1;
}

#endif /* non-Intel compiler */


static int can_use_intel_core_4th_gen_features()
{
	static int the_4th_gen_features_available = -1;
	/* test is performed once */
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();

	return the_4th_gen_features_available;
}
#endif

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

bool file_exists(wstring(fileName))
{
	ifstream infile(fileName);
	return infile.good();
}

// Buffers
wstringstream buff_c[MAX_PATH + 1];
wstringstream tbb0[MAX_PATH + 1];
wstringstream airfile[MAX_PATH + 1];
wstringstream airdir[MAX_PATH + 1];
wstringstream flashfile[MAX_PATH + 1];
wstringstream flashdir[MAX_PATH + 1];
wstringstream cgbin[MAX_PATH + 1];
wstringstream cginstunblock[MAX_PATH + 1];
wstringstream airinstunblock[MAX_PATH + 1];
wstringstream strair[MAX_PATH + 1];
wstringstream airunblock[MAX_PATH + 1];
wstringstream cgd3d9bin[MAX_PATH + 1];
wstringstream cgglbin[MAX_PATH + 1];
wstringstream cgpath[MAX_PATH + 1];
wstringstream cgglpath[MAX_PATH + 1];
wstringstream tbbunblock[MAX_PATH + 1];
wstringstream flashunblock[MAX_PATH + 1];
wstringstream cgd3d9path[MAX_PATH + 1];
wstringstream cgglunblock[MAX_PATH + 1];
wstringstream cgunblock[MAX_PATH + 1];
wstringstream cgd3d9unblock[MAX_PATH + 1];
wstringstream airpath_f[MAX_PATH + 1];
wstringstream slnpath_f[MAX_PATH + 1];
wstringstream airinst[MAX_PATH + 1];
wstringstream cginst[MAX_PATH + 1];
vector<wchar_t> cwd(MAX_PATH + 1, 0);
vector<wchar_t> cgbinpath(MAX_PATH + 1, 0);

// Constants
wstring unblock(L":Zone.Identifier");
wstring air(L"Adobe AIR.dll");
wstring flash(L"Resources\\NPSWF32.dll");
wstring cgfile(L"Cg.dll");
wstring cgglfile(L"CgGL.dll");
wstring cgd3d9file(L"CgD3D9.dll");
wstring cginstaller(L"Cg-3.1_April2012_Setup.exe");
wstring tbbfile(L"tbb.dll");
wstring airwin(L"air15_win.exe");
wstring unblockfiles[] = { cgunblock[0].str(), cgglunblock[0].str(), cgd3d9unblock[0].str(), tbbunblock[0].str(), airunblock[0].str(), flashunblock[0].str() };

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	wcout << L"LoLUpdater Alpha 1 Build 11";
	wcout << endl;
	wcout << L"Patching..." << endl;

// Gets the executable path without filename
	GetModuleFileNameW((HINSTANCE)&__ImageBase, &cwd[0], MAX_PATH + 1);

// appends a backslash to the path for later processing.
	wcsncat(&cwd[0], L"\\", MAX_PATH + 1);

// Attemts to get the CG_BIN_PATH Env-varible
	GetEnvironmentVariableW(L"CG_BIN_PATH",
		&cgbinpath[0],
		MAX_PATH + 1);

// If above attempt failed then install Nvidia CG to populate the variable.
	if (&cgbinpath[0] == L"")
	{
// Downloads Nvidia-CG
		URLDownloadToFileW(
			nullptr,
			L"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			cginst[0].str().c_str(),
			0,
			nullptr
			);
// Unblocks the installer
		DeleteFileW(cginstunblock[0].str().c_str());

// Starts the executable with some arguments
		SHELLEXECUTEINFOW ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = cginst[0].str().c_str();
		ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteExW(&ShExecInfo);
// Wait for process to finish before continuing.
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

// Now we know that the variable name exists in %PATH, populate the cgbinpath variable.
		GetEnvironmentVariableW(L"CG_BIN_PATH",
			&cgbinpath[0],
			MAX_PATH + 1);
	}
	wcsncat(&cgbinpath[0], L"\\", MAX_PATH + 1);

	// Pathbuilder
	buff_c[0] << cwd[0];
#if defined(ENVIRONMENT64)
	buff_c[0] << L":\\Program Files (x86)";
#elif defined(ENVIRONMENT32)
	buff_c[0] << L":\\Program Files";
#endif
	buff_c[0] << L"\\Common Files\\Adobe AIR\\Versions\\1.0\\";

	wstring slnpath(L"RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\");
	wstring airpath(L"RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\");

	if (file_exists(L"lol.exe"))
	{
// Overload if it is Garena
		wstring airpath(L"Air\\Adobe AIR\\Versions\\1.0\\");
		wstring slnpath(L"Game\\");
	}

	slnpath_f[0] << &cwd[0];
	slnpath_f[0] << &slnpath[0];

	airpath_f[0] << &cwd[0];
	airpath_f[0] << &airpath[0];

	tbb0[0] << slnpath_f[0].str();
	tbb0[0] << tbbfile;

	airinst[0] << &cwd[0];
	airinst[0] << airwin;

	cginst[0] << &cwd[0];
	cginst[0] << cginstaller;

	airinstunblock[0] << &airinst[0];
	airinstunblock[0] << &unblock[0];

	cginstunblock[0] << &cginst[0];
	cginstunblock[0] << &unblock[0];

	// End Pathbuilder

	// Gets location of latest cg dll
	cgbin[0] << &cgbinpath[0];
	cgbin[0] << cgfile;

	// Gets location of latest cggl dll
	cgglbin[0] << &cgbinpath[0];
	cgglbin[0] << cgglfile;

	// Gets location of latest cgd3d9 dll
	cgd3d9bin[0] << &cgbinpath[0];
	cgd3d9bin[0] << cgd3d9file;

	// Gets location of latest adobe air dll
	airfile[0] << buff_c[0].str().c_str();
	airfile[0] << air;

	// Gets location of latest "flash" dll
	flashfile[0] << buff_c[0].str().c_str();
	flashfile[0] << flash;

	URLDownloadToFileW(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		airinst[0].str().c_str(),
		0,
		nullptr
		);

	DeleteFileW(airinstunblock[0].str().c_str());
	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = airinst[0].str().c_str();
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	// Working Directory
	cgpath[0] << slnpath_f[0].str();
	cgpath[0] << cgfile;

	cgglpath[0] << slnpath_f[0].str();
	cgglpath[0] << cgglfile;

	cgd3d9path[0] << slnpath_f[0].str();
	cgd3d9path[0] << cgd3d9file;

	airdir[0] << airpath_f[0].str();
	airdir[0] << air;

	flashdir[0] << airpath_f[0].str();
	flashdir[0] << flash;

	cgunblock[0] << cgpath[0].str();
	cgunblock[0] << &unblock[0];

	cgglunblock[0] << cgglpath[0].str();
	cgglunblock[0] << &unblock[0];

	cgd3d9unblock[0] << cgd3d9path[0].str();
	cgd3d9unblock[0] << &unblock[0];

	airunblock[0] << airdir[0].str();
	airunblock[0] << &unblock[0];

	flashunblock[0] << flashdir[0].str();
	flashunblock[0] << &unblock[0];

	tbbunblock[0] << tbb0[0].str();
	tbbunblock[0] << &unblock[0];
	// Working Directory End

#if XP
	// XP
	URLDownloadToFileW(
		nullptr,
		L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Xp.dll",
		tbb0[0].str().c_str(),
		0,
		nullptr
		);
#else
	if (can_use_intel_core_4th_gen_features())
	{
		// AVX2
		URLDownloadToFileW(
			nullptr,
			L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx2.dll",
			tbb0[0].str().c_str(),
			0,
			nullptr
			);
	}
	else
	{
		if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
		{
			// Check if the OS will save the YMM registers
			unsigned long long xcrFeatureMask = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
			avxSupported = (xcrFeatureMask & 0x6) || false;
		}

		// AVX
		if (avxSupported)
		{
			URLDownloadToFileW(
				nullptr,
				L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
				tbb0[0].str().c_str(),
				0,
				nullptr
				);
		}
		else
		{
			//SSE2
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				URLDownloadToFileW(
					nullptr,
					L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse2.dll",
					tbb0[0].str().c_str(),
					0,
					nullptr
					);
			}
			else
			{
				//SSE
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					URLDownloadToFileW(
						nullptr,
						L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
				//Default
				else
				{
					URLDownloadToFileW(
						nullptr,
						L"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Default.dll",
						tbb0[0].str().c_str(),
						0,
						nullptr
						);
				}
			}
		}
	}

#endif

	// Copy all files
	CopyFileW(
		cgbin[0].str().c_str(),
		cgpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgglbin[0].str().c_str(),
		cgglpath[0].str().c_str(),
		false
		);
	CopyFileW(
		cgd3d9bin[0].str().c_str(),
		cgd3d9path[0].str().c_str(),
		false
		);
	CopyFileW(
		airfile[0].str().c_str(),
		airdir[0].str().c_str(),
		false
		);
	CopyFileW(
		flashfile[0].str().c_str(),
		flashdir[0].str().c_str(),
		false
		);
	// End copy all files

	// Unblock _all_ files just to be safe
	for each (wstring i in unblockfiles)
	{
		DeleteFileW(&i[0]);
	}
	// End unblock

	wcout << "LoLUpdater finished!";
	wcout << endl;
	system("pause");

	return 0;
}