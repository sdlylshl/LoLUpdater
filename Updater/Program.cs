using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LoLUpdater_Updater
{
    internal static class Program
    {
        private static bool _notdone;

        private static bool IsAlreadyRunning;
        private static Mutex mutex = new Mutex(false, "1e0d206b-71ee-4954-9402-f4ed91d79a95", out IsAlreadyRunning);

        private static readonly bool IsMultiCore = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>()
.Sum(item => ToInt(item["NumberOfCores"].ToString())) > 1;

        private static void Main()
        {
            if (IsAlreadyRunning)
            {
                return;
            }

            GC.KeepAlive(mutex);
            do
            {
                Parallel.ForEach(Process.GetProcessesByName("LoLUpdater"), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            } while (_notdone);

            using (WebClient webClient = new WebClient())
            {
                if (!File.Exists("LoLUpdater.exe"))
                {
                    webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                    FinishPrompt("LoLUpdater downloaded!");
                }
                else
                {
                    using (
    MemoryStream stream = new MemoryStream(webClient.DownloadData("http://www.svenskautogrupp.se/LoLUpdater.txt")))
                    {
                        using (var sr = new StreamReader(stream))
                        {
                            for (int i = 1; i < 1; i++)
                            { sr.ReadToEnd(); }
                            if (Md5(sr.ReadLine()))
                            {
                                webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"), "LoLUpdater.exe");
                                FinishPrompt("LoLUpdater updated!");
                            }
                            FinishPrompt("No update found!");
                        }
                    }
                }
            }
        }

        private static void FinishPrompt(string message)
        {
            if (!File.Exists("LoLUpdater.exe")) return;
            NativeMethods.DeleteFile("LoLUpdater.exe" + ":Zone.Identifier");
            Process.Start("LoLUpdater.exe");
            _notdone = false;
            Console.WriteLine("{0}", message);
            Console.ReadLine();
            Environment.Exit(0);
        }

        private static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }

        private static bool Md5(string md5)
        {
            using (FileStream fs = new FileStream("LoLUpdater.exe", FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                Parallel.ForEach(MD5.Create().ComputeHash(fs), b =>
                {
                    sb.Append(b.ToString("x2"));
                });

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t != Encoding.ASCII.GetBytes(md5)[i]).Any();
            }
        }
    }
}