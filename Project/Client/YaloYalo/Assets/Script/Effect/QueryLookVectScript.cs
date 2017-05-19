using UnityEngine;

//by Scintil
//로그인씬 장난질을 위한 스크립트

public class QueryLookVectScript : MonoBehaviour
{
    public Vector3 rotatePosition;

	// Update is called once per frame
	void Update ()
    {
        gameObject.transform.LookAt(rotatePosition);
	}
}
