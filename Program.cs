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
        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
    .Cast<ManagementBaseObject>()
    .Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static readonly bool HasSse2 = IsProcessorFeaturePresent(10);
        private static readonly bool HasSse = IsProcessorFeaturePresent(6);
        private static readonly bool IsUnix = (int)Environment.OSVersion.Platform == 6;
        private static readonly bool IsAtLeastWinNt6 = Environment.OSVersion.Version.Major >= 6;
        private static readonly bool Isx64 = Environment.Is64BitProcess;
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

        // TODO: Shorten this or find another method
        private static readonly bool IsAvx2 = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Haswell")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Broadwell")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Skylake")) || new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Any(item => item["Name"].ToString().Contains("Cannonlake"));

        private static readonly string HighestSupportedInstruction = IsMultiCore
                ? (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsAvx2
                    ? "Avx2.dll"
                    : (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsProcessorFeaturePresent(17)
                        ? "Avx.dll"
                        : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Tbb.dll")))
                : (HasSse2 ? "Sse2St.dll" : (HasSse ? "SseSt.dll" : "TbbSt.dll"));

        private static readonly string Tbbmd5 = Path.GetFileNameWithoutExtension(HighestSupportedInstruction);

        private static Uri _tbbVersionUri;

        private static readonly Uri FinaltbbVersionUri = new Uri((Uri.TryCreate(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/"),
            HighestSupportedInstruction, out _tbbVersionUri) ? _tbbVersionUri : new Uri(string.Empty)).ToString());

        private static readonly Uri FlashUri =
            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll");

        private static readonly Uri AirUri =
    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll");

        private static readonly Version CgLatestVersion = new Version("3.1.13");

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static readonly string PmbUninstall = Path.Combine(Isx64
    ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
    : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
    "Pando Networks", "Media Booster", "uninst.exe");

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
            if (File.Exists(PmbUninstall))
            {
                Kill("PMB");
                Process.Start(new ProcessStartInfo { FileName = PmbUninstall, Arguments = "/silent" });
            }
            Kill(LoLProccessStrings);

            if (!Directory.Exists("Backup"))
            {
                Directory.CreateDirectory("Backup");
                if (Directory.Exists("RADS"))
                {
                    CopyToBak("solutions", "lol_game_client_sln", "cg.dll", Sln);
                    CopyToBak("solutions", "lol_game_client_sln", "cgD3D9.dll", Sln);
                    CopyToBak("solutions", "lol_game_client_sln", "cgGL.dll", Sln);
                    CopyToBak("solutions", "lol_game_client_sln", "tbb.dll", Sln);
                    CopyToBak("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), Air);
                    CopyToBak("projects", "lol_air_client", Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), Air);
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
                RemoveReadOnly(Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, string.Empty);

                Cfg("game.cfg", "Config", IsMultiCore);
                Download("tbb.dll", Tbbmd5, FinaltbbVersionUri, "solutions", "lol_game_client_sln", Sln);
                Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), AFlash, FlashUri, "projects", "lol_air_client", Air);
                Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AAir, AirUri, "projects", "lol_air_client", Air);
                Copy(string.Empty,
                    "cg.dll", "solutions", "lol_game_client_sln", Sln, string.Empty);
                Copy(string.Empty,
                    "cgD3D9.dll", "solutions", "lol_game_client_sln", Sln, string.Empty);
            }
            else
            {
                Cfg("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent_zh_MY.cfg",
                    Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);

                Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AFlash, FlashUri, string.Empty, string.Empty, string.Empty);
                Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AAir, AirUri, string.Empty, string.Empty, string.Empty);
                Download(Path.Combine("Game", "tbb.dll"), Tbbmd5, FinaltbbVersionUri, string.Empty, string.Empty, string.Empty);
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cg.dll"));
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cgGL.dll"));
                Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                    Path.Combine("Game", "cgD3D9.dll"));
            }
            Console.WriteLine("Done!");
            Console.ReadLine();
            if (File.Exists("lol_launcher.exe"))
            {
                Process.Start("lol_launcher.exe");
            }
            Environment.Exit(0);
        }

        private static void Download(string file, string md5, Uri uri, string path, string path1, string version)
        {
            using (WebClient webClient = new WebClient())
            {
                if (Directory.Exists("RADS"))
                {
                    RemoveReadOnly(path, path1, file, version);
                    if (File.Exists(DirPath(path, path1, version, file)))
                    {
                        if (Md5(DirPath(path, path1, version, file), md5))
                        {
                            webClient.DownloadFile(
                                uri,
                                DirPath(path, path1, version, file));
                        }
                    }
                    else
                    {
                        webClient.DownloadFile(
                            uri,
                            DirPath(path, path1, version, file));
                    }
                    Unblock(path, path1, file, version);
                }
                else
                {
                    RemoveReadOnly(file, string.Empty, string.Empty, string.Empty);
                    if (File.Exists(file))
                    {
                        if (Md5(file, md5))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    else
                    {
                        webClient.DownloadFile(uri, file);
                    }
                    Unblock(file, string.Empty, string.Empty, string.Empty);
                }
                if (!string.IsNullOrEmpty(_cgBinPath) && new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= CgLatestVersion)
                    return;
                webClient.DownloadFile(
                    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Cg-3.1_April2012_Setup.exe"),
                    "Cg-3.1_April2012_Setup.exe");
            }
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

        private static void Unblock(string file, string path, string path1, string ver)
        {
            if (File.Exists(DirPath(path, path1, ver, file)))
            {
                DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
            }
            if (!File.Exists(file)) return;
            DeleteFile(file + ":Zone.Identifier");
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
            if (!File.Exists(file) || !new FileInfo(file).Attributes
                .Equals(FileAttributes.ReadOnly)) return;
            File.SetAttributes(file,
                FileAttributes.Normal);
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
            if (!File.Exists(Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file))) return;
            File.Copy(
                Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file)
                , Path.Combine("Backup", file),
                true);
        }

        private static string DirPath(string folder, string folder1, string version, string file)
        {
            return Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file);
        }

        private static void Copy(string from, string file, string folder, string folder1, string version, string to)
        {
            if (Directory.Exists("RADS"))
            {
                RemoveReadOnly(folder, folder1, file, Sln);
                if (!File.Exists(Path.Combine(
                    _cgBinPath, file))) return;

                File.Copy(
                 Path.Combine(
                     _cgBinPath, file),
                 Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file), true);
                Unblock(folder, folder1, file, Sln);
            }
            else
            {
                RemoveReadOnly(file, string.Empty, string.Empty, string.Empty);
                if (!File.Exists(Path.Combine(@from, file)) || !Directory.Exists(to)) return;
                File.Copy(Path.Combine(@from, file),
                    Path.Combine(to, file), true);
                Unblock(file, string.Empty, string.Empty, string.Empty);
            }
        }

        private static void Cfg(string file, string path, bool mode)
        {
            if (!File.Exists(Path.Combine(path, file))) return;
            RemoveReadOnly(file, string.Empty, string.Empty, string.Empty);
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
            Unblock(file, string.Empty, string.Empty, string.Empty);
        }

        private static string Version(string folder, string folder1)
        {
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max()) : string.Empty;
        }

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern void DeleteFile(string file);

        private static int ToInt(string value)
        {
            int result;
            int.TryParse(value, out result);
            return result;
        }

        private static bool Md5(string file, string md5)
        {
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                foreach (byte b in MD5.Create().ComputeHash(fs))
                {
                    sb.Append(b.ToString("x2"));
                }

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t != Encoding.ASCII.GetBytes(md5)[i]).Any();
            }
        }
    }
}