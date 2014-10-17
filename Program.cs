using Microsoft.CSharp;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace LoLUpdater
{
    internal static class Program
    {
        private const string Ipf = "IsProcessorFeaturePresent";
        private const string SKernel = "kernel32.dll";
        private const string Backup = "Backup";
        private const string Tbb = "tbb.dll";
        private const string RmBlock = ":Zone.Identifier";
        private const string CfgFile = "game.cfg";
        private const string Dpm1 = "DefaultParticleMultiThreading=1";
        private const string Air = "Adobe AIR.dll";
        private const string Res = "Resources";
        private const string Rads = "RADS";
        private const string Ver2 = "Versions";
        private const string Dep = "deploy";
        private const string Proj = "projects";
        private const string Sol = "solutions";
        private const string AirC = "lol_air_client";
        private const string GameCSln = "lol_game_client_sln";
        private const string Rel = "releases";
        private const string One = "1.0";
        private const string AAir = "Adobe AIR";
        private const string Gme = "Game";
        private static readonly bool Avx = Dll(17, Ipf) & Dll(2, "GetEnabledXStateFeatures");
        private static readonly ManagementBaseObject[] CpuInfo =
            new ManagementObjectSearcher("Select * from Win32_Processor").Get()
                .Cast<ManagementBaseObject>()
                .AsParallel()
                .ToArray();
        private static readonly bool X64 = Environment.Is64BitProcess;
        private static readonly string Pmb = Path.Combine(X64
    ? Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
    : Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
    "Pando Networks", "Media Booster", "uninst.exe");

        // List of processors that support AVX2, might become very long in the fure (find fix), some
        // on the list are not even out yet.
        private static readonly bool Avx2 =
            CpuInfo.Any(
                item =>
                    item["Name"].ToString()
                        .Contains(
                            new List<string>(new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }).AsParallel()
                                .ToString()));
        private static readonly bool Riot = Directory.Exists(Rads);
        private static readonly string[] AdobeFiles = { Path.Combine(Res, "NPSWF32.dll"), Air };

        private static readonly string[] GarenaCfgFiles =
        {
            CfgFile, "GamePermanent.cfg", "GamePermanent_zh_MY.cfg",
            "GamePermanent_en_SG.cfg"
        };
        private static readonly string[] CgFiles = { "Cg.dll", "CgGL.dll", "CgD3D9.dll" };
        private static readonly string[] GameFiles = CgFiles.Concat(new[] { Tbb }).ToArray();

        // Note: Checksums are in SHA512
        private static readonly bool Sse = Dll(6, Ipf);
        private static readonly bool Sse2 = Dll(10, Ipf);

        private static readonly string TbbSum = Avx2
            ? "13d78f0fa6b61a13e5b7cf8e4fa4b071fc880ae1356bd518960175fce7c49cba48460d6c43a6e28556be7309327abec7ec83760cf29b043ef1178904e1e98a07"
            : (Avx
                ? "d81edd17a891a2ef464f3e69ff715595f78c229867d8d6e6cc1819b426316a0bf6df5fa09a7341995290e4efe4b884f8d144e0fe8e519c4779f5cf5679db784c"
                : (Sse2
                    ? "61fea5603739cb3ca7a0f13b3f96e7c0c6bcff418d1404272c9fcf7cb5ce6fef7e21a5ee2026fc6af4ebc596d1d912e8201b298f7d092004d8f5256e22b05b64"
                    : Sse
                        ? "fa1cc95eff4ca2638b88fcdb652a7ed19b4a086bab8ce4a7e7f29324e708b5c855574c5053fe3ea84917ca0293dc97bac8830d5be2770a86ca073791696fcbec"
                        : "0c201b344e8bf0451717d6b15326d21fc91cc5981ce36717bf62013ff5624b35054e580a381efa286cc72b6fe0177499a252876d557295bc4e29a3ec92ebfa58"));

        private static readonly string Adobe = Riot
            ? Path.Combine(Rads, Proj, AirC, Rel, Ver(Proj, AirC), Dep, AAir, Ver2,
                One)
            : Path.Combine("Air", AAir, Ver2, One);
        private static readonly string Game = Riot
            ? Path.Combine(Rads, Sol, GameCSln, Rel, Ver(Sol, GameCSln), Dep)
            : Gme;
        private static readonly string Config = Riot
    ? "Config"
    : Path.Combine(Gme, "DATA", "CFG", "defaults");
        private static int _userInput;
        private static readonly bool Installing = Convert.ToBoolean(_userInput = 1);
        private static readonly Uri TbbUri =
            new Uri(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/"), Avx2
                ? "Avx2.dll"
                : (Avx
                    ? "Avx.dll"
                    : (Sse2 ? "Sse2.dll" : Sse ? "Sse.dll" : "Default.dll")));
        private static string _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
            EnvironmentVariableTarget.User);
        private static bool _notdone;
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool DllType(byte arg);
        private static void Main(string[] args)
        {
            Mutex onlyInstance = new Mutex(true,
            @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: 9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b");
            const string updater = "LoLUpdater Updater.exe";
            if (!onlyInstance.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(onlyInstance);
            using (
                Stream stream =
                    WebRequest.Create(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/SHA512.txt"))
                        .GetResponse()
                        .GetResponseStream())
            {
                if (stream == null) return;
                using (StreamReader streamReader = new StreamReader(stream))
                {
                    if (HashEqual("LoLUpdater.exe",
                        streamReader.ReadLine()))
                    {
                        using (
Stream stream2 =
WebRequest.Create(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/Temp.txt"))
.GetResponse()
.GetResponseStream())
                        {
                            if (stream2 == null) return;
                            using (CSharpCodeProvider cscp = new CSharpCodeProvider())
                            {
                                CompilerParameters parameters = new CompilerParameters
                                {
                                    GenerateInMemory = false,
                                    GenerateExecutable = true,
                                    IncludeDebugInformation = false,
                                    CompilerOptions = "/optimize",
                                    TempFiles = new TempFileCollection(".", false),
                                    OutputAssembly = "LoLUpdater Updater.exe"
                                };
                                parameters.ReferencedAssemblies.Add("System.dll");
                                parameters.ReferencedAssemblies.Add("System.Core.dll");

                                using (StreamReader streamReader2 = new StreamReader(stream2))
                                {
                                    CompilerResults result = cscp.CompileAssemblyFromSource(parameters, streamReader2.ReadToEnd());
                                    Normalize(string.Empty, result.PathToAssembly, true);
                                    Unblock(string.Empty, result.PathToAssembly, true);
                                    Process.Start(result.PathToAssembly);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (File.Exists(updater))
                        {
                            File.Delete(updater);
                        }
                    }
                }

            }

            if (args.Length > 0)
            {
                switch (args[0])
                {
                    case "/?":
                    case "-h":
                    case "--help":
                        Console.WriteLine(
                            string.Join("Command-Line Arguments:",
                                string.Empty,
                                "-install : Installs LoLUpdater",
                                "-uninst : Uninstalls LoLUpdater",
                                "--help /? -h : Shows this menu")
                        );
                        Console.ReadLine();
                        break;

                    case "-install":
                        _userInput = 1;
                        Patch();
                        break;

                    case "-uninst":
                        _userInput = 2;
                        Patch();
                        break;
                }
            }
            else
            {
                _userInput = DisplayMenu();
                Console.Clear();
                Patch();
            }
        }

        private static void Patch()
        {

            do
            {
                Parallel.ForEach(new[]{
            "LoLClient", "LoLLauncher", "LoLPatcher",
            "League of Legends"
        }
                , t =>
                {
                    Parallel.ForEach(
                        Process.GetProcesses()
                            .Where(
                                process =>
                                    string.Equals(process.ProcessName, t, StringComparison.CurrentCultureIgnoreCase))
                            .AsParallel(), process =>
                            {
                                process.Kill();
                                process.WaitForExit();
                            });
                });
            } while (_notdone);
            if (Installing)
            {
                if (File.Exists(Pmb))
                {
                    Process.Start(new ProcessStartInfo { FileName = Pmb, Arguments = "/silent" });
                }
                if (!Directory.Exists(Backup))
                {
                    Directory.CreateDirectory(Backup);
                }
                if (!Directory.Exists(Path.Combine(Backup, Res)))
                {
                    Directory.CreateDirectory(Path.Combine(Backup, Res));
                }
            }
            string adobePath =
Path.Combine(X64
? Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFilesX86)
: Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles), AAir,
Ver2, One);
            if (_userInput < 3)
            {
                Console.WriteLine("Configuring...");

                Parallel.ForEach(GameFiles,
    file =>
    {
        Copy(string.Empty, Game, file, string.Empty, Installing);

    });
                Parallel.ForEach(AdobeFiles,
file =>
{
    Copy(string.Empty, Adobe, file, string.Empty, Installing);

});
                if (Riot)
                {
                    Copy(string.Empty, Config, CfgFile, string.Empty, Installing);
                }
                else
                {
                    Parallel.ForEach(GarenaCfgFiles,
                        file => { Copy(string.Empty, Config, file, string.Empty, Installing); });
                }

                Console.WriteLine(string.Empty);

                if (Installing)
                {

                    if (File.Exists(Path.Combine(adobePath, Air)))
                    {
                        Process airUpdater = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        Path.Combine(adobePath, Res, "Adobe AIR Updater.exe"),
                                    Arguments = "-silent"
                                }
                        };
                        airUpdater.Start();
                        airUpdater.WaitForExit();
                    }
                    else
                    {
                        const string airInstaller = "air15_win.exe";
                        Download(airInstaller,
                            "fd85bd4455b070e8d5d71ff2187a8252d48f85b3b3f8bae53bb25e81e966682fff16a254f1193da8a2d8d73126f40143126cbec54f2f2c7c446680379ee23d14"
                            , new Uri(new Uri("https://labsdownload.adobe.com/pub/labs/flashruntimes/air/"),
                                airInstaller), string.Empty);

                        Normalize(string.Empty, airInstaller, true);
                        Unblock(string.Empty, airInstaller, true);
                        Process airwin = new Process
                        {
                            StartInfo =
                                new ProcessStartInfo
                                {
                                    FileName =
                                        airInstaller,
                                    Arguments = "-silent"
                                }
                        };
                        airwin.Start();
                        airwin.WaitForExit();
                    }

                    if (string.IsNullOrEmpty(_cgBinPath))
                    {
                        CgInstall();
                    }
                    else
                    {
                        if (
                            new Version(FileVersionInfo.GetVersionInfo(Path.Combine(_cgBinPath, "cg.dll")).FileVersion) <
                            new Version("3.1.0.13"))
                        {
                            CgInstall();
                        }
                    }
                }
                else
                {
                    return;
                }
            }
            switch (_userInput)
            {
                case 1:
                    Console.WriteLine("Installing...");
                    Parallel.ForEach(CgFiles, file =>
                    {
                        Copy(_cgBinPath, string.Empty, file, Game, null);

                    });
                    Parallel.ForEach(AdobeFiles, file =>
                    {
                        Copy(adobePath, string.Empty, file, Adobe, null);

                    });

                    if (!Riot)
                    {
                        Parallel.ForEach(GarenaCfgFiles, Cfg);
                    }

                    Download(Tbb, TbbSum, TbbUri, Game);
                    Cfg(CfgFile);
                    FinishedPrompt("Done Installing!");
                    break;

                case 2:
                    Parallel.ForEach(GameFiles,
                                    file =>
                                    {
                                        if (!File.Exists(Path.Combine(Backup, file))) return;
                                        File.Delete(Path.Combine(Backup, file));
                                    });
                    Parallel.ForEach(AdobeFiles,
                file =>
                {
                    if (!File.Exists(Path.Combine(Backup, file))) return;
                    File.Delete(Path.Combine(Backup, file));
                });
                    if (!Riot)
                    {
                        Parallel.ForEach(GarenaCfgFiles,
                            file =>
                            {
                                if (!File.Exists(Path.Combine(Backup, file))) return;
                                File.Delete(Path.Combine(Backup, file));
                            });
                    }
                    if (File.Exists(Path.Combine(Backup, CfgFile)))
                    { File.Delete(Path.Combine(Backup, CfgFile)); }

                    FinishedPrompt("Done Uninstalling!");
                    break;

                case 3:
                    Environment.Exit(0);
                    break;
            }
        }

        private static void CgInstall()
        {
            const string cgInstaller = "Cg-3.1_April2012_Setup.exe";

            Download(cgInstaller,
                "066792a95eaa99a3dde3a10877a4bcd201834223eeee2b05b274f04112e55123df50478680984c5882a27eb2137e4833ed4f3468127d81bc8451f033bba75114",
                new Uri(new Uri("http://developer.download.nvidia.com/cg/Cg_3.1/"),
                    cgInstaller), string.Empty);

            Normalize(string.Empty, cgInstaller, true);
            Unblock(string.Empty, cgInstaller, true);

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

            _cgBinPath = Environment.GetEnvironmentVariable("CG_BIN_PATH",
                EnvironmentVariableTarget.User);
            File.Delete(cgInstaller);
        }

        private static void ByteDl(Stream stream, string path)
        {

            using (MemoryStream memoryStream = new MemoryStream())
            {
                byte[] buffer = new byte[4096];
                int count;
                do
                {
                    count = stream.Read(buffer, 0, buffer.Length);
                    memoryStream.Write(buffer, 0, count);
                } while (count != 0);
                File.WriteAllBytes(path, memoryStream.ToArray());
            }
        }
        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern void DeleteFile(string file);

        private static bool Dll(byte arg, string func)
        {
            bool ok = false;
            IntPtr pDll = LoadLibrary(SKernel);
            if (pDll == IntPtr.Zero) return false;
            IntPtr pFunc = GetProcAddress(pDll, func);
            if (pFunc != IntPtr.Zero)
            {
                DllType bar = (DllType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllType));
                ok = bar(arg);
            }
            FreeLibrary(pDll);
            return ok;
        }

        private static void Copy(string from, string path, string file, string to, bool? mode)
        {
            if (mode.HasValue)
            {
                if (mode.Value)
                {
                    if (path.Equals(Game))
                    {
                        if (File.Exists(Path.Combine(Game, file)))
                        {
                            Normalize(Game, file, false);
                            File.Copy(Path.Combine(
                                Game, file)
                                , Path.Combine(Backup, file),
                                true);
                            Unblock(Backup, file, false);
                        }
                    }
                    if (path.Contains(Adobe))
                    {
                        if (!File.Exists(Path.Combine(Adobe, file))) return;
                        Normalize(Adobe, file, false);
                        File.Copy(Path.Combine(
                            Adobe, file),
                            Path.Combine(Backup, file),
                            true);
                        Unblock(Backup, file, false);
                    }
                    if (!path.Equals(Config)) return;
                    if (!File.Exists(Path.Combine(Config, file))) return;
                    Normalize(Config, file, false);
                    File.Copy(Path.Combine(
                        Config, file),
                        Path.Combine(Backup, file),
                        true);
                    Unblock(Backup, file, false);
                }
                else
                {
                    if (!File.Exists(Path.Combine(Backup, file))) return;
                    if (path.Equals(Game))
                    {
                        Normalize(Backup, file, false);
                        File.Copy(
                            Path.Combine(Backup, file), Path.Combine(
                                Game, file),
                            true);
                        Unblock(Game, file, false);
                    }
                    if (path.Contains(Adobe))
                    {
                        Normalize(Backup, file, false);
                        File.Copy(Path.Combine(Backup, file),
                            Path.Combine(
                                Adobe, file),
                            true);
                        Unblock(Adobe, file, false);
                    }
                    if (!path.Equals(Config)) return;
                    Normalize(Backup, file, false);
                    File.Copy(Path.Combine(Backup, file),
                        Path.Combine(
                            Config, file),
                        true);
                    Unblock(Config, file, false);
                }
            }

            else
            {
                if (!(File.Exists(Path.Combine(@from, file)) & Directory.Exists(to))) return;
                Normalize(string.Empty, Path.Combine(@from, file), false);
                File.Copy(Path.Combine(@from, file), Path.Combine(to, file), true);
                Unblock(string.Empty, Path.Combine(to, file), false);
            }
        }

        private static void Cfg(string file)
        {
            bool multiCore =
            CpuInfo.AsParallel().Sum(item => Convert.ToInt32(item["NumberOfCores"].ToString())) > 1;
            if (!File.Exists(Path.Combine(Config, file))) return;
            Normalize(string.Empty, Path.Combine(Config, file), false);
            string text = File.ReadAllText(Path.Combine(Config, file));
            text = Regex.Replace(text, "\nEnableParticleOptimization=[01]|$",
                string.Format("{0}{1}", Environment.NewLine, "EnableParticleOptimization=1"));
            if (multiCore)
            {
                text = Regex.Replace(text, "\nDefaultParticleMultiThreading=[01]|$",
                    string.Format("{0}{1}", Environment.NewLine, Dpm1));
            }
            else
            {
                if (!text.Contains(Dpm1)) return;
                text = text.Replace(Dpm1, "DefaultParticleMultiThreading=0");
            }
            File.WriteAllText(Path.Combine(Config, file), text);
            Unblock(string.Empty, Path.Combine(Config, file), false);
        }

        private static int DisplayMenu()
        {
            int num = 0;
            Console.WriteLine(
                String.Join(Environment.NewLine,
                    "For a list of Command-Line Arguments, start lolupdater with --help", string.Empty,
                    "Select method:",
                    string.Empty,
                    "1. Install",
                    "2. Uninstall",
                    "4. Exit")
            );
            var readLine = Console.ReadLine();
            if (readLine == null) return num;
            string result = readLine.Trim();
            while (!int.TryParse(result, out num) && num < 1 && num > 3)
            {
                Console.WriteLine("{0} is not a valid input. Please try again.", result);
                result = readLine.Trim();
            }
            return num;
        }

        private static void Download(string file, string sha512, Uri uri, string path)
        {
            using (Stream stream = WebRequest.Create(uri)
    .GetResponse()
    .GetResponseStream())
            {
                if (string.IsNullOrEmpty(path))
                {
                    if (!File.Exists(file))
                    {
                        ByteDl(stream, file);
                    }
                    else
                    {
                        Normalize(string.Empty, file, false);
                        if (HashEqual(file, sha512)) return;
                        ByteDl(stream, file);
                    }
                    Unblock(string.Empty, file, false);
                }
                else
                {
                    if (!File.Exists(Path.Combine(Game, Tbb)))
                    {
                        ByteDl(stream, Path.Combine(Game, Tbb));
                    }
                    else
                    {
                        Normalize(Game, Tbb, false);
                        if (HashEqual(Path.Combine(Game, Tbb), TbbSum)) return;
                        ByteDl(stream, Path.Combine(Game, Tbb));
                    }
                    Unblock(Game, Tbb, false);
                }
            }
        }

        private static void FinishedPrompt(string message)
        {
            string permanentSum = string.Join(string.Empty,
                "ba3d17fc13894ee301bc11692d57222a21a9d9bbc060fb079741926fb10c9b1f5a4409b59dbf63f6a90a2f7aed245d52ead62ee9c6f8942732b405d4dfc13a22",
                "db7dd6d8b86732744807463081f408356f3031277f551c93d34b3bab3dbbd7f9bca8c03bf9533e94c6282c5fa68fa1f5066d56d9c47810d5ebbe7cee0df64db2",
                "cad3b5bc15349fb7a71205e7da5596a0cb53cd14ae2112e84f9a5bd844714b9e7b06e56b5938d303e5f7ab077cfa79f450f9f293de09563537125882d2094a2b",
                TbbSum);
            // flash, air
            string airSum = string.Join(string.Empty,
           "e16c024424405ead77a89fabbb4a95a99e5552f33509d872bb7046cba4afb16f5a5bbf496a46b1b1ee9ef8b9e8ba6720bc8faccb654c5317e8142812e56b4930",
           "33f376d3f3a76a2ba122687b18e0306d45a8c65c89d3a51cc956bf4fa6d9bf9677493afa9b7bb5227fa1b162117440a5976484df6413f77a88ff3759ded37e8e"
           );
            Parallel.ForEach(AdobeFiles, file =>
            {
                Verify(Adobe, file, airSum);
            });
            Parallel.ForEach(GameFiles, file =>
            {
                Verify(Game, file, permanentSum);

            });
            if (!Riot)
            {
                Parallel.ForEach(GarenaCfgFiles, file =>
                {
                    if (!File.Exists(file)) return;
                    string text = File.ReadAllText(Path.Combine(Config, file));

                    CfgVerify(text, file);
                });
            }

            if (File.Exists(Path.Combine(Config, CfgFile)))
            {
                string text2 = File.ReadAllText(Path.Combine(Config, CfgFile));

                CfgVerify(text2, CfgFile);
            }

            Console.WriteLine("{0}", message);
            if (Riot)
            {
                Process.Start("lol.launcher.exe");
            }
            _notdone = false;
            Console.ReadLine();
            Environment.Exit(0);
        }

        private static void CfgVerify(string text, string file)
        {
            Console.WriteLine(text.Contains(Dpm1)
                ? string.Format("DefaultParticleMultiThreading is Enabled in {0}", Path.GetFileNameWithoutExtension(file))
                : string.Format("DefaultParticleMultiThreading is Disabled in {0}", Path.GetFileNameWithoutExtension(file)));
            Console.WriteLine(string.Empty);
            Console.WriteLine(text.Contains("EnableParticleOptimization=1")
                ? string.Format("EnableParticleOptimization is Enabled in {0}", Path.GetFileNameWithoutExtension(file))
                : string.Format("EnableParticleOptimization is Disabled in {0}", Path.GetFileNameWithoutExtension(file)));
        }

        private static void Normalize(string path, string file, bool exe)
        {
            if (path.Contains(Adobe))
            {
                // Check+apply Or Bruteforce?
                if (new FileInfo(Path.Combine(Adobe, file)).Attributes
                    .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine(Adobe, file),
                        FileAttributes.Normal);
                }
            }
            else
            {
                if (new FileInfo(Path.Combine(Game, file)).Attributes
                    .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes(Path.Combine(Game, file),
                        FileAttributes.Normal);
                }
            }

            if (!exe) return;
            if (!new FileInfo(file).Attributes
                .Equals(FileAttributes.ReadOnly)) return;
            File.SetAttributes(file,
                FileAttributes.Normal);
        }

        private static void Unblock(string path, string file, bool exe)
        {
            DeleteFile(path.Contains(Adobe)
                ? string.Format("{0}{1}", Path.Combine(Adobe, file), RmBlock)
                : string.Format("{0}{1}", Path.Combine(Game, file), RmBlock));
            if (exe)
            {
                DeleteFile(string.Format("{0}{1}", path, RmBlock));
            }
        }

        private static bool HashEqual(string file, string sha512)
        {
            using (FileStream fileStream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                fileStream.Seek(0, SeekOrigin.Begin);
                return
                    BitConverter.ToString(SHA512.Create().ComputeHash(fileStream))
                        .Replace("-", string.Empty)
                        .Equals(sha512);
            }
        }

        private static string Ver(string path, string path1)
        {
            if (!Directory.Exists(Path.Combine(Rads, path, path1, Rel))) return string.Empty;
            string dir = Directory.GetDirectories(Path.Combine(Rads, path, path1, Rel)).ToString();
            return dir.Length == 1
                ? dir
                : Path.GetFileName(Directory.GetDirectories(Path.Combine(Rads, path, path1, Rel)).Max());
        }

        private static void Verify(string path, string file, string sha512)
        {
            Console.WriteLine(
               HashEqual(Path.Combine(path, file), sha512)
                   ? "{0} Is the old patched file or the original"
                   : "{0} Succesfully patched!",
               Path.GetFileNameWithoutExtension(Path.Combine(path, file)));

        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibrary(String dllName);
    }
}