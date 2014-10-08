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

namespace LoLUpdaterDll
{
    internal static class Program
    {
        public static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>()
.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        public static readonly string[] LoLUpdaterprocs = { "LoLUpdater Uninstall", "LoLUpdater" };
        public static readonly bool HasSse2 = IsProcessorFeaturePresent(10);
        public static readonly bool HasSse = IsProcessorFeaturePresent(6);
        public static readonly bool IsUnix = (int)Environment.OSVersion.Platform == 6;
        public static readonly bool IsAtLeastWinNt6 = Environment.OSVersion.Version.Major >= 6;
        public static readonly bool Isx64 = Environment.Is64BitProcess;
        public static readonly string SlnFolder = Version("solutions", "lol_game_client_sln");
        public static readonly string AirFolder = Version("projects", "lol_air_client");
        public static readonly string[] LoLProccess = { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" };
        public static readonly string[] Cpulist = { "Haswell", "Broadwell", "Skylake", "Cannonlake" };
        public static readonly bool IsAvx2 = CpuNameExist(Cpulist);

        public static Uri _tempUri;

        public static readonly Uri TbbUri = new Uri((Uri.TryCreate(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/"),
            IsMultiCore
                ? (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsAvx2
                    ? "Avx2.dll"
                    : (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsProcessorFeaturePresent(17)
                        ? "Avx.dll"
                        : (HasSse2 ? "Sse2.dll" : HasSse ? "Sse.dll" : "Tbb.dll")))
                : (HasSse2 ? "Sse2St.dll" : (HasSse ? "SseSt.dll" : "TbbSt.dll")), out _tempUri) ? _tempUri : new Uri(String.Empty)).ToString());

        public static readonly Uri FlashUri =
            new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/NPSWF32.dll");

        public static readonly Uri AirUri =
    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Adobe AIR.dll");

        public static readonly Version LatestCg = new Version("3.1.13");

        public static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);

        public static readonly string Pmb = Path.Combine(Isx64
    ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
    : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
    "Pando Networks", "Media Booster", "uninst.exe");

        private const string Air = "179a1fcfcb54e3e87365e77c719a723f";

        private const string Flash = "9700dbdebffe429e1715727a9f76317b";

        public static readonly string Tbb = IsMultiCore
                ? (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsAvx2
                    ? "db0767dc94a2d1a757c783f6c7994301"
                    : (Isx64 && (IsAtLeastWinNt6 || IsUnix) && IsProcessorFeaturePresent(17)
                        ? "2f178dadd7202b6a13a3409543a6fa86"
                        : (HasSse2 ? "1639aa390bfd02962c5c437d201045cc" : HasSse ? "3bf888228b83c4407d2eea6a5ab532bd" : "44dde7926b6dfef4686f2ddd19c04e2d")))
                : (HasSse2 ? "82ed3be353217c61ff13a01bc85f1395" : (HasSse ? "eacd37174f1a4316345f985dc456a961" : "b389f80072bc877a6ef5ff33ade88a64"));

        public static void Download(string file, string md5, Uri uri, string path, string path1, string version)
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
                    RemoveReadOnly(file, String.Empty, String.Empty, String.Empty);
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
                    Unblock(file, String.Empty, String.Empty, String.Empty);
                }
                if (!String.IsNullOrEmpty(_cgBinPath) && new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) >= LatestCg)
                    return;
                webClient.DownloadFile(
                    new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Resources/Cg-3.1_April2012_Setup.exe"),
                    "Cg-3.1_April2012_Setup.exe");
            }
            Unblock("Cg-3.1_April2012_Setup.exe", String.Empty, String.Empty, String.Empty);

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

        public static void Unblock(string file, string path, string path1, string ver)
        {
            if (File.Exists(DirPath(path, path1, ver, file)))
            {
                DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
            }
            if (!File.Exists(file)) return;
            DeleteFile(file + ":Zone.Identifier");
        }

        public static void CopyFromBak(string folder, string folder1, string file, string to, string version)
        {
            File.Copy(Path.Combine("Backup", file)

                , Path.Combine("RADS", folder, folder1, "releases", version, "deploy", to, file),
                true);
        }

        public static void RemoveReadOnly(string file, string path, string path1, string ver)
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

        public static void Kill(IEnumerable process)
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

        public static void CopyToBak(string folder, string folder1, string file, string version)
        {
            if (!File.Exists(Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file))) return;
            File.Copy(
                Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file)
                , Path.Combine("Backup", file),
                true);
        }

        public static string DirPath(string folder, string folder1, string version, string file)
        {
            return Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file);
        }

        public static void Copy(string from, string file, string folder, string folder1, string version, string to)
        {
            if (Directory.Exists("RADS"))
            {
                RemoveReadOnly(folder, folder1, file, SlnFolder);
                if (!File.Exists(Path.Combine(
                    _cgBinPath, file))) return;

                File.Copy(
                 Path.Combine(
                     _cgBinPath, file),
                 Path.Combine("RADS", folder, folder1, "releases", version, "deploy", file), true);
                Unblock(folder, folder1, file, SlnFolder);
            }
            else
            {
                RemoveReadOnly(file, String.Empty, String.Empty, String.Empty);
                if (!File.Exists(Path.Combine(@from, file)) || !Directory.Exists(to)) return;
                File.Copy(Path.Combine(@from, file),
                    Path.Combine(to, file), true);
                Unblock(file, String.Empty, String.Empty, String.Empty);
            }
        }

        public static void Cfg(string file, string path, bool mode)
        {
            if (!File.Exists(Path.Combine(path, file))) return;
            RemoveReadOnly(file, String.Empty, String.Empty, String.Empty);
            if (mode)
            {
                if (File.ReadAllText(Path.Combine(path, file))
                    .Contains("DefaultParticleMultiThreading=1")) return;
                File.AppendAllText(Path.Combine(path, file),
                    String.Format("{0}{1}", Environment.NewLine, "DefaultParticleMultiThreading=1"));
            }
            else
            {
                var oldLines = File.ReadAllLines(Path.Combine(path, file));
                if (!oldLines.Contains("DefaultParticleMultiThreading=1")) return;
                var newLines = oldLines.Select(line => new { Line = line, Words = line.Split(' ') }).Where(lineInfo => !lineInfo.Words.Contains("DefaultParticleMultiThreading=1")).Select(lineInfo => lineInfo.Line);
                File.WriteAllLines(Path.Combine(path, file), newLines);
            }
            Unblock(file, String.Empty, String.Empty, String.Empty);
        }

        public static string Version(string folder, string folder1)
        {
            return Directory.Exists("RADS") ? Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", folder, folder1, "releases")).Max()) : String.Empty;
        }

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string file);

        public static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        public static bool Md5(string file, string md5)
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

        public static bool CpuNameExist(IEnumerable str)
        {
            return new ManagementObjectSearcher("Select * from Win32_Processor").Get()
                .Cast<ManagementBaseObject>()
                .Any(item => item["Name"].ToString().Contains(str.ToString()));
        }
    }
}