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

namespace Pong
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        System.Windows.Threading.DispatcherTimer timer = new System.Windows.Threading.DispatcherTimer();  // Ein Timer wird erstellt
        public int speed_x = 250; // x und y Geschwindigkeiten
        public int speed_y = 250;
        public int point_counter = 0;
        public MainWindow()
        {
            InitializeComponent();
            timer.Interval = TimeSpan.FromSeconds(0.01); // Das Intervall wird angegeben / Kann man mit den FPS gleichsetzen
            timer.IsEnabled = true; // Der Timer startet
            timer.Tick += animate; //Pro Intervall wird animate ausgeführt
            this.Cursor = Cursors.None; // Der Cursur wird verdeckt
        


        }

        public void animate(object sender, EventArgs e) // Erstellung der Animation
        {
            double ball_left = Canvas.GetLeft(ball); // Die Werte für die variablen wird ermittelt
            double ball_right = Canvas.GetRight(ball);
            double ball_top = Canvas.GetTop(ball);
            double ball_bottom = Canvas.GetBottom(ball);

            ball_left += speed_x * timer.Interval.TotalSeconds; // Die Geschwindigkeit wird zugefügt
            ball_top += speed_y * timer.Interval.TotalSeconds;
            ball_bottom += speed_y * timer.Interval.TotalSeconds;
            ball_right += speed_x * timer.Interval.TotalSeconds;

            Canvas.SetBottom(ball, ball_bottom); // Die Ball wird an die Position des Wertes der Variablen gesetzt
            Canvas.SetTop(ball, ball_top);
            Canvas.SetLeft(ball, ball_left);
            Canvas.SetRight(ball, ball_right);

            double raket_right = Canvas.GetRight(raket);// Das Gleiche für den Schläger
            double raket_left = Canvas.GetLeft(raket);
            double raket_top = Canvas.GetTop(raket);
            double raket_bottom = Canvas.GetBottom(raket);

            Point point = Mouse.GetPosition(this); // Maus Position wird ermittelt
            double p_middle = point.X - raket.ActualWidth / 2;
            Canvas.SetLeft(raket, p_middle); // Der Linke Abstand wird so gesetzt das der Schläger sich durch die Maus bewegt. 
            Canvas.SetTop(raket, MyCanvas.ActualHeight - raket_top / 10); // Die Position wird Dynamisch gestaltet, dadurch das der Schläger sich immer an den Canvasgrößen orientiert
            Canvas.SetRight(raket, p_middle + raket.ActualWidth);
            Canvas.SetBottom(raket, MyCanvas.ActualHeight - raket_bottom / 10);

            if (ball_bottom >= MyCanvas.ActualHeight) //Randbereich Kollosion
            {
                timer.IsEnabled = false;
            }
            if (ball_top <= 0)
            {
                speed_y = -speed_y;
            }

            if (ball_right >= MyCanvas.ActualWidth)
            {
                speed_x = -speed_x;
            }
            if (ball_left <= 0)
            {
                speed_x = -speed_x;
            }

            if (ball_bottom >= raket_top && ball_top <= raket_bottom && ball_left >= raket_left && raket_right >= ball_left) // Schläger Kollision
            {
                speed_y = -speed_y;
                point_counter++;
                lbl_score.Content = point_counter;

            }


        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {

            if (timer.IsEnabled == false && e.Key == Key.Enter) // warum stoppt der Timer wieder? 
            {
                timer.IsEnabled = true;
                Canvas.SetTop(ball, 130); // Die Ausgangspositionen müssen angegeben werden, damit die Kollisionsberechnung stimmen kann
                Canvas.SetBottom(ball, 160);
                Canvas.SetRight(ball, 330);
                Canvas.SetLeft(ball, 310);
                point_counter = 0;
                lbl_score.Content = point_counter;
            }



        }
    }
}
