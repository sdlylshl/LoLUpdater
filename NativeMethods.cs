using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    internal class NativeMethods
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string file);

        [DllImport("kernel32", CharSet = CharSet.Ansi, BestFitMapping = false)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string file);
    }
}