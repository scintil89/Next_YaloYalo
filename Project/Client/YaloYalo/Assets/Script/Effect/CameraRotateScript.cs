using UnityEngine;

//rotatePosition을 중심으로 카메라를 회전합니다

public class CameraRotateScript : MonoBehaviour
{
    public GameObject rotatePosition;

	void Update ()
    {
        gameObject.transform.RotateAround(rotatePosition.transform.position, Vector3.up, 20.0f * Time.deltaTime);
	}
}
