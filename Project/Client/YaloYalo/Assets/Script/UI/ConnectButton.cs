using System;
using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ConnectButton : MonoBehaviour
{
    public InputField Ip;
    public Text text;

    NetworkTcpIp networkTcpIp = NetworkTcpIp.GetInstance();
    Button button;

    void Start ()
    {
        button = gameObject.GetComponent<Button>();
        button.onClick.AddListener(OnClick);
    }

    void OnClick()
    {
        button.enabled = false; //여러번 요청보내는 것을 막기 위해 버튼을 비활성화

        StartCoroutine("TryConnect");
    }

    void TryConnect()
    {
        if (Ip.text == "") //아무것도 입력하지 않으면 로컬로 접속한다
        {
            networkTcpIp.Connect(NetworkConstants.LOCAL_ADDRESS, NetworkConstants.SERVER_PORT);
        }
        else
        {
            networkTcpIp.Connect(Ip.text, NetworkConstants.SERVER_PORT);
        }

        StopCoroutine("CheckConnected");
        StartCoroutine("CheckConnected");
    }

    IEnumerator CheckConnected()
    {
        while(true)
        {
            yield return new WaitForSeconds(0.5f);

            if (networkTcpIp.IsConnected() == true)
            {
                SoundManager.GetInstance().Play(MUSIC_TYPE.LobbyScene);
                SceneManager.LoadScene(ResourceConstants.LOBBY_SCENE);
                break;
            }
            else
            {
                text.text = "Connect Failed";
                button.enabled = true;
                break;
            }
        }

        yield return new WaitForSeconds(0.001f);
    }
}