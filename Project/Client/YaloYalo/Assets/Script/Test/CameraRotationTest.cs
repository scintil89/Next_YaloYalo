using UnityEngine;

public class CameraRotationTest : MonoBehaviour
{
    public Camera myCamera;

    Vector3 rotPivot = Vector3.back; 
    float sensitivity = 2.0f;

    struct RotationConfig
    {
        public float negativeLimit;
        public float positiveLimit;
        public Vector3 axis;
        public float rotationAcc;

        public RotationConfig(float negativeLimit, float positiveLimit, Vector3 axis)
        {
            this.negativeLimit = negativeLimit;
            this.positiveLimit = positiveLimit;
            this.axis = axis;
            rotationAcc = 0.0f;
        }
    }

    RotationConfig verticalRot;
    RotationConfig horizontalRot;

    float downAccLimit = -34.0f;
    float upAccLimit = 0.0f;
    Vector3 verticalAxis = Vector3.right;

    float leftAccLimit = -24.0f;
    float rightAccLimit = 24.0f;
    Vector3 horizonAxis = Vector3.up;

    float wheelMin = 4.0f;
    float wheelMax = 12.0f;
    
    Quaternion originRot;
    Vector3 originPos;
    float originSize;

    //temp variables
    float rotVal;
    float curAcc;

    const string MOUSE_Y = "Mouse Y";
    const string MOUSE_X = "Mouse X";
    const string MOUSE_WHEEL = "Mouse ScrollWheel";

    void Start()
    {
        verticalRot = new RotationConfig(downAccLimit, upAccLimit, verticalAxis);
        horizontalRot = new RotationConfig(leftAccLimit, rightAccLimit, horizonAxis);

        originSize = myCamera.orthographicSize;
        originRot = transform.rotation;
        originPos = transform.position;
    }

    void Update()
    {
        if (Input.GetMouseButton(1)) //1 = right click
        {
            //Vertical Rotation
            ProcessRotation(Input.GetAxis(MOUSE_Y), ref verticalRot);
            //Horizontal Rotation
            ProcessRotation(Input.GetAxis(MOUSE_X), ref horizontalRot);
            //Zoom in-out
            ProcessWheel(Input.GetAxis(MOUSE_WHEEL), wheelMin, wheelMax);
        }

        if (Input.GetMouseButtonUp(1)) //1 = right click
        {
            Init();
        }
    }

    void ProcessRotation(float mouseVal, ref RotationConfig config)
    {
        rotVal = mouseVal * sensitivity;
        curAcc = config.rotationAcc + rotVal;

        if(curAcc > config.negativeLimit && curAcc < config.positiveLimit)
        {
            config.rotationAcc += rotVal;

            transform.RotateAround(rotPivot, config.axis, rotVal);
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

        verticalRot.rotationAcc = 0.0f;
        horizontalRot.rotationAcc = 0.0f;
    }
}