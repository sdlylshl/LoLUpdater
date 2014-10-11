﻿using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal static class Program
    {

        private static ManagementBaseObject[] CpuInfo = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>().ToArray();
        private static readonly bool IsMultiCore = CpuInfo.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static int _userInput;

        private static readonly bool IsInstalling = _userInput == 1;
        private static bool _notdone;
        private static readonly bool IsRads = Directory.Exists("RADS");
        private static readonly bool Isx64 = Environment.Is64BitProcess;
        private static readonly bool IsLinuxorMono = (int)Environment.OSVersion.Platform == 4 || (int)Environment.OSVersion.Platform == 128;
        private static readonly bool IsSupportedPlatform = (Environment.OSVersion.Platform == PlatformID.Win32NT & Environment.OSVersion.Version.Major >= 5 & Environment.OSVersion.Version.Minor >= 1) & IsLinuxorMono;
        private static readonly bool AvxCheck = Isx64 & (IsLinuxorMono || Environment.OSVersion.Version.Major >= 6 & Environment.OSVersion.Version.Minor >= 1);
        private static readonly bool HasSse = IsProcessorFeaturePresent(6);
        private static readonly bool HasSse2 = IsProcessorFeaturePresent(10);

        // test for "XSTATE_MASK_GSSE" and "XSTATE_MASK_AVX" for perfect test.
        private static readonly bool HasAvx = AvxCheck & IsProcessorFeaturePresent(17) & GetProcAddress(LoadLibrary("kernel32.dll"), "GetEnabledXStateFeatures") != null;

        // There is a better way to do the AVX2 check
        private static readonly bool IsAvx2 = AvxCheck & CpuInfo.Any(item => item["Name"].ToString().Contains(new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }.ToString()));

        private static readonly string cgIntaller = "Cg-3.1_April2012_Setup.exe";
        private static readonly string[] cgfiles = { "cg.dll", "cgGL.dll", "cgD3D9.dll" };
        private static readonly string[] files = { cgfiles.ToString(), "tbb.dll" };
        private static readonly string[] cfgfiles = { "game.cfg", "GamePermanent.cfg", "GamePermanent_zh_MY.cfg", "GamePermanent_en_SG.cfg" };
        private static readonly string SlnFolder = Version("solutions", "lol_game_client_sln");
        private static readonly string AirFolder = Version("projects", "lol_air_client");
        private static readonly string[] LoLProccess = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends", "PMB" };

        // Not sure if this tweak is designed for multi-core or not, currently this patch removes
        // this tweak from single core systems, I dont know what effect this has.
        private const string CfgTweak = "DefaultParticleMultiThreading=1";

        private static readonly Uri Uri = new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/");

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


        private static readonly Version LatestCg = new Version("3.1.13");

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

        // cg, cggl, cgd3d9 (in this order)
        private const string[] Md5Values = { "ae87223e882670029450b3f86e8e9300", "68dbb8778903f5cf0a80c00ffbf494d2", "9981b512f27b566d811b53590f6ee526", TbbMd5 };

        private static void Main()
        {
            if (!IsSupportedPlatform)
            {
                Console.WriteLine("Unsupported Platform");
                Console.ReadLine();
                Environment.Exit(0);
            }
            _userInput = DisplayMenu();
            if (!Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
            }
            if (IsRads)
            {
                BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                   , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), IsInstalling);
                BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                    , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), IsInstalling);
                if (IsMultiCore)
                {
                    Parallel.ForEach(files, file =>
                                {
                                    BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                                });
                }
                else
                    foreach (string file in files)
                    {
                        BakCopy(file, "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                    }
            }
            else
            {
                Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                if (IsMultiCore)
                {
                    Parallel.ForEach(files, file =>
                                {
                                    Copy(Path.Combine("Game", file), file, string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                                });
                }
                else
                    foreach (string file in files)
                    {
                        Copy(Path.Combine("Game", file), file, string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                    }
            }
            Console.Clear();
            do
            {
                if (IsMultiCore)
                {
                    Parallel.ForEach(Process.GetProcessesByName(LoLProccess.ToString()), proc =>
                    {
                        proc.Kill();
                        proc.WaitForExit();
                    });
                }
                else
                    foreach (Process proc in Process.GetProcessesByName(LoLProccess.ToString()))
                    {
                        proc.Kill();
                        proc.WaitForExit();
                    }
            } while (_notdone);
            switch (_userInput)
            {
                case 1:
                    Console.WriteLine("Installing, please wait...");
                    if (File.Exists("LoLUpdater Updater.exe"))
                    {
                        FileFix("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty, false);
                    }
                    if (IsRads)
                    {
                        BakCopy("projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty, IsInstalling);
                        BakCopy("projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), string.Empty, IsInstalling);
                        BakCopy(string.Empty, string.Empty, Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, IsInstalling);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbMd5, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, "projects", "lol_air_client", AirFolder);
                        Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, "projects", "lol_air_client", AirFolder);
                        if (IsMultiCore)
                        {
                            Parallel.ForEach(cgfiles, file =>
                                {
                                    Copy(string.Empty,
                                        file, "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                                });
                        }
                        else
                            foreach (string file in files)
                            {
                                Copy(string.Empty,
                                    file, "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                            }
                    }
                    else
                    {
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, string.Empty, string.Empty, string.Empty);
                        Download(Path.Combine("Game", "tbb.dll"), TbbMd5, TbbUri, string.Empty, string.Empty, string.Empty);
                        if (IsMultiCore)
                        {
                            Parallel.ForEach(cfgfiles, file =>
                                {
                                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                                    Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                                });
                        }
                        else
                        {
                            foreach (string file in cfgfiles)
                            {
                                Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, string.Empty, string.Empty, string.Empty, "Backup", IsInstalling);
                                Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                            }
                        }



                        if (IsMultiCore)
                        {
                            Parallel.ForEach(cgfiles, file =>
                                {
                                    Copy(Path.Combine(_cgBinPath, file), string.Empty, string.Empty, string.Empty, string.Empty,
                                        Path.Combine("Game", file), IsInstalling);
                                });
                        }
                        else
                            foreach (string file in files)
                            {
                                Copy(Path.Combine(_cgBinPath, file), string.Empty, string.Empty, string.Empty, string.Empty,
                                    Path.Combine("Game", file), IsInstalling);
                            }
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
        }

        private static void FinishedPrompt(string message)
        {
            Console.Clear();
            if (IsRads)
            {
                Md5Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5);
                Md5Check("projects", "lol_air_client", AirFolder,
                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5);


                if (IsMultiCore)
                {
                    Parallel.ForEach(cgfiles, file =>
                    {
                        Md5Check("solutions", "lol_game_client_sln", SlnFolder,
                file, Md5Values.ToString());
                    });
                }
                else
                {
                    foreach (string file in files)
                    {
                        Md5Check("solutions", "lol_game_client_sln", SlnFolder,
                file, Md5Values.ToString());
                    }
                }
            }
            else
            {
                Md5Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5);
                Md5Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), AirMd5);
                if (IsMultiCore)
                {
                    Parallel.ForEach(cgfiles, file =>
                    {
                        Md5Check(Path.Combine("Game", file), Md5Values.ToString());
                    });
                }
                else
                {
                    foreach (string file in files)
                    {
                        Md5Check(Path.Combine("Game", file), Md5Values.ToString());
                    }
                }

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
                !Md5Compare(DirPath(path, path1, ver, file), md5) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        private static void Md5Check(string file, string md5)
        {
            Console.WriteLine(
                !Md5Compare(file, md5) ? "{0} Is an old patched version or the original" : "{0} Is the latest patched version",
                file);
        }

        static private int DisplayMenu()
        {
            Console.WriteLine("LoLUpdater Menu");
            Console.WriteLine("Press 1, 2 or 3");
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            Console.WriteLine("3. Exit");
            return Convert.ToInt32(Console.ReadLine());
        }

        private static void Download(string file, string md5, Uri uri, string path, string path1, string version)
        {
            using (WebClient webClient = new WebClient())
            {
                if (IsRads)
                {
                    FileFix(path, path1, file, version, true);
                    if (!File.Exists(DirPath(path, path1, version, file)))
                    {
                        webClient.DownloadFile(
                            uri,
                            DirPath(path, path1, version, file));
                    }
                    else
                    {
                        if (Md5Compare(DirPath(path, path1, version, file), md5))
                        {
                            webClient.DownloadFile(
                                uri,
                                DirPath(path, path1, version, file));
                        }
                    }
                    FileFix(path, path1, file, version, false);
                }
                else
                {
                    FileFix(file, String.Empty, String.Empty, String.Empty, true);
                    if (!File.Exists(file))
                    {
                        webClient.DownloadFile(uri, file);
                    }
                    else
                    {
                        if (Md5Compare(file, md5))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    FileFix(file, String.Empty, String.Empty, String.Empty, false);
                }

                // This check is not perfect, fix later.
                if (!String.IsNullOrEmpty(_cgBinPath) && new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= LatestCg) return;
                webClient.DownloadFile(
                    new Uri(Uri,
                cgIntaller), cgIntaller);

                FileFix(cgIntaller, String.Empty, String.Empty, String.Empty, true);
                FileFix(cgIntaller, String.Empty, String.Empty, String.Empty, false);

                Process cg = new Process
                {
                    StartInfo =
                        new ProcessStartInfo
                        {
                            FileName =
                                cgIntaller,
                            Arguments = "/silent /TYPE=compact"
                        }
                };
                cg.Start();
                cg.WaitForExit();
                File.Delete(cgIntaller);
                _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                    EnvironmentVariableTarget.User);
            }
        }

        private static void BakCopy(string file, string path, string path1, string ver, string to, bool mode)
        {
            FileFix(file, path, path1, ver, true);
            if (mode)
            {
                if (!File.Exists(Path.Combine("RADS", path, path1, "releases", ver, "deploy", file))) return;
                File.Copy(
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
            else
            {
                if (!File.Exists(Path.Combine("Backup", file))) return;
                File.Copy(Path.Combine("Backup", file)

                    , Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file),
                    true);
            }
            FileFix(file, path, path1, ver, false);
        }

        private static void FileFix(string file, string path, string path1, string ver, bool mode)
        {
            if (mode)
            {
                if (File.Exists(DirPath(path, path1,
                   ver, file)) &&
                   new FileInfo(DirPath(path, path1,
                       file, ver)).Attributes
                       .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(DirPath(path, path1,
                        ver, file),
                        FileAttributes.Normal);
                }
                if (!File.Exists(file) || !new FileInfo(file).Attributes
                    .Equals(FileAttributes.ReadOnly)) return;
                File.SetAttributes(file,
                    FileAttributes.Normal);
            }
            else
            {
                if (File.Exists(DirPath(path, path1, ver, file)))
                {
                    DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
                }
                if (!File.Exists(file)) return;
                DeleteFile(file + ":Zone.Identifier");
            }
        }

        private static string DirPath(string path, string path1, string ver, string file)
        {
            return Path.Combine("RADS", path, path1, "releases", ver, "deploy", file);
        }

        private static void Copy(string from, string file, string path, string path1, string ver, string to, bool IsInstalling)
        {
            if (IsRads)
            {
                FileFix(path, path1, file, SlnFolder, true);
                if (!File.Exists(Path.Combine(
                    _cgBinPath, file))) return;
                File.Copy(Path.Combine(_cgBinPath, file), DirPath(path, path1, ver, file), true);
                FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty, false);
            }
            else
            {
                FileFix(Path.Combine(@from, file), String.Empty, String.Empty, String.Empty, true);
                if (IsInstalling)
                {
                    if (!File.Exists(Path.Combine(@from, file)) && Directory.Exists(to)) return;
                    File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
                }
                else
                {
                    File.Copy(Path.Combine(to, file), Path.Combine(@from, file), true);
                }

                FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty, false);
            }
        }

        private static void Cfg(string file, string path, bool mode)
        {
            if (!File.Exists(Path.Combine(path, file))) return;
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty, true);
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
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty, false);
        }

        private static string Version(string path, string path1)
        {
            // will not work if custom directories are in folder
            return IsRads ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).Max()) : String.Empty;
        }

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Auto)]
        static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)]string lpFileName);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern void DeleteFile(string lpFileName);

        private static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        private static bool Md5Compare(string file, string md5)
        {
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                foreach (byte b in MD5.Create().ComputeHash(fs))
                {
                    sb.Append(b.ToString("x2"));
                }

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t == Encoding.ASCII.GetBytes(md5)[i]).Any();
            }
        }
    }
}