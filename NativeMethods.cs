using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    public class NativeMethods
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern void DeleteFile(string file);

        [DllImport("kernel32", CharSet = CharSet.Ansi, BestFitMapping = false)]
        internal static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        internal static extern IntPtr LoadLibrary(string file);
    }
}