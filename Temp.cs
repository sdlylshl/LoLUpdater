using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
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
        private static readonly Mutex Mutex = new Mutex(true, @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: 1e0d206b-71ee-4954-9402-f4ed91d79a95");
        private static bool _notdone;

        private static void FileFix()
        {
            DeleteFile("LoLUpdater.exe:Zone.Identifier");
            if (!new FileInfo("LoLUpdater.exe").Attributes
                .Equals(FileAttributes.ReadOnly)) return;
            File.SetAttributes("LoLUpdater.exe",
                FileAttributes.Normal);
        }

        private static void FinishPrompt(string message)
        {
            if (!File.Exists("LoLUpdater.exe")) return;
            FileFix();
            Process.Start("LoLUpdater.exe");
            _notdone = false;
            Console.WriteLine("{0}", message);
            Console.ReadLine();
            Environment.Exit(0);
        }

        private static void Main()
        {
            if (!Mutex.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(Mutex);
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
                    FileFix();
                    {
                        if (Sha512(webClient.DownloadString("http://www.svenskautogrupp.se/LoLUpdater.txt")))
                        {
                            webClient.DownloadFile(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"),
                                "LoLUpdater.exe");
                            FinishPrompt("LoLUpdater updated!");
                        }
                        Console.WriteLine("No update found");
                    }
                }
            }
        }

        private static bool Sha512(string sha512)
        {
            using (FileStream fs = new FileStream("LoLUpdater.exe", FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                StringBuilder sb = new StringBuilder();

                fs.Seek(0, SeekOrigin.Begin);

                Parallel.ForEach(SHA512.Create().ComputeHash(fs), b =>
                {
                    sb.Append(b.ToString("x2"));
                });

                return Encoding.ASCII.GetBytes(sb.ToString()).Where((t, i) => t != Encoding.ASCII.GetBytes(sha512)[i]).AsParallel().Any();
            }
        }
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern void DeleteFile(string file);
    }
}