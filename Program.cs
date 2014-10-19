using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
// Not meant to be modified and redistributed, if you do please include a disclaimer.
namespace lol.updater
{
    internal static class Program
    {
        private const string Flash = "NPSWF32.dll";
        private const string Ipf = "IsProcessorFeaturePresent";
        private const string SKernel = "kernel32.dll";
        private const string Backup = "Backup";
        private const string Tbb = "tbb.dll";
        private const string CfgFile = "game.cfg";
        private const string Dpm1 = "DefaultParticleMultiThreading=1";
        private const string Air = "Adobe AIR.dll";
        private const string Res = "Resources";
        private const string Rads = "RADS";
        private const string Ver2 = "Versions";
        private const string Dep = "deploy";
        private const string Proj = "projects";
        private const string Sol = "solutions";
        private const string AirC = "lol_air_client";
        private const string GameCSln = "lol_game_client_sln";
        private const string Rel = "releases";
        private const string One = "1.0";
        private const string AAir = "Adobe AIR";
        private const string Gme = "Game";

        private static void Main(string[] args)
        {
            var mutex = new Mutex(true,
                @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: {9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b}");
            if (!mutex.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(mutex);
            while (true)
            {
                if (args.Length > 0)
                {
                    switch (args[0])
                    {
                        case "/?":
                        case "-h":
                        case "--help":
                            Console.WriteLine(string.Join("Command-Line Arguments:", string.Empty,
                                "-install : Installs LoLUpdater", "-uninst : Uninstalls LoLUpdater",
                                "--help /? -h : Shows this menu"));
                            Console.ReadLine();
                            break;
                        case "-install":
                            _userInput = 1;
                            Patch();
                            break;
                        case "-uninst":
                            _userInput = 2;
                            Patch();
                            break;
                    }
                }
                else
                {
                    Console.Clear();
                    Console.WriteLine(String.Join("By installing you agree to that the lolupdater-team is not responsible for any damages or lost data if any of such would occur", string.Empty, "For a list of Command-Line Arguments, start lolupdater with --help", string.Empty,
                        "Select method:", string.Empty, "1. Install/Update", "2. Uninstall", "3. Exit"));
                    var input = Console.ReadLine();
                    if (!string.IsNullOrEmpty(input) && (input.Equals("1") || input.Equals("2") || input.Equals("3")))
                    {
                        var key = Convert.ToInt32(input);
                        _userInput = key;
                        Console.Clear();
                        Patch();
                    }
                    else
                    {
                        continue;
                    }
                }
                break;
            }
        }

        private static void Patch()
        {
            Console.WriteLine("Patching League of Legends...");
            var installing = _userInput == 1;
            do
            {
                Parallel.ForEach(new[]
                {
                    "LoLClient", "LoLLauncher", "LoLPatcher",
                    "League of Legends"
                }
                    , t =>
                    {
                        Parallel.ForEach(
                            Process.GetProcesses()
                                .Where(
                                    process =>
                                        string.Equals(process.ProcessName, t,
                                            StringComparison.CurrentCultureIgnoreCase)).AsParallel()
                            , process =>
                            {
                                process.Kill();
                                process.WaitForExit();
                            });
                    });
            } while (_notdone);
            Parallel.ForEach(CgFiles, file => { Copy(string.Empty, Game, file, string.Empty, installing); });
            Copy(string.Empty, Game, Tbb, string.Empty, installing);
            Copy(string.Empty, Adobe, Air, string.Empty, installing);
            Copy(string.Empty, Adobe, Path.Combine(Res, Flash), string.Empty, installing);
            // Just some extra File.Exists check, just in case, can obviously be expanded to verify the legitimacy of a LoL-installation.
            if (Riot & File.Exists("lol.launcher.exe"))
            {
                Copy(string.Empty, Config, CfgFile, string.Empty, installing);
            }
            else
            {
                Parallel.ForEach(GarenaCfgFiles, file => { Copy(string.Empty, Config, file, string.Empty, installing); });
            }
            switch (_userInput)
            {
                case 1:
                    if (File.Exists(Pmb))
                    {
                        var pmbProcess = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        Pmb,
                                    Arguments = "/silent"
                                }
                        };
                        pmbProcess.Start();
                        pmbProcess.WaitForExit();
                    }
                    const string airInstaller = "air15_win.exe";
                    using (
                        var stream =
                            WebRequest.Create(
                                new Uri(new Uri("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/"),
                                    airInstaller))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        ByteDl(stream, airInstaller);
                    }
                    var airwin = new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                            {
                                FileName =
                                    airInstaller,
                                Arguments = "-silent"
                            }
                    };
                    airwin.Start();
                    airwin.WaitForExit();
                    Console.WriteLine(
                        "Do you need/use the Adobe AIR redistributable for anything special? If not press Y to uninstall it (Recommended!), otherwise press N");
                    var input = Console.ReadLine();
                    if (!string.IsNullOrEmpty(input))
                    {
                        if (input.Equals("Y", StringComparison.CurrentCultureIgnoreCase) ||
                            input.Equals("N", StringComparison.CurrentCultureIgnoreCase))
                        {
                            if (input.Equals("Y", StringComparison.CurrentCultureIgnoreCase))
                            {
                                var airUninst = new Process
                                {
                                    StartInfo =
                                        new ProcessStartInfo
                                        {
                                            FileName =
                                                "air15_win.exe",
                                            Arguments = "-uninstall"
                                        }
                                };
                                airUninst.Start();
                                airUninst.WaitForExit();
                            }
                        }
                        else
                        {
                            AirPrompt();
                        }
                    }
                    else
                    {
                        AirPrompt();
                    }
                    using (
                        var stream =
                            WebRequest.Create(
                                new Uri(
                                    "http://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe"))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        const string dX = "dxwebsetup.exe";
                        ByteDl(stream, dX);
                        var directX = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        dX
                                        // Not sure if below commented code actually silent-installs dxwebsetup.exe
                                    // Arguments = string.Format("/c:{0}{1}", @"""dxwsetup.exe /windowsupdate""", "/q /r:n")
                                }
                        };
                        directX.Start();
                        directX.WaitForExit();
                    }
                    if (string.IsNullOrEmpty(_cgBinPath) ||
                        new Version(FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) <
                        new Version("3.1.0.13"))
                    {
                        const string cgInstaller = "Cg-3.1_April2012_Setup.exe";
                        if (File.Exists(cgInstaller))
                        {
                            if (Hash(cgInstaller,
                                "066792a95eaa99a3dde3a10877a4bcd201834223eeee2b05b274f04112e55123df50478680984c5882a27eb2137e4833ed4f3468127d81bc8451f033bba75114")
                                )
                            {
                                FileFix(cgInstaller);
                                CgStart(cgInstaller);
                            }
                            else
                            {
                                CgStart2(cgInstaller);
                            }
                        }
                        else
                        {
                            // Overkill-approach: Search all drives for this file.
                            CgStart2(cgInstaller);
                        }
                    }
                    Parallel.ForEach(CgFiles, file => { Copy(_cgBinPath, string.Empty, file, Game, null); });
                    Copy(AdobePath, string.Empty, Air, Adobe, null);
                    Copy(AdobePath, string.Empty, Path.Combine(Res, Flash), Adobe, null);
                    using (
                        var stream =
                            WebRequest.Create(new Uri(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"), Xp ? "Xp.dll" : Avx2
                                    ? "Avx2.dll"
                                    : (Avx
                                        ? "Avx.dll"
                                        : (Sse2 ? "Sse2.dll" : Sse ? "Sse.dll" : "Default.dll"))))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        if (File.Exists(Path.Combine(Game, Tbb)))
                        {
                            FileFix(Path.Combine(Game, Tbb));
                            ByteDl(stream, Tbb);
                            if (!Hash(Path.Combine(Game, Tbb), Sha512Sum(Tbb)))
                            {
                                ByteDl(stream, Path.Combine(Game, Tbb));
                            }
                            File.Delete(Tbb);
                        }
                        else
                        {
                            ByteDl(stream, Path.Combine(Game, Tbb));
                        }
                    }
                    if (Riot)
                    {
                        Cfg(CfgFile);
                        Process.Start("lol.launcher.exe");
                    }
                    else
                    {
                        Parallel.ForEach(GarenaCfgFiles, Cfg);
                    }
                    _notdone = false;
                    Console.WriteLine("Finished Patching!");
                    Console.ReadLine();
                    Environment.Exit(0);
                    break;
                case 2:
                    Directory.Delete(Backup, true);
                    Console.ReadLine();
                    _notdone = false;
                    Console.WriteLine("Finished Uninstall!");
                    break;
                case 3:
                    Environment.Exit(0);
                    break;
            }
        }

        private static void AirPrompt()
        {
            while (true)
            {
                Console.WriteLine(
                    "Do you need/use the Adobe AIR redistributable for anything special? If not press Y to uninstall it (It will still patch League of Legends), otherwise press N");
                var input = Console.ReadLine();
                if (!string.IsNullOrEmpty(input))
                {
                    if (input.Equals("Y", StringComparison.CurrentCultureIgnoreCase) ||
                        input.Equals("N", StringComparison.CurrentCultureIgnoreCase))
                    {
                        if (!input.Equals("Y", StringComparison.CurrentCultureIgnoreCase)) return;
                        var airUninst = new Process
                        {
                            StartInfo = new ProcessStartInfo
                            {
                                FileName = "air15_win.exe",
                                Arguments = "-uninstall"
                            }
                        };
                        airUninst.Start();
                        airUninst.WaitForExit();
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
                break;
            }
        }

        private static string Sha512Sum(string path)
        {
            using (
                var fileStream = new FileStream(path, FileMode.Open,
                    FileAccess.Read, FileShare.Read))
            {
                fileStream.Seek(0, SeekOrigin.Begin);
                return BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                    .Replace("-", string.Empty);
            }

        }

        private static void CgStart2(string path)
        {
            using (var stream = WebRequest.Create(new Uri(new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/"),
                path))
                .GetResponse()
                .GetResponseStream())
            {
                ByteDl(stream, path);
            }
            CgStart(path);
        }

        private static void CgStart(string path)
        {
            var cg = new Process
            {
                StartInfo =
                    new ProcessStartInfo
                    {
                        FileName =
                            path,
                        Arguments = "/silent /TYPE=compact"
                    }
            };
            cg.Start();
            cg.WaitForExit();
            _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                EnvironmentVariableTarget.User);
        }

        private static void ByteDl(Stream stream, string path)
        {
            using (var memoryStream = new MemoryStream())
            {
                var buffer = new byte[4096];
                int count;
                do
                {
                    count = stream.Read(buffer, 0, buffer.Length);
                    memoryStream.Write(buffer, 0, count);
                } while (count != 0);
                File.WriteAllBytes(path, memoryStream.ToArray());
                FileFix(path);
            }
        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DeleteFile(string file);

        private static bool Dll(byte arg, string func)
        {
            var ok = false;
            var pDll = LoadLibrary(SKernel);
            if (pDll == IntPtr.Zero) return false;
            var pFunc = GetProcAddress(pDll, func);
            if (pFunc != IntPtr.Zero)
            {
                var bar = (DllType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllType));
                ok = bar(arg);
            }
            FreeLibrary(pDll);
            return ok;
        }

        private static void Copy(string from, string path, string file, string to, bool? mode)
        {
            string[] cgSum =
            {
                "ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22"
                ,
                "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2"
                ,
                "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b"
            };

            if (mode.HasValue)
            {
                var dir = Path.Combine(path, file);
                                    var bak = Path.Combine(
                        path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(dir), ".bak"));
                if (mode.Value)
                {
                    if (!File.Exists(dir)) return;
                    if ((file.Equals(CgFiles[0]) && !Hash(dir, cgSum[0])) ||
                        ((file.Equals(CgFiles[1]) && !Hash(dir, cgSum[1])) ||
                        ((file.Equals(CgFiles[2]) && !Hash(dir, cgSum[2])) ||
                        // Todo: Tbb, Flash and Air regardless of version will think that it is the original and be backed up, unless it is the absolute latest version, fix this?
                        ((file.Equals(Tbb) && !Hash(dir, Sha512Sum(Path.Combine(Game, Tbb))))) ||
                        ((file.Equals(Air) && !Hash(dir, Sha512Sum(Path.Combine(Adobe, Air)))) || (file.Contains(Flash) && !Hash(dir, Sha512Sum(Path.Combine(Adobe, Res, Flash)))) || path.Equals(Config)))))
                    {
                        QuickCopy(dir, bak);
                    }
                }
                else
                {
                    var exe = Path.Combine(
                        path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(dir), ".exe"));
                    if (!File.Exists(bak)) return;
                    if (path.Equals(Game) || file.Equals(Air) || file.Contains(Flash) || path.Contains(Config))
                    {
                        QuickCopy(bak, exe);
                    }

                }
            }
            else
            {
                var dir = Path.Combine(from, file);
                var toDir = Path.Combine(to, file);
                if (!File.Exists(dir)) return;
                if ((file.Equals(CgFiles[0]) && !Hash(dir, cgSum[0])) ||
                    (file.Equals(CgFiles[1]) && !Hash(dir, cgSum[1])) ||
                    (file.Equals(CgFiles[2]) && !Hash(dir, cgSum[2])) ||
                    (file.Equals(Air) && !Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Air)))) ||
                    (file.Contains(Flash) & File.Exists(dir) && !Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Res, Flash)))))
                {
                    QuickCopy(dir, toDir);
                }
            }
        }

        private static void QuickCopy(string from, string to)
        {
            FileFix(from);
            File.Copy(from
                , to,
                true);
            FileFix(to);
        }

        private static void FileFix(string file)
        {
            if (new FileInfo(file).Attributes
                .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(file,
                    FileAttributes.Normal);
            }
            DeleteFile(string.Format("{0}:Zone.Identifier", file));
        }

        private static void Cfg(string file)
        {
            var dir = Path.Combine(Config, file);
            if (!File.Exists(dir)) return;
            FileFix(dir);
            var text = File.ReadAllText(dir);
            text = Regex.Replace(text, "\nEnableParticleOptimization=[01]|$",
                string.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
            if (CpuInfo.AsParallel().Sum(item => Convert.ToInt32(item["NumberOfCores"])) > 1)
            {
                text = Regex.Replace(text, "\nDefaultParticleMultiThreading=[01]|$",
                    string.Format("{0}{1}", Environment.NewLine, Dpm1));
            }
            else
            {
                if (!text.Contains(Dpm1)) return;
                text = text.Replace(Dpm1, "DefaultParticleMultiThreading=0");
            }
            File.WriteAllText(dir, text);
            FileFix(dir);
        }

        private static bool Hash(string file, string sha512)
        {
            using (var fileStream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                fileStream.Seek(0, SeekOrigin.Begin);
                return BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                    .Replace("-", string.Empty).Equals(sha512);
            }
        }

        private static string Ver(string path, string path1)
        {
            if (!Directory.Exists(Path.Combine(Rads, path, path1, Rel))) return string.Empty;
            var dir = Directory.GetDirectories(Path.Combine(Rads, path, path1, Rel)).ToString();
            return dir.Length == 1
                ? dir
                : Path.GetFileName(Directory.GetDirectories(Path.Combine(Rads, path, path1, Rel)).Max());
        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibrary(String dllName);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool DllType(byte arg);

        private static readonly bool X64 = Environment.Is64BitProcess;

        private static readonly string AdobePath =
            Path.Combine(X64
                ? Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFilesX86)
                : Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles), AAir,
                Ver2, One);

        private static readonly bool Avx = Dll(17, Ipf) & Dll(2, "GetEnabledXStateFeatures");

        private static readonly IEnumerable<ManagementBaseObject> CpuInfo =
            new ManagementObjectSearcher("Select Name, NumberOfCores from Win32_Processor").Get().AsParallel()
                .Cast<ManagementBaseObject>();

        private static readonly string Pmb = Path.Combine(X64
            ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
            : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
            "Pando Networks", "Media Booster", "uninst.exe");

        private static readonly string[] Avx2Cpus = { "Haswell", "Broadwell", "Skylake", "Cannonlake" };

        private static readonly bool Avx2 =
            CpuInfo.AsParallel().Any(
                item =>
                    item["Name"].ToString().Any(x => Avx2Cpus.Contains(x.ToString(CultureInfo.InvariantCulture))));

        private static readonly bool Riot = Directory.Exists(Rads);

        private static readonly string[] GarenaCfgFiles =
        {
            CfgFile, "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
            "GamePermanent_en_SG.cfg"
        };

        private static readonly string[] CgFiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        private static readonly bool Sse = Dll(6, Ipf);
        private static readonly bool Sse2 = Dll(10, Ipf);

        private static readonly string Adobe = Riot
            ? Path.Combine(Rads, Proj, AirC, Rel, Ver(Proj, AirC), Dep, AAir, Ver2,
                One)
            : Path.Combine("Air", AAir, Ver2, One);

        private static readonly string Game = Riot
            ? Path.Combine(Rads, Sol, GameCSln, Rel, Ver(Sol, GameCSln), Dep)
            : Gme;
        private static readonly string Config = Riot
            ? "Config"
            : Path.Combine(Gme, "DATA", "CFG", "defaults");
        private static readonly OperatingSystem Os = Environment.OSVersion;
        private static readonly bool Xp = (Os.Platform == PlatformID.Win32NT) && Os.Version.Major > 5;
        private static int _userInput;

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static bool _notdone;
    }
}