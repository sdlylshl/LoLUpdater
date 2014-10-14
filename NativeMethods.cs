using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    internal class NativeMethods
    {
        private const string SKernel = "kernel32.dll";

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate bool DllType(byte arg);

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern void DeleteFile(string file);

        protected static bool Dll(byte arg, string func)
        {
            bool ok = false;
            IntPtr pDll = LoadLibrary(SKernel);
            if (pDll == IntPtr.Zero) return false;
            IntPtr pFunc = GetProcAddress(pDll, func);
            if (pFunc != IntPtr.Zero)
            {
                DllType bar = (DllType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllType));
                ok = bar(arg);
            }
            FreeLibrary(pDll);
            return ok;
        }

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(SKernel, CharSet = CharSet.Ansi, BestFitMapping = false)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string proc);

        [DllImport(SKernel, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibrary(String dllName);
    }
}