using System;
using System.Runtime.InteropServices;

namespace LoLUpdater
{
    internal class NativeMethods
    {
        public static const string s_kernel = "kernel32.dll";
        
        [DllImport(s_kernel, CharSet = CharSet.Ansi, BestFitMapping = false]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string proc);
        
        
        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724482%28v=vs.85%29.aspx

        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string file);
        
        [DllImport(s_kernel, CharSet = CharSet.Unicode)]
[return: MarshalAs(UnmanagedType.Bool)] /* unnecessary, isn't it? */
static extern bool FreeLibrary(IntPtr hModule);
        

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate Bool BarType(Byte arg, string stuff); // return value indicates whether function call went well or not.

    Bool Bar(Byte arg, string stuff)
    {
        Bool ok = false;
        IntPtr pDll= LoadLibrary(s_kernel);
        if (pDll != IntPtr.Zero)
        {
            IntPtr pfunc = GetProcAddress(pDll, stuff);
            if (pFunc != IntPtr.Zero)
            {
                BarType bar = (BarType)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(BarType));
                ok = bar(arg, stuff);
            }
            FreeLibrary(pDll);
        }
        return ok;
    }
        
        
    }
}
