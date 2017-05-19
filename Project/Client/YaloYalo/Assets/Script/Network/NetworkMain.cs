using UnityEngine;

//NetworkMain
// - NetworkTcpIp에서 패킷을 꺼내 PacketProcessor에게 전달합니다
// - 어플리케이션 종료 이벤트에 관련한 처리를 합니다.

public class NetworkMain : MonoBehaviour
{
    PacketProcessor packetProcessor;
    NetworkTcpIp networkTcpIp;

    void Start ()
    {
        ResourceManager.LoadData();
        Screen.SetResolution(610, 655, false);

        packetProcessor = gameObject.AddComponent<PacketProcessor>();
        networkTcpIp = NetworkTcpIp.GetInstance();

        DontDestroyOnLoad(this);
    }

    void Update ()
    {
        while(networkTcpIp.QueueEmpty() == false)
        {
            var packet = networkTcpIp.GetPacket();

            packetProcessor.ProcessPacket(packet);
        }
	}

    void OnApplicationQuit()
    {
        networkTcpIp.CloseConnection();
    }
}
