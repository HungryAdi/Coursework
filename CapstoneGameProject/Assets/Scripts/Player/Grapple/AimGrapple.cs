using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AimGrapple : MonoBehaviour {

    private static float rotateScale = 350f;

    private PlayerInfo playerInfo;
    public Transform arrow;
    public SpriteRenderer sr;

    void Start() {
        playerInfo = GetComponentInParent<PlayerInfo>();
    }

    public void ToggleVisibility(bool visible) {
        if (sr) {
            sr.enabled = visible;
        } else {
            sr = arrow.GetComponent<SpriteRenderer>();
            sr.enabled = visible;
        }
        
    }

    void Update() {
        if (Game.instance) {
            ToggleVisibility(!(Game.instance.GameOver() || Game.instance.IsPaused()));
        }
        Vector2 v = new Vector2(GameInput.Horizontal.Get(playerInfo.PlayerNumber), GameInput.Vertical.Get(playerInfo.PlayerNumber));
        if (v != Vector2.zero) {
            float angle = Utilities.GetAngle(v);
            RotateStick(angle);
        } else {
            // Keyboard only works with player 1
            if (playerInfo.PlayerNumber == 1)
            {
            	RotateAim(GameInput.KeyAim.GetRaw(playerInfo.PlayerNumber));
            }
        }
    }

    public Vector2 GetAimDirection() {
        return (transform.rotation * Vector2.up).normalized;
    }

    public void SetAimDirection(Vector2 aimPos) {
        if (arrow) {
            Vector2 aimDir = (aimPos - (Vector2)arrow.transform.position).normalized;
            float angle = Vector2.Angle(Vector2.up, aimDir);
            //Debug.DrawLine(transform.position, transform.position + ((Vector3) aimDir * 5), Color.white, 1);

            transform.rotation = Quaternion.AngleAxis(aimDir.x > 0 ? -angle : angle, Vector3.forward);
        }
    }

    private void RotateAim(float rotateDir) {
        transform.Rotate(0, 0, rotateDir * rotateScale * Time.deltaTime);
    }

    private void RotateStick(float target) {
        transform.rotation = Quaternion.AngleAxis(target, Vector3.forward);
    }
}
