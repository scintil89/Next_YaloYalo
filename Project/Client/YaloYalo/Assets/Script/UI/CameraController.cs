using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Camera myCamera;
    public bool isMainCamera = false;

    Vector3 rotPivot = Vector3.back;
    const float sensitivity = 2.0f;

    float downAccLimit = -34.0f;
    float upAccLimit = 0.0f;
    Vector3 verticalAxis = Vector3.right;
    float verticalAcc = 0.0f;

    float leftAccLimit = -24.0f;
    float rightAccLimit = 24.0f;
    Vector3 horizonAxis = Vector3.up;
    float horizonAcc = 0.0f;

    const float wheelMin = 4.0f;
    const float wheelMax = 12.0f;

    Quaternion originRot;
    Vector3 originPos;
    float originSize;

    float rotVal;
    float curAcc;

    const string MOUSE_Y = "Mouse Y";
    const string MOUSE_X = "Mouse X";
    const string MOUSE_WHEEL = "Mouse ScrollWheel";

    void Start()
    {
        //main 카메라인 경우 윗쪽 플레이어면 180도 회전되므로
        //축을 뒤짚어준다.
        if (isMainCamera && GameManager.playerSide == PlayerSide.TOP)
        {
            verticalAxis *= -1.0f;
            rotPivot = Vector3.forward;
        }

        originSize = myCamera.orthographicSize;
        originRot = transform.rotation;
        originPos = transform.position;
    }

    void Update()
    {
        //소환 시도 시에는 카메라 조작 불가
        if(Input.GetMouseButton(0))
        {
            Init();
            return;
        }

        if (Input.GetMouseButton(1)) //1 = right click
        {
            //Vertical Rotation
            ProcessRotation(Input.GetAxis(MOUSE_Y), downAccLimit, upAccLimit, verticalAxis, ref verticalAcc);
            //Horizontal Rotation
            //ProcessRotation(Input.GetAxis(MOUSE_X), leftAccLimit, rightAccLimit, horizonAxis, ref horizonAcc);
            //Zoom in-out
            ProcessWheel(Input.GetAxis(MOUSE_WHEEL), wheelMin, wheelMax);
        }

        if (Input.GetMouseButtonUp(1)) //1 = right click
        {
            Init();
        }
    }

    void ProcessRotation(float mouseVal, float negativeLimit, float positiveLimit, Vector3 axis, ref float accVal)
    {
        rotVal = mouseVal * sensitivity;
        curAcc = accVal + rotVal;

        var zoomVal = myCamera.orthographicSize;
        var limitDelta = (zoomVal - (wheelMax * 0.8f)) * -1.5f;
        if (curAcc > negativeLimit - limitDelta && curAcc < positiveLimit + limitDelta)
        {
            accVal += rotVal;
            transform.RotateAround(rotPivot, axis, rotVal);
        }
    }

    void ProcessWheel(float wheelVal, float min, float max)
    {
        // -1.0f * => wheel up = zoom in / wheel down = zoom out
        var zoomVal = -1.0f * wheelVal * sensitivity;
        myCamera.orthographicSize = Mathf.Clamp(myCamera.orthographicSize + zoomVal, min, max);
    }

    void Init()
    {
        transform.rotation = originRot;
        transform.position = originPos;
        myCamera.orthographicSize = originSize;
        verticalAcc = 0.0f;
        horizonAcc = 0.0f;
    }
}