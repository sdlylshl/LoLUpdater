using System;
using System.Diagnostics;
using System.IO;

namespace LoLUpdater
{
    internal static class Program
    {
        private static void Main()
        {
            Console.WriteLine("Please Wait!");
            if (File.Exists("LoLUpdater Updater.exe"))
            {
                Unblock("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty);
            }
            if (File.Exists("LoLUpdater Uninstall.exe"))
            {
                Unblock("LoLUpdater Uninstall.exe", string.Empty, string.Empty, string.Empty);
            }
            if (File.Exists(Pmb))
            {
                Kill("PMB");
                Process.Start(new ProcessStartInfo { FileName = Pmb, Arguments = "/silent" });
            }
            Kill(LoLProccess);

            if (!Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
                if (Directory.Exists("RADS"))
                {
                    CopyToBak("solutions", "lol_game_client_sln", "cg.dll", SlnFolder);
                    CopyToBak("solutions", "lol_game_client_sln", "cgD3D9.dll", SlnFolder);
                    CopyToBak("solutions", "lol_game_client_sln", "cgGL.dll", SlnFolder);
                    CopyToBak("solutions", "lol_game_client_sln", "tbb.dll", SlnFolder);
                    CopyToBak("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirFolder);
                    CopyToBak("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), AirFolder);
                    CopyToBak(string.Empty, string.Empty, Path.Combine("Config", "game.cfg"), string.Empty);
                }
                else
                {
                    Copy(Path.Combine("Game", "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "cgGL.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "cgD3D9.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "tbb.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), string.Empty, string.Empty, string.Empty, string.Empty, "Backup");
                }
            }

            if (Directory.Exists("RADS"))
            {
                Cfg("game.cfg", "Config", IsMultiCore);
                Download("tbb.dll", Tbb, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Flash, FlashUri, "projects", "lol_air_client", AirFolder);
                Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air, AirUri, "projects", "lol_air_client", AirFolder);
                Copy(string.Empty,
                    "cg.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty);
                Copy(string.Empty,
                    "cgD3D9.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty);
            }
            else
            {
                Cfg("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent_zh_MY.cfg",
                    Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);

                Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Flash, FlashUri, string.Empty, string.Empty, string.Empty);
                Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air, AirUri, string.Empty, string.Empty, string.Empty);
                Download(Path.Combine("Game", "tbb.dll"), Tbb, TbbUri, string.Empty, string.Empty, string.Empty);
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cg.dll"));
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cgGL.dll"));
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cgD3D9.dll"));
            }
            Console.Clear();
            Console.WriteLine("Done!");
            if (File.Exists("lol_launcher.exe"))
            {
                Process.Start("lol_launcher.exe");
            }
            Console.ReadLine();
            Environment.Exit(0);
        }
    }
}