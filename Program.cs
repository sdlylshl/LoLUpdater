using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal class Program : Methods
    {
        private static readonly string Adobe = Riot
            ? Path.Combine("RADS", "projects", "lol_air_client", "releases", Air, "deploy", "Adobe AIR", "Versions",
                "1.0")
            : Path.Combine("Air", "Adobe AIR", "Versions", "1.0");

        private static readonly string[] Cfgfiles =
        {
            "game.cfg", "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
            "GamePermanent_en_SG.cfg"
        };

        private static readonly string[] Cgfiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        private static int _userInput;
        private static readonly bool Installing = Convert.ToBoolean(_userInput = 1);
        private static readonly bool MultiCore = CpuInfo.AsParallel().Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static readonly Uri TbbUri =
            new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"), Avx2
                ? "Avx2.dll"
                : (Avx
                    ? "Avx.dll"
                    : (Sse2 ? "Sse2.dll" : Sse ? "Sse.dll" : "Default.dll")));

        private static void Main(string[] args)
        {
            if (!OnlyInstance.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(OnlyInstance);
            if (args.Length > 0)
            {
                switch (args[0])
                {
                    case "--help":
                        Start();
                        Console.WriteLine("Command-Line Arguments:\n");
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
                        if (Riot)
                        {
                            Parallel.ForEach(Files,
                                file => { BakCopy(file, "solutions", "lol_game_client_sln", Sln, true); });
                            BakCopy("Adobe AIR.dll", "projects", "lol_air_client", Air,
                                Path.Combine("Adobe AIR", "Versions", "1.0"), true);
                            BakCopy("NPSWF32.dll", "projects", "lol_air_client", Air,
                                Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), true);
                            BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty,
                                true);
                            Cfg("game.cfg", "Config", MultiCore);
                            Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
                            Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                            Download("tbb.dll", TbbSum, TbbUri, "solutions", "lol_game_client_sln", Sln);
                            Parallel.ForEach(Cgfiles, file =>
                            {
                                Copy(Path.Combine(CgBinPath,
                                    file), "solutions", "lol_game_client_sln", Sln, file);
                            });
                        }
                        else
                        {
                            Parallel.ForEach(Files, file => { Copy("Game", file, "Backup", true); });
                            Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll",
                                "Backup", true);
                            Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup",
                                true);
                            Copy(AdobePath, "Adobe AIR.dll", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"),
                                true);
                            Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"),
                                Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), true);
                            Download(Path.Combine("Game", "tbb.dll"), TbbSum, TbbUri, string.Empty, string.Empty,
                                string.Empty);
                            Parallel.ForEach(Cfgfiles, file =>
                            {
                                Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, "Backup", true);
                                Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), MultiCore);
                            });
                            Parallel.ForEach(Cgfiles, file =>
                            {
                                Copy(CgBinPath, file,
                                    "Game", true);
                            });
                        }
                        Updater();
                        FinishedPrompt("Done Installing!");
                        break;

                    case "-uninst":
                        Console.WriteLine("Uninstalling...");
                        Start();
                        if (Riot)
                        {
                            BakCopy("Adobe AIR.dll", "projects", "lol_air_client", Air,
                                Path.Combine("Adobe AIR", "Versions", "1.0"), false);
                            BakCopy("NPSWF32.dll", "projects", "lol_air_client", Air,
                                Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), false);
                            BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty,
                                false);
                            Parallel.ForEach(Files,
                                file => { BakCopy(file, "solutions", "lol_game_client_sln", Sln, false); });
                        }
                        else
                        {
                            Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll",
                                "Backup", false);
                            Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup",
                                false);
                            Parallel.ForEach(Files, file => { Copy("Game", file, "Backup", false); });
                        }
                        Updater();
                        FinishedPrompt("Done Uninstalling!");
                        break;
                }
            }
            _userInput = DisplayMenu();

            Console.Clear();
            Kill();
            if (Installing & !Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
            }
            if (_userInput != 3)
            {
                Updater();
                Console.WriteLine("Configuring...");
                if (Riot)
                {
                    BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                        , Air, Path.Combine("Adobe AIR", "Versions", "1.0"), Installing);
                    BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                        , Air, Path.Combine("Adobe AIR", "Versions", "1.0", "Resources"), Installing);
                    BakCopy(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty, Installing);
                    Parallel.ForEach(Files,
                        file => { BakCopy(file, "solutions", "lol_game_client_sln", Sln, Installing); });
                }
                else
                {
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", "Backup",
                        Installing);
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", "Backup", Installing);
                    Parallel.ForEach(Files, file => { Copy("Game", file, "Backup", Installing); });
                }
                Console.WriteLine("");
                if (!Installing) return;
                Check();
            }
            switch (_userInput)
            {
                case 1:
                    Console.Clear();
                    Console.WriteLine("Installing...");
                    if (Riot)
                    {
                        Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
                        Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                        Cfg("game.cfg", "Config", MultiCore);
                        Download("tbb.dll", TbbSum, TbbUri, "solutions", "lol_game_client_sln", Sln);
                        Parallel.ForEach(Cgfiles, file =>
                        {
                            Copy(Path.Combine(CgBinPath,
                                file), "solutions", "lol_game_client_sln", Sln, file);
                        });
                    }
                    else
                    {
                        Copy(AdobePath, "Adobe AIR.dll", Adobe, true);
                        Copy(AdobePath, Path.Combine("Resources", "NPSWF32.dll"), Adobe, true);
                        Download(Path.Combine("Game", "tbb.dll"), TbbSum, TbbUri, string.Empty, string.Empty,
                            string.Empty);
                        Parallel.ForEach(Cfgfiles, file =>
                        {
                            Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), file, "Backup", Installing);
                            Cfg(file, Path.Combine("Game", "DATA", "CFG", "defaults"), MultiCore);
                        });
                        Parallel.ForEach(Cgfiles, file => { Copy(CgBinPath, file, "Game", Installing); });
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