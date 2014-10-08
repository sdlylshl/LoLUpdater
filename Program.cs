using System;
using System.Collections;
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
        private static readonly string Sln = Version("solutions", "lol_game_client_sln");
        private static readonly string Air = Version("projects", "lol_air_client");
        private static readonly string[] LoLProccessStrings = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" };

        // Md5 checksums last updated 2014-10-08

        private const string Avx2 = "db0767dc94a2d1a757c783f6c7994301";

        private const string Avx = "2f178dadd7202b6a13a3409543a6fa86";

        private const string Sse2 = "1639aa390bfd02962c5c437d201045cc";

        private const string Sse = "3bf888228b83c4407d2eea6a5ab532bd";

        private const string Tbb = "44dde7926b6dfef4686f2ddd19c04e2d";

        private const string Sse2St = "82ed3be353217c61ff13a01bc85f1395";

        private const string SseSt = "eacd37174f1a4316345f985dc456a961";

        private const string TbbSt = "b389f80072bc877a6ef5ff33ade88a64";

        private const string AAir = "179a1fcfcb54e3e87365e77c719a723f";

        private const string AFlash = "9700dbdebffe429e1715727a9f76317b";

        // Will return the appropriate tbb string from above
        private static readonly string Tbbmd5 = Path.GetFileNameWithoutExtension(HighestSupportedInstruction());

        private static Uri _tbbVersionUri;

        private static readonly Uri FinaltbbVersionUri = Uri.TryCreate(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/"),
                HighestSupportedInstruction(), out _tbbVersionUri) ? _tbbVersionUri : new Uri(string.Empty);

        private static readonly Uri FlashUri =
            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll");

        private static readonly Uri AirUri =
    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll");

        private static readonly Version CgLatestVersion = new Version("3.1.13");

        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static readonly bool IsAvx2 = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Haswell")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Broadwell")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Skylake")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Cannonlake"));

        private static readonly bool IsUnix = (int)Environment.OSVersion.Platform == 6;
        private static readonly bool IsAtLeastWinNt6 = Environment.OSVersion.Version.Major >= 6;
        private static readonly bool Isx64 = Environment.Is64BitProcess;
        private static readonly bool HasAvx = IsProcessorFeaturePresent(17);
        private static readonly bool HasSse2 = IsProcessorFeaturePresent(10);
        private static readonly bool HasSse = IsProcessorFeaturePresent(6);

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static readonly string PmbUninstall = Path.Combine(Isx64
    ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
    : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
    "Pando Networks", "Media Booster", "uninst.exe");

        private static void Main()
        {
            if (File.Exists("LoLUpdater.txt"))
            {
                RemoveReadOnly("LoLUpdater.txt", string.Empty, string.Empty, string.Empty);
            }
            if (File.Exists("LoLUpdater Updater.exe"))
            { Unblock("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty); }
            if (File.Exists("LoLUpdater Uninstall.exe"))
            { Unblock("LoLUpdater Uninstall.exe", string.Empty, string.Empty, string.Empty); }
            if (File.Exists(PmbUninstall))
            {
                Kill("PMB"); Process.Start(new ProcessStartInfo { FileName = PmbUninstall, Arguments = "/silent" });
            }
            Console.WriteLine("Please Wait!");
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
                        CopyToBak("solutions", "lol_game_client_sln", "cg.dll", Sln);
                        CopyToBak("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                        CopyToBak("solutions", "lol_game_client_sln", "cgGL.dll", Sln);
                        CopyToBak("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                        CopyToBak("projects", "lol_air_client",
                            Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air);
                        CopyToBak("projects", "lol_air_client",
                            Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
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
                        Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg"), "Backup", "game.cfg");
                        Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg"), "Backup", "GamePermanent.cfg");
                        Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), "Backup", "GamePermanent_zh_MY.cfg");
                        Copy(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), "Backup", "GamePermanent_en_SG.cfg");
                    }
                }

                if (Directory.Exists("RADS"))
                {
                    RemoveReadOnly(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    RemoveReadOnly("solutions", "lol_game_client_sln", "cg.dll", Sln);
                    RemoveReadOnly("solutions", "lol_game_client_sln", "cgGl.dll", Sln);
                    RemoveReadOnly("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    RemoveReadOnly("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    RemoveReadOnly("projects", "lol_air_client",
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
                    Download(webClient, "solutions", "lol_game_client_sln", Sln, "tbb.dll", Tbbmd5, FinaltbbVersionUri);
                    Download(webClient, "projects", "lol_air_client", Air, Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), AFlash, FlashUri);
                    Download(webClient, "projects", "lol_air_client", Air, Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AAir, AirUri);

                    CopyCg(
                        "cg.dll", "solutions", "lol_game_client_sln", Sln);
                    CopyCg(
                        "cgD3D9.dll", "solutions", "lol_game_client_sln", Sln);
                    Unblock("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    Unblock("solutions", "lol_game_client_sln", "cg.dll", Sln);
                    Unblock("solutions", "lol_game_client_sln", "cgGL.dll", Sln);
                    Unblock("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    Unblock("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), Air);
                    Unblock("projects", "lol_air_client",
                        Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
                    Unblock(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty);
                }
                else
                {
                    RemoveReadOnly(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "tbb.dll"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "cg.dll"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "cgGL.dll"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Game", "cgD3D9.dll"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), string.Empty, string.Empty, string.Empty);
                    RemoveReadOnly(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty, string.Empty, string.Empty);

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
                    Download(webClient, string.Empty, string.Empty, string.Empty, Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AFlash, FlashUri);
                    Download(webClient, string.Empty, string.Empty, string.Empty, Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AAir, AirUri);
                    Download(webClient, string.Empty, string.Empty, string.Empty, Path.Combine("Game", "tbb.dll"), Tbbmd5, FinaltbbVersionUri);
                    Unblock(Path.Combine("Game", "DATA", "CFG", "defaults", "game.cfg"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent.cfg"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_zh_MY.cfg"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "DATA", "CFG", "defaults", "GamePermanent_en_SG.cfg"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "tbb.dll"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "cg.dll"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "cgGl.dll"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Game", "cgD3D9.dll"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), string.Empty, string.Empty, string.Empty);
                    Unblock(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty, string.Empty, string.Empty);
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

        private static void Download(WebClient webClient, string path, string path1, string version, string file, string md5, Uri uri)
        {
            if (Md5(DirPath(path, path1, version, file), md5))
            {
                webClient.DownloadFile(
                    uri,
                    DirPath(path, path1, version, file));
            }
            if (Md5(file, md5))
            {
                webClient.DownloadFile(
                    uri,
                    file);
            }
        }

        private static void Unblock(string file, string path1, string path2, string ver)
        {
            if (File.Exists(DirPath(path1, path2, ver, file)))
            {
                DeleteFile(DirPath(path1, path2, ver, file) + ":Zone.Identifier");
            }
            if (File.Exists(file))
            { DeleteFile(file + ":Zone.Identifier"); }
        }

        private static void RemoveReadOnly(string file, string path1, string path2, string ver)
        {
            if (File.Exists(DirPath(path1, path2,
                ver, file)) &&
                new FileInfo(DirPath(path1, path2,
                    file, ver)).Attributes
                    .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(DirPath(path1, path2,
                    ver, file),
                    FileAttributes.Normal);
            }
            if (File.Exists(file) &&
    new FileInfo(file).Attributes
        .Equals(FileAttributes.ReadOnly))
            {
                File.SetAttributes(file,
                    FileAttributes.Normal);
            }
        }

        private static void CgCheck(WebClient webClient)
        {
            if (!string.IsNullOrEmpty(_cgBinPath) && new Version(
                FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= CgLatestVersion)
                return;
            webClient.DownloadFile(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Cg-3.1_April2012_Setup.exe"), "Cg-3.1_April2012_Setup.exe");
            Unblock("Cg-3.1_April2012_Setup.exe", string.Empty, string.Empty, string.Empty);

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

        private static void CopyToBak(string folder, string folder1, string file, string version)
        {
            if (File.Exists(Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file)))
            {
                File.Copy(
                    Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file)
                    , Path.Combine("Backup", file),
                    true);
            }
        }

        private static string DirPath(string folder, string folder1, string version, string file)
        {
            return Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file);
        }

        private static void CopyCg(string file, string folder, string folder1, string version)
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
            if (File.Exists(Path.Combine(from, file)) && Directory.Exists(to))
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

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsProcessorFeaturePresent(uint processorFeature);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern void DeleteFile(string fileName);

        private static string HighestSupportedInstruction()
        {
            if ((Isx64 && IsAtLeastWinNt6 && IsAvx2) || (Isx64 && IsUnix && IsAvx2))
            { return "Avx2.dll"; }
            if (IsMultiCore)
            {
                if ((Isx64 && IsAtLeastWinNt6 && HasAvx) || (Isx64 && IsUnix && HasAvx))
                {
                    return "Avx.dll";
                }
                if (HasSse2)
                {
                    return "Sse2.dll";
                }
                return HasSse ? "Sse.dll" : "Tbb.dll";
            }
            if (HasSse2)
            {
                return "SSE2ST.dll";
            }

            return HasSse ? "SseSt.dll" : "TbbSt.dll";
        }

        private static int ToInt(string value)
        {
            int result;
            int.TryParse(value, out result);
            return result;
        }

        private static bool Md5(string file, string md5)
        {
            byte[] firstHash = Encoding.ASCII.GetBytes(HashFile(file));

            byte[] secondHash = Encoding.ASCII.GetBytes(md5);

            return firstHash.Where((t, i) => t != secondHash[i]).Any();
        }

        private static string HashFile(string filePath)
        {
            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                return HashFile(fs);
            }
        }

        private static string HashFile(Stream stream)
        {
            StringBuilder sb = new StringBuilder();

            if (stream == null) return sb.ToString();
            stream.Seek(0, SeekOrigin.Begin);

            byte[] hash = MD5.Create().ComputeHash(stream);
            foreach (byte b in hash)
                sb.Append(b.ToString("x2"));

            stream.Seek(0, SeekOrigin.Begin);

            return sb.ToString();
        }
    }
}