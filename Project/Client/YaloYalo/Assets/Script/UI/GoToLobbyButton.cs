using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GoToLobbyButton : MonoBehaviour
{
	void Start()
    {
        gameObject.GetComponent<Button>().onClick.AddListener(OnClick);
    }

    void OnClick()
    {
        //효과음
        SoundManager.GetInstance().Play(EFFECT_TYPE.Clicked);

        //로비로 이동
        SceneManager.LoadScene(ResourceConstants.LOBBY_SCENE);
        SoundManager.GetInstance().Play(MUSIC_TYPE.LobbyScene, 0.5f);

        //로직 정리
        var gameScripts = GameObject.Find(ResourceConstants.GAME_SCRIPTS);
        Destroy(gameScripts.GetComponent<UIManager>());
        Destroy(gameScripts.GetComponent<UnitManager>());
        
        //게임 종료 표시 해제
        GameManager.isGameOver = false;
    }
}