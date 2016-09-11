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

    public enum MessageType
    {
        unknown,
        command,
        message,
        attachement,
        error,
        info
    }

    public byte[] ContentData { get; private set; }
    public Direction MsgDirection { get; private set; }
    public MessageType MsgType { get; private set; }
    public bool UseApoapseTransportLayer { get; private set; }

    public NetMessage(byte[] message, Direction direction, bool useApoapseTransportLayer, MessageType messageType = MessageType.unknown)
    {
        ContentData = message;
        MsgDirection = direction;
        UseApoapseTransportLayer = useApoapseTransportLayer;
        MsgType = messageType;
    }

    public string GetReadableType()
    {
        switch (MsgType)
        {
            case MessageType.command:
                return "command";

            case MessageType.message:
                return "message";

            case MessageType.attachement:
                return "attachement";

            case MessageType.error:
                return "error";

            case MessageType.info:
                return "info";

            default:
                return "Unknown";
        }
    }

    private string GetTypeSignature()
    {
        switch (MsgType)
        {
            case MessageType.command:
                return "C";

            case MessageType.message:
                return "M";

            case MessageType.attachement:
                return "A";

            case MessageType.error:
                return "E";

            case MessageType.info:
                return "I";

            default:
                return "0";
        }
    }

    public byte[] GenerateHeader()
    {
        Debug.Assert(UseApoapseTransportLayer);

        return TCPClient.CombineByteArrays(BitConverter.GetBytes((UInt32)ContentData.Length), Encoding.ASCII.GetBytes(GetTypeSignature()));
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