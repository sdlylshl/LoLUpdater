using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Threading.Tasks;

namespace LoLUpdater_Uninstall
{
    internal static class Program
    {
        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
           .Cast<ManagementBaseObject>()
           .Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static readonly string Sln = Version("solutions", "lol_game_client_sln");
        private static readonly string Air = Version("projects", "lol_air_client");
        private static readonly string[] LoLProcces = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" };

        private static void Main()
        {
            Console.Clear();
            Console.WriteLine("Uninstalling...");
            Console.WriteLine("");
            Kill(LoLProcces);
            if (Directory.Exists("RADS"))
            {
                CopyFromBak("Cg.dll", "solutions", "lol_game_client_sln", string.Empty, Sln);
                CopyFromBak("CgD3D9.dll", "solutions", "lol_game_client_sln", string.Empty, Sln);
                CopyFromBak("CgGL.dll", "solutions", "lol_game_client_sln", string.Empty, Sln);
                CopyFromBak("tbb.dll", "solutions", "lol_game_client_sln", string.Empty, Sln);
                CopyFromBak("Adobe AIR.dll", "projects", "lol_air_client",
                    Path.Combine("Adobe Air", "Versions", "1.0"), Air);
                CopyFromBak("NPSWF32.dll", "projects", "lol_air_client",
                    Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), Air);
                Directory.Delete("Backup", true);
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
                Directory.Delete("Backup", true);
            }
            File.Delete("LoLUpdater.exe");
            File.Delete("LoLUpdater Uninstall.exe");
        }

        private static void CopyFromBak(string folder, string folder1, string file, string to, string version)
        {
            File.Copy(Path.Combine("Backup", file)

                , Path.Combine("RADS", folder, folder1, "releases", version, "deploy", to, file),
                true);
        }

        private static string Version(string folder, string folder1)
        {
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max()) : null;
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

        private static int ToInt(string value)
        {
            int result;
            int.TryParse(value, out result);
            return result;
        }

        private static void Copy(string file, string from, string to)
        {
            File.Copy(Path.Combine(from, file),
                Path.Combine(to, file), true);
        }
    }
}