using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MyButton : MySelectable {

    public Sprite selectImage;
    public Sprite deselectImage;


    public bool toggle;
    public string playerPrefsName;
    public GameObject checkMark;

    protected override void Start() {
        press = true;
        if (toggle) {
            SettingsPanelScript.instance.ToggleInfiniteLives(checkMark);
            SettingsPanelScript.instance.ToggleInfiniteLives(checkMark);
        }
        rt = GetComponent<RectTransform>();
    }
    public override void ToggleSelection() {
        base.ToggleSelection();
        Image i = GetComponentInChildren<Image>();
        if (i) {
            i.sprite = selected ? selectImage : deselectImage;
        }
        Text t = GetComponentInChildren<Text>();
        //if (t) {
        //    t.color = selected ? new Color(75/256f, 75/256f, 75/256f) : Color.white;
        //}

    }
}
