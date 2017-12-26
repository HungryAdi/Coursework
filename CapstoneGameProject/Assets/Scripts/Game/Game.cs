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
    public static Game instance;
    public Text countdown;
    private Transform HUD;
    public GameObject playerDisplayPrefab;
    public GameObject offscreenArrowPrefab;
    public Sprite skullSprite;
    public float maxFontSize;
    private Timer timer;
    private float introTimer;
    private bool playersUpdated;
    private List<PlayerInfo> players;
    private List<PlayerInfo> deadPlayers;
    private List<GameObject> playerDisplays;
    private State state;
    // Use this for initialization
    void Start() {
        instance = this;
        timer = FindObjectOfType<Timer>();
        timer.duration = -1;
        introTimer = 3.99f;
        HUD = transform.Find("Canvas/HUD");
        players = new List<PlayerInfo>();
        deadPlayers = new List<PlayerInfo>();
        playerDisplays = new List<GameObject>();
        playersUpdated = false;
        state = State.Intro;
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Grapple"), LayerMask.NameToLayer("Lava")); // ignore collision between grapple and lava rocks
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Grapple"), LayerMask.NameToLayer("Offscreen")); // ignore collision between grapple and offscreen rocks
        StartCoroutine(RunIntroCountdown());
    }

    // Update is called once per frame
    void Update() {
        if (state == State.Started) {
            if (!playersUpdated) {
                playersUpdated = true;
                UpdatePlayers();
            }
            HandleOffscreenPlayers();
            if (GameOver()) {
                StartCoroutine(Navigator.instance.GameOverTransition(GetWinner()));
                state = State.Over;
            }
        }
    }
    // only called once after the game starts
    public void UpdatePlayers() {
        players = new List<PlayerInfo>(FindObjectsOfType<PlayerInfo>());
        players.Sort();
        foreach (PlayerInfo pi in players) {
            GameObject go = Instantiate(playerDisplayPrefab, HUD);
            playerDisplays.Add(go);
            InitHUD(pi, go);
        }
    }

    public GameObject GetPlayerDisplay(int playerNumber) {
        return playerDisplays[playerNumber];
    }

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

    private void InitHUD(PlayerInfo pi, GameObject go) {
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

    public void UpdateHUD(PlayerInfo pi, GameObject go) {
        Text playerText = go.transform.Find("Player Text").GetComponent<Text>();
        playerText.text = "P" + pi.PlayerNumber + ": ";
        playerText.color = pi.color;
        Text livesText = go.transform.Find("Life Count Text").GetComponent<Text>();
        livesText.text = "x" + pi.LivesLeft;
    }

    public bool RunningIntro() {
        return state == State.Intro;
    }
    public bool Started() {
        return state == State.Started;
    }
    public bool IsPaused() {
        return state == State.Paused;
    }

    public int GetWinner() {
        if (players.Count == 0) {
            return 0;
        }
        return players[0].PlayerNumber;
    }

    public void SetState(State s) {
        state = s;
    }

    public void TogglePause(bool activate) {
        state = activate ? State.Paused : (GameOver() ? State.Over : State.Started);
        Time.timeScale = activate ? 0.001f : 1f;
    }

    public bool GameOver() {
        return state != State.Intro && players.Count < PlayerPrefs.GetInt("NumberOfPlayers") && players.Count <= 1 && PlayerPrefs.GetInt("InfiniteLives") == 0;

    }

    public List<PlayerInfo> GetPlayers() {
        return players;
    }

    public List<PlayerInfo> GetDeadPlayers() {
        return deadPlayers;
    }

    public PlayerInfo RemovePlayer(PlayerInfo pi) {
        if (players.Remove(pi)) {
            pi.grappleShooter.Detach();
            if (pi.punchShooter) {
                pi.punchShooter.DeactivatePunch();
            }
            deadPlayers.Add(pi);
            return pi;
        }
        return null;
    }

    public void ClearHUDs() {
        foreach(Transform t in HUD) {
            Destroy(t.gameObject);
        }
    }

    // returns whether or not the given GameObject is on the screen
    public static ScreenState GetScreenState(GameObject go) {
        float colSize = go.CompareTag("Player") ? go.GetComponent<Collider2D>().bounds.size.y : go.GetComponent<Collider2D>().bounds.extents.y;
        if (go.transform.position.y > Camera.main.ViewportToWorldPoint(Vector3.up).y + colSize) {
            return ScreenState.AboveScreen;
        } else if (go.transform.position.y < Camera.main.ViewportToWorldPoint(Vector3.zero).y - colSize) {
            return ScreenState.BelowScreen;
        } else {
            return ScreenState.OnScreen;
        }
    }

    IEnumerator RunIntroCountdown() {
		AudioController.instance.PlayAmbient ("Countdown2", 0.8f);
        while(introTimer > 0f) {
            introTimer -= Time.deltaTime;
            int intTimer = (int)introTimer; // truncate to nearest integer
            if (introTimer > 1f) {
                countdown.text = "" + intTimer;
            } else {
                countdown.text = "GO!";
                countdown.color = (countdown.color == Color.white) ? Color.yellow : Color.white;
                if(state == State.Intro) {
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
