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

        // Save as Temp.txt and use this as the source for the autoupdater in LoLUdpater
        private static bool _notdone;

        private static void Main()
        {
            Mutex mutex = new Mutex(true, @"Global\TOTALLYNOTMYMUTEXVERYRANDOMANDRARE#DOGE: {13b2f78b-9867-49e1-bf73-287b7b136d30}");
            if (!mutex.WaitOne(TimeSpan.Zero, true)) return;
            GC.KeepAlive(mutex);
            do
            {
                Parallel.ForEach(Process.GetProcessesByName("LoLUpdater"), proc =>
                {
                    proc.Kill();
                    proc.WaitForExit();
                });
            } while (_notdone);

            using (Stream stream = WebRequest.Create(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/SHA512.txt"))
        .GetResponse()
        .GetResponseStream())
            {
                if (stream != null)
                    using (StreamReader streamReader = new StreamReader(stream))
                    {

                        if (Sha512(streamReader.ReadToEnd()))
                        {

                            using (Stream stream2 = WebRequest.Create(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"))
                                .GetResponse()
                                .GetResponseStream())
                            {
                                using (MemoryStream memoryStream = new MemoryStream())
                                {
                                    byte[] buffer = new byte[4096];
                                    int count = 0;
                                    do
                                    {
                                        if (stream2 != null) count = stream2.Read(buffer, 0, buffer.Length);
                                        memoryStream.Write(buffer, 0, count);
                                    } while (count != 0);

                                }
                            }
                        }
                    }
                if (new FileInfo("LoLUpdater.exe").Attributes
            .Equals(FileAttributes.ReadOnly))
                {
                    File.SetAttributes("LoLUpdater.exe",
                       FileAttributes.Normal);
                }

                DeleteFile("LoLUpdater.exe:Zone.Identifier");
                _notdone = false;
                Process.Start("LoLUpdater.exe");
                Environment.Exit(0);
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
        private static extern bool DeleteFile(string file);
    }
}