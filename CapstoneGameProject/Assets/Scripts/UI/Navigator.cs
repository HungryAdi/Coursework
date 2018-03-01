using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using UnityEngine.Networking;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Navigator : MonoBehaviour {
    // singleton
    public static Navigator instance;
    public GameObject inGameMenu;
    public GameObject titleScreenMenu;
    public GameObject gameOverCanvas;
    public GameObject creditsCanvas;
    public GameObject resumeMenuButton;
    public GameObject musicSliderButton;
    [HideInInspector]
    public bool titleMenuActive;
    [HideInInspector]
    public int menuActivePlayerNum = 0;
    [HideInInspector]
    public bool audioMenuActive = false;
    private float gameOverTimer;
    public float gameOverTransitionTime; // only half of the transition actually
    private GameObject gameOverOverlay;

    void Start() {
        if (!instance) {
            DontDestroyOnLoad(gameObject);
            instance = this;
        } else if (instance != this) {
            Destroy(gameObject);
        }
        inGameMenu.SetActive(false);
        titleScreenMenu.SetActive(false);
        gameOverCanvas.SetActive(false);
        titleMenuActive = false;
        gameOverTimer = gameOverTransitionTime;
        creditsCanvas.SetActive(false);
    }

    void Update() {
        if (SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Main")) {
            if (Input.GetKeyDown(KeyCode.Escape)) {
                PauseGameAndOpenMenu(1);
            }
        }
        if (SceneManager.GetActiveScene() == SceneManager.GetSceneByName("TitleScene")) {
            if (!titleMenuActive) {
                titleMenuActive = true;
                titleScreenMenu.SetActive(titleMenuActive);
                PlayerPrefs.DeleteAll();
            }
            if(creditsCanvas && creditsCanvas.activeSelf) {
                Animation creditsAnim = creditsCanvas.GetComponentInChildren<Animation>();
                if (!creditsAnim.isPlaying) {
                    ToggleCredits(false);
                }
            }
        } else {
            if (titleMenuActive) {
                titleMenuActive = false;
                titleScreenMenu.SetActive(titleMenuActive);
            }
        }
        if (SceneManager.GetActiveScene() == SceneManager.GetSceneByName("TrainingScene")) {
            for(int i = 1; i < PlayerPrefs.GetInt("NumberOfPlayers") + 1; ++i) {
                if (GameInput.Cancel.WasPressed(i)) {
                    LoadLevel("GameModeScene");
                }
            }

        }
    }

    public void LoadLevel(string action) {
        // Loads the scene based on the given string, or exits the game if the string is Quit
        Resume();
        if (action == "Quit") {
            Application.Quit();
            if (Application.isEditor)
            {
                SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
            }
        } else {
            SceneManager.LoadScene(action);
        }
    }

    public void PauseGameAndOpenMenu(int playerNum) {
        if (Game.instance && Game.instance.Started() && !Game.instance.GameOver()) {
            inGameMenu.SetActive(!inGameMenu.activeSelf);
            Game.instance.TogglePause(true);
            menuActivePlayerNum = playerNum;
            if (EventSystemFirstSelectedSetup.instance) {
                EventSystemFirstSelectedSetup.instance.SetUpInputs(playerNum);
                if(EventSystemFirstSelectedSetup.selected == null)
                    EventSystemFirstSelectedSetup.instance.ChangeSelectedChild(0, inGameMenu);
            }
        }

    }

    public void LoadNextLevel() {
        // load the next level (according to the build settings)
        EventSystemFirstSelectedSetup.instance.SetUpInputs(1);
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    public void Resume() {
        if (EventSystemFirstSelectedSetup.selected) {
            EventSystemFirstSelectedSetup.selected.OnMoveAway();
            EventSystemFirstSelectedSetup.selected = null;
        }

        inGameMenu.SetActive(false);
        menuActivePlayerNum = 0;
        if (EventSystemFirstSelectedSetup.instance) {
            EventSystemFirstSelectedSetup.instance.SetUpInputs(1);
        }
        gameOverCanvas.SetActive(false);

        if (audioMenuActive) {
            AudioController.instance.Toggle();
        }

        if (Game.instance) {
            Game.instance.TogglePause(false);
        }
        // unpause the game (used by the resume button)

    }

    public void ShowGameOver(int winner) {
        GameObject bp = gameOverCanvas.transform.Find("ButtonPanel").gameObject;
        bp.SetActive(true);
        GameObject restartButton = bp.transform.Find("RestartButton").gameObject;
        if (EventSystemFirstSelectedSetup.instance) {
            EventSystemFirstSelectedSetup.instance.ChangeSelectedChild(0, bp);
        }
        Text t = gameOverCanvas.transform.Find("Game Over Text").GetComponent<Text>();
        t.text = "Game Over";
        if (winner > 0) {
            t.text += "\n Player " + winner + " wins!";
        }
        if(PlayerPrefs.GetString("GameMode") == "Solo") {
            t.text = "You lasted " + Game.instance.GetTimer().GetTimeLeft().ToString("N1") + " seconds!";
        }

    }

    public void ToggleCredits(bool activate) {
        Debug.Log(creditsCanvas);
        if (creditsCanvas) {
            creditsCanvas.SetActive(activate);
            titleScreenMenu.transform.Find("Background/ButtonPanel").gameObject.SetActive(!activate);
        } else {
            creditsCanvas = GameObject.Find("CreditsCanvas");
            creditsCanvas.SetActive(activate);
            titleScreenMenu.transform.Find("Background/ButtonPanel").gameObject.SetActive(!activate);
        }
    }

    public IEnumerator GameOverTransition(int winner) {
        gameOverCanvas.SetActive(true);
        gameOverCanvas.transform.Find("ButtonPanel").gameObject.SetActive(false);
        Text t = gameOverCanvas.transform.Find("Game Over Text").GetComponent<Text>();
        t.text = "";
        gameOverOverlay = gameOverCanvas.transform.Find("Overlay").gameObject;
        gameOverOverlay.SetActive(true);
        Image overlayImage = gameOverOverlay.GetComponent<Image>();
        Color c = overlayImage.color;
        if (winner > 0) {
            PlayerInfo pi = Game.instance.GetPlayers()[0];
            Game.instance.RemovePlayer(pi);
            pi.gameObject.SetActive(false);
        }
        while (gameOverTimer > 0) {
            gameOverTimer -= Time.deltaTime;
            overlayImage.color = new Color(c.r, c.b, c.g, 1f - gameOverTimer / gameOverTransitionTime);
            yield return new WaitForSeconds(Time.deltaTime);
        }
        RockSpawner.instance.SpawnEndGameRocks();
        Game.instance.ClearHUD();
        while (gameOverTimer < gameOverTransitionTime) {
            gameOverTimer += Time.deltaTime;
            overlayImage.color = new Color(c.r, c.b, c.g, 1f - gameOverTimer / gameOverTransitionTime);
            yield return new WaitForSeconds(Time.deltaTime);
        }
        ShowGameOver(winner);
    }

    public void ToggleMenuStillFrozen() {
        inGameMenu.SetActive(!inGameMenu.activeSelf);
        //if (!inGameMenu.activeSelf)
            //EventSystemFirstSelectedSetup.instance.ChangeToNewSelected(musicSliderButton);
        //else
            //EventSystemFirstSelectedSetup.instance.ChangeToNewSelected(resumeMenuButton);

    }
}
