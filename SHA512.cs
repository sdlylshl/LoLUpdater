using System;
using System.Linq;
using System.Management;

namespace LoLUpdater
{
    public class Sha512
    {
        public static readonly ManagementBaseObject[] CpuInfo = new ManagementObjectSearcher("Select * from Win32_Processor").Get()
.Cast<ManagementBaseObject>().ToArray();

        public static readonly bool Isx64 = Environment.Is64BitProcess;
        public static readonly bool IsLinuxorMono = (int)Environment.OSVersion.Platform == 4 || (int)Environment.OSVersion.Platform == 128;
        public static readonly bool AvxCheck = Isx64 & (IsLinuxorMono || (Environment.OSVersion.Version.Major >= 6 & Environment.OSVersion.Version.Minor >= 1));
        public static readonly bool HasSse = NativeMethods.IsProcessorFeaturePresent(6);
        public static readonly bool HasSse2 = NativeMethods.IsProcessorFeaturePresent(10);

        // test for "XSTATE_MASK_GSSE" and "XSTATE_MASK_AVX" for perfect test.
        public static readonly bool HasAvx = AvxCheck & NativeMethods.IsProcessorFeaturePresent(17) & NativeMethods.GetProcAddress(NativeMethods.LoadLibrary("kernel32.dll"), "GetEnabledXStateFeatures") != null;

        // There is a better way to do the AVX2 check
        public static readonly bool IsAvx2 = AvxCheck & CpuInfo.Any(item => item["Name"].ToString().Contains(new[] { "Haswell", "Broadwell", "Skylake", "Cannonlake" }.ToString()));

        private static string _TbbSha512 = IsAvx2
                   ? "13d78f0fa6b61a13e5b7cf8e4fa4b071fc880ae1356bd518960175fce7c49cba48460d6c43a6e28556be7309327abec7ec83760cf29b043ef1178904e1e98a07"
                   : (HasAvx
                       ? "d81edd17a891a2ef464f3e69ff715595f78c229867d8d6e6cc1819b426316a0bf6df5fa09a7341995290e4efe4b884f8d144e0fe8e519c4779f5cf5679db784c"
                       : (HasSse2 ? "61fea5603739cb3ca7a0f13b3f96e7c0c6bcff418d1404272c9fcf7cb5ce6fef7e21a5ee2026fc6af4ebc596d1d912e8201b298f7d092004d8f5256e22b05b64" : HasSse ? "fa1cc95eff4ca2638b88fcdb652a7ed19b4a086bab8ce4a7e7f29324e708b5c855574c5053fe3ea84917ca0293dc97bac8830d5be2770a86ca073791696fcbec" : "0c201b344e8bf0451717d6b15326d21fc91cc5981ce36717bf62013ff5624b35054e580a381efa286cc72b6fe0177499a252876d557295bc4e29a3ec92ebfa58"));

        private static string _AirSha512 = "33f376d3f3a76a2ba122687b18e0306d45a8c65c89d3a51cc956bf4fa6d9bf9677493afa9b7bb5227fa1b162117440a5976484df6413f77a88ff3759ded37e8e";
        private static string _FlashSha512 = "e16c024424405ead77a89fabbb4a95a99e5552f33509d872bb7046cba4afb16f5a5bbf496a46b1b1ee9ef8b9e8ba6720bc8faccb654c5317e8142812e56b4930";

        public static string FlashSha512
        {
            get
            {
                return _FlashSha512;
            }
            set
            {
                _FlashSha512 = value;
            }
        }

        public static string AirSha512
        {
            get
            {
                return _AirSha512;
            }
            set
            {
                _AirSha512 = value;
            }
        }

        public static string TbbSha512
        {
            get
            {
                return _TbbSha512;
            }
            set
            {
                _TbbSha512 = value;
            }
        }
    }
}