using System.Runtime.InteropServices;

namespace Packets
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PacketHeader
    {
        public int pktId;
        public int bodySize;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Packet
    {
        public int pktId;
        public int bodySize;
        public string data;
    }
}
