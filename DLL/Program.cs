using System;
using System.Threading;

namespace LoLUpdaterDLL
{
    public class DLL
    {
        public static bool _notdone;
        public static bool IsAlreadyRunning;
        public static Mutex mutex = new Mutex(true, "9bba28e3-c2a3-4c71-a4f8-bb72b2f57c3b", out IsAlreadyRunning);

        public static void Main()
        {
        }

        public static int ToInt(string value)
        {
            int result;
            Int32.TryParse(value, out result);
            return result;
        }
    }
}