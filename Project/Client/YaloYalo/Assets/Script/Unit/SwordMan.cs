using UnityEngine;
using Packets;

//Unit과 기본적으로 같다

public class SwordMan : Unit
{
    public SkinnedMeshRenderer hatSmr;
    public Animator animator;
    public Material enemyMat;
    public Material myMat;

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

        SoundManager.GetInstance().Play(EFFECT_TYPE.SwordManAttack, attackDelay);
    }
}