using UnityEngine;
using System.Collections.Generic;

public class AudioTest : MonoBehaviour
{
    public enum AT
    {
        BowHit, ArrowBomb, Clicked, SparkyAttack
    }

    Dictionary<string, AudioSource> sources = new Dictionary<string, AudioSource>();

	void Start()
    {
        var clips = Resources.LoadAll<AudioClip>("SoundEffects/");
        for(int i = 0; i < clips.Length; i++)
        {
            var clip = clips[i];
            var audioSource = gameObject.AddComponent<AudioSource>();
            audioSource.clip = clip;
            sources.Add(clip.name, audioSource);
        }
    }

    void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            Play(AT.ArrowBomb);
        }
        if (Input.GetKeyDown(KeyCode.B))
        {
            Play(AT.SparkyAttack);
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            Play(AT.Clicked);
        }
    }

    void Play(AT at)
    {
        var source = sources[at.ToString()];
        source.PlayOneShot(source.clip);
    }
}