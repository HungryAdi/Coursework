using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MySlider : MySelectable {

    public int changeVal = 5;
    public Image backG;
    public int value;
    public int minVal;
    public int maxVal;
    public RectTransform circleSlider;
    public GameObject sliderTarget;


    protected override void Start() {
        hMovement = true;
        //ChangeSliderValue(value);
        //SetCircleSlider(Mathf.RoundToInt(value));
    }

    public override void ToggleSelection() {
        //base.ToggleSelection();
        selected = !selected;
        circleSlider.GetComponent<RectTransform>().localScale = selected ? new Vector3(1.25f, 1.25f, 1f) : Vector3.one;
    }


    public void ChangeSliderValue(float movement) {
        if(movement != 0) {
            int sign;
            sign = (movement > 0 ? 1 : -1);

            value = Mathf.Clamp(value + sign * changeVal, minVal, maxVal);
        }

        SetCircleSlider(((float)(minVal - value) / (minVal - maxVal)));
        string eventName = unityEvent.GetPersistentMethodName(1);
        if (sliderTarget != null) {
            sliderTarget.BroadcastMessage(eventName, value);
        }
    }

    public void SetCircleSlider(float percent) {
        RectTransform rt = GetComponent<RectTransform>();
        float width = rt.rect.width;
        float pos = width * percent * 0.95f;
        Debug.Log(pos);
        circleSlider.anchoredPosition = new Vector2(rt.anchoredPosition.x + pos, circleSlider.anchoredPosition.y);
    }

}
