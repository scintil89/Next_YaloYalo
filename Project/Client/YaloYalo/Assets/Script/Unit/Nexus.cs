//건물 = 움직이지 않는 원거리 유닛
using System.Collections;
using UnityEngine;

public class Nexus : Unit
{
    public GameObject explosion;

    void Awake()
    {
        explosion.SetActive(false);
    }

    public override void Kill()
    {
        explosion.SetActive(true);
        particlePlayer.Play(PARTICLE_TYPE.BuildingExplode, gameObject.transform.position);
        SoundManager.GetInstance().Play(EFFECT_TYPE.BuildingExplode);

        StartCoroutine("KillAfter", 1.5f);
    }

    IEnumerator KillAfter(float delay)
    {
        yield return new WaitForSeconds(delay);

        gameObject.transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
        ChangeState<UnitStateDoNothing>();
    }
}