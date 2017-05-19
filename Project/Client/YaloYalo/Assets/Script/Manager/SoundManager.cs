using UnityEngine;
using System.Collections.Generic;
using System.Collections;

//사용법: Resources/SoundEffects에 파일을 추가하고 파일이름과 맞춘다
public enum EFFECT_TYPE : byte
{
    ArrowBomb, BowHit, BowShoot, BuildingExplode,
    Clicked, Lose, PrinceState, SparkyAttack,
    SparkyCharge, SummonFail, SwordManAttack, UnitDead,
    UnitSummoned, VS, Win, FlyingStart, FlyingEnd, CardSelected
}

//사용법: Resources/BGM에 파일을 추가하고 파일이름과 맞춘다
public enum MUSIC_TYPE : byte
{
    GameScene, LobbyScene
}

public class SoundManager : MonoBehaviour
{
    Dictionary<string, AudioClip> effectClips = new Dictionary<string, AudioClip>();
    AudioSource effectPlayer;

    Dictionary<string, AudioClip> musicClips = new Dictionary<string, AudioClip>();
    AudioSource musicPlayer;

    #region Singleton
    static GameObject container = null;
    static SoundManager instance = null;
    static public SoundManager GetInstance()
    {
        if (instance == null)
        {
            container = new GameObject();
            container.name = "SoundManager";
            instance = container.AddComponent<SoundManager>();  
        }

        return instance;
    }
    SoundManager()
    {
    }
    #endregion

    void Awake()
    {
        Init();
        DontDestroyOnLoad(this);
    }

    //음악을 재생한다
    public void Play(MUSIC_TYPE musicType)
    {
        musicPlayer.Stop();
        musicPlayer.clip = musicClips[musicType.ToString()];
        musicPlayer.Play();
    }

    //'delay' 초 후에 음악을 재생한다
    public void Play(MUSIC_TYPE musicType, float delay)
    {
        musicPlayer.Stop();
        musicPlayer.clip = musicClips[musicType.ToString()];
        musicPlayer.PlayDelayed(delay);
    }

    public void StopMusic()
    {
        musicPlayer.Stop();
    }

    //효과음을 재생한다
    public void Play(EFFECT_TYPE effectType)
    {
        effectPlayer.PlayOneShot(effectClips[effectType.ToString()]);
    }

    //'delay' 초 후에 효과음을 재생한다
    public void Play(EFFECT_TYPE effectType, float delay, float pitchRatio = 1.0f)
    {
        PlayInfo playInfo = new PlayInfo(effectType, delay, pitchRatio);

        StartCoroutine("PlayAfter", playInfo);
    }

    IEnumerator PlayAfter(PlayInfo playInfo)
    {
        yield return new WaitForSeconds(playInfo.delay);

        var originPitch = effectPlayer.pitch;
        effectPlayer.pitch *= playInfo.pitchRatio;
        Play(playInfo.effectType);
        effectPlayer.pitch = originPitch;
    }

    void Init()
    {
        musicPlayer = gameObject.AddComponent<AudioSource>();
        musicPlayer.loop = true;
        Utility.Load("BGM/", musicClips);

        effectPlayer = gameObject.AddComponent<AudioSource>();
        effectPlayer.loop = false;
        Utility.Load("SoundEffects/", effectClips);
    }

    #region DECLARE_PLAY_INFO
    struct PlayInfo
    {
        public PlayInfo(EFFECT_TYPE et, float d, float pr)
        {
            effectType = et;
            delay = d;
            pitchRatio = pr;
        }

        public EFFECT_TYPE effectType;
        public float delay;
        public float pitchRatio;
    }
    #endregion
}