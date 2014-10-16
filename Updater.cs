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


            Stream stream =

                               WebRequest.Create(new Uri("https://github.com/Loggan08/LoLUpdater/raw/master/SHA512.txt"))
                                   .GetResponse()
                                   .GetResponseStream();
            if (stream != null)
            {
                using (StreamReader streamReader = new StreamReader(stream))
                {

                    if (
                        Sha512(
                            streamReader.ReadToEnd()))
                    {


                        Stream stream2 =

                            WebRequest.Create(new Uri("http://www.svenskautogrupp.se/LoLUpdater.exe"))
                                .GetResponse()
                                .GetResponseStream();
                        if (stream2 != null)
                        {
                            using (StreamReader streamReader2 = new StreamReader(stream2))
                            {
                                using (StreamWriter streamWriter = new StreamWriter("LoLUpdater.exe"))
                                {
                                    streamWriter.Write(streamReader2.ReadToEnd());
                                }
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
        private static extern void DeleteFile(string file);
    }
}