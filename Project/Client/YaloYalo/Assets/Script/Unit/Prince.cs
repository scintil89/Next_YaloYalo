using Packets;
using UnityEngine;

public class Prince : Unit
{
    public SkinnedMeshRenderer hatSmr;
    public Animator animator;
    public Material enemyMat;
    public Material myMat;

    static string RUN_TRIGGER = "Run";

    enum PrinceState {
        USUAL, SPECIAL
    }
    PrinceState princeState;

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
        switch(princeState)
        {
            case PrinceState.USUAL:
                SoundManager.GetInstance().Play(EFFECT_TYPE.SwordManAttack, attackDelay);
                break;
            case PrinceState.SPECIAL:
                SoundManager.GetInstance().Play(EFFECT_TYPE.SwordManAttack, attackDelay, 1.5f);
                break;
        }
    }

    public override void OnSpecialNtf(PACKET_UNIT_SPECIAL_NTF pkt)
    {
        int princeStateNum = pkt._ival;

        if (princeStateNum == 0)
        {
            princeState = PrinceState.USUAL;
            animator.SetTrigger(MOVE_TRIGGER);
        }
        else if(princeStateNum == 1) 
        {
            princeState = PrinceState.SPECIAL;
            SoundManager.GetInstance().Play(EFFECT_TYPE.PrinceState);
            animator.SetTrigger(RUN_TRIGGER);
        }

        moveSpeed = pkt._fval1 * CalculateManager.toClientValue;
        moveValue = moveValue.normalized * moveSpeed;
    }
}