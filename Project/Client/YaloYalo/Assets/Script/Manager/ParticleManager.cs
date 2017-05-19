using UnityEngine;
using System.Collections.Generic;

//ParticleManager
// -파티클을 특정 위치에서 재생한다
// -내부적으로 자원을 재활용한다

//사용법: 
//1. Resources/Particles에 파일을 추가한다.
//2. 파일이름과 같은 이름의 enum을 추가한다
public enum PARTICLE_TYPE
{
    ArrowHit, BuildingExplode, Summon,
    SparkyAttack, SparkyLevelUp, SparkyCharge, ChargeComplete,
    UnitDead
}

public class ParticleManager : MonoBehaviour
{
    Dictionary<string, GameObject> particlePrefabs = new Dictionary<string, GameObject>();
    List<ParticleSystem> particlePool = new List<ParticleSystem>();
    static Vector3 PARTICLE_DELTA_POS = new Vector3(0.0f, 0.2f, 0.0f);
    
    void Awake()
    {
        Utility.Load("Particles/", particlePrefabs);
    }

    public ParticleSystem GetParticle(PARTICLE_TYPE pType)
    {
        return GetParticleFromPool(pType);
    }

    public static void Play(ParticleSystem ps, Vector3 position)
    {
        ps.Stop(true);
        ps.transform.position = position + PARTICLE_DELTA_POS;
        ps.Play(true);
    }

    public void Play(PARTICLE_TYPE pType, Vector3 position)
    {
        ParticleSystem ps = GetParticle(pType);
        Play(ps, position);
    }

    ParticleSystem GetParticleFromPool(PARTICLE_TYPE particleType)
    {
        //풀에서 파티클을 가져온다
        for (int i = 0; i < particlePool.Count; i++)
        {
            if(particlePool[i].isPlaying == false &&
                particlePool[i].gameObject.name == particleType.ToString())
            {
                return particlePool[i];
            }
        }

        //풀에서 재생가능한 파티클을 찾지 못했으면 새로 할당한다
        return MakeNewParticle(particleType);
    }

    ParticleSystem MakeNewParticle(PARTICLE_TYPE particleType)
    {
        var newParticleObj = Instantiate(particlePrefabs[particleType.ToString()]);

        //이름으로 종류를 구분하므로 이름셋팅
        newParticleObj.name = particleType.ToString();

        //풀에 등록
        var newParticle = newParticleObj.GetComponent<ParticleSystem>();
        particlePool.Add(newParticle);

        return newParticle;
    }
}