using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

using TestClient;
using System.Threading;

// State object for receiving data from remote device.
public class StateObject
{
    // Client socket.
    public Socket workSocket = null;
    // Size of receive buffer.
    public const int BufferSize = 4096;
    // Receive buffer.
    public byte[] buffer = new byte[BufferSize];

    public List<byte> bytes = new List<byte>();
}

class TCPClient
{
    public bool IsConnected { get; private set; } = false;
    public IPEndPoint RemoteEndPoint { get; private set; }

    private Socket m_socket;
    private MainWindow m_mainwindow;
    private NetMessage m_lastMessage;

    public TCPClient(MainWindow mainWindow, string ipAddress, UInt16 port)
    {
        m_mainwindow = mainWindow;

        RemoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);
        m_socket = new Socket(AddressFamily.InterNetwork,  SocketType.Stream, ProtocolType.Tcp);

        m_socket.BeginConnect(RemoteEndPoint, new AsyncCallback(ConnectCallback), m_socket);
    }

    private void ConnectCallback(IAsyncResult ar)
    {
        try
        {
            // Complete the connection.
            m_socket.EndConnect(ar);
            Receive();
        }
        catch (Exception ex)
        {
            m_mainwindow.ShowError(ex.Message);
            return;
        }

        IsConnected = true;
        m_mainwindow.OnConnect();
    }

    public void Send(NetMessage netMessage)
    {
        m_lastMessage = netMessage;
        byte[] finalData;

        if (netMessage.UseApoapseTransportLayer)
        {
            finalData = CombineByteArrays(netMessage.GenerateHeader(), netMessage.ContentData);
        }
        else
            finalData = netMessage.ContentData;

        m_socket.BeginSend(finalData, 0, finalData.Length, 0, new AsyncCallback(SendCallback), m_socket);
    }

    private void SendCallback(IAsyncResult ar)
    {
        try
        {
            // Complete sending the data to the remote device.
            int bytesSent = m_socket.EndSend(ar);

            if (m_lastMessage.UseApoapseTransportLayer)
                Debug.Assert(bytesSent == m_lastMessage.ContentData.Length + NetMessage.HEADER_SIZE_BYTES);
            else
                Debug.Assert(bytesSent == m_lastMessage.ContentData.Length);

            m_mainwindow.AddNetMessageToUI(m_lastMessage, bytesSent);
        }
        catch (Exception e)
        {
            m_mainwindow.ShowError(e.Message);
        }
    }

    private void Receive()
    {
        try
        {
            // Create the state object.
            StateObject state = new StateObject();
            state.workSocket = m_socket;

            // Begin receiving the data from the remote device.
            m_socket.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
        }
        catch (Exception e)
        {
            m_mainwindow.ShowError(e.Message);
        }
    }

    private void ReceiveCallback(IAsyncResult ar)
    {
        try
        {
            StateObject state = (StateObject)ar.AsyncState;
            int bytesRead = m_socket.EndReceive(ar);

            if (bytesRead == 0)
                return;

            Debug.Assert(bytesRead <= StateObject.BufferSize, "The NetMessage is too big for the buffer");

            for (int i = 0; i < bytesRead; i++)
            {
                state.bytes.Add(state.buffer[i]);
            }
            m_socket.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);

            NetMessage message;
            byte[] data = state.bytes.ToArray();

            if (m_mainwindow.IsApoapseTransportProtocolUsed())
            {
                message = new NetMessage(SubArray(data, 1, data.Length), NetMessage.Direction.received, true);
            }
            else
                message = new NetMessage(data, NetMessage.Direction.received, false);

            m_mainwindow.AddNetMessageToUI(message, bytesRead);

            Receive();
        }
        catch (Exception e)
        {
            m_mainwindow.ShowError(e.Message);
        }
    }

    public void Disconnect()
    {
        //m_socket.Disconnect(false);
        m_socket.Close();
        IsConnected = false;
    }

    public static byte[] CombineByteArrays(params byte[][] arrays)
    {
        byte[] rv = new byte[arrays.Sum(a => a.Length)];
        int offset = 0;
        foreach (byte[] array in arrays)
        {
            System.Buffer.BlockCopy(array, 0, rv, offset, array.Length);
            offset += array.Length;
        }
        return rv;
    }

    public static T[] SubArray<T>(T[] data, int index, int length)
    {
        T[] result = new T[length];
        Array.Copy(data, index, result, 0, length);
        return result;
    }
}
