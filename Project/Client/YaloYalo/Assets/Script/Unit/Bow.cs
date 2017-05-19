using UnityEngine;
using System.Collections;

public class Bow : Unit
{
    void Update()
    {
        if (unitState == null)
        {
            return;
        }

        unitState.Update(this);
    }

    public void Shoot(Unit shooter)
    {
        target = shooter.target;

        var shooterPos = shooter.gameObject.transform.position;
        var lookVector = target.transform.position - shooterPos;
        var normalizedLookVector = lookVector.normalized;

        var distSquare = lookVector.magnitude;
        var spdSquare = distSquare / (shooter.attackDelay * shooter.attackDelay);
        moveSpeed = Mathf.Sqrt(spdSquare);
        moveSpeed *= 1.4f;
        moveValue = moveSpeed * new Vector2(normalizedLookVector.x, normalizedLookVector.z);

        gameObject.transform.position = new Vector3(
            shooterPos.x, shooterPos.y + 0.5f, shooterPos.z);
        gameObject.transform.LookAt(target.transform);
        gameObject.transform.eulerAngles = new Vector3(-180.0f, target.transform.eulerAngles.y, -180.0f);

        SoundManager.GetInstance().Play(EFFECT_TYPE.BowShoot);
        LeanTween.rotateAroundLocal(gameObject, Vector3.up, 720.0f, shooter.attackDelay);

        ChangeState<UnitStateWalkAndSeek>();
        StartCoroutine("RemoveSelftAfter", shooter.attackDelay);
    }

    public override void MoveTowardTarget()
    {
        if (target == null || target.gameObject.activeSelf == false)
        {
            RemoveSelftAfter(0.01f);
            return;
        }

        gameObject.transform.position += 
            new Vector3(moveValue.x, 0.0f, moveValue.y) * Time.deltaTime;
    }

    IEnumerator RemoveSelftAfter(float time)
    {
        yield return new WaitForSeconds(time);

        particlePlayer.Play(PARTICLE_TYPE.ArrowHit, gameObject.transform.position);
        SoundManager.GetInstance().Play(EFFECT_TYPE.BowHit);

        transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
        ChangeState<UnitStateDoNothing>();
    }
}