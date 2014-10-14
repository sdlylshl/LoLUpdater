using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal class Program : Methods
    {
        private static readonly string[] cfgfiles = new string[] { "game.cfg", "GamePermanent.cfg", "GamePermanent_zh_MY.cfg", "GamePermanent_en_SG.cfg" };
        private static readonly string[] cgfiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        private static readonly bool IsInstalling = Convert.ToBoolean(_userInput = 1);
        private static readonly bool IsMultiCore = CpuInfo.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;
        private static readonly string Adobe = IsRads ? Path.Combine("RADS", "projects", "lol_air_client", "releases", Air, "deploy", "Adobe AIR", "Versions", "1.0") : Path.Combine("Air", "Adobe AIR", "Versions", "1.0");
        // Todo: Possibly recompile the Tbbs for better performance
        private static Uri TbbUri =
            new Uri( new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"), IsAvx2
                        ? "Avx2.dll"
                        : (HasAvx
                            ? "Avx.dll"
                            : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Default.dll")));

        private static int _userInput;

        private static void Main(string[] args)
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
            if (args.Length > 0)
            {
                switch (args[0])
                {
                    case "--help":
                        Console.WriteLine("Command Line Switches");
                        Console.WriteLine("-install : Installs LoLUpdater");
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
                        Check();
                        if (!Directory.Exists("Backup"))
                        {
                            Directory.CreateDirectory("Backup");
                        }
                        if (IsRads)
                        {
                            Parallel.ForEach(files, file =>
                            {
                                BakCopy(file, "solutions", "lol_game_client_sln", Sln, true);
                            });
                            BakCopy("Adobe AIR.dll", "projects", "lol_air_client", Air, Path.Combine("Adobe AIR", "Versions", "1.0"), true);
                            BakCopy("NPSWF32.dll", "projects", "lol_air_client", Air, Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), true);
                            BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, true);
                            Cfg("game.cfg", "Config", IsMultiCore);
                            Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
                            Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                            Download("tbb.dll", TbbSha512, TbbUri, "solutions", "lol_game_client_sln", Sln);
                            Parallel.ForEach(cgfiles, file =>
                            {
                                Copy(Path.Combine(_cgBinPath,
                                    file), "solutions", "lol_game_client_sln", Sln, file);
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
                            Copy(AdobePath, "Adobe AIR.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), true);
                            Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), true);
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
                            BakCopy("Adobe AIR.dll", "projects", "lol_air_client", Air, Path.Combine("Adobe AIR", "Versions", "1.0"), false);
                            BakCopy("NPSWF32.dll", "projects", "lol_air_client", Air, Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), false);
                            BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, false);
                            Parallel.ForEach(files, file =>
                            {
                                BakCopy(file, "solutions", "lol_game_client_sln", Sln, false);
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

            _userInput = DisplayMenu();
            Console.Clear();
Kill();
            if (IsInstalling & !Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
            }
            if (_userInput != 3)
            {
                Console.WriteLine("Configuring...");
                if (IsRads)
                {
                    BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                       , Air, Path.Combine("Adobe AIR", "Versions", "1.0"), IsInstalling);
                    BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                        , Air, Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), IsInstalling);
                    BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, IsInstalling);
                    Parallel.ForEach(files, file =>
                    {
                        BakCopy(file, "solutions", "lol_game_client_sln", Sln, IsInstalling);
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
                    Check();
                    Console.WriteLine("");
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
                        Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
                        Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                        Cfg("game.cfg", "Config", IsMultiCore);
                        Download("tbb.dll", TbbSha512, TbbUri, "solutions", "lol_game_client_sln", Sln);
                        Parallel.ForEach(cgfiles, file =>
                        {
                            Copy(Path.Combine(_cgBinPath,
                                file), "solutions", "lol_game_client_sln", Sln, file);
                        });
                    }
                    else
                    {
                        Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                        Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
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
        }
    }
}
