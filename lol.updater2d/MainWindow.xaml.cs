using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;

namespace lol.updater2d
{
    /// <summary>
    ///     Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        private int _pointCounter;
        private int _speedX = 250; // x und y Geschwindigkeiten
        private int _speedY = 250;
        private readonly DispatcherTimer _timer = new DispatcherTimer(); // Ein Timer wird erstellt

        public MainWindow()
        {
            InitializeComponent();
            _timer.Interval = TimeSpan.FromSeconds(0.01);
                // Das Intervall wird angegeben / Kann man mit den FPS gleichsetzen
            _timer.IsEnabled = true; // Der Timer startet
            _timer.Tick += Animate; //Pro Intervall wird animate ausgeführt
            Cursor = Cursors.None; // Der Cursur wird verdeckt
        }

        private void Animate(object sender, EventArgs e) // Erstellung der Animation
        {
            var ballLeft = Canvas.GetLeft(ball); // Die Werte für die variablen wird ermittelt
            var ballRight = Canvas.GetRight(ball);
            var ballTop = Canvas.GetTop(ball);
            var ballBottom = Canvas.GetBottom(ball);

            ballLeft += _speedX*_timer.Interval.TotalSeconds; // Die Geschwindigkeit wird zugefügt
            ballTop += _speedY*_timer.Interval.TotalSeconds;
            ballBottom += _speedY*_timer.Interval.TotalSeconds;
            ballRight += _speedX*_timer.Interval.TotalSeconds;

            Canvas.SetBottom(ball, ballBottom); // Die Ball wird an die Position des Wertes der Variablen gesetzt
            Canvas.SetTop(ball, ballTop);
            Canvas.SetLeft(ball, ballLeft);
            Canvas.SetRight(ball, ballRight);

            var raketRight = Canvas.GetRight(raket); // Das Gleiche für den Schläger
            var raketLeft = Canvas.GetLeft(raket);
            var raketTop = Canvas.GetTop(raket);
            var raketBottom = Canvas.GetBottom(raket);

            var point = Mouse.GetPosition(this); // Maus Position wird ermittelt
            var pMiddle = point.X - raket.ActualWidth/2;
            Canvas.SetLeft(raket, pMiddle);
                // Der Linke Abstand wird so gesetzt das der Schläger sich durch die Maus bewegt. 
            Canvas.SetTop(raket, MyCanvas.ActualHeight - raketTop/10);
                // Die Position wird Dynamisch gestaltet, dadurch das der Schläger sich immer an den Canvasgrößen orientiert
            Canvas.SetRight(raket, pMiddle + raket.ActualWidth);
            Canvas.SetBottom(raket, MyCanvas.ActualHeight - raketBottom/10);

            if (ballBottom >= MyCanvas.ActualHeight) //Randbereich Kollosion
            {
                _timer.IsEnabled = false;
            }
            if (ballTop <= 0)
            {
                _speedY = -_speedY;
            }

            if (ballRight >= MyCanvas.ActualWidth)
            {
                _speedX = -_speedX;
            }
            if (ballLeft <= 0)
            {
                _speedX = -_speedX;
            }

            if (!(ballBottom >= raketTop) || !(ballTop <= raketBottom) || !(ballLeft >= raketLeft) ||
                !(raketRight >= ballLeft)) return;
            _speedY = -_speedY;
            _pointCounter++;
            lbl_score.Content = _pointCounter;
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (_timer.IsEnabled || e.Key != Key.Enter) return;
            _timer.IsEnabled = true;
            Canvas.SetTop(ball, 130);
                // Die Ausgangspositionen müssen angegeben werden, damit die Kollisionsberechnung stimmen kann
            Canvas.SetBottom(ball, 160);
            Canvas.SetRight(ball, 330);
            Canvas.SetLeft(ball, 310);
            _pointCounter = 0;
            lbl_score.Content = _pointCounter;
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

        [StructLayout(LayoutKind.Sequential)]
        private struct Rect
        {
            private readonly int Left; // x position of upper-left corner
            private readonly int Top; // y position of upper-left corner
            private readonly int Right; // x position of lower-right corner
            private readonly int Bottom; // y position of lower-right corner
        }
    }
}