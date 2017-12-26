using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
using XInputDotNetPure;
#endif

public class XGameAxis : GameAxis
{
    public static XGameAxis LeftStickHorizonal = new XGameAxis("XInput_LeftStickHorizontal");
    public static XGameAxis LeftStickVertical = new XGameAxis("XInput_LeftStickVertical");
    public static XGameAxis RightStickHorizontal = new XGameAxis("XInput_RightStickHorizontal");
    public static XGameAxis RightStickVertical = new XGameAxis("XInput_RightStickVertical");

    private XGameAxis(string mapping) : base(mapping) { }

    public override float Get(int player = 1)
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        GamePadState state = GamePad.GetState((PlayerIndex)(player - 1));
        if (state.IsConnected)
        {
            switch (mapping)
            {
                case "XInput_LeftStickHorizontal":
                    return state.ThumbSticks.Left.X;
                case "XInput_LeftStickVertical":
                    return state.ThumbSticks.Left.Y;
                case "XInput_RightStickHorizontal":
                    return state.ThumbSticks.Right.X;
                case "XInput_RightStickVertical":
                    return state.ThumbSticks.Right.Y;
            }
        }
#endif

        return 0;
    }

    public override float GetRaw(int player = 1)
    {
        return Mathf.Round(Get(player));
    }

}
