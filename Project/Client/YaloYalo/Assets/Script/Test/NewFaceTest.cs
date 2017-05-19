using UnityEngine;
using System.Collections;

public class NewFaceTest : MonoBehaviour
{
    public Unit swordMan;
    public Unit bowMan;
    Animator swordAnim;
    Animator bowAnim;

    void Start()
    {
        swordAnim = swordMan.GetComponent<Animator>();
        bowAnim = bowMan.GetComponent<Animator>();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            swordAnim.SetTrigger("Move");
            bowAnim.SetTrigger("Move");
        }

        if(Input.GetKeyDown(KeyCode.S))
        {
            swordAnim.SetTrigger("Attack");
            bowAnim.SetTrigger("Attack");
        }

        if(Input.GetKeyDown(KeyCode.D))
        {
            swordAnim.SetTrigger("Stand");
            bowAnim.SetTrigger("Stand");
        }
    }
}