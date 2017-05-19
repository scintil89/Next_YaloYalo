using UnityEngine;
using System.Collections;

public class HpTest2 : MonoBehaviour
{
    public Unit unit;
    public HPBar hpBar;
    
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            GameManager.playerSide = PlayerSide.BOTTOM;
            hpBar.ManualInit();
        }

        if(Input.GetKeyDown(KeyCode.B))
        {
            GameManager.playerSide = PlayerSide.TOP;
            hpBar.ManualInit();
        }
    }
}