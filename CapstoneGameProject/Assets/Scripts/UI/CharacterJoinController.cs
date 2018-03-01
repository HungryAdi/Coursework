using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

//Overall Character Information(Current players, Active Players, etc.)
//Made of Character Panels, in the Character Selection Scene
public class CharacterJoinController : MonoBehaviour {

    //Each players character information
    public class CharacterInfo {
        public bool active;
        public bool joined;
        public bool ready;
        public bool AI;

        public CharacterInfo(bool a, bool j, bool r, bool AI) {
            active = a;
            joined = j;
            ready = r;
            this.AI = AI;
        }
    }

    public static CharacterJoinController instance;

    //SetupVars
    public int maxPlayers = 4;
    private int minPlayers = 2;
    public int defaultLives;
    public int minLives = 1;
    public int maxLives = 10;
    [HideInInspector]
    public int currentSettingsPlayerNum;

    //UI Objects
    public Toggle infinteLivesToggle;
    public Toggle powerUpsToggle;
    public GameObject settingsPanel;

    //Character Join Structures
    public List<CharacterPanel> charPanels;
    public static UniqueLoop<Color> uniqueLoop; //When a color reaches one end of the array return to the other end
    //private Dictionary<CharacterPanel, bool> ReadyToToggleAIPanels; //For storing panels that should become AI panels after settings closed
    private Dictionary<int, CharacterInfo> readyDict = new Dictionary<int, CharacterInfo>(); //Stores all the character panel states

    //Possible Colors for the player, should be changed to skins later probably
    private Color[] colors;


    void Start() {
        instance = this;
        currentSettingsPlayerNum = 1;
        SetupReadyDict(maxPlayers);
        if (PlayerPrefs.GetInt("InfiniteLives") == 1) {
            ToggleInfiniteLives();
        }
        //ReadyToToggleAIPanels = new Dictionary<CharacterPanel, bool>();
        charPanels = new List<CharacterPanel>(FindObjectsOfType<CharacterPanel>());
        charPanels.Sort();
        colors = new Color[] { new Color(0.92f, 0.1f, 0.2f), Color.yellow, Color.cyan, Color.white, new Color(1f, 0.7f, 1f), new Color(1f, 0.56f, 0.1f),
            new Color(0.62f, 0.92f, 0.16f), new Color(0.16f, 0.38f, 0.92f), new Color(0.62f, 0.92f, 0.16f)};
        uniqueLoop = new UniqueLoop<Color>(colors);
        PlayerPrefs.SetInt("NumberOfLives", defaultLives);
        //POWERUPS NOT IMPLEMENTED
        //if (powerUpsToggle) {
        //    powerUpsToggle.isOn = (PlayerPrefs.GetInt("PowerUps", 1) == 1 ? true : false);
        //}

    }

    //Player active, joined and ready
    public void SetReadyPlayer(int playerNum) {
        readyDict[playerNum] = new CharacterInfo(true, true, true,false);
        StartGameIfAllPlayersReady();
    }

    public void SetAIPlayer(int playerNum)
    {
        readyDict[playerNum].AI = true;
    }
    //Player active, not joined, and not ready
    public void SetUnjoinedPlayer(int playerNum) {
        readyDict[playerNum] = new CharacterInfo(true, false, false,false);
    }

    //Player active, joined, and not ready
    public void SetJoinedPlayer(int playerNum) {
        readyDict[playerNum] = new CharacterInfo(true, true, false,false);
    }

    //Player not active, not joined, and not ready
    public void RemovePlayer(int playerNum) {
        //Debug.Log("Remove Player: " + playerNum);
        readyDict[playerNum] = new CharacterInfo(false, false, false,false);
        PlayerPrefs.SetInt("IsAI" + (playerNum), 0);
        //Debug.Log(playerNum - 1);
    }

    //Check if specific player is ready
    public bool IsReady(int playerNum) {
        if (readyDict.ContainsKey(playerNum)) {
            return readyDict[playerNum].ready;
        }
        return false;

    }

    //Check if specific player is active
    public bool IsActive(int playerNum) {
        if (readyDict.ContainsKey(playerNum)) {
            return readyDict[playerNum].active;
        }
        return false;
    }

    //Check if player has joined
    public bool HasJoined(int playerNum) {
        if (readyDict.ContainsKey(playerNum)) {
            return readyDict[playerNum].joined;
        }
        return false;
    }

    //Return the number of total active players
    int GetNumActivePlayers() {
        int count = 0;
        if (readyDict != null) {
            for (int i = 1; i < readyDict.Count + 1; i++) {
                if (readyDict[i].active)
                    count++;
            }
        }
        return count;
    }

    //Return the total number of active, joined, and ready players
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

    //If the total active players = the total ready players return true
    bool CheckAllPlayersReady() {
        if (readyDict != null) {
            return GetNumReadyPlayers() == (GetNumActivePlayers() > 0 ? GetNumActivePlayers() : 1) && GetNumReadyPlayers() >= minPlayers;
        }
        return false;
    }

    //Check if all players ready and start game when they are (runs every update)
    //"NumberOfPlayers"
    public void StartGameIfAllPlayersReady() {
        if (CheckAllPlayersReady()) {
            PlayerPrefs.SetInt("NumberOfPlayers", GetNumReadyPlayers());
            SceneManager.LoadScene("Main");
        }
    }

    //Each player shares the same number of lives through PlayerPrefs
    //"NumberOfLives"
    public void ChangeLivesNumber(int change) {
        int lives = GetCurrentLives();
        lives = Mathf.Clamp(lives + change, minLives, maxLives);
        if (lives == 0)
            lives = defaultLives;
        PlayerPrefs.SetInt("NumberOfLives", lives);
    }

