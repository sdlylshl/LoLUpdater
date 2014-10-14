using System;
using System.Runtime.InteropServices;

namespace LoLUpdater_Updater
{
    internal class NativeMethods
    {

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern void DeleteFile(string file);
    }
}