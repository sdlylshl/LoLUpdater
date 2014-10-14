using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal class Methods : NativeMethods
    {
        public static readonly string AdobePath = Path.Combine(Isx64 ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles) : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86), "Common Files", "Adobe AIR", "Versions", "1.0");
        public static bool _notdone;
        public static readonly string Air = Version("projects", "lol_air_client");

        public static const string cgInstaller = "Cg-3.1_April2012_Setup.exe";

        public static readonly ManagementBaseObject[] CpuInfo = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
        .Cast<ManagementBaseObject>().ToArray();

        // Todo: combine cfgfiles string with "tbb.dll"
        public static const string[] files = { "Cg.dll", "CgGL.dll", "CgD3D9.dll", "tbb.dll" };

        // FeatureID == 2
        // Featuremask == 4
        // Should look something like this
        public static readonly bool HasAvx = IsProcessorFeaturePresent(17) & GetProcAddress(LoadLibrary(NativeMethods.s_kernel), "GetEnabledXStateFeatures") == 2;

        public static readonly bool HasSse = IsProcessorFeaturePresent(6);
        public static readonly bool HasSse2 = IsProcessorFeaturePresent(10);

        // Todo: Proper AVX2 check, this works atm though.
        public static readonly bool IsAvx2 = AvxCheck & CpuInfo.Any(item => item["Name"].ToString().Contains(string.Join(string.Empty, new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }));

        public static readonly bool IsLinuxorMono = (int)Environment.OSVersion.Platform == 4 || (int)Environment.OSVersion.Platform == 128;
        public static readonly bool IsRads = Directory.Exists("RADS");
        public static readonly bool Isx64 = Environment.Is64BitProcess;
        public static readonly string LoLProcc = string.Join(string.Empty, new string[] { "LoLClient", "LoLLauncher", "LoLPatcher", "League of Legends" });
        public static readonly string Sln = Version("solutions", "lol_game_client_sln");

        public static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
    EnvironmentVariableTarget.User);

        public static const string AirSha512 = "33f376d3f3a76a2ba122687b18e0306d45a8c65c89d3a51cc956bf4fa6d9bf9677493afa9b7bb5227fa1b162117440a5976484df6413f77a88ff3759ded37e8e";

        public static const string FlashSha512 = "e16c024424405ead77a89fabbb4a95a99e5552f33509d872bb7046cba4afb16f5a5bbf496a46b1b1ee9ef8b9e8ba6720bc8faccb654c5317e8142812e56b4930";

        public static bool IsSingle;

        public static Mutex mutex = new Mutex(true, "9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b", out IsSingle);

        public static readonly string TbbSha512 = IsAvx2
                   ? "13d78f0fa6b61a13e5b7cf8e4fa4b071fc880ae1356bd518960175fce7c49cba48460d6c43a6e28556be7309327abec7ec83760cf29b043ef1178904e1e98a07"
                   : (HasAvx
                       ? "d81edd17a891a2ef464f3e69ff715595f78c229867d8d6e6cc1819b426316a0bf6df5fa09a7341995290e4efe4b884f8d144e0fe8e519c4779f5cf5679db784c"
                       : (HasSse2 ? "61fea5603739cb3ca7a0f13b3f96e7c0c6bcff418d1404272c9fcf7cb5ce6fef7e21a5ee2026fc6af4ebc596d1d912e8201b298f7d092004d8f5256e22b05b64" : HasSse ? "fa1cc95eff4ca2638b88fcdb652a7ed19b4a086bab8ce4a7e7f29324e708b5c855574c5053fe3ea84917ca0293dc97bac8830d5be2770a86ca073791696fcbec" : "0c201b344e8bf0451717d6b15326d21fc91cc5981ce36717bf62013ff5624b35054e580a381efa286cc72b6fe0177499a252876d557295bc4e29a3ec92ebfa58"));

        public static void BakCopy(string file, string path, string path1, string ver, bool mode)
        {
            FileFix(path, path1, ver, file);
            if (mode & File.Exists(DirPath(path, path1,
                       ver, file)))
            {
                    File.Copy(
                      DirPath(path, path1,
                       ver, file)
                      , Path.Combine("Backup", file),
                      true);
            }
            if (!mode & File.Exists(Path.Combine("Backup", file)))
            {
                    File.Copy(Path.Combine("Backup", file)

                        , DirPath(path, path1,
                        ver, file),
                        true);
            }
            FileFix(path, path1, ver, file);
        }

        public static void BakCopy(string file, string path, string path1, string ver, string to, bool mode)
        {
            FileFix(path, path1, ver, file);
            if (mode & File.Exists(Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)))
            {
                File.Copy(
                    Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file)
                    , Path.Combine("Backup", file),
                    true);
            }
            if (!mode & File.Exists(Path.Combine("Backup", file)))
            {
                File.Copy(Path.Combine("Backup", file)

                    , Path.Combine("RADS", path, path1, "releases", ver, "deploy", to, file),
                    true);
            }
            FileFix(path, path1, ver, file);
        }

        public static void Cfg(string file, string path, bool mode)
        {
            FileFix(Path.Combine(path, file), string.Empty, string.Empty, string.Empty);
            if (File.Exists(Path.Combine(path, file)))
            {
                string text = File.ReadAllText(Path.Combine(path, file));
                text = Regex.Replace(text, "\nEnableParticleOptimization=[01]|$", string.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
                if (mode)
                {
                    text = Regex.Replace(text, "\nDefaultParticleMultiThreading=[01]|$", string.Format("{0}{1}", Environment.NewLine, "DefaultParticleMultiThreading=1"));
                }
                else
                {
                    if (text.Contains("DefaultParticleMultiThreading=1"))
                    {
                        text = text.Replace("DefaultParticleMultiThreading=1", "DefaultParticleMultiThreading=0");
                    }
                }
                File.WriteAllText(Path.Combine(path, file), text);
            }

            FileFix(Path.Combine(path, file), string.Empty, string.Empty, string.Empty);
        }

        public static void Copy(string from, string path, string path1, string ver, string file)
        {
            if (File.Exists(from))
            {
                FileFix(path, path1, Sln, file);
                File.Copy(from, DirPath(path, path1, ver, file), true);
                FileFix(path, path1, Sln, file);
            }
        }

        public static void Copy(string from, string file, string to, bool mode)
        {
            FileFix(Path.Combine(to, file), string.Empty, string.Empty, string.Empty);
            if (mode & File.Exists(Path.Combine(@from, file)))
            {
                File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
            }
            if (!mode & File.Exists(Path.Combine(to, file)))
            {
                File.Copy(Path.Combine(to, file), Path.Combine(@from, file), true);
            }
            FileFix(Path.Combine(to, file), string.Empty, string.Empty, string.Empty);
        }

        public static string DirPath(string path, string path1, string ver, string file)
        {
            return Path.Combine("RADS", path, path1, "releases", ver, "deploy", file);
        }

        static public int DisplayMenu()
        {
            Console.WriteLine("Menu");
            Console.WriteLine("1. Install");
            Console.WriteLine("2. Uninstall");
            Console.WriteLine("3. Exit");
            Console.WriteLine();
// Todo: add code to restrict options to 1-3
            return Convert.ToInt32(Console.ReadLine());
        }

        public static void Download(string file, string Sha512, Uri uri, string path, string path1, string ver)
        {
            using (WebClient webClient = new WebClient())
            {
                if (IsRads)
                {
                    FileFix(path, path1, ver, file);
                    if (!File.Exists(DirPath(path, path1, ver, file)))
                    {
                        webClient.DownloadFile(
                        uri,
                        DirPath(path, path1, ver, file));
                    }
                    else
                    {

                        if (Sha512Equal(DirPath(path, path1, ver, file), Sha512))
                        {
                            webClient.DownloadFile(
                            uri,
                            DirPath(path, path1, ver, file));
                        }
                    }
                    FileFix(path, path1, ver, file);
                }
                else
                {
                    FileFix(file, string.Empty, string.Empty, string.Empty);
                    if (!File.Exists(file))
                    {
                        webClient.DownloadFile(uri, file);
                    }
                    else
                    {
                        FileFix(file, string.Empty, string.Empty, string.Empty);
                        if (Sha512Equal(file, Sha512))
                        {
                            webClient.DownloadFile(uri, file);
                        }
                    }
                    FileFix(file, string.Empty, string.Empty, string.Empty);
                }
            }
        }

        public static void FileFix(string path, string path1, string ver, string file)
        {
            if (IsRads & File.Exists(DirPath(path, path1,
                     ver, file)))
            {
                if (new FileInfo(DirPath(path, path1,
                     ver, file)).Attributes
                     .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(DirPath(path, path1,
                        ver, file),
                        FileAttributes.Normal);
                }
                DeleteFile(DirPath(path, path1, ver, file) + ":Zone.Identifier");
            }
            if (!IsRads & File.Exists(file))
            {
                if (new FileInfo(file).Attributes
                 .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(file,
                      FileAttributes.Normal);
                }
                DeleteFile(file + ":Zone.Identifier");
            }
        }

        public static void FinishedPrompt(string message)
        {
            Console.Clear
            //Permanent checksums
            string check = string.Join(string.Empty, new[] {"ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22", "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2", "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b", TbbSha512});
            if (IsRads)
            {
                Sha512Check("projects", "lol_air_client", Air,
                    Path.Combine("Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), AirSha512);
                Sha512Check("projects", "lol_air_client", Air,
                    Path.Combine("Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512);
                Parallel.ForEach(files, file =>
                {
                    Sha512Check("solutions", "lol_game_client_sln", Sln,
            file, check);
                });
            }
            else
            {
                Sha512Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Resources", "NPSWF32.dll"), FlashSha512);
                Sha512Check(Path.Combine("Air", "Adobe AIR", "Versions", "1.0", "Adobe AIR.dll"), AirSha512);
                Parallel.ForEach(files, file =>
                {
                    Sha512Check(Path.Combine("Game", file), check);
                });
            }

            Console.WriteLine("{0}", message);
            if (File.Exists("lol.launcher.exe"))
            {
                Process.Start("lol.launcher.exe");
            }
            Console.ReadLine();
            Environment.Exit(0);
        }

        public static void Check()
        {
            using (WebClient wc = new WebClient())
            {
                if (new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(AdobePath, "Adobe AIR.dll")).FileVersion) < new Version("15.0.0.297"))
                {
                    Console.WriteLine("Doing prework...");
                    wc.DownloadFile(new Uri("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe"), "air15_win.exe");
                    FileFix("air15_win.exe", string.Empty, string.Empty, string.Empty);
                    Process Air = new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                            {
                                FileName =
                                    "air15_win.exe",
                                Arguments = "-silent"
                            }
                    };

                    Air.Start();
                    Air.WaitForExit();
                    File.Delete("air15_win.exe");
                }
                if (new Version(FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) < new Version("3.1.0.13"))
                {
                    if (new Version(
                    FileVersionInfo.GetVersionInfo(Path.Combine(AdobePath, "Adobe AIR.dll")).FileVersion) < new Version("15.0.0.297"))
                    {
                        Console.WriteLine("");
                        Console.WriteLine("Doing more prework...");
                    }
                    else { Console.WriteLine("Doing prework..."); }
                    wc.DownloadFile(
                    new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe"), cgInstaller);

                    FileFix(cgInstaller, string.Empty, string.Empty, string.Empty);

                    Process cg = new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                            {
                                FileName =
                                    cgInstaller,
                                Arguments = "/silent /TYPE=compact"
                            }
                    };
                    cg.Start();
                    cg.WaitForExit();
                    File.Delete(cgInstaller);
                    _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                        EnvironmentVariableTarget.User);
                }
            }
        }

        public static void Sha512Check(string path, string path1, string ver, string file, string Sha512)
        {
            Console.WriteLine(
                !Sha512Equal(DirPath(path, path1, ver, file), Sha512) ? "{0} Is the old patched file or the original" : "{0} Succesfully patched!",
                Path.GetFileNameWithoutExtension(file));
        }

        public static void Sha512Check(string file, string Sha512)
        {
            Console.WriteLine(
                !Sha512Equal(file, Sha512) ? "{0} Is the old patched file or the original" : "{0} Succesfully patched!",
                Path.GetFileNameWithoutExtension(file));
        }

        public static bool Sha512Equal(string file, string Sha512)
        {
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                Parallel.ForEach(SHA512.Create().ComputeHash(fs), b =>
                {
                    sb.Append(b.ToString("x2"));
                });

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t == Encoding.ASCII.GetBytes(Sha512)[i]).Any();
            }
        }

        public static void Start()
        {
            if (!IsSingle)
            { return; }
            GC.KeepAlive(mutex);
Kill();
        }

        public static void Kill()
        {
            do{
            Parallel.ForEach(Process.GetProcessesByName(LoLProcc), proc =>
            {
                proc.Kill();
                proc.WaitForExit();
            });
} while (_notdone);
        }
        
        public static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        public static string Version(string path, string path1)
        {
            if (Directory.Exists(Path.Combine("RADS", path, path1, "releases")))
            {
                string dir = Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).ToString();
                if (dir.Length == 1)
                {
                    return dir;

                }
                else
                {
                    return Path.GetFileName(Directory.GetDirectories(Path.Combine("RADS", path, path1, "releases")).Max());

                }
            }
            else
            { return string.Empty; }


        }
    }
}
