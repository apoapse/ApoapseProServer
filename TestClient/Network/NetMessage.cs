using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public enum NetMessageEncoding
{
    ASCII,
    hex,
    UTF8
}

public struct NetMessage
{
    public enum Direction
    {
        send,
        received
    }

    public const int HEADER_SIZE_BYTES = 4;

    public byte[] ContentData { get; private set; }
    public Direction MsgDirection { get; private set; }
    public bool UseApoapseTransportLayer { get; private set; }

    public NetMessage(byte[] message, Direction direction, bool useApoapseTransportLayer)
    {
        ContentData = message;
        MsgDirection = direction;
        UseApoapseTransportLayer = useApoapseTransportLayer;
    }


    public byte[] GenerateHeader()
    {
        Debug.Assert(UseApoapseTransportLayer);

        return BitConverter.GetBytes((UInt32)ContentData.Length);
    }

    public static byte[] EncodeString(NetMessageEncoding encoding, string data)
    {
        byte[] output;

        if (encoding == NetMessageEncoding.UTF8)
            output = Encoding.UTF8.GetBytes(data);
        else if (encoding == NetMessageEncoding.hex)
            output = StringToByteArray(data);
        else
            output = Encoding.ASCII.GetBytes(data);

        return output;
    }

    public string DecodeData(NetMessageEncoding encoding)
    {
        string output;

        if (encoding == NetMessageEncoding.UTF8)
            output = Encoding.UTF8.GetString(ContentData);
        else if (encoding == NetMessageEncoding.hex)
        {
            output = BitConverter.ToString(ContentData);
            output = output.Replace("-", " ");
        }
        else
            output = Encoding.ASCII.GetString(ContentData);

        return output;
    }

    private static byte[] StringToByteArray(String hex)
    {
        hex = hex.Replace(" ", string.Empty);

        int NumberChars = hex.Length;
        byte[] bytes = new byte[NumberChars / 2];
        for (int i = 0; i < NumberChars; i += 2)
            bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
        return bytes;
    }
}