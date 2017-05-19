using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Packets;

public class BowMan : Unit
{
    public SkinnedMeshRenderer hatSmr;
    public Animator animator;
    public GameObject bowPrefab;
    List<Bow> bows = new List<Bow>();

    public Material enemyMat;
    public Material myMat;

    void Start()
    {
        var bowObj = Instantiate(bowPrefab);
        bowObj.gameObject.transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
        var bow = bowObj.GetComponent<Bow>();
        bow.particlePlayer = particlePlayer;
        bows.Add(bow);
    }

    public override void OnSummoned(PACKET_UNIT_SUMMON_NTF pkt)
    {
        base.OnSummoned(pkt);

        hatSmr.material = (playerSide == GameManager.playerSide) ? myMat : enemyMat;
    }

    public override void SummonedEffect()
    {
        particlePlayer.Play(PARTICLE_TYPE.Summon, gameObject.transform.position);
        SoundManager.GetInstance().Play(EFFECT_TYPE.UnitSummoned);
    }

    public override void OnMoveToWaypointEnter(Vector2 moveValue, Vector2 waypoint)
    {
        base.OnMoveToWaypointEnter(moveValue, waypoint);

        animator.SetTrigger(MOVE_TRIGGER);
    }

    public override void OnMoveToTargetEnter(Unit target, float moveSpeed, Vector2 serverPos)
    {
        base.OnMoveToTargetEnter(target, moveSpeed, serverPos);

        animator.SetTrigger(MOVE_TRIGGER);
    }

    public override void AttackOnce()
    {
        animator.SetTrigger(ATTACK_TRIGGER);

        StartCoroutine("ShootBow");
    }

    IEnumerator ShootBow()
    {
        yield return new WaitForSeconds(0.3f);

        Bow bow = GetBow();
        bow.Shoot(this);
    }

    Bow GetBow()
    {
        for (int i = 0; i < bows.Count; i++)
        {
            if(bows[i].gameObject.transform.position.y < LogicConstants.UNIT_POOL_USABLE_LINE)
            {
                return bows[i];
            }
        }

        return MakeBow();
    }

    Bow MakeBow()
    {
        GameObject newBowObj = Instantiate(bowPrefab);

        Bow newBow = newBowObj.GetComponent<Bow>();
        newBow.particlePlayer = particlePlayer;

        bows.Add(newBow);

        return newBow;
    }
}