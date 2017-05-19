/*
using UnityEngine;
using System.Collections;

public class HPBarPool : MonoBehaviour
{
    public GameObject hpBar;
    GameObject[] hpBarPool = null;

    int poolSize = 50;
    int ckr = 0;

    // Use this for initialization
    void Start ()
    {
        hpBarPool = new GameObject[poolSize];

        for (int i = 0; i < 10; ++i)
        {
            hpBarPool[i] = Instantiate(hpBar) as GameObject;
            hpBarPool[i].name = "hpBar" + i;
            hpBarPool[i].SetActive(false);
        }
    }
	
	// Update is called once per frame
	void Update ()
    {
	
	}
}
*/