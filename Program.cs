using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal static class Program
    {
        private static readonly string Sln = Version("solutions", "lol_game_client_sln");
        private static readonly string Air = Version("projects", "lol_air_client");
        private static readonly string[] LoLProccessStrings = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" };

        // private static readonly bool TbbUpdateNeeded = (Directory.Exists("RADS") ? new Version(
        // FileVersionInfo.GetVersionInfo(CopyPath("solutions", "lol_game_client_sln", "tbb.dll",
        // Sln) ).FileVersion) : new Version(FileVersionInfo.GetVersionInfo(Path.Combine("Game",
        // "tbb.dll")).FileVersion)) == new Version("4.3.0.0");

        private static readonly bool FlashUpdateNeeded = (Directory.Exists("RADS") ? new Version(
        FileVersionInfo.GetVersionInfo(CopyPath("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air)).FileVersion) : new
        Version(FileVersionInfo.GetVersionInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")).FileVersion)) != new Version("15.0.0.0297");

        private static readonly bool AirUpdateNeeded = (Directory.Exists("RADS") ? new Version(
        FileVersionInfo.GetVersionInfo(CopyPath("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)).FileVersion) : new Version(
        FileVersionInfo.GetVersionInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")).FileVersion)) != new Version("15.0.0.192");

        private static readonly UriBuilder UriBuilder = new UriBuilder("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/");

        private static readonly Version CgLatestVersion = new Version(3, 1, 0013);

        private static readonly string PmbUninstall = Path.Combine(Environment.Is64BitProcess
            ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
            : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
            "Pando Networks", "Media Booster", "uninst.exe");

        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static readonly bool IsHaswell = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Haswell"));

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static void Main()
        {
            if (File.Exists(PmbUninstall))
            {
                Kill("PMB"); Process.Start(new ProcessStartInfo { FileName = PmbUninstall, Arguments = "/silent" });
            }
            UriBuilder.Path += HighestInstructionSupported();
            Console.WriteLine("Patching...");
            Console.WriteLine("");
            Kill(LoLProccessStrings);
            using (WebClient webClient = new WebClient())
            {
                if (!Directory.Exists("Backup"))
                {
                    if (Directory.Exists("RADS"))
                    {
                        Directory.CreateDirectory("Backup");
                        CopyX("solutions", "lol_game_client_sln", "cg.dll", string.Empty, Sln);
                        CopyX("solutions", "lol_game_client_sln", "cgD3D9.dll", string.Empty, Sln);
                        CopyX("solutions", "lol_game_client_sln", "cgGL.dll", string.Empty, Sln);
                        CopyX("solutions", "lol_game_client_sln", "tbb.dll", string.Empty, Sln);
                        CopyX("projects", "lol_air_client", "Adobe AIR.dll",
                            Path.Combine("Adobe Air", "Versions", "1.0"), Air);
                        CopyX("projects", "lol_air_client", "NPSWF32.dll",
                            Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), Air);
                        if (!File.Exists(Path.Combine("Config", "game.cfg"))) return;
                        Copy("game.cfg", "Config", "Backup");
                    }
                    else if (!Directory.Exists("Game")) return;
                    Directory.CreateDirectory("Backup");
                    Copy("Cg.dll", "Game", "Backup");
                    Copy("CgGL.dll", "Game", "Backup");
                    Copy("CgD3D9.dll", "Game", "Backup");
                    Copy("tbb.dll", "Game", "Backup");
                    Copy("Adobe AIR.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Backup");
                    Copy("NPSWF32.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"),
                        "Backup");
                    GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "game.cfg");
                    GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent.cfg");
                    GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_zh_MY.cfg");
                    GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_en_SG.cfg");
                }

                if (Directory.Exists("RADS"))
                {
                    GReadOnly("Config", "game.cfg");
                    CgCheck(new List<object>
                {
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "cgD3D9.dll")).ProductVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "cgGL.dll")).ProductVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "cg.dll")).ProductVersion)
                }, webClient);
                    ReadOnly("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    ReadOnly("solutions", "lol_game_client_sln", "cg.dll", Sln);
                    ReadOnly("solutions", "lol_game_client_sln", "cgGl.dll", Sln);
                    ReadOnly("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    ReadOnly("projects", "lol_air_client", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    ReadOnly("projects", "lol_air_client", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air);

                    if (IsMultiCore)
                    {
                        if (File.Exists(Path.Combine("Config", "game.cfg")))
                        {
                            Cfg("game.cfg", "Config", true);
                        }
                    }
                    else
                    {
                        if (File.Exists(Path.Combine("Config", "game.cfg")))
                        {
                            Cfg("game.cfg", "Config", false);
                        }
                    }
                    if (!File.Exists(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                        "releases", Sln, "deploy", "tbb.dll")))
                    {
                        webClient.DownloadFile(
                            new Uri(UriBuilder.Uri.ToString()),
                            CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                    }
                    else
                    {
                        // if (!TbbUpdateNeeded) {
                        webClient.DownloadFile(
                        new Uri(UriBuilder.Uri.ToString()),
                            CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                        // }
                    }

                    if (!File.Exists(CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)))
                    {
                        webClient.DownloadFile(
                            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll"),
                            CopyPath("projects", "lol_air_client",
                                Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                    }
                    else
                    {
                        if (FlashUpdateNeeded)
                        {
                            webClient.DownloadFile(
                                new Uri(
                                    "https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll"),
                                CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                        }
                    }

                    if (!File.Exists(CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air)))
                    {
                        webClient.DownloadFile(
                            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll"),
                            CopyPath("projects", "lol_air_client",
                                Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                    }
                    else
                    {
                        if (AirUpdateNeeded)
                        {
                            webClient.DownloadFile(
                                new Uri(
                                    "https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe Air.dll"),
                                CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                        }
                    }

                    Copy(
                        "Cg.dll", "solutions", "lol_game_client_sln", Sln);
                    Copy(
                        "CgD3D9.dll", "solutions", "lol_game_client_sln", Sln);
                    UnblockFile("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    UnblockFile("solutions", "lol_game_client_sln", "cg.dll", Sln);
                    UnblockFile("solutions", "lol_game_client_sln", "cgGL.dll", Sln);
                    UnblockFile("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    UnblockFile("projects", "lol_air_client", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air);
                    UnblockFile("projects", "lol_air_client", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    UnblockGFile("Config", "game.cfg");
                }
                else if (!Directory.Exists("Game")) return;
                GReadOnly("Game", Path.Combine("DATA", "CFG", "defaults", "game.cfg"));
                GReadOnly("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent.cfg"));
                GReadOnly("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"));
                GReadOnly("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"));
                CgCheck(new List<object>
                {
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "cg.dll")).ProductVersion),
                                            new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "cgGL.dll")).ProductVersion),
                                              new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "cgD3D9.dll")).ProductVersion)
                }, webClient);
                GReadOnly("Game", "tbb.dll");
                GReadOnly("Game", "Cg.dll");
                GReadOnly("Game", "CgGl.dll");
                GReadOnly("Game", "CgD3D9.dll");
                GReadOnly("Air", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                GReadOnly("Air", Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));

                if (IsMultiCore)
                {
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg")))
                    {
                        Cfg("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), true);
                    }
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg")))
                    {
                        Cfg("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), true);
                    }
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg")))
                    {
                        Cfg("GamePermanent_zh_MY.cfg",
                            Path.Combine("Game", "DATA", "CFG", "defaults"), true);
                    }
                    if (!File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")))
                        return;
                    Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), true);
                }
                else
                {
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg")))
                    {
                        Cfg("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), false);
                    }
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg")))
                    {
                        Cfg("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), false);
                    }
                    if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg")))
                    {
                        Cfg("GamePermanent_zh_MY.cfg",
                            Path.Combine("Game", "DATA", "CFG", "defaults"), false);
                    }
                    if (!File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")))
                        return;
                    Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), false);
                }
                Copy("cg.dll",
                    _cgBinPath,
                    "Game");
                Copy("cgGL.dll",
                    _cgBinPath,
                    "Game");
                Copy("cgD3D9.dll", _cgBinPath, "Game");

                if (!File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")))
                {
                    webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll"),
                        Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                }
                else
                {
                    if (FlashUpdateNeeded)
                    {
                        webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll"),
                           Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                    }
                }

                if (!File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")))
                {
                    webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll"),
                        Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                }
                else
                {
                    if (AirUpdateNeeded)
                    {
                        webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll"),
                          Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                    }
                }

                if (!File.Exists(Path.Combine("Game", "tbb.dll")))
                {
                    webClient.DownloadFile(
                            new Uri(UriBuilder.Uri.ToString()),
                        Path.Combine("Game", "tbb.dll"));
                }
                else
                {
                    // if (!TbbUpdateNeeded) {
                    webClient.DownloadFile(
                        new Uri(UriBuilder.Uri.ToString()),
                        Path.Combine("Game", "tbb.dll"));
                    // }
                }

                UnblockGFile("Game", Path.Combine("DATA", "CFG", "defaults", "game.cfg"));
                UnblockGFile("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent.cfg"));
                UnblockGFile("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"));
                UnblockGFile("Game", Path.Combine("DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"));
                UnblockGFile("Game", "tbb.dll");
                UnblockGFile("Game", "cg.dll");
                UnblockGFile("Game", "cgGl.dll");
                UnblockGFile("Game", "cgD3D9.dll");
                UnblockGFile("Air", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                UnblockGFile("Air", Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
            }
        }

        private static void GCopy(string path, string file)
        {
            if (File.Exists(Path.Combine(path, file)))
            {
                Copy(file, path, "Backup");
            }
        }

        private static void UnblockFile(string path1, string path2, string file, string ver)
        {
            if (File.Exists(CopyPath(path1, path2, file, ver)))
            {
                Unblock(CopyPath(path1, path2, file, ver));
            }
        }

        private static void ReadOnly(string path1, string path2, string file, string ver)
        {
            if (File.Exists(CopyPath(path1, path2,
                file, ver)) &&
                new FileInfo(CopyPath(path1, path2,
                    file, ver)).Attributes
                    .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(CopyPath(path1, path2,
                    file, ver),
                    FileAttributes.Normal);
            }
        }

        private static void UnblockGFile(string path, string file)
        {
            if (File.Exists(Path.Combine(path, file)))
            {
                Unblock(Path.Combine(path, file));
            }
        }

        private static void GReadOnly(string path, string file)
        {
            if (File.Exists(Path.Combine(path, file)) &&
                new FileInfo(Path.Combine(path, file)).Attributes
                    .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(Path.Combine(path, file),
                    FileAttributes.Normal);
            }
        }

        private static void CgCheck(IEnumerable<object> list, WebClient webClient)
        {
            if (!list.Cast<Version>().Any(var => var < CgLatestVersion)) return;
            if (!string.IsNullOrEmpty(_cgBinPath) || new Version(
                FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= CgLatestVersion)
                return;
            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Cg-3.1_April2012_Setup.exe"), "Cg-3.1_April2012_Setup.exe");
            Unblock("Cg-3.1_April2012_Setup.exe");

            Process cg = new Process
            {
                StartInfo =
                    new ProcessStartInfo
                    {
                        FileName =
                            "Cg-3.1_April2012_Setup.exe",
                        Arguments = "/silent /TYPE=compact"
                    }
            };
            cg.Start();
            cg.WaitForExit();
            File.Delete("Cg-3.1_April2012_Setup.exe");
            _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                EnvironmentVariableTarget.User);
        }

        private static void Kill(IEnumerable process)
        {
            if (IsMultiCore)
            {
                Parallel.ForEach(Process.GetProcessesByName(process.ToString()), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            }
            else
            {
                foreach (Process proc in Process.GetProcessesByName(process.ToString()))
                {
                    proc.Kill();
                    proc.WaitForExit();
                }
            }
        }

        private static void CopyX(string folder, string folder1, string file, string to, string version)
        {
            File.Copy(
                Path.Combine("RADS", folder, folder1, "releases", version, "deploy", to, file)
                , Path.Combine("Backup", file),
                true);
        }

        private static string CopyPath(string folder, string folder1, string file, string version)
        {
            return Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file);
        }

        private static void Copy(string file, string folder, string folder1, string version)
        {
            File.Copy(
              Path.Combine(
                  _cgBinPath, file),
              Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file), true);
        }

        private static void Copy(string file, string from, string to)
        {
            File.Copy(Path.Combine(from, file),
                Path.Combine(to, file), true);
        }

        private static void Cfg(string file, string path, bool mode)
        {
            if (mode)
            {
                if (File.ReadAllText(Path.Combine(path, file))
                    .Contains("DefaultParticleMultiThreading=1")) return;
                File.AppendAllText(Path.Combine(path, file),
                    string.Format("{0}{1}", Environment.NewLine, "DefaultParticleMultiThreading=1"));
            }
            else
            {
                var oldLines = File.ReadAllLines(Path.Combine(path, file));
                if (!oldLines.Contains("DefaultParticleMultiThreading=1")) return;
                var newLines = oldLines.Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains("DefaultParticleMultiThreading=1")).Select(lineInfo => lineInfo.Line);
                File.WriteAllLines(Path.Combine(path, file), newLines);
            }
        }

        private static string Version(string folder, string folder1)
        {
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max()) : null;
        }

        private static string HighestInstructionSupported()
        {
            if (IsHaswell)
            { return "Haswell.dll"; }
            if (IsMultiCore)
            {
                if (Environment.Is64BitProcess && (ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 17)
                {
                    return "AVX.dll";
                }
                if ((ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 10)
                {
                    return "SSE2.dll";
                }
                return (ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 6 ? "SSE.dll" : "tbb.dll";
            }
            if (IsMultiCore) return null;
            if ((ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 10)
            {
                return "SSE2ST.dll";
            }
            return (ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 6 ? "SSEST.dll" : "tbbST.dll";
        }

        private static int ToInt(string value)
        {
            int result;
            int.TryParse(value, out result);
            return result;
        }

        [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DeleteFile(string name);

        private static void Unblock(string fileName)
        {
            DeleteFile(fileName + ":Zone.Identifier");
        }
    }
}