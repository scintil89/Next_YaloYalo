using UnityEngine;
using UnityEngine.UI;
using Packets;

public class MatchButton : MonoBehaviour
{
    public InputField nickName;
    public InputField roomNumber;
    public Text text;

    NetworkTcpIp networkTcpIp = NetworkTcpIp.GetInstance();
    Button _button;
    public Image _touchBlocker;

    int parsedRoomNumber = -1;

    void Start ()
    {
        _button = gameObject.GetComponent<Button>();
        _button.onClick.AddListener(OnClick);
        EnableButton();
	}

    private void EnableButton()
    {
        _button.enabled = true;
        _touchBlocker.color = new Color(0,0,0,0);
        _touchBlocker.raycastTarget = false;
    }

    private void DisableButton()
    {
        _button.enabled = false;
        _touchBlocker.color = new Color(0, 0, 0, 200.0f / 255.0f);
        _touchBlocker.raycastTarget = true;
    }

	void OnClick()
    {
        DisableButton();

        bool isNumeric = int.TryParse(roomNumber.text, out parsedRoomNumber);
        if (string.IsNullOrEmpty(nickName.text) == true ||
            isNumeric == false)
        {
            text.text = "Please fill all item";
            EnableButton();
            return;
        }
        
        StartCoroutine("SendMatchReqPkt");
        GameManager.myName = nickName.text;

        SoundManager.GetInstance().Play(EFFECT_TYPE.Clicked);
        text.text = "Please wait...";
    }

    public void OnMatchFailed()
    {
        text.text = "Match Failed. Try Again";
        EnableButton();
    }

    void SendMatchReqPkt()
    {
        var pktBody = new PACKET_MATCH_JOIN_REQ();
#if DEBUG
        pktBody._uuid = Time.realtimeSinceStartup.ToString();
#else
        pktBody._uuid = SystemInfo.deviceUniqueIdentifier;
#endif
        pktBody._nickname = nickName.text;
        pktBody._roomNum = parsedRoomNumber;

        networkTcpIp.SendPacket(pktBody, PacketId.ID_PACKET_MATCH_JOIN_REQ);
    }
}