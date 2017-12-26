using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
using XInputDotNetPure;
#endif

public class XGameButton : GameButton
{
    public static XGameButton LeftTrigger = new XGameButton("XInput_LeftTrigger", 1);
    public static XGameButton RightTrigger = new XGameButton("XInput_RightTrigger", 1);
    public static XGameButton A = new XGameButton("XInput_A", 0);
    public static XGameButton B = new XGameButton("XInput_B", 0);
    public static XGameButton Start = new XGameButton("XInput_Start", 0);
    public static XGameButton RightBumper = new XGameButton("XInput_RightBumper", 0);

    private XGameButton(string mapping, float axisOnValue) : base(mapping, true, axisOnValue) { }

    public override float GetAxisRaw(int player = 1)
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        GamePadState state = GamePad.GetState((PlayerIndex)(player - 1));
        float val = 0;
        if (state.IsConnected)
        {
            switch (mapping)
            {
                case "XInput_LeftTrigger":
                    val = state.Triggers.Left;
                    break;
                case "XInput_RightTrigger":
                    val = state.Triggers.Right;
                    break;
                case "XInput_A":
                    val = (float)state.Buttons.A;
                    break;
                case "XInput_B":
                    val = (float)state.Buttons.B;
                    break;
                case "XInput_Start":
                    val = (float)state.Buttons.Start;
                    break;
                case "XInput_RightBumper":
                    val = (float) state.Buttons.RightShoulder;
                    break;
            }

            return Mathf.Round(val);
        }
#endif

        return 1 - onValue; // TODO make a offValue parameter that we can return here instead.
    }

}