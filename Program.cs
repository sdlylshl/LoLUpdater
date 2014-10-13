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

        private const string AirMd5 = "179a1fcfcb54e3e87365e77c719a723f";
        private const string FlashMd5 = "9700dbdebffe429e1715727a9f76317b";

        private static readonly string TbbMd5 = IsAvx2
                    ? "75caa85c6289710e6011c0857802ddd8"
                    : (HasAvx
                        ? "c2575d2a1515780f665acf19c3c8e9a9"
                        : (HasSse2 ? "8628d73f3c5fd9e745c48fa53472e6b9" : HasSse ? "3767791e8a35acaba2fe0b684692a0b5" : "dd81da1072d2528b5da490cbe4d1c2e7"));

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
                        Parallel.ForEach(files, file =>
                        {
                            Copy("Game", file, "Backup", true);
                        });
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup", true);
                        Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", true);
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
            Console.WriteLine("Menu");
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            Console.WriteLine("4. Exit");
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
                        FileFix(path, path1, file, ver);
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
                        FileFix(file, String.Empty, String.Empty, String.Empty);
                        if (Md5(file, md5))
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