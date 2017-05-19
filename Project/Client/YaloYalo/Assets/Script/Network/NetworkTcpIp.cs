using UnityEngine;
using System;
using System.Net.Sockets;
using System.Collections.Generic;
using Packets;

//NetworkTcpIp
// - 비동기 콜백 TcpIp 통신을 합니다
// - 비동기 콜백은 내부적으로 별도의 스레드에서 동작하므로 
//   PacketQueue를 사용할 때는 Lock으로 감싸야 합니다.

public class NetworkTcpIp : Singleton<NetworkTcpIp>
{
    int bufferSize = NetworkConstants.BUFFER_SIZE;
    bool isConnected = false;
    Socket socket;
    AsyncCallback recvCallback;
    AsyncCallback sendCallback;
    Queue<Packet> packetQueue;

    public NetworkTcpIp()
    {
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        recvCallback = new AsyncCallback(RecvCallback);
        sendCallback = new AsyncCallback(SendCallback);
        packetQueue = new Queue<Packet>();
    }

    public void CloseConnection()
    {
        if (socket.Connected == true)
        {
            socket.Close();
            packetQueue.Clear();
            isConnected = false;
        }
    }

    public bool IsConnected()
    {
        return isConnected;
    }

    public void Connect(string ip, int port)
    {
        try
        {
            socket.BeginConnect(ip, port, ConnectCallback, 0);
        }
        catch
        {
            isConnected = false;
        }
    }

    void ConnectCallback(IAsyncResult asyncResult)
    {
        try
        {
            socket.EndConnect(asyncResult);
            isConnected = true;
        }
        catch
        {
            return;
        }

        AsyncRecvData asyncRecvData = new AsyncRecvData(bufferSize, socket);

        socket.BeginReceive(
            asyncRecvData.buffer,
            0,
            asyncRecvData.buffer.Length,
            SocketFlags.None,
            recvCallback,
            asyncRecvData);
    }

    public bool QueueEmpty()
    {
        lock(this)
        {
            return packetQueue.Count == 0 ? true : false;
        }
    }

    public Packet GetPacket()
    {
        lock(this)
        {
            return packetQueue.Dequeue();
        }
    }

    public void SendPacket<T>(T data, PacketId pktId)
    {
        if(isConnected == false)
        {
            return;
        }

        AsyncSendData asyncSendData = new AsyncSendData(socket);

        //성능 이슈로 포인터 기반으로 동작하게 바꿈
        unsafe
        {
            #region prepare datas for copying to send buffer

            string jsonedData = JsonUtility.ToJson(data);

            int packetId = (int)pktId;
            int bodySize = jsonedData.Length;

            asyncSendData.buffer = new byte[NetworkConstants.PACKET_HEADER_SIZE + bodySize];
            asyncSendData.sendNum = NetworkConstants.PACKET_HEADER_SIZE + bodySize;

            #endregion

            #region copy packet id to send buffer 

            byte* packetIdPos = (byte*)&packetId;

            for (int i = 0; i < NetworkConstants.INT_SIZE; i++)
            {
                asyncSendData.buffer[i] = packetIdPos[i];
            }

            #endregion

            #region copy body size to send buffer 

            byte* bodySizePos = (byte*)&bodySize;

            for (int i = 0; i < NetworkConstants.INT_SIZE; i++)
            {
                asyncSendData.buffer[i+NetworkConstants.INT_SIZE] = bodySizePos[i];
            }

            #endregion

            #region copy body to send buffer

            char[] bodyPos = jsonedData.ToCharArray();

            for (int i = 0; i < bodySize; i++)
            {
                asyncSendData.buffer[i + NetworkConstants.PACKET_HEADER_SIZE] = (byte)bodyPos[i];
            }

            #endregion
        }

        try
        {
            socket.BeginSend(
                    asyncSendData.buffer,
                    0,
                    asyncSendData.buffer.Length,
                    SocketFlags.None,
                    sendCallback,
                    asyncSendData);
        }
        catch(SocketException except)
        {
            ProcessException(except);
            return;
        }
    }

