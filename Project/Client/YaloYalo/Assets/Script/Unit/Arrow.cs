using UnityEngine;
using Packets;

public class Arrow : Unit
{
    public ParticleSystem effect;

    void Awake()
    {
        //Arrow는 체력이 없다
        if (hpBar) hpBar.gameObject.SetActive(false);
    }

    public override void OnSummoned(PACKET_UNIT_SUMMON_NTF pkt)
    {
        playerSide = GameManager.GetPlayerSide(pkt._playerSide);
        unitName = pkt._unitName;
        unitId = pkt._unitNum;

        gameObject.transform.position = new Vector3(
            pkt._posX * CalculateManager.toClientValue,
            2.0f, //잘 보이게 좀 위에 설정한다
            pkt._posY * CalculateManager.toClientValue);

        //위치를 설정하고 재생한다
        effect.Stop(true);
        effect.time = 0.0f; //처음장면으로 되돌림
        effect.Play(true);
        SoundManager.GetInstance().Play(EFFECT_TYPE.ArrowBomb, 0.05f);

        ChangeState<UnitStateDoNothing>();
    }

    public override void Kill()
    {
        gameObject.transform.position = LogicConstants.UNIT_DISAPPEAR_POS;
    }
}