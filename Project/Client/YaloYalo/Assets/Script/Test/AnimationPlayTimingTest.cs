using UnityEngine;
using System.Collections;

public class AnimationPlayTimingTest : MonoBehaviour
{
    public SwordMan swordMan;
    public Prince prince;
    public GoblinBarrel goblin;
    public float attackDelay = 0.24f;

    void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            swordMan.attackDelay = attackDelay;
            swordMan.AttackOnce();
        }

        if (Input.GetKeyDown(KeyCode.B))
        {
            prince.attackDelay = attackDelay;
            prince.AttackOnce();
        }

        if (Input.GetKeyDown(KeyCode.C))
        {
            goblin.attackDelay = attackDelay;
            goblin.AttackOnce();
        }
    }
}