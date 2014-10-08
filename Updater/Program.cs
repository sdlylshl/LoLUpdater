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
        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>()
.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static void Main()
        {
            using (WebClient webClient = new WebClient())
            {
                if (!File.Exists("LoLUpdater.exe"))
                {
                    Console.WriteLine("LoLUpdater not found, downloading...");

                    webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                    if (!File.Exists("LoLUpdater.exe")) return;
                    DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
                    Process.Start("LoLUpdater.exe");
                    Environment.Exit(0);
                }
                else
                {
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
                            }
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("No update found, press Enter to re-download");
                            Console.ReadLine();
                            webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                        }
                    }
                    Kill("LoLUpdater.exe");
                    webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                    if (!File.Exists("LoLUpdater.exe")) return;
                    DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
                    Process.Start("LoLUpdater.exe");
                    Environment.Exit(0);
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

        private static void Kill(string process)
        {
            if (IsMultiCore)
            {
                Parallel.ForEach(Process.GetProcessesByName(process), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            }
            else
            {
                foreach (Process proc in Process.GetProcessesByName(process))
                {
                    proc.Kill();
                    proc.WaitForExit();
                }
            }
        }
    }
}