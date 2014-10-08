using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace LoLUpdater_Updater
{
    internal static class Program
    {
        private static bool _notdone;

        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>()
.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static void Main()
        {
            do
            {
                if (IsMultiCore)
                {
                    Parallel.ForEach(Process.GetProcessesByName("LoLUpdater"), proc =>
                    {
                        proc.Kill();
                        proc.WaitForExit();
                    });
                }
                if (IsMultiCore) continue;
                foreach (Process proc in Process.GetProcessesByName("LoLUpdater"))
                {
                    proc.Kill();
                    proc.WaitForExit();
                }
            } while (_notdone);

            using (WebClient webClient = new WebClient())
            {
                if (!File.Exists("LoLUpdater.exe"))
                {
                    try
                    {
                        Console.WriteLine("LoLUpdater not found, downloading...");
                        webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                        if (File.Exists("LoLUpdater.exe")) return;
                        DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
                        Process.Start("LoLUpdater.exe");
                        _notdone = false;
                        Environment.Exit(0);
                    }
                    catch (Exception)
                    {
                        Console.WriteLine("Critical error occured!");
                        Console.ReadLine();
                        Environment.Exit(0);
                    }
                }
                else
                {
                    try
                    {
                        using (MemoryStream stream = new MemoryStream(webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt")))
                        {
                            webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt");
                            stream.Position = 0;
                            var sr = new StreamReader(stream);

                            var current = new Version(FileVersionInfo.GetVersionInfo("LoLUpdater.exe").FileVersion);
                            var latest = new Version(sr.ReadToEnd());
                            if (current > latest)
                            {
                                Console.WriteLine(
                                    "You are using a newer version then the latest one, please report this bug at www.lolupdater.com");
                                Console.ReadLine();
                            }
                            if (current == latest)
                            {
                                Console.WriteLine("LoLUpdater up to date!");
                                Console.ReadLine();
                            }
                            if (current < latest)
                            {
                                Console.WriteLine("LoLUpdater has an update!, downloading...");
                                webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"),
                                    "LoLUpdater.exe");
                            }
                            if (File.Exists("LoLUpdater.exe")) return;
                            DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
                            Process.Start("LoLUpdater.exe");
                            _notdone = false;
                            Environment.Exit(0);
                        }
                    }
                    catch (Exception)
                    {
                        try
                        {
                            Console.WriteLine("File corrupt, redownloading...");
                            webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                            if (File.Exists("LoLUpdater.exe")) return;
                            DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
                            Process.Start("LoLUpdater.exe");
                            _notdone = false;
                            Environment.Exit(0);
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("Critical error occured!");
                            Console.ReadLine();
                            Environment.Exit(0);
                        }
                    }
                }
            }
        }

        private static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern void DeleteFile(string file);
    }
}