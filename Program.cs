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
        private static readonly string Launch = Version("projects", "lol_launcher");
        private static readonly string Patch = Version("projects", "lol_patcher");
        private static readonly string[] LoLProcces = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" };
        private static readonly Version TbblatestVersion = new Version(4, 3, 0, 0);
        private static readonly Version FlashLatestVersion = new Version(15, 0, 0, 192);
        private static readonly Version AirLatestVersion = new Version(15, 0, 0, 297);
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
            try
            {
                if (Directory.Exists("RADS"))
                {
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln",
                                        "tbb.dll", Sln)) &&
         new FileInfo(CopyPath("solutions", "lol_game_client_sln",
                                        "tbb.dll", Sln)).Attributes
         .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("solutions", "lol_game_client_sln",
                                        "tbb.dll", Sln),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln",
                        "Cg.dll", Sln)) &&
    new FileInfo(CopyPath("solutions", "lol_game_client_sln",
                        "Cg.dll", Sln)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("solutions", "lol_game_client_sln",
                                        "CgGl.dll", Sln),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln",
        "CgGl.dll", Sln)) &&
    new FileInfo(CopyPath("solutions", "lol_game_client_sln",
        "CgGl.dll", Sln)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("solutions", "lol_game_client_sln",
                                        "CgGl.dll", Sln),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln",
    "CgD3D9.dll", Sln)) &&
    new FileInfo(CopyPath("solutions", "lol_game_client_sln",
    "CgD3D9.dll", Sln)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("solutions", "lol_game_client_sln",
                                        "CgD3D9.dll", Sln),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher",
    "CgD3D9.dll", Patch)) &&
    new FileInfo(CopyPath("projects", "lol_patcher",
    "CgD3D9.dll", Patch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_patcher",
                                        "CgD3D9.dll", Patch),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher",
    "Cg.dll", Patch)) &&
    new FileInfo(CopyPath("projects", "lol_patcher",
    "Cg.dll", Patch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_patcher",
                                        "Cg.dll", Patch),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher",
    "CgGl.dll", Patch)) &&
    new FileInfo(CopyPath("projects", "lol_patcher",
    "CgGl.dll", Patch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_patcher",
                                        "CgGl.dll", Launch),
                            FileAttributes.Normal);
                    }

                    if (File.Exists(CopyPath("projects", "lol_launcher",
    "CgD3D9.dll", Launch)) &&
    new FileInfo(CopyPath("projects", "lol_launcher",
    "CgD3D9.dll", Launch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_launcher",
                                        "CgD3D9.dll", Launch),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_launcher",
    "Cg.dll", Launch)) &&
    new FileInfo(CopyPath("projects", "lol_launcher",
    "Cg.dll", Launch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_launcher",
                                        "Cg.dll", Launch),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_launcher",
    "CgGl.dll", Launch)) &&
    new FileInfo(CopyPath("projects", "lol_launcher",
    "CgGl.dll", Launch)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_launcher",
                                        "CgGl.dll", Launch),
                            FileAttributes.Normal);
                    }

                    if (File.Exists(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)) &&
    new FileInfo(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air)) &&
    new FileInfo(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air)).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(CopyPath("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln)))
                    {
                        Unblock(CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                    }

                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln", "Cg.dll", Sln)))
                    {
                        Unblock(CopyPath("solutions", "lol_game_client_sln", "Cg.dll", Sln));
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln", "CgGl.dll", Sln)))
                    {
                        Unblock(CopyPath("solutions", "lol_game_client_sln", "CgGl.dll", Sln));
                    }

                    if (File.Exists(CopyPath("projects", "lol_air_client",
                            Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air)))
                    {
                        Unblock(CopyPath("projects", "lol_air_client",
                            Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air));
                    }
                    if (File.Exists(CopyPath("projects", "lol_air_client",
                            Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)))
                    {
                        Unblock(CopyPath("projects", "lol_air_client",
                            Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                    }
                    if (File.Exists(CopyPath("projects", "lol_launcher", "CgD3D9.dll", Launch)))
                    {
                        Unblock(CopyPath("projects", "lol_launcher", "CgD3D9.dll", Launch));
                    }
                    if (File.Exists(CopyPath("projects", "lol_launcher", "CgGl.dll", Launch)))
                    {
                        Unblock(CopyPath("projects", "lol_launcher", "CgGl.dll", Launch));
                    }
                    if (File.Exists(CopyPath("projects", "lol_launcher", "Cg.dll", Launch)))
                    {
                        Unblock(CopyPath("projects", "lol_launcher", "Cg.dll", Launch));
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher", "CgD3D9.dll", Patch)))
                    {
                        Unblock(CopyPath("projects", "lol_patcher", "CgD3D9.dll", Patch));
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher", "CgGl.dll", Patch)))
                    {
                        Unblock(CopyPath("projects", "lol_patcher", "CgGl.dll", Patch));
                    }
                    if (File.Exists(CopyPath("projects", "lol_patcher", "Cg.dll", Patch)))
                    {
                        Unblock(CopyPath("projects", "lol_patcher", "Cg.dll", Patch));
                    }
                    if (File.Exists(CopyPath("solutions", "lol_game_client_sln", "CgD3D9.dll", Sln)))
                    {
                        Unblock(CopyPath("solutions", "lol_game_client_sln", "CgD3D9.dll", Sln));
                    }
                }
                else if (Directory.Exists("Game"))
                {
                    if (File.Exists(Path.Combine("Game", "tbb.dll")) &&
    new FileInfo(Path.Combine("Game", "tbb.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Game", "tbb.dll"),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(Path.Combine("Game", "Cg.dll")) &&
    new FileInfo(Path.Combine("Game", "Cg.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Game", "Cg.dll"),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(Path.Combine("Game", "CgGl.dll")) &&
    new FileInfo(Path.Combine("Game", "Cg.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Game", "CgGl.dll"),
                            FileAttributes.Normal);
                    }
                    if (File.Exists(Path.Combine("Game", "CgD3D9.dll")) &&
    new FileInfo(Path.Combine("Game", "CgD3D9.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Game", "CgD3D9.dll"),
                            FileAttributes.Normal);
                    }

                    if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")) &&
    new FileInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"),
                            FileAttributes.Normal);
                    }

                    if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")) &&
    new FileInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")).Attributes
    .Equals(FileAttributes.ReadOnly))
                    {
                        File.SetAttributes(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"),
                            FileAttributes.Normal);
                    }

                    if (File.Exists(Path.Combine("Game", "tbb.dll")))
                    {
                        Unblock(Path.Combine("Game", "tbb.dll"));
                    }

                    if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")))
                    {
                        Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                    }
                    if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")))
                    {
                        Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                    }
                    if (File.Exists(Path.Combine("Game", "CgD3D9.dll")))
                    {
                        Unblock(Path.Combine("Game", "CgD3D9.dll"));
                    }
                    if (File.Exists(Path.Combine("Game", "CgGl.dll")))
                    {
                        Unblock(Path.Combine("Game", "CgGl.dll"));
                    }
                    if (File.Exists(Path.Combine("Game", "Cg.dll")))
                    {
                        Unblock(Path.Combine("Game", "Cg.dll"));
                    }
                }

                if (File.Exists(Path.Combine("Config", "game.cfg")) &&
                    new FileInfo(Path.Combine("Config", "game.cfg")).Attributes.Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine("Config", "game.cfg"), FileAttributes.Normal);
                }
                if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg")) &&
                    new FileInfo(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg")).Attributes.Equals(
                        FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg"), FileAttributes.Normal);
                }
                if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg")) &&
                    new FileInfo(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg")).Attributes.Equals(
                        FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg"),
                        FileAttributes.Normal);
                }
                if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg")) &&
                    new FileInfo(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg")).Attributes
                        .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"),
                        FileAttributes.Normal);
                }
                if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")) &&
                    new FileInfo(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg")).Attributes
                        .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"),
                        FileAttributes.Normal);
                }
                if (IsMultiCore)
                {
                    if (File.Exists(Path.Combine("Config", "game.cfg")))
                    {
                        Cfg("game.cfg", "Config", true);
                    }
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
                    if (File.Exists(Path.Combine("Config", "game.cfg")))
                    {
                        Cfg("game.cfg", "Config", false);
                    }
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
                if (File.Exists(PmbUninstall))
                {
                    Kill("PMB");
                    Process.Start(new ProcessStartInfo { FileName = PmbUninstall, Arguments = "/verysilent" });
                }
                Console.WriteLine("Patching...");
                Console.WriteLine("");
                Kill(LoLProcces);
                using (WebClient webClient = new WebClient())
                {
                    List<object> list = new List<object>
                {
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_patcher",
                            "releases", Patch, "deploy", "CgGl.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_patcher",
                            "releases", Patch, "deploy", "CgD3D9.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_patcher",
                            "releases", Patch, "deploy", "Cg.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_launcher",
                            "releases", Launch, "deploy", "CgGl.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_launcher",
                            "releases", Launch, "deploy", "CgD3D9.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "projects", "lol_launcher",
                            "releases", Launch, "deploy", "Cg.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "CgD3D9.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "CgGl.dll")).FileVersion),
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "Cg.dll")).FileVersion)
                };

                    List<object> garenalist = new List<object>
                {
                    new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "Cg.dll")).FileVersion),
                                            new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "CgGl.dll")).FileVersion),
                                              new Version(
                        FileVersionInfo.GetVersionInfo(Path.Combine("Game", "CgD3D9.dll")).FileVersion)
                };

                    if (!Directory.Exists("Backup"))
                    {
                        if (Directory.Exists("RADS"))
                        {
                            Directory.CreateDirectory("Backup");
                            CopyX("solutions", "lol_game_client_sln", "Cg.dll", string.Empty, Sln);
                            CopyX("solutions", "lol_game_client_sln", "CgD3D9.dll", string.Empty, Sln);
                            CopyX("solutions", "lol_game_client_sln", "CgGL.dll", string.Empty, Sln);
                            CopyX("solutions", "lol_game_client_sln", "tbb.dll", string.Empty, Sln);
                            CopyX("projects", "lol_air_client", "Adobe AIR.dll",
                                Path.Combine("Adobe Air", "Versions", "1.0"), Air);
                            CopyX("projects", "lol_air_client", "NPSWF32.dll",
                                Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), Air);
                            if (!File.Exists(Path.Combine("Config", "game.cfg"))) return;
                            Copy("game.cfg", "Config", "Backup");
                        }
                        else if (!File.Exists("Game")) return;
                        Directory.CreateDirectory("Backup");
                        Copy("Cg.dll", "Game", "Backup");
                        Copy("CgGL.dll", "Game", "Backup");
                        Copy("CgD3D9.dll", "Game", "Backup");
                        Copy("tbb.dll", "Game", "Backup");
                        Copy("Adobe AIR.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Backup");
                        Copy("NPSWF32.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"),
                            "Backup");
                        if (File.Exists(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg")))
                        {
                            Copy("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), "Backup");
                        }

                        if (File.Exists(Path.Combine(Path.Combine("Game", "DATA", "CFG", "defaults"),
                            "GamePermanent.cfg")))
                        {
                            Copy("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"),
                                "Backup");
                        }
                        if (
                            File.Exists(Path.Combine(Path.Combine("Game", "DATA", "CFG", "defaults"),
                                "GamePermanent_zh_MY.cfg")))
                        {
                            Copy("GamePermanent_zh_MY.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"),
                                "Backup");
                        }
                        if (
                            !File.Exists(Path.Combine(Path.Combine("Game", "DATA", "CFG", "defaults"),
                                "GamePermanent_en_SG.cfg"))) return;
                        Copy("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"),
                            "Backup");
                    }

                    if (Directory.Exists("RADS"))
                    {
                        CgCheck(list, webClient);

                        if (File.Exists(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                        "releases", Sln, "deploy", "tbb.dll")) &&
                        new FileInfo(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "tbb.dll")).Attributes.Equals(FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                                "releases", Sln, "deploy", "tbb.dll"), FileAttributes.Normal);
                        }
                        if (!File.Exists(Path.Combine("RADS", "solutions", "lol_game_client_sln",
                            "releases", Sln, "deploy", "tbb.dll")))
                        {
                            webClient.DownloadFile(
                                new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/"),
                                    HighestInstructionSupported()),
                                CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                            Unblock(CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                        }
                        else
                        {
                            var tbbcurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(CopyPath("solutions", "lol_game_client_sln",
                                        "tbb.dll", Sln)).FileVersion);

                            if (tbbcurrent < TbblatestVersion)
                            {
                                webClient.DownloadFile(
                                    new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/"),
                                        HighestInstructionSupported()),
                                    CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                                Unblock(CopyPath("solutions", "lol_game_client_sln", "tbb.dll", Sln));
                            }
                        }

                        if (File.Exists(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)) &&
                            new FileInfo(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)).Attributes.Equals
                                (FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air),
                                FileAttributes.Normal);
                        }
                        if (!File.Exists(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air)))
                        {
                            webClient.DownloadFile(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/NPSWF32.dll"),
                                CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                            Unblock(CopyPath("projects", "lol_air_client",
                                Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                        }
                        else
                        {
                            var flashcurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air))
                                        .FileVersion);

                            if (flashcurrent < FlashLatestVersion)
                            {
                                webClient.DownloadFile(
                                    new Uri(
                                        "https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/NPSWF32.dll"),
                                    CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                                Unblock(CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air));
                            }
                        }
                        if (File.Exists(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air)) &&
        new FileInfo(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air)).Attributes.Equals
            (FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air),
                                FileAttributes.Normal);
                        }
                        if (!File.Exists(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air)))
                        {
                            webClient.DownloadFile(
                                new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/Adobe AIR.dll"),
                                CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                            Unblock(CopyPath("projects", "lol_air_client",
                                Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                        }
                        else
                        {
                            var aircurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air))
                                        .FileVersion);

                            if (aircurrent < AirLatestVersion)
                            {
                                webClient.DownloadFile(
                                    new Uri(
                                        "https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/Adobe AIR.dll"),
                                    CopyPath("projects", "lol_air_client",
                                        Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                                Unblock(CopyPath("projects", "lol_air_client",
                                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "Adobe AIR.dll"), Air));
                            }
                        }

                        Copy(
                            "Cg.dll", "solutions", "lol_game_client_sln", Sln);
                        Copy(
                            "Cg.dll", "projects", "lol_launcher", Launch);
                        Copy(
                            "Cg.dll", "projects", "lol_patcher", Patch);

                        Copy(
                            "CgGL.dll", "projects", "lol_launcher", Launch);
                        Copy(
                            "CgGL.dll", "projects", "lol_patcher", Patch);
                        Copy(
                            "CgD3D9.dll", "solutions", "lol_game_client_sln", Sln);
                        Copy(
                            "CgD3D9.dll", "projects", "lol_launcher", Launch);
                        Copy(
                            "CgD3D9.dll", "projects", "lol_patcher", Patch);
                    }
                    else if (Directory.Exists("Game"))
                    {
                        CgCheck(garenalist, webClient);
                        Copy("Cg.dll",
                            _cgBinPath,
                            "Game");
                        Copy("CgGL.dll",
                            _cgBinPath,
                            "Game");
                        Copy("CgD3D9.dll", _cgBinPath, "Game");

                        if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")) &&
    new FileInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")).Attributes.Equals(FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FileAttributes.Normal);
                        }
                        if (!File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")))
                        {
                            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/NPSWF32.dll"),
                                Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                            Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                        }
                        else
                        {
                            var aircurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll")).FileVersion);

                            if (aircurrent >= AirLatestVersion) return;
                            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/NPSWF32.dll"),
                                Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                            Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"));
                        }

                        if (File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")) &&
    new FileInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")).Attributes.Equals(FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), FileAttributes.Normal);
                        }
                        if (!File.Exists(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")))
                        {
                            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/Adobe AIR.dll"),
                                Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                            Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                        }
                        else
                        {
                            var aircurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll")).FileVersion);

                            if (aircurrent >= AirLatestVersion) return;
                            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/Adobe AIR.dll"),
                                Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                            Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"));
                        }

                        if (File.Exists(Path.Combine("Game", "tbb.dll")) &&
                        new FileInfo(Path.Combine("Game", "tbb.dll")).Attributes.Equals(FileAttributes.ReadOnly))
                        {
                            File.SetAttributes(Path.Combine("Game", "tbb.dll"), FileAttributes.Normal);
                        }
                        if (!File.Exists(Path.Combine("Game", "tbb.dll")))
                        {
                            webClient.DownloadFile(
                                new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/"),
                                    HighestInstructionSupported()),
                                Path.Combine("Game", "tbb.dll"));
                            Unblock(Path.Combine("Game", "tbb.dll"));
                        }
                        else
                        {
                            var tbbcurrent =
                                new Version(
                                    FileVersionInfo.GetVersionInfo(Path.Combine("Game", "tbb.dll")).FileVersion);

                            if (tbbcurrent >= TbblatestVersion) return;
                            webClient.DownloadFile(
                                new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/"),
                                    HighestInstructionSupported()),
                                Path.Combine("Game", "tbb.dll"));
                            Unblock(Path.Combine("Game", "tbb.dll"));
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private static void CgCheck(IEnumerable<object> list, WebClient webClient)
        {
            if (!list.Cast<Version>().Any(var => var < CgLatestVersion)) return;
            if (new Version(
                FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) <
                CgLatestVersion)
            {
                webClient.DownloadFile(
                    "https://github.com/Loggan08/LoLUpdater/raw/master/LoLUpdater/Resources/Cg-3.1_April2012_Setup.exe",
                    Path.Combine(Path.GetTempFileName(), "Cg-3.1_April2012_Setup.exe"));
                Unblock(Path.Combine(Path.GetTempFileName(), "Cg-3.1_April2012_Setup.exe"));
            }

            Process cg = new Process
            {
                StartInfo =
                    new ProcessStartInfo
                    {
                        FileName =
                            Path.Combine(Path.GetTempFileName(), "Cg-3.1_April2012_Setup.exe"),
                        Arguments = "/silent /TYPE=compact"
                    }
            };
            cg.Start();
            cg.WaitForExit();
            File.Delete(Path.Combine(Path.GetTempFileName(), "Cg-3.1_April2012_Setup.exe"));
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
            return Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max());
        }

        private static Uri HighestInstructionSupported()
        {
            if (IsHaswell)
            { return new Uri("Haswell.dll"); }
            if (IsMultiCore)
            {
                if ((ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 17 && Environment.Is64BitProcess)
                {
                    return new Uri("AVX.dll");
                }
                if ((ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 10)
                {
                    return new Uri("SSE2.dll");
                }
                return (ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 6 ? new Uri("SSE.dll") : new Uri("Tbb.dll");
            }
            if (IsMultiCore) return null;
            if ((ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 10)
            {
                return new Uri("SSE2ST.dll");
            }
            return (ushort)new ManagementObject("Win32_Processor.DeviceID='CPU0'")["Architecture"] == 6 ? new Uri("SSEST.dll") : new Uri("TbbST.dll");
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