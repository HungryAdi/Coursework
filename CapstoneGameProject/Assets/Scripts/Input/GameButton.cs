using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameButton {

    protected string mapping;
    protected bool isAxis;
    protected float onValue;

    // Will need to change 4 to the number of players eventually if needed.
    public bool[] isAxisDown = new bool[4];
    public bool[] isAxisUp = new bool[4];

    public GameButton(string mapping, bool isAxis = false, float onValue = 1)
    {
        this.mapping = mapping;
        this.isAxis = isAxis;
        this.onValue = onValue;
    }

    public string GetMapping()
    {
        return mapping;
    }

    public void SetMapping(string mapping)
    {
        this.mapping = mapping;
    }

    public virtual float GetAxisRaw(int player = 1)
    {
        if (isAxis)
        {
            return Mathf.Round(Input.GetAxisRaw(mapping + player));
        }

        return 0;
    }

    public float GetOnValue(int player = 1)
    {
        return onValue;
    }

    public virtual bool IsPressed(int player = 1)
    {
        if (isAxis
            && GetAxisRaw(player) == onValue)
        {
            return true;
        }

        return isAxis ? false : Input.GetButton(mapping + player);
    }

    public virtual bool WasPressed(int player = 1)
    {
        if (isAxis
            && !isAxisDown[player-1]
            && GetAxisRaw(player) == onValue)
        {
            isAxisDown[player-1] = true;
            return true;
        }
        
        return isAxis ? false : Input.GetButtonDown(mapping + player);
    }

    public virtual bool WasReleased(int player = 1)
    {
        if (isAxis
            && !isAxisUp[player-1]
            && GetAxisRaw(player) != onValue)
        {
            isAxisUp[player-1] = true;
            return true;
        }

        return isAxis ? false : Input.GetButtonUp(mapping + player);
    }

}