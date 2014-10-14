using System;
using System.Runtime.InteropServices;
private const string s_kernel = "kernel32.dll";
namespace LoLUpdater
{
    internal class NativeMethods
    {
        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string file);

        [DllImport(s_kernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsProcessorFeaturePresent(uint feature);

        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string file);
    }
}
