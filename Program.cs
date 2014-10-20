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

namespace lol.updater
{
    internal static class Program
    {
        private const string SKernel = "kernel32.dll";

        private static void Main(string[] args)
        {
            Console.BackgroundColor = ConsoleColor.DarkBlue;
            Console.ForegroundColor = ConsoleColor.White;
            Console.Clear();
            var mutex = new Mutex(true,
                @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: {9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b}");
            if (!mutex.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(mutex);
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
                Console.WriteLine(string.Join(Environment.NewLine,
                    "By installing you agree to that the lolupdater-team is not responsible for any damages or lost data if any of such would occur",
                    string.Empty, "For a list of Command-Line Arguments, start lolupdater with --help", string.Empty,
                    "Select method:", string.Empty, "1. Install/Update", "2. Uninstall", "3. Exit"));
                var key = 0;
                try
                {
                    key = Convert.ToInt32(Console.ReadLine());
                }
                catch (FormatException)
                {
                    Main(args);
                }
                catch (OverflowException)
                {
                    Main(args);
                }
                finally
                {
                    if (key == 1 || key == 2 || key == 3)
                    {
                        if (key == 3)
                        {
                            Environment.Exit(0);
                        }
                        _userInput = key;
                        Console.Clear();
                        Patch();
                    }
                    else
                    {
                        Main(args);
                    }
                }
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
            Copy(string.Empty, Game, Constants[3], string.Empty, installing);
            Copy(string.Empty, Adobe, Constants[6], string.Empty, installing);
            Copy(string.Empty, Adobe, Path.Combine(Constants[7], Constants[0]), string.Empty, installing);
            string[] garenaCfgFiles =
            {
                Constants[4], "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
                "GamePermanent_en_SG.cfg"
            };
            // Just some extra File.Exists check, just in case, can obviously be expanded to verify the legitimacy of a LoL-installation.
            if (Riot & File.Exists("lol.launcher.exe"))
            {
                Copy(string.Empty, Config, Constants[4], string.Empty, installing);
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
                    if (string.IsNullOrEmpty(_cgBinPath) || (Hash(Path.Combine(_cgBinPath, CgFiles[0]),
                            "ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22")
                            ||
                            Hash(Path.Combine(_cgBinPath, CgFiles[1]),
                                "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2")
                            ||
                            Hash(Path.Combine(_cgBinPath, CgFiles[2]),
                                "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b")))
                    {
                        if (File.Exists(Constants[20]))
                        {
                            if (Hash(Constants[20],
                                "066792a95eaa99a3dde3a10877a4bcd201834223eeee2b05b274f04112e55123df50478680984c5882a27eb2137e4833ed4f3468127d81bc8451f033bba75114")
                                )
                            {
                                ReadOnlyToNormal(Constants[20]);
                                CgStart();
                            }
                            else
                            {
                                DownloadCg();
                            }
                        }
                        else
                        {
                            DownloadCg();
                        }
                    }

                    Parallel.ForEach(CgFiles, file => { Copy(_cgBinPath, string.Empty, file, Game, null); });
                    using (
                        var stream =
                            WebRequest.Create(
                                new Uri(new Uri("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/"),
                                    Constants[19]))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        ByteDl(stream, Constants[19]);
                    }
                    var airwin = new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                            {
                                FileName =
                                    Constants[19],
                                Arguments = "-silent"
                            }
                    };
                    airwin.Start();
                    airwin.WaitForExit();

                    Copy(AdobePath, string.Empty, Constants[6], Adobe, null);
                    Copy(AdobePath, string.Empty, Path.Combine(Constants[7], Constants[0]), Adobe, null);
                    Console.WriteLine(
                        "Do you need/use the Adobe AIR redistributable for anything special? If not press Y to uninstall it (Recommended!), otherwise press N");
                    var input = Console.ReadLine();
                    if (!string.IsNullOrEmpty(input) &&
                        Options.Contains(input, StringComparer.InvariantCultureIgnoreCase))
                    {
                        if (!input.Equals(Options[0], StringComparison.CurrentCultureIgnoreCase)) return;
                            var airUninst = new Process
                            {
                                StartInfo = new ProcessStartInfo
                                {
                                    FileName = Constants[19],
                                    Arguments = "-uninstall"
                                }
                            };
                            airUninst.Start();
                            airUninst.WaitForExit();
                    }
                    else
                    {
                        AirPrompt();
                    }

                    var os = Environment.OSVersion;
                    using (
                        var stream =
                            WebRequest.Create(new Uri(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"),
                                os.Platform == PlatformID.Win32NT && os.Version.Major > 5
                                    ? "Xp.dll"
                                    : CpuInfo.AsParallel().Any(
                                        item =>
                                            item["Name"].ToString()
                                                .Any(
                                                    x =>
                                                        new[] {"Haswell", "Broadwell", "Skylake", "Cannonlake"}.Contains
                                                            (x.ToString(CultureInfo.InvariantCulture))))
                                        ? "Avx2.dll"
                                        : (Dll(17, Constants[1]) & Dll(2, "GetEnabledXStateFeatures")
                                            ? "Avx.dll"
                                            : (Dll(10, Constants[1])
                                                ? "Sse2.dll"
                                                : Dll(6, Constants[1]) ? "Sse.dll" : "Default.dll"))))
                                .GetResponse()
                                .GetResponseStream())
                    {
                        if (File.Exists(Path.Combine(Game, Constants[3])))
                        {
                            ReadOnlyToNormal(Path.Combine(Game, Constants[3]));
                            try
                            {
                                ByteDl(stream, Constants[3]);
                            }
                            catch (WebException ex)
                            {
                                Console.WriteLine(
                                    "{0} If you lost internet connection please rerun lolupdater later with an internet connection, else report bug to the lolupdater-team",
                                    ex.Message);
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(
                                    "{0} Please report that you got here to the lolupdater team on the website",
                                    ex.Message);
                            }
                            if (File.Exists(Constants[3]))
                            {
                                if (!Hash(Path.Combine(Game, Constants[3]), Sha512Sum(Constants[3])))
                                {
                                    ByteDl(stream, Path.Combine(Game, Constants[3]));
                                }
                                File.Delete(Constants[3]);
                            }
                            else
                            {
                                Console.WriteLine("Error patching tbb, please contact the lolupdater team");
                            }
                        }
                        else
                        {
                            ByteDl(stream, Path.Combine(Game, Constants[3]));
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
            }
        }

        private static void AirPrompt()
        {
            Console.WriteLine(
                "Do you need/use the Adobe AIR redistributable for anything special? If not press Y to uninstall it (Recommended!), otherwise press N");
            var input = Console.ReadLine();

            if (string.IsNullOrEmpty(input) || !Options.Contains(input, StringComparer.InvariantCultureIgnoreCase))
                return;
            if (!input.Equals(Options[0], StringComparison.CurrentCultureIgnoreCase)) return;
            var airUninst = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = Constants[19],
                    Arguments = "-uninstall"
                }
            };
            airUninst.Start();
            airUninst.WaitForExit();
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

        private static void DownloadCg()
        {
            using (var stream = WebRequest.Create(new Uri(new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/"),
                Constants[20]))
                .GetResponse()
                .GetResponseStream())
            {
                ByteDl(stream, Constants[20]);
            }
            CgStart();
        }

        private static void CgStart()
        {
            var cg = new Process
            {
                StartInfo =
                    new ProcessStartInfo
                    {
                        FileName =
                            Constants[20],
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
                UnblockFile(path);
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
                    path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(file), ".bak"));
                if (mode.Value)
                {
                    if (!File.Exists(dir) || (!file.Equals(CgFiles[0]) || !Hash(dir,
                        "9e1af39c5ba05b5c76ff186c6dffff36f547cdbc116c3eb3ac67c9100ecfffac352ff9180e832a83f68dd18791a38c15338da430f7cdf849ff46c861b179e412")) &&
                        (!file.Equals(CgFiles[1]) || !Hash(dir,
                            "090da4f0cb70313674bfd8efaad93d06c532941bde6cb31461106087ded89887ad0ab1c2d1e5670cc5fc564704ccc99214f3b7aff02fc7f1482af8307856c0f6")) &&
                        (!file.Equals(CgFiles[2]) || !Hash(dir,
                            "3b0388e24097e108a8a21361f1319da1951d1714a0303aa0ef7c9c79e40943c80c5e7b05d8997a40b140acbaef97ced3ebdb9831c0d16475c01208e6ca09c50e")) &&
                        (!file.Equals(Constants[3]) || !Hash(dir,
                            "80893899429650508b05da062f538a1fa20ec45ca84d42706f583bb86627a389305914424731be5721815039ccc1018e23a08eacfae651978621112d63570efa")) &&
                        (!file.Equals(Constants[6]) || !Hash(dir,
                            "cb508d33a84530a4b588f960b67404a1c10f7617f3a6db5a6b84a4f2742438dcd72a27191246e763b67afdaf075cc155a2a5c9a9d4d65578afb7857b0d16b2ef")) &&
                        (!file.Contains(Constants[0]) || !Hash(dir,
                            "12a3f24ecc788d7751c25fe99adcd7fdb11472e28b998e905dbd5f8687ea663a9eec0b83a543557f3e61ef6ed7104e11e17d981c7dce886644d782db18ffaa92")) &&
                        !path.Equals(Config)) return;
                    QuickCopy(dir, bak);
                }
                else
                {
                    var exe = Path.Combine(
                        path, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(file), ".exe"));
                    if (!File.Exists(bak) || !path.Equals(Game) && !file.Equals(Constants[6]) && !file.Contains(Constants[0]) &&
                        !path.Contains(Config)) return;
                    QuickCopy(bak, exe);
                }
            }
            else
            {
                var dir = Path.Combine(from, file);
                var toDir = Path.Combine(to, file);
                if (!File.Exists(dir) || (!file.Equals(CgFiles[0]) || Hash(dir, Path.Combine(_cgBinPath, CgFiles[0]))) &&
                    (!file.Equals(CgFiles[1]) || Hash(dir, Path.Combine(_cgBinPath, CgFiles[1]))) &&
                    (!file.Equals(CgFiles[2]) || Hash(dir, Path.Combine(_cgBinPath, CgFiles[2]))) &&
                    (!file.Equals(Constants[6]) || Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Constants[6])))) &&
                    (!file.Contains(Constants[0]) ||
                     Hash(toDir, Sha512Sum(Path.Combine(AdobePath, Constants[7], Constants[0]))))) return;
                QuickCopy(dir, toDir);
            }
        }

        private static void QuickCopy(string from, string to)
        {
            ReadOnlyToNormal(from);
            File.Copy(from
                , to,
                true);
            UnblockFile(to);
        }

        private static void ReadOnlyToNormal(string file)
        {
            if (!new FileInfo(file).Attributes
                .Equals(FileAttributes.ReadOnly)) return;
            File.SetAttributes(file,
                FileAttributes.Normal);
        }

        private static void UnblockFile(string file)
        {
            DeleteFile(string.Format("{0}:ZConstants[16].Identifier", file));
        }

        private static void Cfg(string file)
        {
            var dir = Path.Combine(Config, file);
            if (!File.Exists(dir)) return;
            ReadOnlyToNormal(dir);
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
            UnblockFile(dir);
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
                : Path.GetFileName(
                    Directory.GetDirectories(Path.Combine(Constants[8], path, path1, Constants[15])).Max());
        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibrary(string dllName);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool DllType(byte arg);

        private static readonly string[] Constants =
        {
            "NPSWF32.dll", "IsProcessorFeaturePresent",
            "kernel32.dll", "tbb.dll", "game.cfg",
            "DefaultParticleMultiThreading=1",
            "Adobe AIR.dll", "Resources",
            "RADS", "Versions", "deploy",
            "projects", "solutions", "lol_air_client", "lol_game_client_sln", "releases", "1.0", "Adobe AIR", "Game",
            "air15_win.exe", "Cg-3.1_April2012_Setup.exe"
        };

        private static readonly string[] Options = {"Y", "N"};
        private static readonly bool X64 = Environment.Is64BitOperatingSystem;

        private static readonly string AdobePath =
            Path.Combine(X64
                ? Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFilesX86)
                : Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles), Constants[17],
                Constants[9], Constants[16]);

        private static readonly IEnumerable<ManagementBaseObject> CpuInfo =
            new ManagementObjectSearcher("Select Name, NumberOfCores from Win32_Processor").Get().AsParallel()
                .Cast<ManagementBaseObject>();

        private static readonly bool Riot = Directory.Exists(Constants[8]);
        private static readonly string[] CgFiles = {"Cg.dll", "CgGL.dll", "CgD3D9.dll"};

        private static readonly string Adobe = Riot
            ? Path.Combine(Constants[8], Constants[11], Constants[13], Constants[15], Ver(Constants[11], Constants[13]),
                Constants[10], Constants[17], Constants[9],
                Constants[16])
            : Path.Combine("Air", Constants[17], Constants[9], Constants[16]);

        private static readonly string Game = Riot
            ? Path.Combine(Constants[8], Constants[12], Constants[14], Constants[15], Ver(Constants[12], Constants[14]),
                Constants[10])
            : Constants[18];

        private static readonly string Config = Riot
            ? "Config"
            : Path.Combine(Constants[17], "DATA", "CFG", "defaults");

        private static int _userInput;

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static bool _notdone;
    }
}