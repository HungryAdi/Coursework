using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
using XInputDotNetPure;
#endif

public class GameInput : MonoBehaviour {

    public static GameButton Shoot = new GameButton("LeftTrigOSX_P", true, 1);
    public static GameButton UseWeapon = new GameButton("RightTrigOSX_P", true, 1);
    public static GameButton Submit = new GameButton("Submit_P");
    public static GameButton Cancel = new GameButton("Cancel_P");
    public static GameButton StartButton = new GameButton("Start_P");
    public static GameButton Swipe = new GameButton("Swipe_P");

    private static GameButton[] axisButtons = { Shoot, UseWeapon };

    public static GameAxis Horizontal = new GameAxis("Horizontal_P");
    public static GameAxis Vertical = new GameAxis("Vertical_P");
    public static GameAxis RightHorizontal = new GameAxis("RightHorizontalOSX_P");
    public static GameAxis RightVertical = new GameAxis("RightVerticalOSX_P");
    public static GameAxis KeyAim = new GameAxis("KeyAim_P");

    private static GameAxis[] gameAxes = { Horizontal, Vertical, RightHorizontal, RightVertical };

    private int numPlayers = 4;

    static GameInput()
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        Shoot = XGameButton.LeftTrigger;
        UseWeapon = XGameButton.RightTrigger;
        Submit = XGameButton.A;
        Cancel = XGameButton.B;
        StartButton = XGameButton.Start;
        Swipe = XGameButton.RightBumper;

        axisButtons = new GameButton[] { Shoot, UseWeapon, Submit, Cancel, StartButton };

        Horizontal = XGameAxis.LeftStickHorizonal;
        Vertical = XGameAxis.LeftStickVertical;
        RightHorizontal = XGameAxis.RightStickHorizontal;
        RightVertical = XGameAxis.RightStickVertical;

        gameAxes = new GameAxis[] { Horizontal, Vertical, RightHorizontal, RightVertical };
#endif
    }

    void Start()
    {
        ResetControlllerRumble();
    }

    void LateUpdate()
    {
        // Replace player<=1 with player<=number of players in game
        for (int player=1; player<=numPlayers; player++)
        {
            foreach (GameAxis gameAxis in gameAxes)
            {
                gameAxis.UpdateTimers(player);
            }

            foreach (GameButton axisButton in axisButtons)
            {
                if (axisButton.isAxisDown[player-1]
                    && axisButton.GetAxisRaw(player) != axisButton.GetOnValue())
                {
                    axisButton.isAxisDown[player-1] = false;
                } else if (axisButton.isAxisUp[player-1]
                    && axisButton.GetAxisRaw(player) == axisButton.GetOnValue())
                {
                    axisButton.isAxisUp[player-1] = false;
                }
            }
        }
    }

    void ResetControlllerRumble()
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        for (int i=0; i<4; i++)
        {
            GamePad.SetVibration((PlayerIndex)i, 0, 0);
        }
#endif
    }

}