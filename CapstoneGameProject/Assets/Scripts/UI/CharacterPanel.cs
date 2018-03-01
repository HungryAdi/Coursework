using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class CharacterPanel : MonoBehaviour, System.IComparable {
    //UI Objects
    public CharacterJoinController charJoin;
    public GameObject joinText;
    public MySelectable[] childSelectableObjects;
    public GameObject AIPanel;
    public GameObject instructions;
    public GameObject readyLeave;
    public Image playerNameImage;
    public GameObject rockPrefab;

    //Activate/Deactivate Objects
    public GameObject player; //Jeffrey
    private GameObject rock;  //Jeffrey


    //Settings Panel Objects
    public GameObject settingsPanel;
    public GameObject[] settingsPanelObjects;
    private bool settingsActive = false;

    //Vars
    public int playerNum;
    private string readyText = "Ready!";
    private bool colorChanged;
    private bool joinedLastFrame; //Jeffrey

    //Selected Information
    private MySelectable selected;
    private int selectedNum;
    private Color currentColor;

    // Use this for initialization
    void Start() {
        colorChanged = false;
        playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
        charJoin.SetAIPlayerPref(playerNum, 0);
        if (childSelectableObjects.Length > 0)
            ChangeSelectedChild(0);
        if (playerNum == 1) {
            TogglePanel(true, false);
        }
        if (instructions) {
            instructions.SetActive(false);
        }
        if (readyLeave) {
            readyLeave.SetActive(false);
        }
    }

    //To make sure that something is selected when the object is enabled
    //void OnEnable() {
    //    if (childSelectableObjects.Length > 0)
    //        ChangeSelectedChild(0);
    //}

    // Update is called once per frame
    void Update() {
        //Controller Player Movement
        Vector2 v = new Vector2(GameInput.Horizontal.GetRawDelayed(playerNum), GameInput.Vertical.GetRawDelayed(playerNum));
        if (!charJoin.HasJoined(playerNum)) //player has joined CHECK
        {
            if (v.y != 0 || v.x != 0) {
                if (v.y != 0) {
                    ChangeSelectedChild(selectedNum - (v.y > 0 ? 1 : -1));
                }

                if (v.x != 0) {
                    selected.OnMoveTrigger(v.x);
                }
            }
        }

        //Controller and Keyboard Player Button Presses
        if ((GameInput.Submit.WasPressed(playerNum) || Input.GetKeyDown(GetKeyCode(playerNum, false, true)))) {
            if (!charJoin.IsActive(playerNum) || (playerNum != 1 && (charJoin.GetAIPlayerPref(playerNum,0) ? false : true)))//Player is not active
                TogglePanel(true, false);
            else {
                if (selected && selected.enabled)//Player active and selected active
                    selected.OnPress();
                if (charJoin.HasJoined(playerNum) && joinedLastFrame) {
                    playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Ready_Exclamation");
                    charJoin.SetReadyPlayer(playerNum);
                }
            }
        }
        joinedLastFrame = charJoin.HasJoined(playerNum);

        if (Input.GetKeyDown(GetKeyCode(playerNum, true, true))) {
            if (joinText.activeSelf)//Player is not active
                ToggleAIPanel(true);
        }
        if (GameInput.Cancel.WasPressed(playerNum) || Input.GetKey(GetKeyCode(playerNum, false, false))) //Back Button Pressed
        {
            if (charJoin.HasJoined(playerNum)) //Set Ready to False
                ToggleReady(false);
            else
                charJoin.CharExit(playerNum); //Set Active to False
        }

    }

    public void TogglePlayer(bool activate) {
        if (player) {
            player.SetActive(activate);
        }
    }

    public void ToggleRock(bool activate) {
        if (rock) {
            rock.SetActive(activate);
        }
    }

    //Jeffrey
    private void InitColor(bool activate) {
        if (!colorChanged && activate) {
            colorChanged = true;
            ChangeSelectedColor(1);
        } else if (!activate) {
            CharacterJoinController.uniqueLoop.ReturnItem(playerNum);
            colorChanged = false;
        }
    }

    public void TogglePanel(bool activate, bool settings) {
        //int totAI = charJoin.ResetAI();
        if (activate) {
            charJoin.SetAIPlayerPref(playerNum, 0);
            player = PlayerSpawner.instance.SpawnPlayer(childSelectableObjects[0].transform.position, playerNum);
            player.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.FreezeAll;
            player.GetComponent<PlayerInfo>().ToggleAim(!activate);
            if (playerNum > 1 && AIPanel.activeSelf)
            {
                ToggleAIPanel(false);
                charJoin.ChangeAINumber(1, playerNum + 1);
                int tot = Utilities.CountTotalAI();
                charJoin.SetupAllAI(tot);
            }
        } else {
            charJoin.RemovePlayer(playerNum);
            Destroy(player);
            joinText.SetActive(true);
            ToggleObjects(activate);
            playerNameImage.gameObject.SetActive(activate);
            int tot = Utilities.CountTotalAI();
            for(int i = 1; i < charJoin.maxPlayers; i++)
            {
                if (charJoin.charPanels[i].AIPanel.activeSelf)
                {
                    charJoin.charPanels[i].ToggleAIPanel(false);
                    PlayerPrefs.SetInt("IsAI" + (i + 1),0);
                }
            }
            for(int i = 0; i < tot; i++)
            {
                charJoin.ChangeAINumber(1,2);
            }
            charJoin.SetupAllAI(tot);
            return;
        }

        InitColor(activate);
        charJoin.SetUnjoinedPlayer(playerNum); //Not sure about this part needing to be here
        ToggleObjects(activate);
        playerNameImage.gameObject.SetActive(activate);
        joinText.SetActive(!activate);


        childSelectableObjects = GetComponentsInChildren<MySelectable>();
        playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
        if (childSelectableObjects.Length > 0) //Select Child
            ChangeSelectedChild(0);

        //charJoin.SetupAllAI(totAI);
    }

    public void ToggleAIPanel(bool activate) {
        InitColor(activate);
        if (!colorChanged && activate) {
            colorChanged = true;
            ChangeSelectedColor(1);
        }
        if (activate) {
            charJoin.SetReadyPlayer(playerNum);
            //Debug.Log("Called");
            charJoin.SetAIPlayer(playerNum);
        } else {
            charJoin.RemovePlayer(playerNum);
        }
        if (AIPanel)
            AIPanel.SetActive(activate);

    }

    private KeyCode GetKeyCode(int num, bool ai, bool activate) //Jeffrey
    {
        if (activate) {
            switch (num) {
                case 1:
                    return !ai ? KeyCode.Alpha1 : KeyCode.F1;
                case 2:
                    return !ai ? KeyCode.Alpha2 : KeyCode.F2;
                case 3:
                    return !ai ? KeyCode.Alpha3 : KeyCode.F3;
                case 4:
                    return !ai ? KeyCode.Alpha4 : KeyCode.F4;
                default:
                    return !ai ? KeyCode.Alpha0 : KeyCode.F10;
            }
        } else {
            switch (num) {
                case 1:
                    return KeyCode.Q;
                case 2:
                    return KeyCode.W;
                case 3:
                    return KeyCode.E;
                case 4:
                    return KeyCode.R;
                default:
                    return KeyCode.Tab;
            }
        }

    }

    public void ChangeSelectedChild(int pos) {
        int len = (settingsActive ? settingsPanelObjects.Length : childSelectableObjects.Length);
        if (pos >= len)
            pos = 0;
        else if (pos < 0)
            pos = len - 1;
        ChangeSelectedObject(pos);
    }

    //For Character Portrait Color Change
    public void ChangeSelectedColor(int change) {
        if (CharacterJoinController.uniqueLoop != null) {
            if (change == 1) {
                currentColor = CharacterJoinController.uniqueLoop.GetNextItem(playerNum);
            } else {
                currentColor = CharacterJoinController.uniqueLoop.GetPreviousItem(playerNum);
            }
            if (player) {
                player.GetComponent<PlayerInfo>().SetColor(currentColor);
            }

            if (AIPanel) {
                AIPanel.transform.Find("Panel/PortraitBackG/PlayerPortrait").GetComponent<Image>().color = currentColor;
            }
            PlayerPrefs.SetFloat("ColorR" + playerNum, currentColor.r);
            PlayerPrefs.SetFloat("ColorG" + playerNum, currentColor.g);
            PlayerPrefs.SetFloat("ColorB" + playerNum, currentColor.b);
        }
    }

    //Not sure if this is neccessary
    public void ChangeSelectedColorArrows(int changeInPos) {
        ChangeSelectedColor(changeInPos);
    }

    public void ToggleReady(bool join) {
        if (instructions)
            instructions.SetActive(join);
        GameObject readyButton = childSelectableObjects[1].gameObject;
        if (!charJoin)
            charJoin = GameObject.FindGameObjectWithTag("CombinedPlayerController").GetComponent<CharacterJoinController>();
        if (join && !charJoin.HasJoined(playerNum)) //Joined and not joined(in charJoin dictionary)
        {
            charJoin.SetJoinedPlayer(playerNum);
            ToggleObjects(!join); //Deactivate old UI Objects or Activate when UnReady
            playerNameImage.gameObject.SetActive(join);
            player.GetComponent<PlayerInfo>().ToggleFreezeMovement(!join);
            player.GetComponent<PlayerInfo>().ToggleAim(join);
            if (!rock) //Jeffrey
            {
                rock = Instantiate(rockPrefab, childSelectableObjects[0].transform.position, Quaternion.identity);
                rock.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.FreezeAll;
                rock.name = "Rock " + playerNum;
            } else {
                rock.SetActive(join);
            }
            instructions.SetActive(join);
            readyLeave.SetActive(join);
        } else if (!join && charJoin.HasJoined(playerNum)) //Not Joined and Joined(in charJoin dictionary)
        {
            charJoin.SetUnjoinedPlayer(playerNum);
            playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
            ToggleObjects(!join);
            player.transform.position = childSelectableObjects[0].transform.position;
            player.GetComponent<PlayerInfo>().ToggleFreezeMovement(!join);
            player.GetComponent<PlayerInfo>().ToggleAim(join);
            player.GetComponent<Blob>().Restart();
            rock.SetActive(join);
            instructions.SetActive(join);
            readyLeave.SetActive(join);
        }
        charJoin.SetAIPlayerPref(playerNum, 0);       //Reset PlayerPrefsAI to 0
    }

    private void ChangeSelectedObject(int pos) {
        if (selected != null) {
            selected.OnMoveAway();
        }

        selected = childSelectableObjects[pos];
        selectedNum = pos;
        if (selected != null) {
            selected.OnMoveTo();
        }
    }

    private void ToggleObjects(bool activate) {
        foreach (MySelectable g in childSelectableObjects) {
            g.gameObject.SetActive(activate);
        }
    }

    public int CompareTo(object obj) {
        return playerNum - ((CharacterPanel)obj).playerNum;
    }

}
