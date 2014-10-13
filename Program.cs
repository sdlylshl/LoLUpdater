using Microsoft.CSharp;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal static class Program
    {
        private static ManagementBaseObject[] CpuInfo = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>().ToArray();

        private static readonly bool IsMultiCore = CpuInfo.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static int _userInput;
        private static bool IsInstalling;
        private static bool _notdone;
        private static readonly bool IsRads = Directory.Exists("RADS");
        private static readonly bool Isx64 = Environment.Is64BitProcess;
        private static readonly bool IsLinuxorMono = (int)Environment.OSVersion.Platform == 4 || (int)Environment.OSVersion.Platform == 128;
        private static readonly bool AvxCheck = Isx64 & (IsLinuxorMono || (Environment.OSVersion.Version.Major >= 6 & Environment.OSVersion.Version.Minor >= 1));
        private static readonly bool HasSse = NativeMethods.IsProcessorFeaturePresent(6);
        private static readonly bool HasSse2 = NativeMethods.IsProcessorFeaturePresent(10);

        private static bool IsSingle;
        private static Mutex mutex = new Mutex(true, "9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b", out IsSingle);

        // test for "XSTATE_MASK_GSSE" and "XSTATE_MASK_AVX" for perfect test.
        private static readonly bool HasAvx = AvxCheck & NativeMethods.IsProcessorFeaturePresent(17) & NativeMethods.GetProcAddress(NativeMethods.LoadLibrary("kernel32.dll"), "GetEnabledXStateFeatures") != null;

        // There is a better way to do the AVX2 check
        private static readonly bool IsAvx2 = AvxCheck & CpuInfo.Any(item => item["Name"].ToString().Contains(new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }.ToString()));

        private static readonly string cgInstaller = "Cg-3.1_April2012_Setup.exe";

        private static readonly string[] cgfiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        private static readonly string[] cfgfiles = new string[] { "game.cfg", "GamePermanent.cfg", "GamePermanent_zh_MY.cfg", "GamePermanent_en_SG.cfg" };

        // make the files string shorter
        private static readonly string[] files = { "Cg.dll", "CgGL.dll", "CgD3D9.dll", "tbb.dll" };

        private static readonly string SlnFolder = Version("solutions", "lol_game_client_sln");
        private static readonly string AirFolder = Version("projects", "lol_air_client");

        // Not sure if this tweak is designed for multi-core or not, currently this patch removes
        // this tweak from single core systems, I dont know what effect this has.
        private const string CfgTweak = "DefaultParticleMultiThreading=1";

        private static readonly string LoLProcc = string.Join(string.Empty, new string[] { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" });
        private static readonly Uri Uri = new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Binaries/");

        private static readonly Uri TbbUri =
            new Uri(Uri,
                IsMultiCore
                    ? (IsAvx2
                        ? "Avx2.dll"
                        : (HasAvx
                            ? "Avx.dll"
                            : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Tbb.dll")))
                    : (HasSse2 ? "SseSt.dll" : (HasSse ? "SseSt.dll" : "TbbSt.dll")));

        private static readonly Uri FlashUri = new Uri(Uri, "NPSWF32.dll");
        private static readonly Uri AirUri = new Uri(Uri, "Adobe AIR.dll");

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private const string AirMd5 = "179a1fcfcb54e3e87365e77c719a723f";
        private const string FlashMd5 = "9700dbdebffe429e1715727a9f76317b";

        private static readonly string TbbMd5 = IsMultiCore
                ? (IsAvx2
                    ? "db0767dc94a2d1a757c783f6c7994301"
                    : (HasAvx
                        ? "2f178dadd7202b6a13a3409543a6fa86"
                        : (HasSse2 ? "1639aa390bfd02962c5c437d201045cc" : HasSse ? "3bf888228b83c4407d2eea6a5ab532bd" : "44dde7926b6dfef4686f2ddd19c04e2d")))
                : (HasSse2 ? "82ed3be353217c61ff13a01bc85f1395" : (HasSse ? "eacd37174f1a4316345f985dc456a961" : "b389f80072bc877a6ef5ff33ade88a64"));

        private static void Main(string[] args)
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
            Development(args);

            if (!Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
            }

            _userInput = DisplayMenu();
            IsInstalling = Convert.ToBoolean(_userInput);
            Console.Clear();
            do
            {
                Parallel.ForEach(Process.GetProcessesByName(LoLProcc), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            } while (_notdone);
            if (IsRads)
            {
                BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                   , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), IsInstalling);
                BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                    , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), IsInstalling);
                Parallel.ForEach(files, file =>
                {
                    BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, IsInstalling);
                });
            }
            else
            {
                Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup", IsInstalling);
                Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", IsInstalling);
                Parallel.ForEach(files, file =>
                {
                    Copy("Game", file, "Backup", IsInstalling);
                });
            }

            switch (_userInput)
            {
                case 1:
                    Console.WriteLine("Installing");
                    if (File.Exists("LoLUpdater Updater.exe"))
                    {
                        FileFix("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty);
                    }
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), true);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), true);
                        BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, true);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbMd5, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, "projects", "lol_air_client", AirFolder);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", SlnFolder, file);
                        });
                    }
                    else
                    {
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbMd5, TbbUri, string.Empty, string.Empty, string.Empty);
                        Parallel.ForEach(cfgfiles, file =>
                        {
                            Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, "Backup", IsInstalling);
                            Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                        });
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(_cgBinPath, file, "Game", IsInstalling);
                        });
                    }
                    FinishedPrompt("Done Installing!");
                    break;

                case 2:
                    Directory.Delete("Backup", true);
                    FinishedPrompt("Done Uninstalling!");
                    break;

                default:
                    Environment.Exit(0);
                    break;
            }
            switch (args[0])
            {
                case "-dev":
                    Development(args);
                    break;

                case "--help":
                    Console.WriteLine("Command Line Switches");
                    Console.WriteLine("-install : Installs LoLUpdater with default settings");
                    Console.WriteLine("-uninst : Uninstalls LoLUpdater");
                    Console.WriteLine("--help /? -h : Shows this menu");
                    Console.ReadLine();
                    break;

                case "/?":
                    goto case "--help";

                case "-h":
                    goto case "--help";

                case "-install":
                    Console.WriteLine("Installing");
                    Start();

                    if (!Directory.Exists("Backup"))
                    {
                        Directory.CreateDirectory("Backup");
                    }
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                           , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), true);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                            , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), true);
                        Parallel.ForEach(files, file =>
                        {
                            BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, true);
                        });
                    }
                    else
                    {
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup", true);
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", true);
                        Parallel.ForEach(files, file =>
                        {
                            Copy("Game", file, "Backup", true);
                        });
                    }
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), true);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), true);
                        BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, true);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbMd5, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, "projects", "lol_air_client", AirFolder);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", SlnFolder, file);
                        });
                    }
                    else
                    {
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbMd5, TbbUri, string.Empty, string.Empty, string.Empty);
                        Parallel.ForEach(cfgfiles, file =>
                        {
                            Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, "Backup", true);
                            Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                        });
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(_cgBinPath, file,
                                "Game", true);
                        });
                    }
                    FinishedPrompt("Done Installing!");
                    break;

                case "-uninst":
                    Console.WriteLine("Uninstalling");
                    Start();
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                           , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), false);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                            , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), false);
                        Parallel.ForEach(files, file =>
                        {
                            BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, false);
                        });
                    }
                    else
                    {
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup", false);
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", false);
                        Parallel.ForEach(files, file =>
                        {
                            Copy("Game", file, "Backup", false);
                        });
                    }
                    if (File.Exists("LoLUpdater Updater.exe"))
                    {
                        FileFix("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty);
                    }
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), false);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), false);
                        BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, false);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbMd5, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, "projects", "lol_air_client", AirFolder);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", SlnFolder, file);
                        });
                    }
                    else
                    {
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbMd5, TbbUri, string.Empty, string.Empty, string.Empty);
                        Parallel.ForEach(cfgfiles, file =>
                        {
                            Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, "Backup", false);
                            Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                        });
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(_cgBinPath, file,
                                "Game", false);
                        });
                    }
                    Directory.Delete("Backup", false);
                    FinishedPrompt("Done Uninstalling!");
                    break;
            }
        }

        private static void Development(string[] args)
        {
            using (WebClient wc = new WebClient())
            {
                wc.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Program.cs"), Path.Combine("Temp", "Program.cs"));
                wc.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/NativeMethods.cs"), Path.Combine("Temp", "NativeMethods.cs"));
                using (CSharpCodeProvider Cscp = new CSharpCodeProvider())
                {
                    CompilerParameters parameters = new CompilerParameters(new string[] { "System.dll", "System.Management.dll" })
                    {
                        GenerateInMemory = false,
                        GenerateExecutable = false,
                        TempFiles = new TempFileCollection("Temp"),
                        IncludeDebugInformation = false,
                        CompilerOptions = "/optimize",
                        MainClass = "Program.Main " + args[0],
                        OutputAssembly = Path.Combine("Temp", "LoLUpdater.dll")
                    };

                    CompilerResults result = Cscp.CompileAssemblyFromFile(parameters, new string[] { Path.Combine("Temp", "Program.cs"), Path.Combine("Temp", "NativeMethods.cs") });
                    File.Delete(Path.Combine("Temp", "Program.cs"));
                    File.Delete(Path.Combine("Temp", "NativeMethods.cs"));
                    Assembly assembly = Assembly.Load(AssemblyName.GetAssemblyName(result.PathToAssembly));
                    if (!Md5(result.PathToAssembly, assembly.GetHashCode().ToString()))
                    {
                        Console.WriteLine("test1");
                        Console.ReadLine();
                        assembly.GetType("Program").GetMethod("Main").Invoke(Activator.CreateInstance(assembly.GetType("Program.Main " + args[0]), BindingFlags.NonPublic | BindingFlags.Instance, null, new object[] { args }, null), new object[] { args });
                    }
                }
            }
        }

        private static void Start()
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
            do
            {
                Parallel.ForEach(Process.GetProcessesByName(LoLProcc), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            } while (_notdone);
        }

        private static void FinishedPrompt(string message)
        {
            Console.Clear();

            string[] Md5 = new string[4];
            Md5[0] = "ae87223e882670029450b3f86e8e9300";
            Md5[1] = "68dbb8778903f5cf0a80c00ffbf494d2";
            Md5[2] = "9981b512f27b566d811b53590f6ee526";
            Md5[3] = TbbMd5;
            string MD5 = string.Join(string.Empty, Md5);
            if (IsRads)
            {
                Md5Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5);
                Md5Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5);
                Parallel.ForEach(files, file =>
                {
                    Md5Check("solutions", "lol_game_client_sln", SlnFolder,
            file, MD5);
                });
            }
            else
            {
                Md5Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5);
                Md5Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), AirMd5);
                Parallel.ForEach(files, file =>
                {
                    Md5Check(Path.Combine("Game", file), MD5);
                });
            }

            Console.WriteLine("{0}", message);
            if (File.Exists("lol_launcher.exe"))
            {
                Process.Start("lol_launcher.exe");
            }
            _notdone = false;
            Console.ReadLine();
            Environment.Exit(0);
        }

        private static void Md5Check(string path, string path1, string ver, string file, string md5)
        {
            Console.WriteLine(
                !Md5(DirPath(path, path1, ver, file), md5) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        private static void Md5Check(string file, string md5)
        {
            Console.WriteLine(
                !Md5(file, md5) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        static private int DisplayMenu()
        {
            Console.WriteLine("Old Menu");
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            Console.WriteLine("3. Exit");
            Console.WriteLine();
            return Convert.ToInt32(Console.ReadLine());
        }

        private static void Download(string file, string md5, Uri uri, string path, string path1, string ver)
        {
            using (WebClient webClient = new WebClient())
            {
                if (IsRads)
                {
                    if (!File.Exists(DirPath(path, path1, ver, file)))
                    {
                        webClient.DownloadFile(
                            uri,
                            DirPath(path, path1, ver, file));
                    }
                    else
                    {
                        if (Md5(DirPath(path, path1, ver, file), md5))
                        {
                            webClient.DownloadFile(
                                uri,
                                DirPath(path, path1, ver, file));
                        }
                    }
                    FileFix(path, path1, file, ver);
                }
                else
                {
                    if (!File.Exists(file))
                    {
                        webClient.DownloadFile(uri, file);
                    }
                    else
                    {
                        if (Md5(file, md5))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    FileFix(file, String.Empty, String.Empty, String.Empty);
                }

                // checks this 3 times each install, 1 time might be enough, 2 free extra checks
                // just cause some users might be pesky.
                if (string.IsNullOrEmpty(_cgBinPath) || new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) <= new Version("3.1.0.13"))
                {
                    webClient.DownloadFile(
                    new Uri(Uri,
                cgInstaller), cgInstaller);

                    FileFix(cgInstaller, String.Empty, String.Empty, String.Empty);

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
                    File.Delete(cgInstaller);
                    _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                        EnvironmentVariableTarget.User);
                }
            }
        }

        private static void BakCopy(string file, string path, string path1, string ver, bool mode)
        {
            if (mode)
            {
                if (File.Exists(DirPath(path, path1,
                       ver, file)))
                {
                    File.Copy(
                      DirPath(path, path1,
                       ver, file)
                      , Path.Combine("Backup", file),
                      true);
                }
            }
            else
            {
                if (File.Exists(DirPath(path, path1,
                     ver, file)))
                {
                    File.Copy(Path.Combine("Backup", file)

                        , DirPath(path, path1,
                        ver, file),
                        true);
                }
            }
            FileFix(path, path1, ver, file);
        }

        private static void BakCopy(string file, string path, string path1, string ver, string to, bool mode)
        {
            if (mode)
            {
                File.Copy(
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
            else
            {
                File.Copy(Path.Combine("Backup", file)

                    , Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file),
                    true);
            }
            FileFix(path, path1, ver, file);
        }

        private static void FileFix(string path, string path1, string ver, string file)
        {
            if (IsRads)
            {
                if (new FileInfo(DirPath(path, path1,
                     ver, file)).Attributes
                     .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(DirPath(path, path1,
                        ver, file),
                        FileAttributes.Normal);
                }
                NativeMethods.DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
            }
            else
            {
                if (new FileInfo(file).Attributes
                 .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(file,
                      FileAttributes.Normal);
                }
                NativeMethods.DeleteFile(file + ":Zone.Identifier");
            }
        }

        private static string DirPath(string path, string path1, string ver, string file)
        {
            return Path.Combine("RADS", path, path1, "releases", ver, "deploy", file);
        }

        private static void Copy(string from, string path, string path1, string ver, string file)
        {
            if (File.Exists(from))
            {
                File.Copy(from, DirPath(path, path1, ver, file), true);
                FileFix(path, path1, file, SlnFolder);
            }
        }

        private static void Copy(string from, string file, string to, bool mode)
        {
            if (mode)
            {
                File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
            }
            else
            {
                File.Copy(Path.Combine(to, file), Path.Combine(@from, file), true);
            }
            FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty);
        }

        private static void Cfg(string file, string path, bool mode)
        {
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty);
            if (File.Exists(Path.Combine(path, file)))
            {
                FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty);
                if (mode)
                {
                    if (File.ReadAllText(Path.Combine(path, file))
                        .Contains(CfgTweak)) return;
                    File.AppendAllText(Path.Combine(path, file),
                        String.Format("{0}{1}", Environment.NewLine, CfgTweak));
                }
                else
                {
                    var oldLines = File.ReadAllLines(Path.Combine(path, file));
                    if (!oldLines.Contains(CfgTweak)) return;
                    var newLines = oldLines.Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains(CfgTweak)).Select(lineInfo => lineInfo.Line);
                    File.WriteAllLines(Path.Combine(path, file), newLines);
                }
            }

            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty);
        }

        private static string Version(string path, string path1)
        {
            // will not work if custom directories are in folder
            return IsRads ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).Max()) : String.Empty;
        }

        private static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        private static bool Md5(string file, string md5)
        {
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                Parallel.ForEach(MD5.Create().ComputeHash(fs), b =>
                {
                    sb.Append(b.ToString("x2"));
                });

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t == Encoding.ASCII.GetBytes(md5)[i]).Any();
            }
        }
    }
}