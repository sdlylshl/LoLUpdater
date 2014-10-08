using System;
using System.Collections;
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

        private static Uri _tbbVersionUri;

        private static readonly Uri FinaltbbVersionUri = Uri.TryCreate(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/"),
                HighestSupportedInstruction(), out _tbbVersionUri) ? _tbbVersionUri : new Uri(string.Empty);

        private static readonly Uri FlashUri =
            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll");

        private static readonly Uri AirUri =
    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll");

        private static readonly Version CgLatestVersion = new Version("3.1.13");

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
            Console.WriteLine("Patching...");
            Console.WriteLine("");
            Kill(LoLProccessStrings);
            using (WebClient webClient = new WebClient())
            {
                CgCheck(webClient);
                if (!Directory.Exists("Backup"))
                {
                    Directory.CreateDirectory("Backup");
                    if (Directory.Exists("RADS"))
                    {
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
                    else
                    {
                        Copy("cg.dll", "Game", "Backup");
                        Copy("cgGL.dll", "Game", "Backup");
                        Copy("cgD3D9.dll", "Game", "Backup");
                        Copy("tbb.dll", "Game", "Backup");
                        Copy("Adobe AIR.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Backup");
                        Copy("NPSWF32.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"),
                            "Backup");
                        GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "game.cfg");
                        GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent.cfg");
                        GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_zh_MY.cfg");
                        GCopy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_en_SG.cfg");
                    }
                }

                if (Directory.Exists("RADS"))
                {
                    RemoveReadOnly("Config", string.Empty, string.Empty, "game.cfg", string.Empty);
                    RemoveReadOnly(string.Empty, "solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    RemoveReadOnly(string.Empty, "solutions", "lol_game_client_sln", "cg.dll", Sln);
                    RemoveReadOnly(string.Empty, "solutions", "lol_game_client_sln", "cgGl.dll", Sln);
                    RemoveReadOnly(string.Empty, "solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    RemoveReadOnly(string.Empty, "projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    RemoveReadOnly(string.Empty, "projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air);

                    if (IsMultiCore)
                    {
                        if (File.Exists(Path.Combine("Config", "game.cfg")))
                        {
                            Cfg("game.cfg", "Config", true);
                        }
                    }
                    if (!IsMultiCore)
                    {
                        if (File.Exists(Path.Combine("Config", "game.cfg")))
                        {
                            Cfg("game.cfg", "Config", false);
                        }
                    }
                    webClient.DownloadFile(
                        FinaltbbVersionUri,
                        CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));

                    webClient.DownloadFile(
                        FlashUri,
                        CopyPath("projects", "lol_air_client",
                            Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));

                    webClient.DownloadFile(
                        AirUri,
                        CopyPath("projects", "lol_air_client",
                            Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air));

                    Copy(
                        "cg.dll", "solutions", "lol_game_client_sln", Sln);
                    Copy(
                        "cgD3D9.dll", "solutions", "lol_game_client_sln", Sln);
                    UnblockFile(string.Empty, "solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    UnblockFile(string.Empty, "solutions", "lol_game_client_sln", "cg.dll", Sln);
                    UnblockFile(string.Empty, "solutions", "lol_game_client_sln", "cgGL.dll", Sln);
                    UnblockFile(string.Empty, "solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    UnblockFile(string.Empty, "projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air);
                    UnblockFile(string.Empty, "projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    UnblockFile("Config", string.Empty, string.Empty, "game.cfg", string.Empty);
                }
                else
                {
                    RemoveReadOnly("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "game.cfg"), string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent.cfg"), string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, "tbb.dll", string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, "cg.dll", string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, "cgGL.dll", string.Empty);
                    RemoveReadOnly("Game", string.Empty, string.Empty, "cgD3D9.dll", string.Empty);
                    RemoveReadOnly("Air", string.Empty, string.Empty, Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), string.Empty);
                    RemoveReadOnly("Air", string.Empty, string.Empty, Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty);

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
                        if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")))
                        { Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), true); }
                    }
                    if (!IsMultiCore)
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
                        if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")))
                        { Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), false); }
                    }
                    Copy("cg.dll",
                        _cgBinPath,
                        "Game");
                    Copy("cgGL.dll",
                        _cgBinPath,
                        "Game");
                    Copy("cgD3D9.dll", _cgBinPath, "Game");

                    webClient.DownloadFile(
                        FlashUri,
                        Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));

                    webClient.DownloadFile(
                        AirUri,
                        Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));

                    webClient.DownloadFile(
                        FinaltbbVersionUri,
                        Path.Combine("Game", "tbb.dll"));

                    UnblockFile("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "game.cfg"), string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent.cfg"), string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, Path.Combine("DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, "tbb.dll", string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, "cg.dll", string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, "cgGl.dll", string.Empty);
                    UnblockFile("Game", string.Empty, string.Empty, "cgD3D9.dll", string.Empty);
                    UnblockFile("Air", string.Empty, string.Empty, Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), string.Empty);
                    UnblockFile("Air", string.Empty, string.Empty, Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty);
                }
                Console.WriteLine("Done!");
                Console.ReadLine();
                if (File.Exists("lol_launcher.exe"))
                {
                    Process.Start("lol_launcher.exe");
                }
                Environment.Exit(0);
            }
        }

        private static void GCopy(string path, string file)
        {
            if (File.Exists(Path.Combine(path, file)))
            {
                Copy(file, path, "Backup");
            }
        }

        private static void UnblockFile(string path, string path1, string path2, string file, string ver)
        {
            if (File.Exists(CopyPath(path1, path2, file, ver)))
            {
                Unblock(CopyPath(path1, path2, file, ver));
            }
            if (File.Exists(Path.Combine(path, file)))
            {
                Unblock(Path.Combine(path, file));
            }
        }

        private static void RemoveReadOnly(string path, string path1, string path2, string file, string ver)
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
            if (File.Exists(Path.Combine(path, file)) &&
    new FileInfo(Path.Combine(path, file)).Attributes
        .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(Path.Combine(path, file),
                    FileAttributes.Normal);
            }
        }

        private static void CgCheck(WebClient webClient)
        {
            if (!string.IsNullOrEmpty(_cgBinPath) && new Version(
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
            if (File.Exists(Path.Combine("RADS", folder, folder1, "releases", version, "deploy", to, file)))
            {
                File.Copy(
                    Path.Combine("RADS", folder, folder1, "releases", version, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
        }

        private static string CopyPath(string folder, string folder1, string file, string version)
        {
            if (File.Exists(Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file)))
            {
                Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file);
            }
            return null;
        }

        private static void Copy(string file, string folder, string folder1, string version)
        {
            if (File.Exists(Path.Combine(
                  _cgBinPath, file)))
            {
                File.Copy(
                 Path.Combine(
                     _cgBinPath, file),
                 Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file), true);
            }
        }

        private static void Copy(string file, string from, string to)
        {
            if (File.Exists(Path.Combine(from, file)))
            {
                File.Copy(Path.Combine(from, file),
                    Path.Combine(to, file), true);
            }
        }

        private static void Cfg(string file, string path, bool mode)
        {
            if (!File.Exists(Path.Combine(path, file))) return;
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
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max()) : string.Empty;
        }

        private static string HighestSupportedInstruction()
        {
            if (IsHaswell)
            { return "AVX2.dll"; }
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