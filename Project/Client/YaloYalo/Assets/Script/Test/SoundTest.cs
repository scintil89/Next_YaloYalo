/*
using UnityEngine;
using System.Collections;

public class SoundTest : MonoBehaviour
{
    AudioClip one;
    AudioClip two;
    AudioClip main;

    private AudioSource source;
 
    void Awake()
    {
        one = Resources.Load<AudioClip>(ResourceConstants.SFX_BOWHIT);
        two = Resources.Load<AudioClip>(ResourceConstants.SFX_BOW_SHOOT);
        main = Resources.Load<AudioClip>(ResourceConstants.GAME_SCENE_MUSIC);

        source = gameObject.AddComponent<AudioSource>();
    }

    void Start()
    {
        source.PlayOneShot(main);
    }
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            source.PlayOneShot(one);
        }
        if (Input.GetKeyDown(KeyCode.B))
        {
            source.PlayOneShot(two);
        }
    }
}
*/