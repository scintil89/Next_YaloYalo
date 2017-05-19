using UnityEngine;

public class HpBarPositionTest : MonoBehaviour
{
    public HPBar sword;
    public HPBar bow;
    public HPBar nexus;
    public HPBar king;
    public HPBar goblin;
    public HPBar prince;
    public HPBar sparky;

    public Unit swordU;
    public Unit bowU;
    public Unit nexusU;
    public Unit kingU;
    public Unit goblinU;
    public Unit princeU;
    public Unit sparkyU;

    void Start()
    {
        var pm = gameObject.AddComponent<ParticleManager>();

        GameManager.playerSide = PlayerSide.BOTTOM;

        sword.ManualInit();
        bow.ManualInit();
        nexus.ManualInit();
        king.ManualInit();
        goblin.ManualInit();
        prince.ManualInit();
        sparky.ManualInit();

        var pkt = new Packets.PACKET_UNIT_SUMMON_NTF();
        pkt._hp = 100;
        pkt._playerSide = false;
        pkt._unitName = "testUnit";
        pkt._unitNum = 100;

        swordU.particlePlayer = pm;
        swordU.OnSummoned(pkt);

        bowU.particlePlayer = pm;
        bowU.OnSummoned(pkt);
        
        nexusU.OnSummoned(pkt);

        kingU.OnSummoned(pkt);

        goblinU.particlePlayer = pm;
        goblinU.OnSummoned(pkt);

        princeU.particlePlayer = pm;
        princeU.OnSummoned(pkt);

        sparkyU.particlePlayer = pm;
        sparkyU.OnSummoned(pkt);
    }
}