using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class CharacterJoinController : MonoBehaviour {
    public class CharacterInfo {
        public bool active;
        public bool joined;
        public bool ready;

        public CharacterInfo(bool a, bool j, bool r) {
            active = a;
            joined = j;
            ready = r;
        }
    }
    public static CharacterJoinController instance;
    int maxPlayers = 4;
    public int defaultLives;
    public int minLives = 1;
    public int maxLives = 10;
    public int currentSettingsPlayerNum = 1;
    public Toggle infinteLivesToggle;
    public Toggle powerUpsToggle;
    public GameObject settingsPanel;
    private List<CharacterPanel> charPanels;
    public static UniqueLoop<Color> uniqueLoop;
    private Dictionary<CharacterPanel, bool> ReadyToToggleAIPanels;
    private int lives;
    private Color[] colors;
    private Dictionary<int, CharacterInfo> readyDict = new Dictionary<int, CharacterInfo>();
    // Use this for initialization
    void Start() {
        SetupReadyDict(maxPlayers);
        ReadyToToggleAIPanels = new Dictionary<CharacterPanel, bool>();
        instance = this;
        ChangeLivesNumber(0);
        charPanels = new List<CharacterPanel>(FindObjectsOfType<CharacterPanel>());
        charPanels.Sort();
        colors = new Color[] { Color.red, Color.blue, Color.yellow, Color.green, Color.grey, Color.cyan, Color.magenta, Color.white, new Color(1f, 0.7f, 0.2f), new Color(0f, 0.3f, 0.1f) };
        uniqueLoop = new UniqueLoop<Color>(colors);
        lives = PlayerPrefs.GetInt("NumberOfLives", defaultLives);

        if (powerUpsToggle) {
            powerUpsToggle.isOn = (PlayerPrefs.GetInt("PowerUps", 1) == 1 ? true : false);
        }

    }

    // Update is called once per frame

    public void SetReadyPlayer(int playerNum) {
        readyDict[playerNum - 1] = new CharacterInfo(true, true, true);
        StartGameIfAllPlayersReady();
    }
    public void SetUnjoinedPlayer(int playerNum) {
        readyDict[playerNum - 1] = new CharacterInfo(true, false, false);
    }

    public void SetJoinedPlayer(int playerNum) {
        readyDict[playerNum - 1] = new CharacterInfo(true, true, false);
    }

    public void RemovePlayer(int playerNum) {
        readyDict[playerNum - 1] = new CharacterInfo(false, false, false);
    }

    public bool IsReady(int playerNum) {
        if (readyDict.ContainsKey(playerNum - 1)) {
            return readyDict[playerNum - 1].ready;
        }
        return false;

    }

    public bool IsActive(int playerNum) {
        if (readyDict.ContainsKey(playerNum - 1)) {
            return readyDict[playerNum - 1].active;
        }
        return false;
    }

    public bool HasJoined(int playerNum) {
        if (readyDict.ContainsKey(playerNum - 1)) {
            return readyDict[playerNum - 1].joined;
        }
        return false;
    }

    public int GetNumReadyPlayers() {
        int count = 0;
        if (readyDict != null) {
            foreach (int k in readyDict.Keys) {
                if (readyDict[k].active && readyDict[k].ready) {
                    count++;
                }
            }
        }
        return count;
    }
    bool CheckAllPlayersReady() {
        if (readyDict != null) {
            return GetNumReadyPlayers() == (GetNumActivePlayers() > 0 ? GetNumActivePlayers() : 1);
        }
        return false;
    }

    int GetNumActivePlayers() {
        int count = 0;
        if (readyDict != null) {
            for (int i = 0; i < readyDict.Count; i++) {
                if (readyDict[i].active)
                    count++;
            }
        }
        return count;
    }

    public void StartGameIfAllPlayersReady() {
        if (CheckAllPlayersReady()) {
            PlayerPrefs.SetInt("NumberOfPlayers", GetNumReadyPlayers());
            SceneManager.LoadScene("Main");
        }
    }

    public void ChangeLivesNumber(int change) {
        lives = PlayerPrefs.GetInt("NumberOfLives", defaultLives);
        lives = Mathf.Clamp(lives + change, minLives, maxLives);
        if (lives == 0)
            lives = defaultLives;
        //Debug.Log(lives);
        PlayerPrefs.SetInt("NumberOfLives", lives);
    }

    public void ChangeAINumber(int change) {
        if (change > 0) {
            int pNum = FindNextOpenAI();
            if (pNum < charPanels.Count) {
                ReadyToToggleAIPanels[charPanels[pNum]] = true;
                //Debug.Log("pNum: " + (pNum + 1));
                PlayerPrefs.SetInt("IsAI" + (pNum + 1), 1);
                settingsPanel.GetComponent<SettingsPanelScript>().AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
                //charPanels[pNum].ToggleAIPanel(true);
            }
        } else {
            int pNum = FindNextOpenAI();
            if (pNum > 1) {
                int newNum = pNum - 1;
                ReadyToToggleAIPanels[charPanels[newNum]] = false;
                //Debug.Log("new Num: " + newNum);
                PlayerPrefs.SetInt("IsAI" + (newNum + 1), 0);
                //Debug.Log("Before: " + readyDict.Count);
                RemovePlayer(newNum + 1);

                foreach (int playerNum in readyDict.Keys) {
                    //Debug.Log(playerNum);
                }
                //Debug.Log("After: " + readyDict.Count);

                settingsPanel.GetComponent<SettingsPanelScript>().AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
            }
        }
    }

    public int GetCurrentLives() {
        return lives;
    }

    public void ToggleInfiniteLives()//Used by controller
    {
        infinteLivesToggle.isOn = !infinteLivesToggle.isOn;
    }

    public void TogglePowerUps()//Used by controller
    {
        powerUpsToggle.isOn = !powerUpsToggle.isOn;
    }

    public void ButtonTogglePowerUps()//Used by controller and mouse
    {
        PlayerPrefs.SetInt("PowerUps", (PlayerPrefs.GetInt("PowerUps", 1) == 1 ? 0 : 1));
    }

    public void ToggleSettings(int playerNum) {
        currentSettingsPlayerNum = playerNum;
        settingsPanel.SetActive(!settingsPanel.activeSelf);
        //Debug.Log(Utilities.CountTotalAI());
        SettingsPanelScript sp = settingsPanel.GetComponent<SettingsPanelScript>();
        sp.AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
        foreach (CharacterPanel c in charPanels) {
            if (c.AIPanel)
                c.AIPanel.transform.GetChild(0).gameObject.SetActive(!settingsPanel.activeSelf);
            c.gameObject.SetActive(!settingsPanel.activeSelf);
            c.TogglePlayer(!settingsPanel.activeSelf);
        }
        if (!settingsPanel.activeSelf) {
            foreach (CharacterPanel c in ReadyToToggleAIPanels.Keys) {
                //Debug.Log(c.playerNum);
                //Debug.Log(ReadyToToggleAIPanels[c]);
                c.ToggleAIPanel(ReadyToToggleAIPanels[c]);
            }
            ReadyToToggleAIPanels.Clear();
        }
    }

    public int FindNextOpenAI() {
        for (int i = 1; i < charPanels.Count; i++) {
            if (readyDict[i].active == false) {
                if (ReadyToToggleAIPanels.ContainsKey(charPanels[i])) {
                    if (ReadyToToggleAIPanels[charPanels[i]] == false) {
                        return i;
                    }
                } else {
                    return i;
                }

            }
        }
        return charPanels.Count;
    }

    public void CharExit(int playerNum) {
        if (playerNum != 1 && readyDict[playerNum - 1].active) {
            charPanels[playerNum - 1].TogglePanel(false, false);
            return;
        } else if (playerNum == 1) {
            Navigator.instance.LoadLevel("TitleScene");
        }

    }

    public void SetupReadyDict(int numPlayers) {
        readyDict[0] = new CharacterInfo(true, false, false);
        for (int i = 1; i < numPlayers; i++) {
            readyDict[i] = new CharacterInfo(false, false, false);
        }
    }



}
