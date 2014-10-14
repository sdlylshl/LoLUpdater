using System;
using System.Runtime.InteropServices;

namespace LoLUpdater_Updater
{
    internal class NativeMethods
    {
        private const string SKernel = "kernel32.dll";

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool BarType(Byte arg);

        protected static bool Bar(byte arg, string stuff)
        {
            bool ok = false;
            IntPtr pDll = LoadLibrary(SKernel);
            if (pDll != IntPtr.Zero)
            {
                IntPtr pFunc = GetProcAddress(pDll, stuff);
                if (pFunc != IntPtr.Zero)
                {
                    BarType bar = (BarType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(BarType));
                    ok = bar(arg);
                }
                FreeLibrary(pDll);
            }
            return ok;
        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)] /* unnecessary, isn't it? */
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        // return value indicates whether function call went well or not.
        [DllImport(SKernel, SetLastError = true)]
        private static extern IntPtr LoadLibrary(String dllName);

        // return value indicates whether function call went well or not.
    }
}