using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MySelectableArrow : MySelectable {

    public Image leftArrow;
    public Image rightArrow;
    public Image centerObject;
    public Sprite ArrowSelected;
    public Sprite ArrowDeselected;
    public Sprite centerObjectSelected;
    public Sprite centerObjectDeselected;

    protected override void Start() {
        hMovement = true;
        rt = GetComponent<RectTransform>();
    }

    public override void ToggleSelection() {
        base.ToggleSelection();
        leftArrow.sprite = selected ? ArrowSelected : ArrowDeselected;
        rightArrow.sprite = selected ? ArrowSelected : ArrowDeselected;
        centerObject.sprite = selected ? centerObjectSelected : centerObjectDeselected;
        if (rt) {
            rt.localScale = selected ? new Vector3(1.25f, 1.25f, 1f) : Vector3.one;
        } else {
            rt = GetComponent<RectTransform>();
        }
    }




}