    //Set a normal player to an AI
    //"IsAI"
    public void ChangeAINumber(int change,int startingPos) {
        int pNum = (change <= 0 ? FindNextOpenAI(true,startingPos) : FindNextOpenAI(false,startingPos));
        //Debug.Log(pNum);
        //Debug.Log(charPanels.Count + 1);
        if (change > 0) { //Adding AI
            if (pNum < charPanels.Count + 1 && pNum > 0) {
                //ReadyToToggleAIPanels[charPanels[pNum - 1]] = true;
                SetAIPlayerPref(pNum, 1);
                //Debug.Log(pNum);
                settingsPanel.GetComponent<SettingsPanelScript>().AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
            }
            else
            {
                settingsPanel.GetComponent<SettingsPanelScript>().AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
            }
            return;
        } else {// Removing AI
            if (pNum > 1) {
                int newNum = pNum - 1;
                //ReadyToToggleAIPanels[charPanels[newNum]] = false;
                SetAIPlayerPref(pNum, 0);
                RemovePlayer(newNum + 1);
                settingsPanel.GetComponent<SettingsPanelScript>().AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
            }
        }
    }

    public int GetCurrentLives() {
        return PlayerPrefs.GetInt("NumberOfLives", defaultLives);
    }

    public void ToggleInfiniteLives() //Used by controller
    {
        infinteLivesToggle.isOn = !infinteLivesToggle.isOn;
    }

    //POWERUPS NOT CURRENTLY IMPLEMENTED
    //public void TogglePowerUps() //Used by controller
    //{
    //    powerUpsToggle.isOn = !powerUpsToggle.isOn;
    //}

    //public void ButtonTogglePowerUps() //Used by controller and mouse
    //{
    //    PlayerPrefs.SetInt("PowerUps", (PlayerPrefs.GetInt("PowerUps", 1) == 1 ? 0 : 1));
    //}

    //Settings Panel Toggle(Universal for all players, but only player who opened can used settings)
    public void ToggleSettings() {
        //USED FOR MULTIPLE SETTINGS BUTTONS FOR MULTIPLE PLAYERS
        //currentSettingsPlayerNum = playerNum;

        //Settings for main player

        settingsPanel.SetActive(!settingsPanel.activeSelf);
        SettingsPanelScript sp = settingsPanel.GetComponent<SettingsPanelScript>();
        sp.AIText.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
        foreach (CharacterPanel c in charPanels) {
            if (c.AIPanel)
                c.AIPanel.transform.GetChild(0).gameObject.SetActive(!settingsPanel.activeSelf);
            c.gameObject.SetActive(!settingsPanel.activeSelf);
            c.TogglePlayer(!settingsPanel.activeSelf);
            c.ToggleRock(!settingsPanel.activeSelf);
        }
        //Set AI Panels when settings panel is turned off
        if (!settingsPanel.activeSelf) {
            int tot = ResetAI();
            SetupAllAI(tot);
        //    foreach (CharacterPanel c in ReadyToToggleAIPanels.Keys) {
        //        c.ToggleAIPanel(ReadyToToggleAIPanels[c]);
        //    }
        //    ReadyToToggleAIPanels.Clear();
        }
    }

    //Find the next inactive spot to place an AI
    public int FindNextOpenAI(bool remove, int startingPos) {
        //Debug.Log("next open AI");
        for (int i = startingPos; i < charPanels.Count + 1; i++) {
            //Debug.Log("Beggining of for loop: " + i);
            if (readyDict[i].active == false)
            {
                //Debug.Log("PlayerPref: " + (i) + (GetAIPlayerPref(i, 0)));
                if (GetAIPlayerPref(i,0))
                {
                    if (remove)
                        return i - 1;
                    else
                        return i;
                }

            }
        }
        return charPanels.Count;
    }

    //If player one return to main menu, otherwise set player inactive
    public void CharExit(int playerNum) {
        if (playerNum != 1 && readyDict[playerNum].active) {
            charPanels[playerNum - 1].TogglePanel(false, false);
            //SetupAllAI(ResetAI());
            return;
        } else if (playerNum == 1) {
            Navigator.instance.LoadLevel("GameModeScene");
        }

    }

    //Set all players except player one to inactive, not joined, and not ready
    //Set player one to active, not joined, not ready
    public void SetupReadyDict(int numPlayers) {
        readyDict[1] = new CharacterInfo(true, false, false,false);
        for (int i = 2; i < numPlayers + 1; i++) {
            readyDict[i] = new CharacterInfo(false, false, false,false);
        }
    }

    public int ResetAI()
    {

        int totAI = Utilities.CountTotalAI();
        for (int i = 2; i < readyDict.Count + 1; i++)
        {
            if (charPanels[i - 1].AIPanel.activeSelf)
            {
                //Debug.Log("Reset Panel: " + i);
                //ChangeAINumber(-1);
                charPanels[i - 1].AIPanel.SetActive(false);
            }
        }
        return totAI;



    }

    public void SetupAllAI(int totAI)
    {
        //Debug.Log("Called: ");
        if(charPanels != null)
        {
            ////Debug.Log(totAI);
            //for (int i = 0; i < totAI; i++)
            //{
            //    ChangeAINumber(1);
            //}

            foreach (CharacterPanel c in charPanels)
            {
                if (GetAIPlayerPref(c.playerNum, 1))
                {
                    //Debug.Log("Player Num: " + c.playerNum);
                    c.ToggleAIPanel(true);
                }
            }
        }

    }

    public void SetAIPlayerPref(int pNum,int value)
    {
        PlayerPrefs.SetInt("IsAI" + pNum, value);
    }

    public bool GetAIPlayerPref(int pNum, int value)
    {
        return PlayerPrefs.GetInt("IsAI" + pNum, 0) == value;
    }




}
