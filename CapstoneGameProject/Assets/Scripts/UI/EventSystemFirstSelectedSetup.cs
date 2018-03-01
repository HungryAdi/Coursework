using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class EventSystemFirstSelectedSetup : MonoBehaviour {

    private EventSystem es;
    public MySelectable[] selectableObjects;
    public bool audioControlsMenu = false;
    public static MySelectable selected;
    private int selectedNum;
    private bool gg = false;
    public static EventSystemFirstSelectedSetup instance;
    private StandaloneInputModule sim;

    public void Start() {
        if (!instance) {
            DontDestroyOnLoad(gameObject);
            instance = this;
        } else if (instance != this) {
            Destroy(gameObject);
        }
        sim = GetComponent<StandaloneInputModule>();
        selectableObjects = Navigator.instance.GetComponentsInChildren<MySelectable>();
        //ChangeToNewSelected();
        if (SceneManager.GetActiveScene().name == "TitleScene") {
            ChangeSelectedChild(0, Navigator.instance.titleScreenMenu);
        }
    }

    public void OnEnable() {
        es = GetComponent<EventSystem>();
        SceneManager.sceneLoaded += OnSceneChange;
    }

    public void OnDisable() {
        SceneManager.sceneLoaded -= OnSceneChange;
    }

    public void OnSceneChange(Scene scene, LoadSceneMode mode)
    {
        selected = null;
    }

    public void Update() {
        if (Game.instance && Game.instance.GameOver() && Game.instance.Started() && !gg) {
            gg = true;
        }
        Vector2 v = new Vector2(GameInput.Horizontal.GetRawDelayed(1), GameInput.Vertical.GetRawDelayed(1));
        if (selected && selected.gameObject.activeSelf)
        {
            if ((SceneManager.GetActiveScene().name == "TitleScene" || SceneManager.GetActiveScene().name == "Main"))
            {
                if (v.x != 0 && SceneManager.GetActiveScene().name == "TitleScene")
                {
                    ChangeSelectedChild(selectedNum - (v.x > 0 ? -1 : 1), Navigator.instance.titleScreenMenu);
                }
                if (SceneManager.GetActiveScene().name == "Main")
                {
                    if (Navigator.instance.gameOverCanvas.activeSelf && v.x != 0)
                    {
                        ChangeSelectedChild(selectedNum - (v.x > 0 ? 1 : -1), Navigator.instance.gameOverCanvas);
                    }
                    if (Navigator.instance.inGameMenu.activeSelf && (v.y != 0 || v.x != 0))
                    {
                        if (v.x != 0)
                        {
                            selected.OnMoveTrigger(v.x);
                        }
                        if (v.y != 0)
                        {
                            if (audioControlsMenu && AudioController.instance)
                            {
                                ChangeSelectedChild(selectedNum - (v.y > 0 ? 1 : -1), AudioController.instance.gameObject);
                            }
                            else
                            {
                                ChangeSelectedChild(selectedNum - (v.y > 0 ? 1 : -1), Navigator.instance.inGameMenu);
                            }
                        }

                    }

                }
                if ((GameInput.Submit.WasPressed(1)))
                {
                    if (selected && selected.enabled)
                    {
                        selected.OnPress();
                    }

                }
            }
        }

    }

    public void ChangeSelectedChild(int pos, GameObject go) {
        if (go) {
            selectableObjects = go.GetComponentsInChildren<MySelectable>();
            if (selectableObjects.Length > 0)
            {
                int len = selectableObjects.Length;
                if (pos >= len)
                    pos = 0;
                else if (pos < 0)
                    pos = len - 1;
                ChangeSelectedObject(pos);
            }

        }
    }

    private void ChangeSelectedObject(int pos) {
        if (selected != null) {
            selected.OnMoveAway();
        }
        if (selectableObjects.Length > 0) {
            selected = selectableObjects[pos];
            selectedNum = pos;
        }

        if (selected != null) {
            selected.OnMoveTo();
        }
    }

    public void SetUpInputs(int playerNum) {
        sim.horizontalAxis = "Horizontal_P" + playerNum;
        sim.verticalAxis = "Vertical_P" + playerNum;
        sim.submitButton = "Submit_P" + playerNum;
        sim.cancelButton = "Cancel_P" + playerNum;
    }
}
