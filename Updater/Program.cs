using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Threading.Tasks;

namespace LoLUpdater_Updater
{
    internal static class Program
    {
        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
            .Cast<ManagementBaseObject>()
            .Sum(item => int.Parse(item["NumberOfCores"].ToString())) > 1;

        private static readonly string[] LoLUpdaterprocs = { "LoLUpdater Uninstall", "LoLUpdater" };

        private static void Main()
        {
            using (WebClient webClient = new WebClient())
            {
                if (!File.Exists("LoLUpdater.exe"))
                {
                    Console.WriteLine("LoLUpdater not found, downloading...");

                    webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                }
                else
                {
                    Kill(LoLUpdaterprocs);
                    using (MemoryStream stream = new MemoryStream(webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt")))
                    {
                        webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt");
                        stream.Position = 0;
                        var sr = new StreamReader(stream);
                        try
                        {
                            var current = new Version(FileVersionInfo.GetVersionInfo("LoLUpdater.exe").FileVersion);
                            var latest = new Version(sr.ReadToEnd());
                            if (current < latest)
                            {
                                Console.WriteLine("LoLUpdater has an update!, downloading...");

                                webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                            }
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("LoLUpdater is corrupt, redownloading...");
                            webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                        }
                    }
                    Kill(LoLUpdaterprocs);
                }
                if (!File.Exists("LoLUpdater Uninstall.exe"))
                {
                    Console.WriteLine("LoLUpdater Uninstaller not found, downloading...");

                    webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater Uninstall.exe"), "LoLUpdater Uninstall.exe");
                }
                else
                {
                    Kill(LoLUpdaterprocs);
                    using (MemoryStream stream = new MemoryStream(webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt")))
                    {
                        webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt");
                        stream.Position = 0;
                        var sr = new StreamReader(stream);
                        try
                        {
                            sr.ReadLine();
                            while ((sr.ReadLine()) != null)
                            {
                                var current = new Version(FileVersionInfo.GetVersionInfo("LoLUpdater Uninstall.exe").FileVersion);
                                var latest = new Version(sr.ReadToEnd());
                                if (current >= latest) continue;
                                Console.WriteLine("LoLUpdater Uninstall update found!, downloading...");

                                webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater Uninstall.exe"), "LoLUpdater Uninstall.exe");
                            }
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("LoLUpdater Uninstall is corrupt, redownloading...");
                            webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater Uninstall.exe"), "LoLUpdater Uninstall.exe");
                        }
                    }
                    Kill(LoLUpdaterprocs);
                }
            }
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
    }
}