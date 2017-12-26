using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CombinedPlayers : MonoBehaviour {

    private float screenSizeWidth;
    //private Vector2 groundPos;
    public GameObject playerObject;
    private GameObject[] playerList = new GameObject[3];
    private PlayerInfo[] playerInfoList = new PlayerInfo[3];
    [HideInInspector]
    public GameObject activePlayer;
    public int activePlayerNum = 1;
    public bool pulling = false;
	// Use this for initialization
	void Start () {
        screenSizeWidth = Camera.main.orthographicSize * Camera.main.aspect * 2;
        //groundPos = GameObject.FindGameObjectWithTag("Ground").transform.position;
        SetupPlayerPositions();
        SetupPlayerInfos();
        SwitchMainPlayer(1);
        //foreach(HookShooter qh in GetComponentsInChildren<HookShooter>())
            //qh.cP = this;
        //foreach (BlobHook bh in GetComponentsInChildren<BlobHook>())
           // bh.cP = this;

        //Debug.Log((screenSizeWidth/2));
    }
	
	// Update is called once per frame
	void Update () {
        //Debug.Log(activePlayer.transform.position);
        if (!pulling)
        {
            if (activePlayer.transform.position.x > (screenSizeWidth / 2))
            {
                if (activePlayerNum > 0)
                    SwitchMainPlayer(activePlayerNum - 1);
                else
                    SwitchMainPlayer(2);
            }
            else if (activePlayer.transform.position.x < -(screenSizeWidth / 2))
            {
                if (activePlayerNum < 2)
                    SwitchMainPlayer(activePlayerNum + 1);
                else
                    SwitchMainPlayer(0);
            }
        }

	}

    void SetupPlayerPositions(){
        GameObject centerPlayerObject = Instantiate(playerObject, transform.position, Quaternion.identity);
        Vector2 leftPos = centerPlayerObject.transform.position;
        leftPos.x -= screenSizeWidth;
        GameObject leftPlayerObject = (Instantiate(playerObject, leftPos, Quaternion.identity));
        Vector2 rightPos = centerPlayerObject.transform.position;
        rightPos.x += screenSizeWidth;
        GameObject rightPlayerObject = (Instantiate(playerObject, rightPos, Quaternion.identity));
        playerList = new GameObject[] { leftPlayerObject,centerPlayerObject,rightPlayerObject};
        foreach(GameObject g in playerList)
        {
            g.transform.SetParent(transform);
        }
    }

    void SetupPlayerInfos(){
        for (int i = 0; i < playerList.Length; i++)
            playerInfoList[i] = playerList[i].GetComponentInChildren<PlayerInfo>();
    }


    void SwitchMainPlayer(int pos){
        //Position of other players in the arrays
        int leftPos = GrabOtherPlayer(pos, true);
        int rightPos = GrabOtherPlayer(pos, false);

        //PlayerObjects including arm and hand
        GameObject centerPlayerObject = playerList[pos];
        GameObject leftPlayerObject = playerList[leftPos];
        GameObject rightPlayerObject = playerList[rightPos];

        //Player transforms
        Transform centerPlayerPos = playerInfoList[pos].transform;
        Transform leftPlayerPos = playerInfoList[leftPos].transform;
        Transform rightPlayerPos = playerInfoList[rightPos].transform;
        //Activate current center player
        centerPlayerObject.SetActive(true);
        centerPlayerObject.transform.SetParent(transform);
        centerPlayerPos.gameObject.GetComponent<Rigidbody2D>().isKinematic = false;
        Vector2 vel = playerInfoList[activePlayerNum].GetComponent<Rigidbody2D>().velocity;
        activePlayer = playerInfoList[pos].gameObject;
        activePlayer.GetComponent<Rigidbody2D>().velocity = vel;
        activePlayerNum = pos;


        //Deactivate other players
        DeactivateInactivePlayers();
        Vector2 cenPos = centerPlayerPos.position;
        cenPos.x -= screenSizeWidth;
        leftPlayerPos.transform.position = cenPos;
        cenPos.x += 2 * screenSizeWidth;
        rightPlayerPos.transform.position = cenPos;

        leftPlayerObject.transform.SetParent(playerInfoList[pos].transform);
        rightPlayerObject.transform.SetParent(playerInfoList[pos].transform);
    }

    int GrabOtherPlayer(int pos, bool left)
    {
        if (pos > 0 && left)
            return pos - 1;
        else if (left)
            return 2;
        else if (pos < 2 && !left)
            return pos + 1;
        else
            return 0;
    }

    public void SetAllActive()
    {
        float currentPosY = playerInfoList[activePlayerNum].transform.position.y;
        for(int i = 0; i < playerList.Length; i++)
        {
            playerList[i].SetActive(true);
            Vector2 pos = playerInfoList[i].transform.position;
            pos.y = currentPosY;
            playerInfoList[i].transform.position = pos;
        }

    }
    
    public void DeactivateInactivePlayers()
    {
        
        for(int i = 0; i < playerList.Length; i++)
        {
            if (i != activePlayerNum)
            {
                playerInfoList[i].GetComponent<Rigidbody2D>().isKinematic = true;

            }

        }
    }

    public void CheckRockAttach(Grapple bh)
    {
        if(bh.grappleShooter.gameObject != activePlayer)
        {
            for(int i = 0; i < playerList.Length; i++)
            {
                if(bh.grappleShooter.gameObject == playerInfoList[i].gameObject)
                {
                    SwitchMainPlayer(i);
                    //Debug.Log("here");
                    pulling = true;
                }
                else
                {
                    playerInfoList[i].GetComponent<GrappleShooter>().Detach();
                }
            }
        }
    }


}
