using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Game : MonoBehaviour {
    public enum ScreenState { // where the given object is on the screen
        AboveScreen,
        OnScreen,
        BelowScreen
    }

    public enum State { // what state the game is currently in
        Intro,
        Started,
        Paused,
        Over
    }
    public static Game instance; // singleton
    public Text countdown; // text for the starting countdown sequence
    private Transform HUD; // transform that holds the player display prefabs
    public GameObject playerDisplayPrefab; // prefab that shows the player number and remaining lives
    public GameObject offscreenArrowPrefab; // prefab for arrows that show where the offscreen players are
    public Sprite skullSprite; // sprite for infinite lives mode
    public float maxFontSize; // variable for controlling font size during the initial countdown sequence
    private Timer timer; // currently unused timer that used to be for displaying the time spent in the match
    private float introTimer; // variable to keep track of the initial countdown
    private bool playersUpdated; // checks to see if the players list has been updated (called in the first frame of update, since a lot of things need to wait until the end of start)
    private List<PlayerInfo> players; // the players in the game that are currently alive
    private List<PlayerInfo> deadPlayers; // the players in the game that are currently dead
    private List<GameObject> playerDisplays; // a list of the player display prefabs for reference
    private List<PlayerInfo> kingRockList; // which players are attached to the king rock
    private State state; // the state the game is currently in
    // Use this for initialization
    void Start() {
        instance = this;
        timer = FindObjectOfType<Timer>();
        timer.duration = -1;
        if (PlayerPrefs.GetString("GameMode") == "Solo") {
            timer.render = true;
        }
        introTimer = 3.99f;
        HUD = transform.Find("Canvas/HUD");
        players = new List<PlayerInfo>();
        deadPlayers = new List<PlayerInfo>();
        playerDisplays = new List<GameObject>();
        kingRockList = new List<PlayerInfo>();
        playersUpdated = false;
        state = State.Intro;
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Grapple"), LayerMask.NameToLayer("Lava")); // ignore collision between grapple and lava rocks
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Grapple"), LayerMask.NameToLayer("Offscreen")); // ignore collision between grapple and offscreen rocks
        StartCoroutine(RunIntroCountdown());
    }

    // Update is called once per frame
    void Update() {
        if (state == State.Started) {
            UpdatePlayers();
            HandleOffscreenPlayers();
            HandleKingRock();
        }
    }

    // points for king rock
    private void HandleKingRock() {
        if (PlayerPrefs.GetString("GameMode") == "King of the Hill") {
            for (int i = 0; i < players.Count; ++i) {
                UpdateHUD(players[i], playerDisplays[i]);
            }
            if (kingRockList.Count == 1) {
                kingRockList[0].Points += Time.deltaTime * 5;
                if (GameOver() && state != State.Over) { // checks to see if the game is over
                    state = State.Over;
                    int winner = GetWinner();
                    for(int i = 0; i < players.Count; ++i) {
                        if(players[i].PlayerNumber != winner) {
                            RemovePlayer(players[i]);
                        }
                    }
                    StartCoroutine(Navigator.instance.GameOverTransition(winner));
                }
            }
        }
    }
    // only called once after the game starts, updates the players' displays and renders them to the screen
    public void UpdatePlayers() {
        if (!playersUpdated) {
            playersUpdated = true;
            players = new List<PlayerInfo>(FindObjectsOfType<PlayerInfo>());
            players.Sort();
            if (PlayerPrefs.GetString("GameMode") != "Solo") {
                foreach (PlayerInfo pi in players) {
                    GameObject go = Instantiate(playerDisplayPrefab, HUD);
                    playerDisplays.Add(go);
                    InitHUD(pi, go);
                }
            }
        }
    }

    // getter for timer
    public Timer GetTimer() {
        return timer;
    }

    // used by PlayerInfo to get the player display
    public GameObject GetPlayerDisplay(int playerNumber) {
        return playerDisplays[playerNumber];
    }

    // spawns offscreen arrow for the players that are offscreen
    private void HandleOffscreenPlayers() {
        foreach (PlayerInfo pi in players) {
            if (GetScreenState(pi.gameObject) != ScreenState.OnScreen && pi.WasOnScreen()) {
                Vector3 spawnPos = Camera.main.ViewportToWorldPoint(Vector3.up);
                spawnPos.x = pi.transform.position.x;
                spawnPos.y -= 1f; // lazy way to move the arrow down for now
                spawnPos.z = 0;
                GameObject arrow = Instantiate(offscreenArrowPrefab, spawnPos, Quaternion.identity);
                arrow.GetComponent<OffscreenArrow>().pi = pi;
            }
        }
    }

    // initializes the player displays
    private void InitHUD(PlayerInfo pi, GameObject go) {
        if (PlayerPrefs.GetString("GameMode") != "Solo") {
            // set the RectTransform's position
            RectTransform rt = go.GetComponent<RectTransform>();
            float width = rt.anchorMax.x - rt.anchorMin.x;
            rt.anchorMin = new Vector2(pi.PlayerNumber / (players.Count + 1f) - width / 2f, rt.anchorMin.y); // center is where player spawns, offset by half width on both sides
            rt.anchorMax = new Vector2(pi.PlayerNumber / (players.Count + 1f) + width / 2f, rt.anchorMax.y);
            if (PlayerPrefs.GetInt("InfiniteLives") == 1) {
                go.transform.Find("Life Image").GetComponent<Image>().sprite = skullSprite;
            }
            UpdateHUD(pi, go);
        }
    }

    public void AddToKingRock(PlayerInfo pi) {
        kingRockList.Add(pi);
    }
    public bool RemoveFromKingRock(PlayerInfo pi) {
        return kingRockList.Remove(pi);
    }
    // updates the player displays
    public void UpdateHUD(PlayerInfo pi, GameObject go) {
        if (PlayerPrefs.GetString("GameMode") != "Solo" && pi && go) {
            Text playerText = go.transform.Find("Player Text").GetComponent<Text>();
            playerText.text = "P" + pi.PlayerNumber + ": ";
            playerText.color = pi.color;
            Text livesText = go.transform.Find("Life Count Text").GetComponent<Text>();
            livesText.text = "x" + pi.LivesLeft;
            if (PlayerPrefs.GetString("GameMode") == "King of the Hill") {
                livesText.text = "" + (int)pi.Points;
            }
        }
    }
    // checks if the game state is Intro
    public bool RunningIntro() {
        return state == State.Intro;
    }
    // checks if the game state is Started
    public bool Started() {
        return state == State.Started;
    }
    // checks if the game state is Paused
    public bool IsPaused() {
        return state == State.Paused;
    }
    // returns the winner's player number, 0 if no winner
    public int GetWinner() {
        if(PlayerPrefs.GetString("GameMode") == "King of the Hill") {
            for(int i = 0; i < players.Count; ++i) {
                if(players[i].Points >= 100) {
                    return players[i].PlayerNumber;
                }
            }
            return 0;
        }
        if (players.Count == 0) {
            return 0;
        }
        return players[0].PlayerNumber;
    }
    // pauses the game (time scale is set to a very low number so it seems paused but update is still called)
    public void TogglePause(bool activate) {
        state = activate ? State.Paused : State.Started;
        Time.timeScale = activate ? 0.001f : 1f;
    }
    // returns whether or not the game is over
    public bool GameOver() {
        return state == State.Over ||
            (state != State.Intro && players.Count < PlayerPrefs.GetInt("NumberOfPlayers") && players.Count <= 1 && PlayerPrefs.GetInt("InfiniteLives") == 0) ||
            (state != State.Intro && PlayerPrefs.GetString("GameMode") == "King of the Hill" && GetWinner() != 0);

    }
    // gets the list of players currently alive
    public List<PlayerInfo> GetPlayers() {
        return players;
    }
    // gets the list of players currently dead
    public List<PlayerInfo> GetDeadPlayers() {
        return deadPlayers;
    }
    // removes an alive player from the game and puts them in the dead player list
    public PlayerInfo RemovePlayer(PlayerInfo pi) {
        if (players.Remove(pi)) {
            pi.grappleShooter.Detach();
            if (pi.punchShooter) {
                pi.punchShooter.ResetPunch();
            }
            deadPlayers.Add(pi);
            if (GameOver() && state != State.Over) { // checks to see if the game is over after a player is removed
                if(PlayerPrefs.GetString("GameMode") == "Solo") {
                    timer.Pause();
                    timer.render = false;
                }
                state = State.Over;
                StartCoroutine(Navigator.instance.GameOverTransition(GetWinner()));
            }
            return pi;
        }
        return null;
    }
    // clears all of the player displays in the HUD
    public void ClearHUD() {
        foreach (Transform t in HUD) {
            Destroy(t.gameObject);
        }
    }

    // returns whether or not the given GameObject is on the screen
    public static ScreenState GetScreenState(GameObject go) {
        float colSize = go.CompareTag("Player") ? go.GetComponent<Collider2D>().bounds.size.y : go.GetComponent<Collider2D>().bounds.extents.y;
        if (go.transform.position.y > RockSpawner.highestSpawn + colSize) {
            return ScreenState.AboveScreen;
        } else if (go.transform.position.y < RockSpawner.lowestSpawn - colSize) {
            return ScreenState.BelowScreen;
        } else {
            return ScreenState.OnScreen;
        }
    }

    // coroutine for the initial countdown sequence
    IEnumerator RunIntroCountdown() {
        AudioController.instance.PlayAmbient("Countdown2", 0.8f);
        while (introTimer > 0f) {
            introTimer -= Time.deltaTime;
            int intTimer = (int)introTimer; // truncate to nearest integer
            if (introTimer > 1f) { // timer is still going
                countdown.text = "" + intTimer;
            } else { // timer is over
                countdown.text = "GO!";
                countdown.color = (countdown.color == Color.white) ? Color.yellow : Color.white; // flicker between yellow and white
                if (state == State.Intro) {
                    state = State.Started;
                    timer.StartTimer();
                }
            }
            countdown.fontSize = (int)(maxFontSize * (introTimer - intTimer));
            yield return new WaitForSeconds(Time.deltaTime);
        }
        countdown.text = "";
    }
}
