using UnityEngine;

//Logic에서 가져다 쓰는 상수값입니다

public static class LogicConstants
{
    static public float DAMAGED_EFFECT_TIME = 0.5f;
    static public Vector3 UNIT_DISAPPEAR_POS = new Vector3(0.0f, -100.0f, 0.0f);

    //UNIT_POOL_USABLE_LINE 보다 y값이 작으면 해당 객체를 사용할 수 있다  
    static public float UNIT_POOL_USABLE_LINE = -50.0f;
}

