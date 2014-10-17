using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    internal static class NativeMethods
    {
        internal static extern void DeleteFile(string file);
        [DllImport(Program.SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(Program.SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        internal static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(Program.SKernel, CharSet = CharSet.Unicode)]
        internal static extern IntPtr LoadLibrary(String dllName);
    }

}
