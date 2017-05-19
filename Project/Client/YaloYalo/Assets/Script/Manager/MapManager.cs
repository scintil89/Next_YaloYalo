using UnityEngine;

public class MapManager : MonoBehaviour
{
    public static GameObject validSummonAreaPrefab;

    public static GameObject validSummonAreaForUnit;
    public static Vector3 scaleForUnit = new Vector3(2.47f, 2.8f, 1.0f);

    public static GameObject validSummonAreaForMagic;
    public static Vector3 scaleForMagic = new Vector3(2.498f, 6.09f, 1.0f);

    //강 영역도 표시한다

    static MapManager()
    {
        validSummonAreaPrefab = (GameObject)Resources.Load(ResourceConstants.VALID_SUMMON_AREA_PREFAB);
        validSummonAreaPrefab.transform.position = new Vector3(0.0f, 0.04f, -6.51f);
    }

    static public void OnGameSceneInit()
    {
        validSummonAreaForUnit = Instantiate(validSummonAreaPrefab);
        validSummonAreaForUnit.transform.localScale = scaleForUnit;
        validSummonAreaForUnit.SetActive(false);

        validSummonAreaForMagic = Instantiate(validSummonAreaPrefab);
        validSummonAreaForMagic.transform.localScale = scaleForMagic;
        validSummonAreaForMagic.SetActive(false);
    }
}