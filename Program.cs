// Todo: Verify downloaded files
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
namespace LoLUpdater
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
        private static readonly bool X64 = Environment.Is64BitProcess;
        private static readonly string AdobePath =
            Path.Combine(X64
                ? Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFilesX86)
                : Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles), AAir,
                Ver2, One);
        private static readonly bool Avx = Dll(17, Ipf) & Dll(2, "GetEnabledXStateFeatures");
        private static readonly ManagementBaseObject[] CpuInfo =
            new ManagementObjectSearcher("Select * from Win32_Processor").Get()
                .Cast<ManagementBaseObject>()
                .AsParallel().ToArray();
        private static readonly string Pmb = Path.Combine(X64
            ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
            : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
            "Pando Networks", "Media Booster", "uninst.exe");
        // List of processors that support AVX2, might become very long in the fure (find fix), some
        // on the list are not even out yet.
        private static readonly bool Avx2 =
            CpuInfo.Any(
                item =>
                    item["Name"].ToString()
                        .Contains(
                            new List<string>(new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }).AsParallel()
                                .ToString()));
        private static readonly bool Riot = Directory.Exists(Rads);
        private static readonly string[] GarenaCfgFiles =
        {
            CfgFile, "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
            "GamePermanent_en_SG.cfg"
        };
        private static readonly string[] CgFiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        // Note: Checksums are in SHA512
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
        private static readonly string TbbSum = Avx2
            ? "13d78f0fa6b61a13e5b7cf8e4fa4b071fc880ae1356bd518960175fce7c49cba48460d6c43a6e28556be7309327abec7ec83760cf29b043ef1178904e1e98a07"
            : (Avx
                ? "d81edd17a891a2ef464f3e69ff715595f78c229867d8d6e6cc1819b426316a0bf6df5fa09a7341995290e4efe4b884f8d144e0fe8e519c4779f5cf5679db784c"
                : (Sse2
                    ? "61fea5603739cb3ca7a0f13b3f96e7c0c6bcff418d1404272c9fcf7cb5ce6fef7e21a5ee2026fc6af4ebc596d1d912e8201b298f7d092004d8f5256e22b05b64"
                    : Sse
                        ? "fa1cc95eff4ca2638b88fcdb652a7ed19b4a086bab8ce4a7e7f29324e708b5c855574c5053fe3ea84917ca0293dc97bac8830d5be2770a86ca073791696fcbec"
                        : "0c201b344e8bf0451717d6b15326d21fc91cc5981ce36717bf62013ff5624b35054e580a381efa286cc72b6fe0177499a252876d557295bc4e29a3ec92ebfa58"));
        private static int _userInput;
        private static string _airSum;
        private static string _flashSum;
        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);
        private static bool _notdone;
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool DllType(byte arg);

        private static void Main(string[] args)
        {
            while (true)
            {
                Mutex mutex = new Mutex(true, @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: {9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b}");
                if (!mutex.WaitOne(TimeSpan.Zero, true)) return;
                GC.KeepAlive(mutex);
                if (args.Length > 0)
                {
                    switch (args[0])
                    {
                        case "/?":
                        case "-h":
                        case "--help":
                            Console.WriteLine(string.Join("Command-Line Arguments:", string.Empty, "-install : Installs LoLUpdater", "-uninst : Uninstalls LoLUpdater", "--help /? -h : Shows this menu"));
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
                    Console.WriteLine(String.Join(Environment.NewLine, "For a list of Command-Line Arguments, start lolupdater with --help", string.Empty, "Select method:", string.Empty, "1. Install/Update", "2. Uninstall", "3. Exit"));
                    string input = Console.ReadLine();
                    if (input != null && (input.Equals("1") || input.Equals("2") || input.Equals("3")))
                    {
                        int key = Convert.ToInt32(input);
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
            bool installing = _userInput == 1;
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
                                            StringComparison.CurrentCultureIgnoreCase))
                                .AsParallel(), process =>
                                {
                                    process.Kill();
                                    process.WaitForExit();
                                });
                    });
            } while (_notdone);
            Console.WriteLine("Configuring...");
            if (!Directory.Exists(Backup) & installing)
            {
                Directory.CreateDirectory(Backup);
            }
            Parallel.ForEach(CgFiles, file =>
            {
                Copy(string.Empty, Game, file, string.Empty, installing);
            });
            Copy(string.Empty, Game, Tbb, string.Empty, installing);
            Copy(string.Empty, Adobe, Air, string.Empty, installing);
            Copy(string.Empty, Adobe, Path.Combine(Res, Flash), string.Empty, installing);
            if (Riot)
            {
                Copy(string.Empty, Config, CfgFile, string.Empty, installing);
            }
            else
            {
                Parallel.ForEach(GarenaCfgFiles, file =>
                {
                    Copy(string.Empty, Config, file, string.Empty, installing);
                });
            }
            switch (_userInput)
            {
                case 1:
                    Console.WriteLine("Installing...");
                    if (File.Exists(Pmb))
                    {
                        Process.Start(new ProcessStartInfo { FileName = Pmb, Arguments = "/silent" });
                    }
                    if (File.Exists(Path.Combine(AdobePath, Air)))
                    {
                        Process airUpdater = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        Path.Combine(AdobePath, Res, "Adobe AIR Updater.exe"),
                                    Arguments = "-silent"
                                }
                        };
                        airUpdater.Start();
                        airUpdater.WaitForExit();
                    }
                    else
                    {
                        // Redownloads Adobe Air Installer regardless if it already exists in the exe directory (gets beta updates faster but requires more bandwidth)
                        const string airInstaller = "air15_win.exe";
                        using (
                            Stream stream =
                                WebRequest.Create(
                                    new Uri(new Uri("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/"),
                                        airInstaller))
                                    .GetResponse()
                                    .GetResponseStream())
                        {
                            ByteDl(stream, airInstaller);
                        }
                        FileFix(airInstaller);
                        Process airwin = new Process
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
                    }
                    using (
                        FileStream fileStream = new FileStream(Path.Combine(AdobePath, Air), FileMode.Open,
                            FileAccess.Read, FileShare.Read))
                    {
                        fileStream.Seek(0, SeekOrigin.Begin);
                        _airSum = BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                            .Replace("-", string.Empty);
                    }
                    using (
                        FileStream fileStream = new FileStream(Path.Combine(AdobePath, Res, Flash), FileMode.Open,
                            FileAccess.Read, FileShare.Read))
                    {
                        fileStream.Seek(0, SeekOrigin.Begin);
                        _flashSum = BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                            .Replace("-", string.Empty);
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
                                CgStart(cgInstaller);
                            }
                            else
                            {
                                CgStart2(cgInstaller);
                            }
                        }
                        else
                        {
                            CgStart2(cgInstaller);
                        }
                    }
                    Parallel.ForEach(CgFiles, file =>
                    {
                        Copy(_cgBinPath, string.Empty, file, Game, null);
                    });
                    Copy(AdobePath, string.Empty, Air, Adobe, null);
                    Copy(AdobePath, string.Empty, Path.Combine(Res, Flash), Adobe, null);
                    using (
                        Stream stream =
                            WebRequest.Create(new Uri(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"), Avx2
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
                            if (!Hash(Path.Combine(Game, Tbb), TbbSum))
                            {
                                ByteDl(stream, Path.Combine(Game, Tbb));
                            }
                        }
                        else
                        {
                            ByteDl(stream, Path.Combine(Game, Tbb));
                        }
                        Console.WriteLine("Intel Threading Building Blocks (tbb.dll) is the latest lolupdater build");
                        FileFix(Path.Combine(Game, Tbb));
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
                    Console.WriteLine("Done Installing/Updating!");
                    _notdone = false;
                    Console.ReadLine();
                    Environment.Exit(0);
                    break;
                case 2:
                    Directory.Delete(Backup);
                    Console.ReadLine();
                    Console.WriteLine("Done Uninstalling!");
                    _notdone = false;
                    break;
                case 3:
                    Environment.Exit(0);
                    break;
            }
        }
        private static void CgStart2(string cgInstaller)
        {
            using (Stream stream = WebRequest.Create(new Uri(new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/"),
                cgInstaller))
                .GetResponse()
                .GetResponseStream())
            {
                ByteDl(stream, cgInstaller);
            }
            FileFix(cgInstaller);
            CgStart(cgInstaller);
        }
        private static void CgStart(string cgInstaller)
        {
            Process cg = new Process
            {
                StartInfo =
                    new ProcessStartInfo
                    {
                        FileName =
                            cgInstaller,
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
            using (MemoryStream memoryStream = new MemoryStream())
            {
                byte[] buffer = new byte[4096];
                int count;
                do
                {
                    count = stream.Read(buffer, 0, buffer.Length);
                    memoryStream.Write(buffer, 0, count);
                } while (count != 0);
                File.WriteAllBytes(path, memoryStream.ToArray());
            }
        }
        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DeleteFile(string file);
        private static bool Dll(byte arg, string func)
        {
            bool ok = false;
            IntPtr pDll = LoadLibrary(SKernel);
            if (pDll == IntPtr.Zero) return false;
            IntPtr pFunc = GetProcAddress(pDll, func);
            if (pFunc != IntPtr.Zero)
            {
                DllType bar = (DllType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllType));
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
            string gameDir = Path.Combine(Game, file);
            string adobeDir = Path.Combine(Adobe, file);
            if (mode.HasValue)
            {
                string bakDir = Path.Combine(Backup, file);
                string configDir = Path.Combine(Config, file);
                if (mode.Value)
                {
                    if (path.Contains(CgFiles[0]) & File.Exists(gameDir) && !Hash(file, cgSum[0]))
                    {
                        FileFix(gameDir);
                        File.Copy(gameDir
                            , bakDir,
                            true);
                        FileFix(gameDir);
                    }
                    if (path.Contains(CgFiles[0]) & File.Exists(gameDir) && !Hash(file, cgSum[0]))
                    {
                        FileFix(gameDir);
                        File.Copy(gameDir
                            , bakDir,
                            true);
                        FileFix(gameDir);
                    }
                    if (path.Contains(CgFiles[1]) & File.Exists(gameDir) && !Hash(file, cgSum[1]))
                    {
                        FileFix(gameDir);
                        File.Copy(gameDir
                            , bakDir,
                            true);
                        FileFix(gameDir);
                    }
                    if (path.Contains(CgFiles[2]) & File.Exists(gameDir) && !Hash(file, cgSum[2]))
                    {
                        FileFix(gameDir);
                        File.Copy(gameDir
                            , bakDir,
                            true);
                        FileFix(gameDir);
                    }
                    if (path.Contains(Tbb) & File.Exists(gameDir) && !Hash(file, TbbSum))
                    {
                        FileFix(gameDir);
                        File.Copy(gameDir
                            , bakDir,
                            true);
                        FileFix(gameDir);
                    }
                    if (path.Contains(Air) & File.Exists(adobeDir) && !Hash(file, _airSum))
                    {
                        FileFix(adobeDir);
                        File.Copy(adobeDir
                            , bakDir,
                            true);
                        FileFix(adobeDir);
                    }
                    if (path.Contains(Flash) & File.Exists(adobeDir) && !Hash(file, _flashSum))
                    {
                        FileFix(adobeDir);
                        File.Copy(adobeDir
                            , Path.Combine(Backup, file),
                            true);
                        FileFix(adobeDir);
                    }
                    if (!path.Contains(Config) | !File.Exists(configDir)) return;
                    FileFix(adobeDir);
                    File.Copy(configDir,
                        Path.Combine(
                            Config, string.Format("{0}{1}", Path.GetFileNameWithoutExtension(configDir), ".bak")),
                        true);
                    FileFix(adobeDir);
                }
                else
                {
                    if (!File.Exists(bakDir)) return;
                    FileFix(bakDir);
                    if (path.Equals(gameDir))
                    {
                        File.Copy(bakDir, gameDir
                            ,
                            true);
                        FileFix(bakDir);
                    }
                    if (path.Equals(adobeDir))
                    {
                        File.Copy(bakDir, adobeDir
                            ,
                            true);
                        FileFix(bakDir);
                    }
                    if (!path.Equals(Path.Combine(Adobe, Res, Flash))) return;
                    File.Copy(bakDir, adobeDir
                        ,
                        true);
                    FileFix(bakDir);
                }
            }
            else
            {
                if (!File.Exists(Path.Combine(@from, file))) return;
                if (CgFiles.Any(file.Contains))
                {
                    if (file.Contains(CgFiles[0]))
                    {
                        if (Hash(Path.Combine(@from, file), cgSum[0]))
                        {
                            Console.WriteLine("The file {0} from Cg Toolkit is the latest verison",
                                Path.GetFileNameWithoutExtension(gameDir));
                        }
                        else
                        {
                            CgCheck(@from, file, to, gameDir);
                        }
                    }
                    if (file.Contains(CgFiles[1]))
                    {
                        if (Hash(Path.Combine(@from, file), cgSum[1]))
                        {
                            Console.WriteLine("The file {0} from Cg Toolkit is the latest verison",
                                Path.GetFileNameWithoutExtension(gameDir));
                        }
                        else
                        {
                            CgCheck(@from, file, to, gameDir);
                        }
                    }
                    if (file.Contains(CgFiles[2]))
                    {
                        if (Hash(Path.Combine(@from, file), cgSum[2]))
                        {
                            Console.WriteLine("The file {0} from Cg Toolkit is the latest verison",
                                Path.GetFileNameWithoutExtension(gameDir));
                        }
                        else
                        {
                            CgCheck(@from, file, to, gameDir);
                        }
                    }
                }
                if (!@from.Equals(AdobePath)) return;
                if (file.Contains(Air))
                {
                    if (Hash(Path.Combine(@from, Air), _airSum))
                    {
                        Console.WriteLine("{0} Is the latest BETA version",
                            Path.GetFileNameWithoutExtension(adobeDir));
                    }
                    else
                    {
                        FileFix(Path.Combine(@from, file));
                        File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
                        FileFix(Path.Combine(@from, file));
                        Console.WriteLine("{0} Is the latest BETA version",
                            Path.GetFileNameWithoutExtension(adobeDir));
                    }
                }
                if (!file.Contains(Flash)) return;
                if (Hash(Path.Combine(@from, Air), _airSum))
                {
                    Console.WriteLine("{0} Is the latest BETA version",
                        Path.GetFileNameWithoutExtension(adobeDir));
                }
                else
                {
                    FileFix(Path.Combine(@from, file));
                    File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
                    FileFix(Path.Combine(@from, file));
                    Console.WriteLine("{0} Is the latest BETA version",
                        Path.GetFileNameWithoutExtension(adobeDir));
                }
            }
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

        private static void CgCheck(string @from, string file, string to, string gameDir)
        {
            FileFix(Path.Combine(@from, file));
            File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
            Console.WriteLine("The file {0} from Cg Toolkit is the latest verison",
                Path.GetFileNameWithoutExtension(gameDir));
        }
        private static void Cfg(string file)
        {
            bool multiCore =
                CpuInfo.AsParallel().Sum(item => Convert.ToInt32(item["NumberOfCores"].ToString())) > 1;
            if (!File.Exists(Path.Combine(Config, file))) return;
            FileFix(Path.Combine(Config, file));
            string text = File.ReadAllText(Path.Combine(Config, file));
            text = Regex.Replace(text, "\nEnableParticleOptimization=[01]|$",
                string.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
            if (multiCore)
            {
                text = Regex.Replace(text, "\nDefaultParticleMultiThreading=[01]|$",
                    string.Format("{0}{1}", Environment.NewLine, Dpm1));
            }
            else
            {
                if (!text.Contains(Dpm1)) return;
                text = text.Replace(Dpm1, "DefaultParticleMultiThreading=0");
            }
            File.WriteAllText(Path.Combine(Config, file), text);
            FileFix(Path.Combine(Config, file));
        }
        private static bool Hash(string file, string sha512)
        {
            using (FileStream fileStream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                fileStream.Seek(0, SeekOrigin.Begin);
                return BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                    .Replace("-", string.Empty).ToLower().Equals(sha512);
            }
        }
        private static string Ver(string path, string path1)
        {
            if (!Directory.Exists(Path.Combine(Rads, path, path1, Rel))) return string.Empty;
            string dir = Directory.GetDirectories(Path.Combine(Rads, path, path1, Rel)).ToString();
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
    }
}
