using UnityEngine;
using Packets;

public class CardController : MonoBehaviour
{
    static NetworkTcpIp m_networkTcpIp = NetworkTcpIp.GetInstance();
    static float revertPosition = 1;
    
    //유닛에디터에서 세팅
    public SpriteRenderer m_cardSprite;
    public GameObject m_unitPrev;
    public string m_unitName;
    public bool m_isMagic;
    public int m_yaloCost;
    public SpriteRenderer m_numberSprite;

    //CardDisplayer에서 세팅
    public delegate void CardUsedCallback(int cardIndex);
    public CardUsedCallback m_cardUsedCallback { get; set; }
    public YaloController m_yaloController { get; set; }
    public int m_index { get; set; }

    public Vector3 m_defaultPosition { get; set; }
    Camera m_uiCamera;
    GameObject validAreaObject;
    SpriteRenderer validAreaSprite;
    bool m_isCardSelected = false;
    
    void Start()
    {
        m_uiCamera = GameObject.Find("UI Camera").GetComponent<Camera>();

        m_defaultPosition = gameObject.transform.position;

        bool isTopPlayer = (GameManager.playerSide == PlayerSide.TOP) ? true : false;
        revertPosition = isTopPlayer ? -1.0f : 1.0f;

        validAreaObject = m_isMagic ? MapManager.validSummonAreaForMagic : MapManager.validSummonAreaForUnit;
        validAreaSprite = validAreaObject.GetComponent<SpriteRenderer>();

        m_unitPrev = Instantiate(m_unitPrev);
        m_unitPrev.name = m_unitName + "Prev";
        m_unitPrev.SetActive(false);
    }

    void Update()
    {
        if (m_isCardSelected == true)
        {
            var pos = m_uiCamera.ScreenToWorldPoint(Input.mousePosition);
            var isValidPos = IsValidPosition(ref pos);

            UpdatetPrev(isValidPos, ref pos);
            UpdateCard(isValidPos, ref pos);
        }
    }

    void UpdatetPrev(bool isValidPos, ref Vector3 newPos)
    {
        if (isValidPos)
        {
            m_unitPrev.SetActive(true);
            m_unitPrev.transform.position = new Vector3(newPos.x, 1.0f, newPos.z);
        }
        else
        {
            m_unitPrev.SetActive(false);
        }
    }

    void UpdateCard(bool isValidPos, ref Vector3 newPos)
    {
        if (isValidPos)
        {
            if (LeanTween.isTweening(gameObject) == true)
            {
                LeanTween.cancel(gameObject);
                gameObject.transform.eulerAngles = new Vector3(90.0f, 0.0f, 0.0f);
            }
            m_cardSprite.enabled = false;
            m_numberSprite.enabled = false;
        }
        else
        {
            m_cardSprite.enabled = true;
            m_numberSprite.enabled = true;
            gameObject.transform.position = new Vector3(newPos.x, 1.0f, newPos.z);
        }
    }

    void OnMouseUp()
    {
        if (!enabled) return;

        m_isCardSelected = false;
        validAreaObject.SetActive(false);
        m_unitPrev.SetActive(false);
        m_cardSprite.enabled = true;
        m_numberSprite.enabled = true;
        
        if (IsValidPosition() == true)
        {
            //얄로가 충분하지 않은 경우
            if (GameManager.currentYaloNumber < m_yaloCost)
            {
                gameObject.transform.position = m_defaultPosition;

                LeanTween.cancel(gameObject);
                LeanTween.rotateY(gameObject, 20.0f, 0.1f).setEase(LeanTweenType.easeOutQuad).setEaseShake().setLoopCount(5);
                SoundManager.GetInstance().Play(EFFECT_TYPE.SummonFail);

                return;
            }

            //소환 요청
            RequestSummonUnit();

            //카드 사용
            m_cardUsedCallback(m_index);

            //얄로 개수 갱신
            m_yaloController.DecreaseYalo(m_yaloCost);
        }
        else
        {
            //제자리로 돌려보내기
            gameObject.transform.position = m_defaultPosition;
        }
    }

    void RequestSummonUnit()
    {
        var packet = new PACKET_UNIT_SUMMON_REQ();

        var pos = m_uiCamera.ScreenToWorldPoint(Input.mousePosition);
        packet._posX = pos.x * CalculateManager.toServerValue * revertPosition;
        packet._posY = pos.z * CalculateManager.toServerValue * revertPosition;
        packet._unitName = m_unitName;

        m_networkTcpIp.SendPacket(packet, PacketId.ID_PACKET_UNIT_SUMMON_REQ);
    }

    bool IsValidPosition()
    {
        var pos = m_uiCamera.ScreenToWorldPoint(Input.mousePosition);
        return IsValidPosition(ref pos);
    }

    bool IsValidPosition(ref Vector3 pos)
    {
        var validBound = validAreaSprite.bounds;

        if (validBound.min.x < pos.x &&
            pos.x < validBound.max.x &&
            validBound.min.z < pos.z &&
            pos.z < validBound.max.z)
        {
            return true;
        }

        return false;
    }

    void OnMouseDown()
    {
        if (!enabled) return;
        SoundManager.GetInstance().Play(EFFECT_TYPE.CardSelected);
        m_isCardSelected = true;
        validAreaObject.SetActive(true);
    }
}