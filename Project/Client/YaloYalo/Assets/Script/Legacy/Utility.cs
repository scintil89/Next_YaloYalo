/*
using System.Runtime.InteropServices;
using System;
using System.Text;

public class Utility
{
    static public void StringToArray(ref string str, ref byte[] array, int pos)
    {
        var cstr = str.ToCharArray();
        for(int i = 0; i < str.Length; i++)
        {
            array[pos + i] = (byte)cstr[i];
        }
    }

    static public void PacketHeaderToArray(ref PacketHeader packet, int packetHeaderSize, ref byte[] array)
    {
        IntPtr packetPointer = Marshal.AllocHGlobal(packetHeaderSize);

        Marshal.StructureToPtr(packet, packetPointer, false);
        Marshal.Copy(packetPointer, array, 0, packetHeaderSize);

        Marshal.FreeHGlobal(packetPointer);
    }

    static public T GetStructureFromArray<T>(ref byte[] array, int pos, int size) where T : new()
    {
        T data = new T();

        IntPtr arrayPointer = Marshal.AllocHGlobal(size);
        Marshal.Copy(array, pos, arrayPointer, size);
        data = (T)Marshal.PtrToStructure(arrayPointer, data.GetType());
        Marshal.FreeHGlobal(arrayPointer);

        return data;
    }

    static public string GetStringFromArray(ref byte[] array, int pos, int size)
    {
        return Encoding.ASCII.GetString(array, pos, size);
    }

    static public void ShiftMemory(ref byte[] array, int srcIdx, int dstIdx, int size)
    {
        for (int i = 0; i < size; i++)
        {
            array[dstIdx + i] = array[srcIdx + i];
        }
    }
}
*/