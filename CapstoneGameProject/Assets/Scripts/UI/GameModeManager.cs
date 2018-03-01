using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameModeManager : MonoBehaviour {
    public static string[] modeNameList = { "Classic", "Solo", "Training", };//"King of the Hill"};
    public static string[] descriptionList = {"Last blob standing wins!",
        "Fight against the clock to stay alive as long as possible!",
        "Test out the ropes in this mode where there is stationary rocks and no lava!", };
        //"Battle for control over the central rock!"};
    public Sprite[] spriteList;
    public Image image;
    public Text modeText;
    public Text descriptionText;
    private class GameMode {
        public string mode;
        public string description;
        public Sprite sprite;

        public GameMode(string m, string d, Sprite s) {
            mode = m;
            description = d;
            sprite = s;
        }
    }
    private UniqueLoop<GameMode> gameModeLoop;
    private List<GameMode> gameModeList;
    private GameMode selectedGameMode;
	// Use this for initialization
	void Start () {
        gameModeList = new List<GameMode>();
		for(int i = 0; i < modeNameList.Length; ++i) {
            gameModeList.Add(new GameMode(modeNameList[i], descriptionList[i], spriteList[i]));
        }
        gameModeLoop = new UniqueLoop<GameMode>(gameModeList);
        selectedGameMode = gameModeLoop.GetNextItem(0);
        selectedGameMode = gameModeLoop.GetPreviousItem(0); // so that it always starts with the first item in the list
        UpdateUI();
	}
	
	// Update is called once per frame
	void Update () {
        float x = GameInput.Horizontal.GetRawDelayed();
        if(x == 1) {
            AudioController.instance.PlaySFX("ButtonSelectSound", 0.85f);
            selectedGameMode = gameModeLoop.GetNextItem(0);
            UpdateUI();
        } else if(x == -1) {
            AudioController.instance.PlaySFX("ButtonSelectSound", 0.85f);
            selectedGameMode = gameModeLoop.GetPreviousItem(0);
            UpdateUI();
        }
        if (GameInput.Submit.WasPressed()) {
            PlayerPrefs.SetString("GameMode", selectedGameMode.mode);
            AudioController.instance.PlaySFX("ButtonPressSound", 0.85f);
            switch (selectedGameMode.mode) {
                case "Classic":
                case "King of the Hill":
                    Navigator.instance.LoadLevel("CharacterReadyScene");
                    break;
                case "Solo":
                    PlayerPrefs.SetInt("NumberOfPlayers", 1);
                    PlayerPrefs.SetInt("InfiniteLives", 0);
                    PlayerPrefs.SetInt("NumberOfLives", 1);
                    Navigator.instance.LoadLevel("Main");
                    break;
                case "Training":
                    PlayerPrefs.SetInt("InfiniteLives", 1);
                    PlayerPrefs.SetInt("NumberOfPlayers", Input.GetJoystickNames().Length);
                    Navigator.instance.LoadLevel("TrainingScene");
                    break;
            }
        }
        if (GameInput.Cancel.WasPressed()) {
            Navigator.instance.LoadLevel("TitleScene");
        }
    }
    private void UpdateUI() {
        modeText.text = selectedGameMode.mode;
        descriptionText.text = selectedGameMode.description;
        image.sprite = selectedGameMode.sprite;
    }
}
