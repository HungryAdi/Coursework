using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PunchShooter : MonoBehaviour {

    public float MinPunchForce;
    public float MaxPunchForce;
    public float MinShakeAmount;
    public float MaxShakeAmount;
    public float MinScale;
    public float MaxScale;
    public float ChargeTime;
    public float TimeTillBurnout;
    public float SwipeForce;

    [HideInInspector]
    public Aim aimPunch;
    Transform punchPivot;
    GameObject punchArc;
    PlayerInfo playerInfo;

    [HideInInspector]
    public Punch punch;
    Vector2 initPunchLocalPos;
    Rigidbody2D punchRB2D;
    SpriteRenderer punchRenderer;
    TrailRenderer swipeTrailRenderer;
    float punchTimer = 0f;
    bool finishedFizzling;
    [HideInInspector]
    public bool isFizzling;
    [HideInInspector]
    public bool isPunchLoaded;
    [HideInInspector]
    bool isSwiping;

	void Start ()
	{
        // Fail-safes added here since AI can't punch yet but this is still called.
		Transform punchParent = transform.Find("Punch");
        if (punchParent)
        {
            Transform punchPivot = punchParent.Find("PunchPivot");
            aimPunch = punchPivot.GetComponent<Aim>();
            aimPunch.HorizontalStick = GameInput.RightHorizontal;
            aimPunch.VerticalStick = GameInput.RightVertical;

            punchArc = punchPivot.Find("PunchArc").gameObject;
            punchArc.SetActive(false);

            swipeTrailRenderer = punchParent.GetComponentInChildren<TrailRenderer>();
            swipeTrailRenderer.enabled = false;
        }

		playerInfo = GetComponent<PlayerInfo> ();

		punch = GetComponentInChildren<Punch>();
        if (punch)
        {
            punch.Owner = playerInfo;
            initPunchLocalPos = punch.transform.localPosition;

            punchRB2D = punch.GetComponent<Rigidbody2D>();
            punchRenderer = punch.GetComponent<SpriteRenderer>();
            punchRenderer.color = playerInfo.color;
        }
    }
	
	void Update ()
    {
        punchTimer += Time.deltaTime;

        if (playerInfo
            && playerInfo.CanAct()
            && punch)
        {
            if (!IsFizzling())
            {
                punchRenderer.enabled = true;

                if (GameInput.Swipe.WasPressed(playerInfo.PlayerNumber)
                    && !IsPunchLoaded()
                    && !WasPunchShot())
                {
                    Swipe();
                }
                else if (!IsSwiping()
                    && !WasPunchShot())
                {
                    if (GameInput.UseWeapon.WasPressed(playerInfo.PlayerNumber))
                    {
                        LoadPunch();
                    }
                    else if (GameInput.UseWeapon.WasReleased(playerInfo.PlayerNumber)
                             && !FinishedFizzling()
                             && IsPunchLoaded())
                    {
                        ReleasePunch();
                    }

                    if (GameInput.UseWeapon.IsPressed(playerInfo.PlayerNumber)
                        && IsPunchLoaded()
                        && !FinishedFizzling())
                    {
                        PunchHeld();
                    }
                }
            }
        }
        else if (punchRenderer)
        {
            punchRenderer.enabled = false;
        }

        if (aimPunch.GetRaw() == Vector2.zero
            && !IsSwiping() 
            && !playerInfo.isAI)
        {
            aimPunch.SetAimDirection(Vector2.down);
        }
	}

    public bool IsPunchLoaded()
    {
        return isPunchLoaded;
    }

    public bool WasPunchShot()
    {
        return punch.Shot;
    }

    public bool IsSwiping()
    {
        return isSwiping;
    }

    public bool IsFizzling()
    {
        return isFizzling;
    }

    // Used to make sure we don't launch a fizzled punch.
    bool FinishedFizzling()
    {
        return finishedFizzling;
    }

    public void ResetPunch()
    {
        if (punch)
        {
            punch.transform.parent = aimPunch.transform;
            punch.transform.localPosition = initPunchLocalPos;
            punch.transform.localRotation = Quaternion.identity;
            punch.transform.localScale = new Vector2(MinScale, MinScale);

            punchRB2D.simulated = false;

            punchRenderer.color = Utilities.GetColorWithAlpha(punchRenderer.color, 1);

            punch.playersHit.Clear();
            punch.swipesHit.Clear();

            isFizzling = false;
            isPunchLoaded = false;
            isSwiping = false;
            finishedFizzling = true;
        }
    }

    void Swipe()
    {
        if (!isSwiping)
        {
            StartCoroutine(SwipeRoutine());
        }
    }

    public void LoadPunch()
    {
        ResetPunch();

        punchTimer = 0;
        finishedFizzling = false;
        punch.Shot = false;
        punchRenderer.color = Utilities.GetColorWithAlpha(punchRenderer.color, 1);
        isPunchLoaded = true; // Was released gets called on first frame for some reason.
    }

    public void ReleasePunch()
    {
        isPunchLoaded = false;

        float power = Mathf.Clamp(MinPunchForce + ((punchTimer / ChargeTime) * (MaxPunchForce - MinPunchForce)), MinPunchForce, MaxPunchForce);

        punch.Owner = playerInfo;
        punch.transform.parent = null;

        punchRB2D.simulated = true;
        punchRB2D.AddForce(aimPunch.GetAimDirection() * power);
        punch.ShotDirection = aimPunch.GetAimDirection();
        punch.Shot = true;
        punch.previousLocation = transform.position; // Want initial box cast to be from player to the fist. Allows for melee punches.
        if (punch.Owner)
            punch.Owner.pState = PlayerInfo.PlayerState.Idle;
        StartCoroutine(punch.WaitForPunchStop());
    }

    public void PunchHeld()
    {
        if ((punchTimer - ChargeTime) > TimeTillBurnout)
        {
            StartCoroutine(FizzleOut());
        }
        else
        {
            //Linear time scale
            float scale = Mathf.Clamp(MinScale + ((punchTimer / ChargeTime) * (MaxScale - MinScale)), MinScale, MaxScale);

            punch.transform.localScale = new Vector2(scale, scale);
        }
        if (!playerInfo.alreadyHit)
            playerInfo.pState = PlayerInfo.PlayerState.Punching;
    }

    IEnumerator FizzleOut()
    {
        isFizzling = true;

        float scaleSteps = punch.transform.localScale.x / 20f;
        for (float a = 1; a > 0; a -= 0.05f)
        {
            punchRenderer.color = Utilities.GetColorWithAlpha(punchRenderer.color, a);

            punch.transform.localScale -= new Vector3(scaleSteps, scaleSteps);
            if (punch.transform.localScale.x < 0 || punch.transform.localScale.y < 0)
            {
                punch.transform.localScale = Vector3.zero;
            }

            yield return new WaitForSeconds(0.05f);
        }

        finishedFizzling = true;
        isFizzling = false;

        ResetPunch();
    }

    IEnumerator SwipeRoutine()
    {
        punch.swipedPlayers.Clear();
        punch.Owner = playerInfo;
        isSwiping = true;
        aimPunch.DisableAiming = true;

        float initialRot = aimPunch.transform.localRotation.eulerAngles.z;
        float swipeRange = 60f;
        float swipeTime = 0.25f;

        Quaternion start = Quaternion.Euler(0, 0, aimPunch.transform.rotation.eulerAngles.z - swipeRange);
        Quaternion end = Quaternion.Euler(0, 0, aimPunch.transform.rotation.eulerAngles.z + swipeRange);

        float swipeTimer = 0;
        punchArc.SetActive(true);
        punchArc.transform.localRotation = Quaternion.Euler(0, 0, initialRot); // Arc follows initial angle.
        swipeTrailRenderer.enabled = true;

        while (swipeTimer < swipeTime)
        {
            aimPunch.transform.rotation = Quaternion.Lerp(start, end, swipeTimer / swipeTime);
            yield return new WaitForEndOfFrame();
            swipeTimer += Time.deltaTime;
        }

        swipeTrailRenderer.enabled = false;
        swipeTrailRenderer.Clear();

        punchArc.SetActive(false);
        aimPunch.DisableAiming = false;
        aimPunch.transform.localRotation = Quaternion.Euler(0, 0, initialRot);

        isSwiping = false;

        ResetPunch(); // Sometimes the swipe gets deformed after a while. This resets it.
    }
}
