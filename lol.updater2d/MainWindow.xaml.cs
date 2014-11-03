using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;

namespace lol.updater2d
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct Rect
        {
            private readonly int Left;        // x position of upper-left corner
            private readonly int Top;         // y position of upper-left corner
            private readonly int Right;       // x position of lower-right corner
            private readonly int Bottom;      // y position of lower-right corner
        }

        // use async to learn C# 6?
        private static void Main()
        {

            var init = Process.GetProcessesByName("lol.updater");
                var proc = init[0];
                while (proc.MainWindowHandle == IntPtr.Zero)
                {
                    // Discard cached information about the process
                    // because MainWindowHandle might be cached.
                    proc.Refresh();

                    Thread.Sleep(10);
                }

                var handle = proc.MainWindowHandle;
                var r = new Rect();
                GetWindowRect(handle, ref r);


           


        }

        [DllImport("user32.dll")]
        private static extern bool GetWindowRect(IntPtr hwnd, ref Rect rectangle);


    }
}
