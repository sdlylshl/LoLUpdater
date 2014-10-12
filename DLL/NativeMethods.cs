using System;
using System.Runtime.InteropServices;

namespace LoLUpdaterDLL
{
    public class NativeMethods
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string FileName);
    }
}