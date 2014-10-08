using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace LoLUpdater_Uninstall
{
    internal static class Program
    {
        private static void Main()
        {
            Console.WriteLine("Please wait!");
            Kill(LoLProccess);
            if (Directory.Exists("RADS"))
            {
                CopyFromBak("Cg.dll", "solutions", "lol_game_client_sln", string.Empty, SlnFolder);
                CopyFromBak("CgD3D9.dll", "solutions", "lol_game_client_sln", string.Empty, SlnFolder);
                CopyFromBak("CgGL.dll", "solutions", "lol_game_client_sln", string.Empty, SlnFolder);
                CopyFromBak("tbb.dll", "solutions", "lol_game_client_sln", string.Empty, SlnFolder);
                CopyFromBak("Adobe AIR.dll", "projects", "lol_air_client",
                    Path.Combine("Adobe Air", "Versions", "1.0"), AirFolder);
                CopyFromBak("NPSWF32.dll", "projects", "lol_air_client",
                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), AirFolder);
            }
            else
            {
                Copy("Cg.dll", "Backup", "Game");
                Copy("CgGL.dll", "Backup", "Game");
                Copy("CgD3D9.dll", "Backup", "Game");
                Copy("Tbb.dll", "Backup", "Game");
                Copy("NPSWF32.dll", "Backup",
                    Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"));
                Copy("Adobe AIR.dll", "Backup", Path.Combine("Air", "Adobe AIR", "Versions", "1.0"));
            }
            Directory.Delete("Backup", true);
            File.Delete("LoLUpdater.exe");
            File.Delete("LoLUpdater Uninstall.exe");
        }
    }
}