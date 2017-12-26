using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// May seem weird to write a wrapper class for Axis.
// However different operating systems map to different axis.
// We will deal with that here in the future.
public class GameAxis
{
    protected string mapping;

    //Maps a value to the delay timer map for all players.
    //For instance, If player 1 held the joystick to the right (Horizontal axis = 1):
    //Horizontal's valueDelayTimerMap
    //(1, [0.5, 0, 0, 0]) - Each val in float array gets decremented each frame.
    //GetDelayed returns true if value inside float array <= 0 for specific player.
    public Dictionary<float, float[]> valueDelayTimerMap = new Dictionary<float, float[]>();

    public static float Timer = 0.5f;

    // We want GetRawDelayed to return true for the entire frame.
    bool delayedOnThisFrame = false;

    public GameAxis(string mapping)
    {
        this.mapping = mapping;
    }

    public string GetMapping()
    {
        return mapping;
    }

    public void SetMapping(string mapping)
    {
        this.mapping = mapping;
    }

    public void UpdateTimers(int player = 1)
    {
        // Allows players to use axis like a button as well.
        bool setZero = GetRaw(player) == 0;

        foreach (float val in valueDelayTimerMap.Keys)
        {
            if (setZero)
            {
                valueDelayTimerMap[val][player - 1] = 0;
            }
            else
            {
                valueDelayTimerMap[val][player - 1] -= Time.unscaledDeltaTime;
                valueDelayTimerMap[val][player - 1] = Mathf.Clamp(valueDelayTimerMap[val][player - 1], 0, Timer);
            }
        }

        delayedOnThisFrame = false;
    }

    public virtual float Get(int player = 1)
    {
        return Input.GetAxis(mapping + player);
    }

    public virtual float GetRaw(int player = 1)
    {
        return Mathf.Round(Input.GetAxisRaw(mapping + player));
    }

    public virtual float GetRawDelayed(int player = 1)
    {
        float value = GetRaw(player);

        if (value != 0)
        {
            return IsUsingDelayed(value, player) ? value : 0;
        }

        return 0;
    }

    public bool IsUsingDelayed(float value, int player = 1)
    {
        bool used = GetRaw(player) == value;

        if (used)
        {
            if (!valueDelayTimerMap.ContainsKey(value))
            {
                valueDelayTimerMap.Add(value, new float[4]);
            }

            if (valueDelayTimerMap[value][player - 1] <= 0.0f
                || delayedOnThisFrame)
            {
                delayedOnThisFrame = true;
                valueDelayTimerMap[value][player - 1] = Timer;
                return true;
            }
        }

        return false;
    }

}