using UnityEngine;
using System.Collections;

public class ParticleTest : MonoBehaviour
{
    public GameObject summonFxPrefab;
    //public GameObject arrowHitEffect;
    //public GameObject buildingExplodeEffect;
    ParticleSystem summonParticle;
    //ParticleSystem arrowHit

    void Start()
    {
        summonParticle = Instantiate(summonFxPrefab).GetComponent<ParticleSystem>();
    }

    void Update()
    {
        if(Input.GetMouseButton(0))
        {
            var clickPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            summonParticle.gameObject.transform.position = new Vector3(clickPos.x, 0.0f, clickPos.z);            
            summonParticle.Play(true);
        }
    }
}