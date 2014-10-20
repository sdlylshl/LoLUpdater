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
        private static readonly string[] Constants =
        {
            "NPSWF32.dll", "IsProcessorFeaturePresent",
            "kernel32.dll", "tbb.dll", "game.cfg",
            "DefaultParticleMultiThreading=1",
            "Adobe AIR.dll", "Resources",
            "RADS", "Versions", "deploy",
            "projects", "solutions", "lol_air_client", "lol_game_client_sln", "releases", "1.0", "Adobe AIR", "Game"
        };

        private const string SKernel = "kernel32.dll";

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
                    Console.WriteLine(Dll(4, "SetXStateFeaturesMask"));
                    Console.Clear();
                    Console.WriteLine(String.Join(Environment.NewLine,
                        "By installing you agree to that the lolupdater-team is not responsible for any damages or lost data if any of such would occur",
                        string.Empty, "For a list of Command-Line Arguments, start lolupdater with --help", string.Empty,
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
            Copy(string.Empty, Game, Constants[4], string.Empty, installing);
            Copy(string.Empty, Adobe, Constants[7], string.Empty, installing);
            Copy(string.Empty, Adobe, Path.Combine(Constants[8], Constants[1]), string.Empty, installing);
            string[] garenaCfgFiles =
            {
               Constants[5], "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
                "GamePermanent_en_SG.cfg"
            };
            // Just some extra File.Exists check, just in case, can obviously be expanded to verify the legitimacy of a LoL-installation.
            if (Riot & File.Exists("lol.launcher.exe"))
            {
                Copy(string.Empty, Config, Constants[5], string.Empty, installing);
            }
            else
            {
                Parallel.ForEach(garenaCfgFiles, file => { Copy(string.Empty, Config, file, string.Empty, installing); });
            }
            switch (_userInput)
            {
                case 1:
                    var pmb = Path.Combine(X64
                        ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
                        : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                        "Pando Networks", "Media Booster", "uninst.exe");
                    if (File.Exists(pmb))
                    {
                        var pmbProcess = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        pmb,
                                    Arguments = "/silent"
                                }
                        };
                        pmbProcess.Start();
                        pmbProcess.WaitForExit();
                    }
                    const string adobeAirInstaller = "Constants[7]15_win.exe";
                    using (
                        var stream =
                            WebRequest.Create(
                                new Uri(new Uri("https://labsdownload.adobe.com/pub/labs/_constants[1]runtimes/Constants[7]/"),
                                    adobeAirInstaller))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        ByteDl(stream, adobeAirInstaller);
                    }
                    var airwin = new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                            {
                                FileName =
                                    adobeAirInstaller,
                                Arguments = "-silent"
                            }
                    };
                    airwin.Start();
                    airwin.WaitForExit();
                    Console.WriteLine(
                        "Do you need/use the Adobe Constants[7] redistributable for anything special? If not press Y to uninstall it (Recommended!), otherwise press N");
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
                                                "Constants[7]15_win.exe",
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
                    Copy(AdobePath, string.Empty, Constants[7], Adobe, null);
                    Copy(AdobePath, string.Empty, Path.Combine(Constants[8], Constants[1]), Adobe, null);

                    var os = Environment.OSVersion;
                    using (
                        var stream =
                            WebRequest.Create(new Uri(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/_constants[4]/"),
                                (os.Platform == PlatformID.Win32NT) && os.Version.Major > 5
                                    ? "Xp.dll"
                                    : CpuInfo.AsParallel().Any(
                                        item =>
                                            item["Name"].ToString()
                                                .Any(
                                                    x =>
                                                        new[] {"Haswell", "Broadwell", "Skylake", "Cannonlake"}.Contains
                                                            (x.ToString(CultureInfo.InvariantCulture))))
                                        ? "Avx2.dll"
                                        : (Dll(17, Constants[2]) & Dll(2, "GetEnabledXStateFeatures")
                                            ? "Avx.dll"
                                            : (Dll(10, Constants[2]) ? "Sse2.dll" : Dll(6, Constants[2]) ? "Sse.dll" : "Default.dll"))))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        if (File.Exists(Path.Combine(Game, Constants[4])))
                        {
                            FileFix(Path.Combine(Game, Constants[4]));
                            try
                            {
                                ByteDl(stream, Constants[4]);
                            }
                            catch (WebException ex)
                            {
                                Console.WriteLine(
                                    "{0} If you lost internet connection please rerun lolupdater later with an internet connection, else report bug to the lolupdater-team",
                                    ex.Message);
                            }
                            if (!Hash(Path.Combine(Game, Constants[4]), Sha512Sum(Constants[4])))
                            {
                                ByteDl(stream, Path.Combine(Game, Constants[4]));
                            }
                            File.Delete(Constants[4]);
                        }
                        else
                        {
                            ByteDl(stream, Path.Combine(Game, Constants[4]));
                        }
                    }
                    if (Riot)
                    {
                        Cfg(Constants[5]);
                        Process.Start("lol.launcher.exe");
                    }
                    else
                    {
                        Parallel.ForEach(garenaCfgFiles, Cfg);
                    }
                    _notdone = false;
                    Console.WriteLine("Finished Patching!");
                    Console.ReadLine();
                    Environment.Exit(0);
                    break;
                case 2:
                    _notdone = false;
                    Console.WriteLine("Finished Uninstall!");
                    Console.ReadLine();
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
                    "Do you need/use the Adobe Constants[7] redistributable for anything special? If not press Y to uninstall it (It will still patch League of Legends), otherwise press N");
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
                                FileName = "Constants[7]15_win.exe",
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
            var pDll = LoadLibrary(Constants[3]);
            var pFunc = GetProcAddress(pDll, func);
            if (pDll == IntPtr.Zero) return false;
            if (pFunc != IntPtr.Zero)
            {
                var bar = (DllType) Marshal.GetDelegateForFunctionPointer(pFunc, typeof (DllType));
                ok = bar(arg);
            }
            FreeLibrary(pDll);
            return ok;
        }

        private static void Copy(string from, string path, string file, string to, bool? mode)
        {
            if (mode.HasValue)
            {
                var dir = Path.Combine(path, file);
                var bak = Path.Combine(
                    path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(dir), ".bak"));
                if (mode.Value)
                {
                    if (!File.Exists(dir)) return;
                    if ((file.Equals(CgFiles[0]) &&
                         !Hash(dir,
                             "ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22")) ||
                        (file.Equals(CgFiles[1]) &&
                         !Hash(dir,
                             "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2")) ||
                        (file.Equals(CgFiles[2]) &&
                         !Hash(dir,
                             "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b")) ||
                        (file.Equals(Constants[4]) && Hash(dir, TbbSum)) ||
                        (file.Equals(Constants[7]) &&
                         Hash(dir,
                             "cb508d33a84530a4b588f960b67404a1c10f7617f3a6db5a6b84a4f2742438dcd72a27191246e763b67afdaf075cc155a2a5c9a9d4d65578afb7857b0d16b2ef")) ||
                        (file.Contains(Constants[1]) &&
                         Hash(dir,
                             "12a3f24ecc788d7751c25fe99adcd7fdb11472e28b998e905dbd5f8687ea663a9eec0b83a543557f3e61ef6ed7104e11e17d981c7dce886644d782db18ffaa92")) ||
                        path.Equals(Config))
                    {
                        QuickCopy(dir, bak);
                    }
                }
                else
                {
                    var exe = Path.Combine(
                        path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(dir), ".exe"));
                    if (!File.Exists(bak)) return;
                    if (path.Equals(Game) || file.Equals(Constants[7]) || file.Contains(Constants[1]) || path.Contains(Config))
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
                if ((file.Equals(CgFiles[0]) && !Hash(dir, Path.Combine(_cgBinPath, "cg.dll"))) ||
                    (file.Equals(CgFiles[1]) && !Hash(dir, Path.Combine(_cgBinPath, "cgGL.dll"))) ||
                    (file.Equals(CgFiles[2]) && !Hash(dir, Path.Combine(_cgBinPath, "cgD3D9.dll"))) ||
                    (file.Equals(Constants[7]) && !Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Constants[7])))) ||
                    (file.Contains(Constants[1]) & File.Exists(dir) &&
                     !Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Constants[8], Constants[1])))))
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
            DeleteFile(string.Format("{0}:ZConstants[16].Identifier", file));
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
                    string.Format("{0}{1}", Environment.NewLine, Constants[6]));
            }
            else
            {
                if (!text.Contains(Constants[6])) return;
                text = text.Replace(Constants[6], "DefaultParticleMultiThreading=0");
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
            if (!Directory.Exists(Path.Combine(Constants[8], path, path1, Constants[15]))) return string.Empty;
            var dir = Directory.GetDirectories(Path.Combine(Constants[8], path, path1, Constants[15])).ToString();
            return dir.Length == 1
                ? dir
                : Path.GetFileName(Directory.GetDirectories(Path.Combine(Constants[8], path, path1, Constants[15])).Max());
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
                : Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles), Constants[18],
                Constants[10], Constants[16]);

        private static readonly IEnumerable<ManagementBaseObject> CpuInfo =
            new ManagementObjectSearcher("Select Name, NumberOfCores from Win32_Processor").Get().AsParallel()
                .Cast<ManagementBaseObject>();

        private static readonly bool Riot = Directory.Exists(Constants[9]);
        private static readonly string[] CgFiles = {"Cg.dll", "CgGL.dll", "CgD3D9.dll"};

        private static readonly string Adobe = Riot
            ? Path.Combine(Constants[8], Constants[11], Constants[14], Constants[15], Ver(Constants[11], Constants[14]), Constants[10], Constants[18], Constants[9],
                Constants[16])
            : Path.Combine("Constants[7]", Constants[18], Constants[9], Constants[16]);

        private static readonly string Game = Riot
            ? Path.Combine(Constants[8], Constants[12], Constants[15], Constants[15], Ver(Constants[12], Constants[15]), Constants[10])
            : Constants[19];

        private static readonly string Config = Riot
            ? "Config"
            : Path.Combine(Constants[19], "DATA", "CFG", "defaults");

        private static int _userInput;

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static bool _notdone;
    }
}