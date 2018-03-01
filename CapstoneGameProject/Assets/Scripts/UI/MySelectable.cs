using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;

public class MySelectable : Selectable {
    public UnityEvent unityEvent;
    public GameObject target;
    protected bool selected = false;
    protected bool press = false;
    protected bool hMovement = false;
    protected RectTransform rt;

    public virtual void OnMoveTo()
    {
        //Debug.Log("Move to: " + name);
        ToggleSelection();
    }

    public virtual void OnMoveAway()
    {
		AudioController.instance.PlaySFX ("ButtonSelectSound",0.85f);
        ToggleSelection();
    }

    public virtual void ToggleSelection()
    {
		
        selected = !selected;
        ChangeSize(selected);
    }

    public virtual void OnMoveTrigger(float movement)
    {
        if (hMovement)
        {
            string eventName = unityEvent.GetPersistentMethodName(0);
            if(target != null)
                target.BroadcastMessage(eventName, movement);
                
        }
    }

    public virtual void OnPress()
    {
        if (press)
        {
			AudioController.instance.PlaySFX ("ButtonPressSound",0.85f);
            //Debug.Log(name);
            unityEvent.Invoke();
        }
    }

    public void ChangeSelectedLives(int change)
    {
        CharacterJoinController.instance.ChangeLivesNumber(change);
        Image i = transform.GetChild(0).GetComponent<Image>();
        i.sprite = Resources.Load<Sprite>("LobbyUI/" + CharacterJoinController.instance.GetCurrentLives());
    }

    public void ChangeNumAIs(int change)
    {
        CharacterJoinController.instance.ChangeAINumber(change,1);
        Image i = transform.GetChild(0).GetComponent<Image>();
        i.sprite = Resources.Load<Sprite>("LobbyUI/" + Utilities.CountTotalAI());
    }

    void ChangeSize(bool MovingTo)
    {
        Selectable selectable;
        RectTransform rt;
        selectable = GetComponent<Selectable>();
        rt = GetComponent<RectTransform>();
        if(MovingTo)
            rt.localScale = new Vector3(1.25f, 1.25f, 1f);
        else
        rt.localScale = Vector3.one;
    }
}
