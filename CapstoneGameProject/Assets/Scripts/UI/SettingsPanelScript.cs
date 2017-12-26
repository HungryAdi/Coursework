using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsPanelScript : MonoBehaviour {

    public static SettingsPanelScript instance;
    public MySelectable[] childSelectableObjects;
    public Image AIText;
    public Image playerNameImage;
    public MySelectable selected;
    private int selectedNum = 0;

    void Start()
    {
        instance = this;
        childSelectableObjects = GetComponentsInChildren<MySelectable>();
        ChangeSelectedChild(0);
        playerNameImage.sprite = Resources.Load<Sprite>("LobbyUI/Player-" + CharacterJoinController.instance.currentSettingsPlayerNum);


    }

    void Update()
    {
        int pNum = CharacterJoinController.instance.currentSettingsPlayerNum;
        Vector2 v = new Vector2(GameInput.Horizontal.GetRawDelayed(pNum), GameInput.Vertical.GetRawDelayed(pNum));
        if ((v.y != 0 || v.x != 0))
        {
            if (v.y != 0)
            {
                ChangeSelectedChild(selectedNum - (v.y > 0 ? 1 : -1));
            }

            if (v.x != 0)
            {
                selected.OnMoveTrigger(v.x);
                //if (selected == panelJoinedObjects[0] && !settingsActive) {
                //    ChangeSelectedColor((v.x > 0 ? 1 : -1));
                //} else if (settingsPanel && selected == settingsPanelObjects[0]) {
                //    ChangeSelectedLives(v.x > 0 ? 1 : -1);
                //}
            }
        }

        if (selected && selected.enabled && (GameInput.Submit.WasPressed(pNum) || Input.GetKeyDown(GetKeyCode(pNum, false, true))))
        {
            selected.OnPress();
        }
    }

    public void ChangeSelectedChild(int pos)
    {
        int len = childSelectableObjects.Length;
        if (pos >= len)
            pos = 0;
        else if (pos < 0)
            pos = len - 1;

        ChangeSelectedObject(pos);
    }

    private void ChangeSelectedObject(int pos)
    {
        if (selected != null)
        {
            selected.OnMoveAway();
        }

        selected = childSelectableObjects[pos];
        selectedNum = pos;


        if (selected != null)
        {
            selected.OnMoveTo();
        }

    }

    public void ToggleInfiniteLives(GameObject check)
    {
        PlayerPrefs.SetInt("InfiniteLives", PlayerPrefs.GetInt("InfiniteLives", 0) == 0 ? 1 : 0);
        check.SetActive(PlayerPrefs.GetInt("InfiniteLives", 0) == 0 ? false : true);
        childSelectableObjects = GetComponentsInChildren<MySelectable>();

    }


    private KeyCode GetKeyCode(int num, bool ai, bool activate)
    {
        if (activate)
        {
            switch (num)
            {
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
        }
        else
        {
            switch (num)
            {
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

}
