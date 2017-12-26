using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PunchShooter : MonoBehaviour {

    public GameObject PunchPrefab;
    public float MinPunchForce;
    public float MaxPunchForce;
    public float MinShakeAmount;
    public float MaxShakeAmount;
    public float MinScale;
    public float MaxScale;
    public float ChargeTime;
    public float TimeTillBurnout;

    AimGrapple aimGrapple;
    Transform arrow;
    PlayerInfo playerInfo;

    [HideInInspector]
    public Punch punch;
    Rigidbody2D punchRB2D;
    ShakeEffect punchShake;
    float punchTimer = 0f;
    bool finishedFizzling;
    bool isFizzling;

	void Start ()
	{
		aimGrapple = GetComponentInChildren<AimGrapple> ();
		arrow = aimGrapple.transform.GetChild (0); // Should be the arrow.
		playerInfo = GetComponent<PlayerInfo> ();

		punch = Instantiate (PunchPrefab).GetComponent<Punch> ();

		punch.gameObject.SetActive (false);
		punch.GetComponent<SpriteRenderer> ().color = playerInfo.color;
		punch.Owner = playerInfo;

		punchRB2D = punch.GetComponent<Rigidbody2D> ();
		punchShake = punch.GetComponent<ShakeEffect> ();

	}
	
	void Update ()
    {
        if (isFizzling)
        {
            UpdatePunchPosition();
            return;
        }

        punchTimer += Time.deltaTime;

        if (playerInfo.CanAct()
            && playerInfo)
        {
            if (GameInput.UseWeapon.WasPressed(playerInfo.PlayerNumber))
            {
                punch.gameObject.SetActive(true);
                punch.transform.localScale = new Vector2(MinScale, MinScale);
                punchRB2D.velocity = Vector2.zero;
                punchShake.ShakeAmount = MinShakeAmount;
                punchTimer = 0;
                finishedFizzling = false;
                punch.Shot = false;
                punch.GetComponent<SpriteRenderer>().color = Utilities.GetColorWithAlpha(punch.GetComponent<SpriteRenderer>().color, 1);
            }
            else if (GameInput.UseWeapon.WasReleased(playerInfo.PlayerNumber)
                     && !finishedFizzling)
            {
                float power = Mathf.Clamp(MinPunchForce + ((punchTimer / ChargeTime) * (MaxPunchForce - MinPunchForce)), MinPunchForce, MaxPunchForce);

                punchRB2D.AddForce(aimGrapple.GetAimDirection() * power);
                punchShake.ShakeAmount = 0;
                punch.ShotDirection = aimGrapple.GetAimDirection();
                punch.Shot = true;
                StartCoroutine(punch.WaitForPunchStop());
            }

            if (GameInput.UseWeapon.IsPressed(playerInfo.PlayerNumber)
                && !finishedFizzling)
            {
                if ((punchTimer - ChargeTime) > TimeTillBurnout)
                {
                    StartCoroutine(FizzleOut());
                }
                else
                {
                    float shake = Mathf.Clamp(MinShakeAmount + ((punchTimer / ChargeTime) * (MaxShakeAmount - MinShakeAmount)), MinShakeAmount, MaxShakeAmount);
                    punchShake.ShakeAmount = shake;

                    float scale = Mathf.Clamp(MinScale + ((punchTimer / ChargeTime) * (MaxScale - MinScale)), MinScale, MaxScale);
                    punch.transform.localScale = new Vector2(scale, scale);

                    UpdatePunchPosition();
                }
            }
        }
        else if (punch.gameObject.activeSelf)
        {
            punch.gameObject.SetActive(false);
        }
	}

    public void DeactivatePunch()
    {
        punch.gameObject.SetActive(false);
    }

    void UpdatePunchPosition()
    {
        punch.transform.position = arrow.transform.position;
        punch.transform.rotation = arrow.transform.rotation;
    }

    IEnumerator FizzleOut()
    {
        isFizzling = true;

        punchShake.ShakeAmount = 0;
        float scaleSteps = punch.transform.localScale.x / 20f;
        SpriteRenderer sr = punch.GetComponent<SpriteRenderer>();

        for (float a = 1; a > 0; a -= 0.05f)
        {
            sr.color = Utilities.GetColorWithAlpha(sr.color, a);
            punch.transform.localScale -= new Vector3(scaleSteps, scaleSteps);

            yield return new WaitForSeconds(0.05f);
        }

        finishedFizzling = true;
        isFizzling = false;

        DeactivatePunch();
    }
}
