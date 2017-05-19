using System.Collections.Generic;
using UnityEngine;

static public class Utility
{
	static public void Rotate(GameObject origin, GameObject target, float time)
    {
        Rotate(origin, target.transform.position, time);
    }

    static public void Rotate(GameObject origin, Vector3 target, float time)
    {
        //현재 액션 취소
        LeanTween.cancel(origin);
        //원래값 저장
        var originalRotation = origin.transform.eulerAngles;
        //회전값 획득
        origin.transform.LookAt(target);
        var rotateVal = origin.transform.eulerAngles;
        //회전에 제한을 둔다
        rotateVal.x = Mathf.Clamp(rotateVal.x, -5.0f, 5.0f);
        rotateVal.z = Mathf.Clamp(rotateVal.z, -5.0f, 5.0f);
        //원래값으로 복구
        origin.transform.eulerAngles = originalRotation;
        //액션 실행
        LeanTween.rotateLocal(origin, rotateVal, 0.3f).setEase(LeanTweenType.easeInSine);
    }

    static public void Swap<T>(ref T a, ref T b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }

    static public void Load<LOAD_T>(string directory, Dictionary<string, LOAD_T> storage)
                where LOAD_T : Object
    {
        var prefabs = Resources.LoadAll<LOAD_T>(directory);
        for(int i = 0; i < prefabs.Length; i++)
        {
            var prefab = prefabs[i];
            storage.Add(prefab.name, prefab);
        }
    }

    static public void Load<LOAD_T>(string directory, List<LOAD_T> storage)
                where LOAD_T : Object
    {
        var prefabs = Resources.LoadAll<LOAD_T>(directory);
        for (int i = 0; i < prefabs.Length; i++)
        {
            var prefab = prefabs[i];
            storage.Add(prefab);
        }
    }
   
    static public IEnumerable<int> GetSerialRandomNumbersOneByOne(int inclusiveMin, int exclusiveMax)
    {
        var numbers = GenerateRandomNumberList(inclusiveMin, exclusiveMax);
        for(int i = 0; i < numbers.Count; i++)
        {
            yield return numbers[i];
        }
    }

    static public List<int> GenerateRandomNumberList(int inclusiveMin, int exclusiveMax)
    {
        var numbers = new List<int>();
        for (int i = inclusiveMin; i < exclusiveMax; i++)
        {
            numbers.Add(i);
        }

        Random.InitState((int)System.DateTime.Now.Ticks);
        for (int i = 0; i < numbers.Count; i++)
        {
            int randomIndex = Random.Range(0, numbers.Count - 1);
            int tmp = numbers[i];
            numbers[i] = numbers[randomIndex];
            numbers[randomIndex] = tmp;
        }

        return numbers;
    }
}