using System.Runtime.InteropServices;

namespace LoLUpdater_Updater
{
    public internal NativeMethods
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern void DeleteFile(string file);
    }
}
