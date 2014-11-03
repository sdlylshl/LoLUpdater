using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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
            Execute()
            }

        // using async to learn C# 6
        private void Execute()
        {


            // add code if process not found (should not happen though)
            Process[] initial = GetProcessesByName("lol.updater");
            var proc = inital[0];

            while (proc.MainWindowHandle == IntPtr.Zero)
            {
                // Discard cached information about the process
                // because MainWindowHandle might be cached.
                proc.Refresh();

                Thread.Sleep(10);
            }

            var handle = proc.MainWindowHandle;
            Rect r = new Rect();
            GetWindowRect(handle, ref r);


        }

        [DllImport("user32.dll", Charset = Unicode)]
        public static extern bool GetWindowRect(IntPtr hwnd, ref Rect rectangle);


    }
}
