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
        private static readonly bool IsInstalling = Convert.ToBoolean(_userInput = 1);
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

        // Make the files string shorter
        private static readonly string[] files = { "Cg.dll", "CgGL.dll", "CgD3D9.dll", "tbb.dll" };

        private static readonly string SlnFolder = Version("solutions", "lol_game_client_sln");
        private static readonly string AirFolder = Version("projects", "lol_air_client");

        private static readonly string LoLProcc = string.Join(string.Empty, new string[] { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" });
        private static readonly Uri Uri = new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/");

        // Possibly recompile the Tbbs for better performance
        private static readonly Uri TbbUri =
            new Uri(Uri,
                IsAvx2
                        ? "Avx2.dll"
                        : (HasAvx
                            ? "Avx.dll"
                            : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Tbb.dll")));

        private static readonly Uri FlashUri = new Uri(Uri, "NPSWF32.dll");
        private static readonly Uri AirUri = new Uri(Uri, "Adobe AIR.dll");

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private const string AirSha512 = "33f376d3f3a76a2ba122687b18e0306d45a8c65c89d3a51cc956bf4fa6d9bf9677493afa9b7bb5227fa1b162117440a5976484df6413f77a88ff3759ded37e8e";
        private const string FlashSha512 = "e16c024424405ead77a89fabbb4a95a99e5552f33509d872bb7046cba4afb16f5a5bbf496a46b1b1ee9ef8b9e8ba6720bc8faccb654c5317e8142812e56b4930";

        private static readonly string TbbSha512 = IsAvx2
                    ? "13d78f0fa6b61a13e5b7cf8e4fa4b071fc880ae1356bd518960175fce7c49cba48460d6c43a6e28556be7309327abec7ec83760cf29b043ef1178904e1e98a07"
                    : (HasAvx
                        ? "d81edd17a891a2ef464f3e69ff715595f78c229867d8d6e6cc1819b426316a0bf6df5fa09a7341995290e4efe4b884f8d144e0fe8e519c4779f5cf5679db784c"
                        : (HasSse2 ? "61fea5603739cb3ca7a0f13b3f96e7c0c6bcff418d1404272c9fcf7cb5ce6fef7e21a5ee2026fc6af4ebc596d1d912e8201b298f7d092004d8f5256e22b05b64" : HasSse ? "fa1cc95eff4ca2638b88fcdb652a7ed19b4a086bab8ce4a7e7f29324e708b5c855574c5053fe3ea84917ca0293dc97bac8830d5be2770a86ca073791696fcbec" : "0c201b344e8bf0451717d6b15326d21fc91cc5981ce36717bf62013ff5624b35054e580a381efa286cc72b6fe0177499a252876d557295bc4e29a3ec92ebfa58"));

        private static void Main(string[] args)
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
            _userInput = DisplayMenu();
            Console.Clear();

            Parallel.ForEach(Process.GetProcessesByName(LoLProcc), proc =>
            {
                proc.Kill();
                proc.WaitForExit();
            });

            if (IsInstalling & !Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
            }
            if (_userInput != 3)
            {
                if (IsRads)
                {
                    BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                       , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), IsInstalling);
                    BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                        , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), IsInstalling);
                    BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, IsInstalling);
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
                if (IsInstalling)
                {
                    CgCheck();
                }
            }
            switch (_userInput)
            {
                case 1:
                    Console.WriteLine("Installing...");
                    if (File.Exists("LoLUpdater Updater.exe"))
                    {
                        FileFix("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty);
                    }
                    if (IsRads)
                    {
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbSha512, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirSha512, AirUri, "projects", "lol_air_client", AirFolder);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", SlnFolder, file);
                        });
                    }
                    else
                    {
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirSha512, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbSha512, TbbUri, string.Empty, string.Empty, string.Empty);
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
                    FinishedPrompt("Done Uninstalling!");
                    break;

                case 3:
                    Environment.Exit(0);
                    break;
            }
            switch (args[0])
            {
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
                    Console.WriteLine("Installing...");
                    Start();
                    CgCheck();
                    if (!Directory.Exists("Backup"))
                    {
                        Directory.CreateDirectory("Backup");
                    }
                    if (IsRads)
                    {
                        Parallel.ForEach(files, file =>
                        {
                            BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, true);
                        });
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), true);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), true);
                        BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, true);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbSha512, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirSha512, AirUri, "projects", "lol_air_client", AirFolder);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", SlnFolder, file);
                        });
                    }
                    else
                    {
                        Parallel.ForEach(files, file =>
                        {
                            Copy("Game", file, "Backup", true);
                        });
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup", true);
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", true);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirSha512, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbSha512, TbbUri, string.Empty, string.Empty, string.Empty);
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
                    Console.WriteLine("Uninstalling...");
                    Start();
                    if (IsRads)
                    {
                        BakCopy("Adobe AIR.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), false);
                        BakCopy("NPSWF32.dll", "projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), false);
                        BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, false);
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
                    Directory.Delete("Backup", false);
                    FinishedPrompt("Done Uninstalling!");
                    break;
            }
        }

        private static void Start()
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
            Parallel.ForEach(Process.GetProcessesByName(LoLProcc), proc =>
            {
                proc.Kill();
                proc.WaitForExit();
            });
        }

        private static void FinishedPrompt(string message)
        {
            Console.Clear();

            string[] SHA512 = new string[4];
            SHA512[0] = "ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22";
            SHA512[1] = "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2";
            SHA512[2] = "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b";

            SHA512[3] = TbbSha512;
            string Sha512 = string.Join(string.Empty, SHA512);
            if (IsRads)
            {
                Sha512Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirSha512);
                Sha512Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512);
                Parallel.ForEach(files, file =>
                {
                    Sha512Check("solutions", "lol_game_client_sln", SlnFolder,
            file, Sha512);
                });
            }
            else
            {
                Sha512Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512);
                Sha512Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), AirSha512);
                Parallel.ForEach(files, file =>
                {
                    Sha512Check(Path.Combine("Game", file), Sha512);
                });
            }

            Console.WriteLine("{0}", message);
            if (File.Exists("lol_launcher.exe"))
            {
                Process.Start("lol_launcher.exe");
            }
            Console.ReadLine();
            Environment.Exit(0);
        }

        private static void Sha512Check(string path, string path1, string ver, string file, string Sha512)
        {
            Console.WriteLine(
                !Sha512Equal(DirPath(path, path1, ver, file), Sha512) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        private static void Sha512Check(string file, string Sha512)
        {
            Console.WriteLine(
                !Sha512Equal(file, Sha512) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        static private int DisplayMenu()
        {
            Console.WriteLine("Menu");
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            Console.WriteLine("3. Exit");
            Console.WriteLine();
            return Convert.ToInt32(Console.ReadLine());
        }

        private static void Download(string file, string Sha512, Uri uri, string path, string path1, string ver)
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
                        FileFix(path, path1, file, ver);
                        if (Sha512Equal(DirPath(path, path1, ver, file), Sha512))
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
                        FileFix(file, String.Empty, String.Empty, String.Empty);
                        if (Sha512Equal(file, Sha512))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    FileFix(file, String.Empty, String.Empty, String.Empty);
                }
            }
        }

        private static void CgCheck()
        {
            using (WebClient webclient = new WebClient())
            {
                if (string.IsNullOrEmpty(_cgBinPath) || new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) <= new Version("3.1.0.13"))
                {
                    webclient.DownloadFile(
                    new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"), cgInstaller);

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
            FileFix(path, path1, ver, file);
            if (mode & File.Exists(DirPath(path, path1,
                       ver, file)))
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
            if (!mode & File.Exists(Path.Combine("Backup", file)))
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
            FileFix(path, path1, ver, file);
            if (mode & File.Exists(Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)))
            {
                File.Copy(
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
            if (!mode & File.Exists(Path.Combine("Backup", file)))
            {
                File.Copy(Path.Combine("Backup", file)

                    , Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file),
                    true);
            }
            FileFix(path, path1, ver, file);
        }

        private static void FileFix(string path, string path1, string ver, string file)
        {
            if (IsRads & File.Exists(DirPath(path, path1,
                     ver, file)))
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
            if (!IsRads & File.Exists(file))
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
                FileFix(path, path1, file, SlnFolder);
                File.Copy(from, DirPath(path, path1, ver, file), true);
                FileFix(path, path1, file, SlnFolder);
            }
        }

        private static void Copy(string from, string file, string to, bool mode)
        {
            FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty);
            if (mode & File.Exists(Path.Combine(@from, file)))
            {
                File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
            }
            if (!mode & File.Exists(Path.Combine(to, file)))
            {
                File.Copy(Path.Combine(to, file), Path.Combine(@from, file), true);
            }
            FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty);
        }

        // Todo: Make this method shorter
        private static void Cfg(string file, string path, bool mode)
        {
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty);
            if (File.Exists(Path.Combine(path, file)))
            {
                if (File.ReadAllText(Path.Combine(path, file))
.Contains("EnableParticleOptimization=0"))
                {
                    File.WriteAllLines(Path.Combine(path, file), File.ReadAllLines(Path.Combine(path, file)).Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains("EnableParticleOptimization=0")).Select(lineInfo => lineInfo.Line));

                    File.AppendAllText(Path.Combine(path, file),
                      String.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
                }
                if (!File.ReadAllText(Path.Combine(path, file))
    .Contains("EnableParticleOptimization=1"))
                {
                    File.AppendAllText(Path.Combine(path, file),
                      String.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
                }
                if (mode)
                {
                    if (File.ReadAllText(Path.Combine(path, file))
.Contains("DefaultParticleMultiThreading=0"))
                    {
                        File.WriteAllLines(Path.Combine(path, file), File.ReadAllLines(Path.Combine(path, file)).Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains("DefaultParticleMultiThreading=0")).Select(lineInfo => lineInfo.Line));

                        File.AppendAllText(Path.Combine(path, file),
                          String.Format("{0}{1}", Environment.NewLine, "DefaultParticleMultiThreading=1"));
                    }

                    if (!File.ReadAllText(Path.Combine(path, file))
                        .Contains("DefaultParticleMultiThreading=1"))
                    {
                        File.AppendAllText(Path.Combine(path, file),
                        String.Format("{0}{1}", Environment.NewLine, "DefaultParticleMultiThreading=1"));
                    }
                }
                else
                {
                    var oldLines = File.ReadAllLines(Path.Combine(path, file));
                    if (oldLines.Contains("DefaultParticleMultiThreading=1"))
                    {
                        File.WriteAllLines(Path.Combine(path, file), oldLines.Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains("DefaultParticleMultiThreading=1")).Select(lineInfo => lineInfo.Line));
                    }
                }
            }

            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty);
        }

        private static string Version(string path, string path1)
        {
            // will not work if custom directories with !format x.x.x.x where x = (int)x
            return IsRads ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).Max()) : String.Empty;
        }

        private static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        private static bool Sha512Equal(string file, string Sha512)
        {
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                Parallel.ForEach(SHA512.Create().ComputeHash(fs), b =>
                {
                    sb.Append(b.ToString("x2"));
                });

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t == Encoding.ASCII.GetBytes(Sha512)[i]).Any();
            }
        }
    }
}