    void RecvCallback(IAsyncResult asyncResult)
    {
        if (isConnected == false)
        {
            return;
        }

        AsyncRecvData asyncRecvData = (AsyncRecvData)asyncResult.AsyncState;

        try
        {
            asyncRecvData.recvNum += asyncRecvData.socket.EndReceive(asyncResult);
            asyncRecvData.readPos = 0;
        }
        catch(SocketException except)
        {
            ProcessException(except);
            return;
        }

        //성능 이슈로 포인터 기반으로 동작하게 고침
        #region make packet
        while (true)
        {
            if (asyncRecvData.recvNum < NetworkConstants.PACKET_HEADER_SIZE)
            {
                break;
            }

            unsafe
            {
                //패킷 헤더의 위치를 가리키는 포인터를 선언한다.
                fixed(byte* packetHeaderPos = &asyncRecvData.buffer[asyncRecvData.readPos])
                {
                    //해당 위치에서 PacketHeader를 읽는다
                    PacketHeader* packetHeader = (PacketHeader*)packetHeaderPos;

                    int packetSize = NetworkConstants.PACKET_HEADER_SIZE + packetHeader->bodySize;

                    if (asyncRecvData.recvNum < packetSize)
                    {
                        break;
                    }

                    Packet packet = new Packet();
                    packet.bodySize = packetHeader->bodySize;
                    packet.pktId = packetHeader->pktId;

                    packet.data = NetworkConstants.ENCODING.GetString(
                        asyncRecvData.buffer, 
                        asyncRecvData.readPos + NetworkConstants.PACKET_HEADER_SIZE, 
                        packetHeader->bodySize);

                    lock(this)
                    {
                        packetQueue.Enqueue(packet);
                    }

                    asyncRecvData.readPos += packetSize;
                    asyncRecvData.recvNum -= packetSize;
                }
            }
        }
        #endregion

        #region shift remain bytes to front
        for (int i = 0; i < asyncRecvData.recvNum; i++)
        {
            asyncRecvData.buffer[i] = asyncRecvData.buffer[asyncRecvData.readPos + i];
        }
        #endregion

        asyncRecvData.socket.BeginReceive(
           asyncRecvData.buffer,
           asyncRecvData.recvNum,
           asyncRecvData.buffer.Length - asyncRecvData.recvNum,
           SocketFlags.None,
           recvCallback,
           asyncRecvData);
    }

    void SendCallback(IAsyncResult asyncResult)
    {
        if (isConnected == false)
        {
            return;
        }

        AsyncSendData asyncSendData = (AsyncSendData)asyncResult;

        int sendNum = 0;

        try
        {
            sendNum = asyncSendData.socket.EndSend(asyncResult);
        }
        catch(SocketException except)
        {
            ProcessException(except);
            return;
        }
 
        if (sendNum < asyncSendData.sendNum)
        {
            socket.BeginSend(
                asyncSendData.buffer,
                sendNum,
                asyncSendData.buffer.Length - sendNum,
                SocketFlags.Truncated,
                sendCallback,
                asyncSendData);
        }
    }

    void ProcessException(SocketException except)
    {
        var errorCode = (SocketError)except.ErrorCode;
 
        if( errorCode == SocketError.ConnectionAborted ||
            errorCode == SocketError.Disconnecting ||
            errorCode == SocketError.HostDown ||
            errorCode == SocketError.Shutdown ||
            errorCode == SocketError.SocketError ||
            errorCode == SocketError.ConnectionReset)
        {
            NotifyCloseConnect();
        }
    }

    void NotifyCloseConnect()
    {
        Packet packet = new Packet();
        packet.pktId = (int)PacketId.ID_PACKET_CONNECT_CLOSE_NTF;
 
        lock (this)
        {
            packetQueue.Enqueue(packet);
        }
    }
}

public class AsyncRecvData
{
    public byte[] buffer;
    public Socket socket;
    public int recvNum;
    public int readPos;

    public AsyncRecvData(int bufferSize, Socket socket)
    {
        recvNum = 0;
        readPos = 0;
        buffer = new byte[bufferSize];
        this.socket = socket;
    }
}

public class AsyncSendData
{
    public byte[] buffer;
    public Socket socket;
    public int sendNum;

    public AsyncSendData(Socket socket)
    {
        this.socket = socket;
        sendNum = 0;
    }
}