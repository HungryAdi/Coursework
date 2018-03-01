using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Punch : MonoBehaviour
{
    public PlayerInfo Owner;
    public float ForceScale;
    public float MinPushForce;
    public float MaxPushForce;

    [HideInInspector]
    public bool Shot;
    [HideInInspector]
    public Vector2 ShotDirection;
    [HideInInspector]
    public PunchShooter punchShooter;
    [HideInInspector]
    public Vector2 previousLocation;

    public Dictionary<int, PlayerInfo> playersHit = new Dictionary<int, PlayerInfo>(); // Using this so that players are hit only once.
    public Dictionary<int, Vector2> swipesHit = new Dictionary<int, Vector2>(); // Using this so that we are deflected by a swipe only once. Maps to hit normal.

    Rigidbody2D rb2d;

    static float WaitAfterShot = 0.05f;
    static float AlmostStopped = 0.025f;
    static float SwipeDeflect = 1f;
    static float StunLength = 1f;

    HashSet<int> punchedPlayers = new HashSet<int>();

    // Make sure the same player doesn't get Swiped more than once per swipe. Gets cleared upon swipe begin within PunchShooter.cs
    public HashSet<int> swipedPlayers = new HashSet<int>();

    AudioSource source;
    AudioClip hurtSound;

    void Start()
    {
        rb2d = GetComponent<Rigidbody2D>();
        source = GetComponent<AudioSource>();
        punchShooter = GetComponentInParent<PunchShooter>();
    }

    void Update()
    {
        Renderer renderer = GetComponent<Renderer>();

        if (Shot)
        {
            Vector2 dir = (Vector2) renderer.bounds.center - previousLocation;
            RaycastHit2D[] hits = Physics2D.BoxCastAll(previousLocation, renderer.bounds.size, transform.rotation.eulerAngles.z, dir.normalized, dir.magnitude);

            HashSet<int> playerInstIds = new HashSet<int>();
            HashSet<int> swipeInstIds = new HashSet<int>();
            Vector2 deflectionNormal = Vector2.zero;

            foreach (RaycastHit2D hit in hits)
            {
                PlayerInfo playerInfo = hit.collider.gameObject.GetComponent<PlayerInfo>();
                if (playerInfo
                    && !playerInfo.Equals(Owner)
                    && !playersHit.ContainsKey(playerInfo.GetInstanceID())
                    && !swipesHit.ContainsKey(playerInfo.GetInstanceID()))
                {
                    playersHit.Add(playerInfo.GetInstanceID(), playerInfo);
                    playerInstIds.Add(playerInfo.GetInstanceID());
                } else
                {
                    PlayerInfo parentPlayer = hit.collider.gameObject.GetComponentInParent<PlayerInfo>();
                    if (parentPlayer != null
                         && hit.collider.name.Equals("PunchArc")
                         && !swipesHit.ContainsKey(parentPlayer.GetInstanceID()))
                    {
                        deflectionNormal = hit.normal;
                        swipesHit.Add(parentPlayer.GetInstanceID(), hit.normal);
                        swipeInstIds.Add(parentPlayer.GetInstanceID());
                    }
                }
            }

            playerInstIds.ExceptWith(swipeInstIds); // Don't hit players who deflected the punch.

            // Punch players
            foreach (int playerInstId in playerInstIds)
            {
                PlayerInfo playerToHit = playersHit[playerInstId];
                if (playerToHit)
                {
                    PunchPlayer(playerToHit);
                }
            }

            // The punch should be deflected.
            if (deflectionNormal != Vector2.zero)
            {
                float currentVel = rb2d.velocity.magnitude;
                rb2d.velocity = deflectionNormal * currentVel * SwipeDeflect;
                ShotDirection = rb2d.velocity.normalized;
                transform.rotation = Quaternion.Euler(0, 0, Utilities.GetAngle(deflectionNormal));
                Owner = null; // Allows players to be hit by their own deflected punches.
            }

            previousLocation = renderer.bounds.center;
        }
        else if (punchShooter.IsSwiping())
        {
            // Swipe players away. Can't do it in OnTriggerEnter2D since the punch isn't being physics simulated.
            // Doing a raycast from blob center to end of swipe. This will hit players inside of the swipe as well.
            Vector2 dir = renderer.bounds.center - punchShooter.transform.position;
            RaycastHit2D[] hits = Physics2D.BoxCastAll(punchShooter.transform.position, renderer.bounds.size, transform.rotation.eulerAngles.z, dir.normalized, dir.magnitude);
            foreach (RaycastHit2D hit in hits)
            {
                PlayerInfo hitPlayer = hit.collider.gameObject.GetComponent<PlayerInfo>();
                if (hitPlayer
                    && !hitPlayer.Equals(Owner)
                    && !swipedPlayers.Contains(hitPlayer.GetInstanceID()))
                {

                    hitPlayer.rb.velocity = Vector2.zero;
                    hitPlayer.rb.AddForce(dir * punchShooter.SwipeForce);
                    hitPlayer.grappleShooter.Detach();
                    hitPlayer.pState = PlayerInfo.PlayerState.Swiped;
                    swipedPlayers.Add(hitPlayer.GetInstanceID());
                }
            }
        }
    }

    public void PunchPlayer(PlayerInfo playerInfo)
    {
        if (!punchedPlayers.Contains(playerInfo.gameObject.GetInstanceID()))
        {
            float punchForce = Mathf.Clamp(rb2d.velocity.magnitude * ForceScale, MinPushForce, MaxPushForce);

            playerInfo.rb.AddForce(ShotDirection.normalized * punchForce);
            playerInfo.grappleShooter.Detach();
            playerInfo.OnHit();
            if (Random.value >= 0.5f)
            {
                hurtSound = Resources.Load<AudioClip>("Audio/SFX/HitSound1");
            }
            else
            {
                hurtSound = Resources.Load<AudioClip>("Audio/SFX/HitSound2");
            }

            playerInfo.pState = PlayerInfo.PlayerState.Hit;

            if (!source.isPlaying)
            {
                source.PlayOneShot(hurtSound, 0.6f);
            }

            punchedPlayers.Add(playerInfo.gameObject.GetInstanceID());
            playerInfo.ShakeController(0.25f);
            playerInfo.Stun(StunLength);
        }
    }

    public IEnumerator WaitForPunchStop()
    {
        yield return new WaitForSeconds(WaitAfterShot);

        while (gameObject.activeSelf
               && Shot
               && rb2d.velocity.magnitude >= AlmostStopped)
        {
            yield return new WaitForEndOfFrame();
        }

        // Player didn't choose to punch again so we set punch to disabled.
        if (Shot)
        {
            Shot = false; // Otherwise they'll be a raycast from end of punch back to beginning.
            punchShooter.ResetPunch();
        }

        punchedPlayers.Clear();
    }
}
