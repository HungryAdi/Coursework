using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Aim : MonoBehaviour {

    private static float rotateScale = 350f;

    private PlayerInfo playerInfo;
    public Transform toAim;
    public SpriteRenderer sr;
    public bool DisableAiming;

    public GameAxis HorizontalStick = GameInput.Horizontal;
    public GameAxis VerticalStick = GameInput.Vertical;

    void Start() {
        playerInfo = GetComponentInParent<PlayerInfo>();
    }

    public void ToggleVisibility(bool visible) {
        if (sr) {
            sr.enabled = visible;
        } else {
            sr = toAim.GetComponent<SpriteRenderer>();
            sr.enabled = visible;
        }
    }

    void Update() {
        if (Game.instance) {
            ToggleVisibility(!(Game.instance.GameOver() || Game.instance.IsPaused()));
        }

        if (!DisableAiming)
        {
            Vector2 v = new Vector2(HorizontalStick.Get(playerInfo.PlayerNumber), VerticalStick.Get(playerInfo.PlayerNumber));
            if (v != Vector2.zero)
            {
                float angle = Utilities.GetAngle(v);
                RotateStick(angle);
            }
        }
    }

    // Raw stick values for the aim. Mainly used to see if the player is trying to aim at all.
    public Vector2 GetRaw()
    {
        if (playerInfo)
        {
            return new Vector2(HorizontalStick.Get(playerInfo.PlayerNumber), VerticalStick.Get(playerInfo.PlayerNumber));
        }
        else
        {
            return Vector2.zero;
        }
    }

    public Vector2 GetAimDirection() {
        return (transform.rotation * Vector2.up).normalized;
    }

    // Used if you want to aim at an object. aimPos would be the position of the object you are aiming at.
    public void AimAt(Vector2 aimPos) {
        Vector2 aimDir = (aimPos - (Vector2) transform.position).normalized;
        float angle = Utilities.GetAngle(aimDir);

        transform.rotation = Quaternion.Euler(0, 0, angle);

        //Vector2 aimDir = (aimPos - (Vector2)toAim.transform.position).normalized;
        //float angle = Vector2.Angle(Vector2.up, aimDir);
        Debug.DrawLine(transform.position, transform.position + ((Vector3) aimDir * 5), Color.white, 1);

        //transform.rotation = Quaternion.AngleAxis(aimDir.x > 0 ? -angle : angle, Vector3.forward);
    }

    // Used if you want to force set the aim to a position. aimDir = Vector2.up if you want to aim straight up.
    public void SetAimDirection(Vector2 aimDir)
    {
        float angle = Utilities.GetAngle(aimDir);
        RotateStick(angle);
    }

    public void RotateAim(float rotateDir) {
        transform.Rotate(0, 0, rotateDir * rotateScale * Time.deltaTime);
    }

    private void RotateStick(float target) {
        transform.rotation = Quaternion.AngleAxis(target, Vector3.forward);
    }
}
