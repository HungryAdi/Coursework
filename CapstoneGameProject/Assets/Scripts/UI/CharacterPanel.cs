using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class CharacterPanel : MonoBehaviour, System.IComparable {
    public CharacterJoinController charJoin;
    public GameObject joinText;
    public GameObject settingsPanel;
    public MySelectable[] childSelectableObjects;
    public GameObject[] panelJoinedObjects;
    public GameObject AIPanel;
    public GameObject instructions;
    public GameObject readyLeave;
    public GameObject rockPrefab;
    public Image playerNameImage;
    public int playerNum;
    public GameObject[] settingsPanelObjects;
    private string readyText = "Ready!";
    private MySelectable selected;
    private int selectedNum;
    private bool settingsActive = false;
    private bool colorChanged;
    public GameObject player;
    private GameObject rock;
    private Color currentColor;
    public bool SetReady = false;
    private bool joinedLastFrame;
    // Use this for initialization
    void Start() {
        colorChanged = false;
        childSelectableObjects = GetComponentsInChildren<MySelectable>();
        playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
        PlayerPrefs.SetInt("IsAI" + playerNum, 0);
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

    void OnEnable() {
        if (childSelectableObjects.Length > 0)
            ChangeSelectedChild(0);
    }

    // Update is called once per frame
    void Update() {
        if (SetReady) {
            charJoin.SetReadyPlayer(playerNum);
            SetReady = false;
        }
        Vector2 v = new Vector2(GameInput.Horizontal.GetRawDelayed(playerNum), GameInput.Vertical.GetRawDelayed(playerNum));
        if (!joinText.activeSelf && !charJoin.HasJoined(playerNum)) {
            if (v.y != 0 || v.x != 0) {
                if (v.y != 0) {
                    ChangeSelectedChild(selectedNum - (v.y > 0 ? 1 : -1));
                }

                if (v.x != 0) {
                    selected.OnMoveTrigger(v.x);
                }
            }
        }

        if ((GameInput.Submit.WasPressed(playerNum) || Input.GetKeyDown(GetKeyCode(playerNum, false, true)))) {
            if (!charJoin.IsActive(playerNum) || (playerNum != 1 && (PlayerPrefs.GetInt("IsAI" + playerNum, 0) == 0 ? false : true)))//Player is not active
            {
                Debug.Log(charJoin.IsActive(playerNum));
                TogglePanel(true, false);
            } else {
                if (selected && selected.enabled) {
                    selected.OnPress();
                }
                if (charJoin.HasJoined(playerNum) && joinedLastFrame) {
                    playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Ready_Exclamation");
                    charJoin.SetReadyPlayer(playerNum);
                }
            }
        }
        joinedLastFrame = charJoin.HasJoined(playerNum);

        if (Input.GetKeyDown(GetKeyCode(playerNum, true, true))) {
            if (joinText.activeSelf)//Player is not active
            {
                ToggleAIPanel(true);
            }
        }
        if (GameInput.Cancel.WasPressed(playerNum) || Input.GetKey(GetKeyCode(playerNum, false, false))) {
            if (charJoin.HasJoined(playerNum)) {
                ToggleReady(false);
            } else {
                charJoin.CharExit(playerNum);
            }
        }

    }

    public void TogglePlayer(bool activate) {
        if (player) {
            player.SetActive(activate);
        }
    }

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
        if (activate) {
            player = PlayerSpawner.instance.SpawnPlayer(panelJoinedObjects[0].transform.position, playerNum - 1);
            player.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.FreezeAll;
            player.GetComponent<PlayerInfo>().ToggleAim(!activate);
        } else {
            charJoin.RemovePlayer(playerNum);
            Destroy(player);
            joinText.SetActive(true);
            ToggleObjects(activate);
            playerNameImage.gameObject.SetActive(activate);
            return;
        }
        InitColor(activate);
        charJoin.SetUnjoinedPlayer(playerNum);
        ToggleObjects(activate);
        playerNameImage.gameObject.SetActive(activate);
        joinText.SetActive(!activate);
        if (AIPanel)
            AIPanel.SetActive(false);
        childSelectableObjects = GetComponentsInChildren<MySelectable>();
        playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
        if (childSelectableObjects.Length > 0)
            ChangeSelectedChild(0);
    }

    public void ToggleAIPanel(bool activate) {
        InitColor(activate);
        if (!colorChanged && activate) {
            colorChanged = true;
            ChangeSelectedColor(1);
        }
        if (activate) {
            charJoin.SetReadyPlayer(playerNum);
        } else {
            charJoin.RemovePlayer(playerNum);
        }
        if (AIPanel)
            AIPanel.SetActive(activate);

    }

    private KeyCode GetKeyCode(int num, bool ai, bool activate) {
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




    public void ChangeSelectedColorArrows(int changeInPos) {
        ChangeSelectedColor(changeInPos);
    }

    public void ToggleReady(bool join) {
        //Debug.Log("ToggleReady");
        if (instructions) {
            instructions.SetActive(join);
        }
        GameObject readyButton = panelJoinedObjects[1];
        if (!charJoin)
            charJoin = GameObject.FindGameObjectWithTag("CombinedPlayerController").GetComponent<CharacterJoinController>();
        if (join && !charJoin.HasJoined(playerNum)) {
            charJoin.SetJoinedPlayer(playerNum);
            ToggleObjects(!join);
            playerNameImage.gameObject.SetActive(join);
            player.GetComponent<PlayerInfo>().ToggleFreezeMovement(!join);
            player.GetComponent<PlayerInfo>().ToggleAim(join);
            if (!rock) {
                rock = Instantiate(rockPrefab, panelJoinedObjects[0].transform.position, Quaternion.identity);
                rock.GetComponent<Rigidbody2D>().constraints = RigidbodyConstraints2D.FreezeAll;
                rock.name = "Rock " + playerNum;
            } else {
                rock.SetActive(join);
            }
            instructions.SetActive(join);
            readyLeave.SetActive(join);
        } else if (!join && charJoin.HasJoined(playerNum)) {
            charJoin.SetUnjoinedPlayer(playerNum);
            playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + playerNum);
            ToggleObjects(!join);
            player.transform.position = panelJoinedObjects[0].transform.position;
            player.GetComponent<PlayerInfo>().ToggleFreezeMovement(!join);
            player.GetComponent<PlayerInfo>().ToggleAim(join);
            player.GetComponent<Blob>().Restart();
            rock.SetActive(join);
            instructions.SetActive(join);
            readyLeave.SetActive(join);
        }

        PlayerPrefs.SetInt("IsAI" + playerNum, 0);

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
        foreach (GameObject g in panelJoinedObjects) {
            g.SetActive(activate);
        }
    }

    public int CompareTo(object obj) {
        return playerNum - ((CharacterPanel)obj).playerNum;
    }

}
