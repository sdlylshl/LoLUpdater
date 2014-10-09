using System;
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

        // Some of these processors are not even out, included them to skip to have to do future updates.
        private static readonly string[] Cpulist = { "Haswell", "Broadwell", "Skylake", "Cannonlake" };

        private static readonly bool IsAvx2 = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
        .Cast<ManagementBaseObject>()
        .Any(item => item["Name"].ToString().Contains(Cpulist.ToString()));

        private static int _userInput;
        private static readonly bool IsInstalling = _userInput == 1;
        private static bool _notdone;
        private static readonly bool HasSse2 = IsProcessorFeaturePresent(10);
        private static readonly bool HasSse = IsProcessorFeaturePresent(6);
        private static readonly bool IsUnix = (int)Environment.OSVersion.Platform == 6;
        private static readonly bool IsAtLeastWinNt6 = Environment.OSVersion.Version.Major >= 6;
        private static readonly bool Isx64 = Environment.Is64BitProcess;
        private static readonly string SlnFolder = Version("solutions", "lol_game_client_sln");
        private static readonly string AirFolder = Version("projects", "lol_air_client");
        private static readonly string[] LoLProccess = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends", "PMB" };
        private const string Tweak = "DefaultParticleMultiThreading=1";

        private static Uri _tempUri;

        private static readonly Uri TbbUri = new Uri((Uri.TryCreate(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/"),
            IsMultiCore
                ? (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsAvx2
                    ? "Avx2.dll"
                    : (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsProcessorFeaturePresent(17)
                        ? "Avx.dll"
                        : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Tbb.dll")))
                : (HasSse2 ? "Sse2St.dll" : (HasSse ? "SseSt.dll" : "TbbSt.dll")), out _tempUri) ? _tempUri : new Uri(String.Empty)).ToString());

        private static readonly Uri FlashUri =
            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll");

        private static readonly Uri AirUri =
    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll");

        private static readonly Version LatestCg = new Version("3.1.13");

        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        private static readonly string Pmb = Path.Combine(Isx64
    ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
    : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
    "Pando Networks", "Media Booster", "uninst.exe");

        private const string AirMd5 = "179a1fcfcb54e3e87365e77c719a723f";

        private const string FlashMd5 = "9700dbdebffe429e1715727a9f76317b";
        private const string CgMd5 = "ae87223e882670029450b3f86e8e9300";
        private const string CgGlMd5 = "68dbb8778903f5cf0a80c00ffbf494d2";
        private const string CgD3D9Md5 = "9981b512f27b566d811b53590f6ee526";

        private static readonly string TbbMd5 = IsMultiCore
                ? (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsAvx2
                    ? "db0767dc94a2d1a757c783f6c7994301"
                    : (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsProcessorFeaturePresent(17)
                        ? "2f178dadd7202b6a13a3409543a6fa86"
                        : (HasSse2 ? "1639aa390bfd02962c5c437d201045cc" : HasSse ? "3bf888228b83c4407d2eea6a5ab532bd" : "44dde7926b6dfef4686f2ddd19c04e2d")))
                : (HasSse2 ? "82ed3be353217c61ff13a01bc85f1395" : (HasSse ? "eacd37174f1a4316345f985dc456a961" : "b389f80072bc877a6ef5ff33ade88a64"));

        private static void Main()
        {
            _userInput = DisplayMenu();
            Console.Clear();
            do
            {
                if (IsMultiCore)
                {
                    Parallel.ForEach(Process.GetProcessesByName(LoLProccess.ToString()), proc =>
                    {
                        proc.Kill();
                        proc.WaitForExit();
                    });
                }
                if (IsMultiCore) continue;
                foreach (Process proc in Process.GetProcessesByName(LoLProccess.ToString()))
                {
                    proc.Kill();
                    proc.WaitForExit();
                }
            } while (_notdone);
            if (_userInput == 2)
            {
                Console.WriteLine("Uninstalling, please wait...");
                if (Directory.Exists("RADS"))
                {
                    BakCopy("Cg.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("CgD3D9.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("CgGL.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("tbb.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("Adobe AIR.dll", "projects", "lol_air_client"
                        , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0"), IsInstalling);
                    BakCopy("NPSWF32.dll", "projects", "lol_air_client"
                        , AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), IsInstalling);
                }
                else
                {
                    Copy("Game", "cg.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "cgGL.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "cgD3D9.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "tbb.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "cg.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", string.Empty, string.Empty, string.Empty, "Backup");
                }
                Directory.Delete("Backup", true);
                FinishedPrompt("Done Uninstalling!");
            }
            else
            {
                Console.WriteLine("Installing, please wait...");
                if (File.Exists(Pmb))
                {
                    Process.Start(new ProcessStartInfo { FileName = Pmb, Arguments = "/silent" });
                }
                if (File.Exists("LoLUpdater Updater.exe"))
                {
                    FileFix("LoLUpdater Updater.exe", string.Empty, string.Empty, string.Empty, false);
                }
                if (!Directory.Exists("Backup"))
                {
                    Directory.CreateDirectory("Backup");
                }
                if (Directory.Exists("RADS"))
                {
                    BakCopy("solutions", "lol_game_client_sln", "cg.dll", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("solutions", "lol_game_client_sln", "cgD3D9.dll", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("solutions", "lol_game_client_sln", "cgGL.dll", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("solutions", "lol_game_client_sln", "tbb.dll", SlnFolder, string.Empty, IsInstalling);
                    BakCopy("projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), string.Empty, IsInstalling);
                    BakCopy("projects", "lol_air_client", AirFolder, Path.Combine("Adobe Air", "Versions", "1.0", "Resources"), string.Empty, IsInstalling);
                    BakCopy(string.Empty, string.Empty, Path.Combine("Config", "game.cfg"), string.Empty, string.Empty, IsInstalling);
                    Cfg("game.cfg", "Config", IsMultiCore);
                    Download("tbb.dll", TbbMd5, TbbUri, "solutions", "lol_game_client_sln", SlnFolder);
                    Download(Path.Combine("Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, "projects", "lol_air_client", AirFolder);
                    Download(Path.Combine("Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, "projects", "lol_air_client", AirFolder);
                    Copy(string.Empty,
                        "cg.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty);
                    Copy(string.Empty,
                        "cgD3D9.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty);
                    Copy(string.Empty,
    "cgGL.dll", "solutions", "lol_game_client_sln", SlnFolder, string.Empty);
                }
                else
                {
                    Copy("Game", "cg.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "cgGL.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "cgD3D9.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy("Game", "tbb.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0"), "Adobe AIR.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources"), "NPSWF32.dll", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), "game.cfg", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent.cfg", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_zh_MY.cfg", string.Empty, string.Empty, string.Empty, "Backup");
                    Copy(Path.Combine("Game", "DATA", "CFG", "defaults"), "GamePermanent_en_SG.cfg", string.Empty, string.Empty, string.Empty, "Backup");
                    Cfg("game.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                    Cfg("GamePermanent.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                    Cfg("GamePermanent_zh_MY.cfg",
                        Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);
                    Cfg("GamePermanent_en_SG.cfg", Path.Combine("Game", "DATA", "CFG", "defaults"), IsMultiCore);

                    Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5, FlashUri, string.Empty, string.Empty, string.Empty);
                    Download(Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5, AirUri, string.Empty, string.Empty, string.Empty);
                    Download(Path.Combine("Game", "tbb.dll"), TbbMd5, TbbUri, string.Empty, string.Empty, string.Empty);
                    Copy(Path.Combine(_cgBinPath, "cg.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                        Path.Combine("Game", "cg.dll"));
                    Copy(Path.Combine(_cgBinPath, "cgGL.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                        Path.Combine("Game", "cgGL.dll"));
                    Copy(Path.Combine(_cgBinPath, "cgD3D9.dll"), string.Empty, string.Empty, string.Empty, string.Empty,
                        Path.Combine("Game", "cgD3D9.dll"));
                }
                FinishedPrompt("Done Installing!");
            }
        }

        private static void FinishedPrompt(string message)
        {
            Console.Clear();
            Md5Check("projects", "lol_air_client", AirFolder,
                  Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Adobe AIR.dll"), AirMd5);
            Md5Check("projects", "lol_air_client", AirFolder,
    Path.Combine("Air", "Adobe Air", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashMd5);
            if (Directory.Exists("RADS"))
            {
                Md5Check("solutions", "lol_game_client_sln", SlnFolder,
        "cg.dll", CgMd5);
                Md5Check("solutions", "lol_game_client_sln", SlnFolder,
    "cgGL.dll", CgGlMd5);
                Md5Check("solutions", "lol_game_client_sln", SlnFolder,
    "cgD3D9.dll", CgD3D9Md5);
                Md5Check("solutions", "lol_game_client_sln", SlnFolder,
    "tbb.dll", TbbMd5);
            }
            else
            {
                Md5Check(string.Empty, string.Empty, string.Empty, Path.Combine("Game", "cg.dll"), CgMd5);
                Md5Check(string.Empty, string.Empty, string.Empty, Path.Combine("Game", "cgGL.dll"), CgMd5);
                Md5Check(string.Empty, string.Empty, string.Empty, Path.Combine("Game", "cgD3D9.dll"), CgMd5);
                Md5Check(string.Empty, string.Empty, string.Empty, Path.Combine("Game", "tbb.dll"), TbbMd5);
            }

            Console.WriteLine("{0}", message);
            if (File.Exists("lol_launcher.exe"))
            {
                Process.Start("lol_launcher.exe");
            }
            _notdone = false;
            Console.ReadLine();
            Environment.Exit(0);
        }

        // Todo: add MD5 checksums for original files.
        private static void Md5Check(string path, string path1, string ver, string file, string md5)
        {
            if (Directory.Exists("RADS"))
            {
                Console.WriteLine(
                    Md5(DirPath(path, path1, ver, file), md5) ? "{0} Is the latest patched version" : "{0} Is an old patched version or the original",
                    file);
            }
            else
            {
                Console.WriteLine(
                    Md5(file, md5) ? "{0} Is the latest patched version" : "{0} Is an old patched version or the original",
                    file);
            }
        }

        static private int DisplayMenu()
        {
            Console.WriteLine("LoLUpdater");
            Console.WriteLine();
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            return Convert.ToInt32(Console.ReadLine());
        }

        private static void Download(string file, string md5, Uri uri, string path, string path1, string version)
        {
            using (WebClient webClient = new WebClient())
            {
                if (Directory.Exists("RADS"))
                {
                    FileFix(path, path1, file, version, true);
                    if (!File.Exists(DirPath(path, path1, version, file)))
                    {
                        webClient.DownloadFile(
                            uri,
                            DirPath(path, path1, version, file));
                    }
                    else
                    {
                        if (Md5(DirPath(path, path1, version, file), md5))
                        {
                            webClient.DownloadFile(
                                uri,
                                DirPath(path, path1, version, file));
                        }
                    }
                    FileFix(path, path1, file, version, false);
                }
                else
                {
                    FileFix(file, String.Empty, String.Empty, String.Empty, true);
                    if (!File.Exists(file))
                    {
                        webClient.DownloadFile(uri, file);
                    }
                    else
                    {
                        if (Md5(file, md5))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    FileFix(file, String.Empty, String.Empty, String.Empty, false);
                }
                // Check already exists for the latest cg, but a double check might not hurt.
                if (!String.IsNullOrEmpty(_cgBinPath) && new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= LatestCg) return;
                webClient.DownloadFile(
                    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Cg-3.1_April2012_Setup.exe"),
                    "Cg-3.1_April2012_Setup.exe");

                FileFix("Cg-3.1_April2012_Setup.exe", String.Empty, String.Empty, String.Empty, true);
                FileFix("Cg-3.1_April2012_Setup.exe", String.Empty, String.Empty, String.Empty, false);

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
        }

        private static void BakCopy(string file, string path, string path1, string ver, string to, bool mode)
        {
            FileFix(file, path, path1, ver, true);
            if (mode)
            {
                if (!File.Exists(Path.Combine("RADS", path, path1, "releases", ver, "deploy", file))) return;
                File.Copy(
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
            else
            {
                if (!File.Exists(Path.Combine("Backup", file))) return;
                File.Copy(Path.Combine("Backup", file)

                    , Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file),
                    true);
            }
            FileFix(file, path, path1, ver, false);
        }

        private static void FileFix(string file, string path, string path1, string ver, bool mode)
        {
            if (mode)
            {
                if (File.Exists(DirPath(path, path1,
                   ver, file)) &&
                   new FileInfo(DirPath(path, path1,
                       file, ver)).Attributes
                       .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(DirPath(path, path1,
                        ver, file),
                        FileAttributes.Normal);
                }
                if (!File.Exists(file) || !new FileInfo(file).Attributes
                    .Equals(FileAttributes.ReadOnly)) return;
                File.SetAttributes(file,
                    FileAttributes.Normal);
            }
            else
            {
                if (File.Exists(DirPath(path, path1, ver, file)))
                {
                    DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
                }
                if (!File.Exists(file)) return;
                DeleteFile(file + ":Zone.Identifier");
            }
        }

        private static string DirPath(string path, string path1, string ver, string file)
        {
            return Path.Combine("RADS", path, path1, "releases", ver, "deploy", file);
        }

        private static void Copy(string from, string file, string path, string path1, string ver, string to)
        {
            if (Directory.Exists("RADS"))
            {
                FileFix(path, path1, file, SlnFolder, true);
                if (!File.Exists(Path.Combine(
                    _cgBinPath, file))) return;
                File.Copy(
                    Path.Combine(
                        _cgBinPath, file),
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", file), false);
                FileFix(path, path1, file, SlnFolder, false);
            }
            else
            {
                FileFix(Path.Combine(@from, file), String.Empty, String.Empty, String.Empty, true);
                if (!File.Exists(Path.Combine(@from, file)) && Directory.Exists(to)) return;
                File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
                FileFix(Path.Combine(to, file), String.Empty, String.Empty, String.Empty, false);
            }
        }

        private static void Cfg(string file, string path, bool mode)
        {
            if (!File.Exists(Path.Combine(path, file))) return;
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty, true);
            if (mode)
            {
                if (File.ReadAllText(Path.Combine(path, file))
                    .Contains(Tweak)) return;
                File.AppendAllText(Path.Combine(path, file),
                    String.Format("{0}{1}", Environment.NewLine, Tweak));
            }
            else
            {
                var oldLines = File.ReadAllLines(Path.Combine(path, file));
                if (!oldLines.Contains(Tweak)) return;
                var newLines = oldLines.Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains(Tweak)).Select(lineInfo => lineInfo.Line);
                File.WriteAllLines(Path.Combine(path, file), newLines);
            }
            FileFix(Path.Combine(path, file), String.Empty, String.Empty, String.Empty, false);
        }

        private static string Version(string path, string path1)
        {
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).Max()) : String.Empty;
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
            Int32.TryParse(value, out result);
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

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t == Encoding.ASCII.GetBytes(md5)[i]).Any();
            }
        }
    }
}