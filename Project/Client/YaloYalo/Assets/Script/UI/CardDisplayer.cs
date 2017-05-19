using UnityEngine;
using System.Collections.Generic;

/*
    CardDisplayer 역할

    1. 카드목록을 디스플레이해준다
    2. 카드가 사용되면 카드를 재배치해준다     
*/
public class CardDisplayer : MonoBehaviour
{
    public YaloController yaloController;
    public Vector3 cardStartPos;
    public float cardPosDelta;

    const int DISPLAY_CARD_NUMBER = 4;
    const int PREV_CARD_INDEX = DISPLAY_CARD_NUMBER;
    Vector2 DISPLAY_CARD_SCALE = new Vector2(0.25f, 0.25f);
    Vector2 PREV_CARD_SCALE = new Vector2(0.17f, 0.17f);

    int cardTypeNumber;

    List<Vector3> cardPositionList = new List<Vector3>();
    List<GameObject> cardPrefabList = new List<GameObject>();
    List<CardController> cardControllerList = new List<CardController>();

    void Start()
    {
        InitData();
        CreateCard();
    }

    void InitData()
    {
        cardPrefabList = ResourceManager.cardPrefabList;
        cardTypeNumber = cardPrefabList.Count;

        for (int i = 0; i < DISPLAY_CARD_NUMBER; i++)
        {
            cardPositionList.Add(new Vector3(
                cardStartPos.x + (cardPosDelta * i),
                1.0f,
                cardStartPos.z));
        }

        cardPositionList.Add(new Vector3(6.3f, 1.0f, -7.5f)); //Prev Position
        cardPositionList.Add(new Vector3(-100.0f, -100.0f, -100.0f)); //Out Position
    }

    void CreateCard()
    {
        var randomIndices = Utility.GenerateRandomNumberList(0, cardTypeNumber);

        for (int i = 0; i < cardTypeNumber; i++)
        {
            //프리팹으로 카드생성
            var cardObj = Instantiate(cardPrefabList[randomIndices[i]]);
            
            //카드컨트롤러 값 세팅
            var cardController = cardObj.GetComponent<CardController>();
            cardController.m_cardUsedCallback = OnCardUsed;
            cardController.m_yaloController = yaloController;
            cardController.m_index = i;
            
            //기타 설정
            cardObj.transform.position = cardPositionList[i];
            cardController.m_unitPrev.SetActive(false);

            //리스트에 저장한다
            cardControllerList.Add(cardController);

            //미리보기 카드면 컨트롤 불가로 설정함
            //(화면 밖 카드면 컨트롤 자체가 불가해서 설정 필요 없음)
            if (i == PREV_CARD_INDEX)
            {
                cardController.transform.localScale = PREV_CARD_SCALE;
                cardController.enabled = false;
            }
        }
    }
    
    //사용 : CardController에 CallBack으로 등록한다.
    //호출 : Card가 사용되었을 때 호출된다.
    public void OnCardUsed(int cardIndex)
    {
        //자신의 뒤에 있는 카드를 한칸씩 당기고 인덱스를 갱신한다
        //그리고 자신을 맨 끝에다가 위치시키고 인덱스를 갱신한다.
        var pivotCard = cardControllerList[cardIndex];

        //cardIndex의 범위 :  [0, DISPLAY_CARD_NUMBER)
        for (int i = cardIndex + 1; i < cardTypeNumber; i++)
        {
            var backCard = cardControllerList[i];
            backCard.gameObject.transform.position = cardPositionList[i - 1];
            backCard.m_defaultPosition = cardPositionList[i - 1];
            backCard.m_index = i - 1;
            cardControllerList[i - 1] = backCard;

            //CASE : 미리보기 카드 -> 카드 목록으로 이동
            if (i == PREV_CARD_INDEX)
            {
                backCard.transform.localScale = DISPLAY_CARD_SCALE;
                backCard.enabled = true;
            }
            //CASE : 화면 밖 카드 -> 미리보기 카드
            if(i == PREV_CARD_INDEX + 1)
            {
                backCard.transform.localScale = PREV_CARD_SCALE;
                backCard.enabled = false;
            }
        }

        pivotCard.gameObject.transform.position = cardPositionList[cardTypeNumber - 1];
        pivotCard.m_defaultPosition = cardPositionList[cardTypeNumber - 1];
        pivotCard.m_index = cardTypeNumber - 1;
        cardControllerList[cardTypeNumber - 1] = pivotCard;
    }
}