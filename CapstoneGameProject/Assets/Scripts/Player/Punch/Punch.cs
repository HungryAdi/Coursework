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

    Rigidbody2D rb2d;

    static float WaitAfterShot = 0.05f;
    static float AlmostStopped = 0.025f;

    Vector2 previousLocation;
    HashSet<int> punchedPlayers = new HashSet<int>();

    AudioSource source;
    AudioClip hurtSound;

    void Start()
    {
        rb2d = GetComponent<Rigidbody2D>();
        source = GetComponent<AudioSource>();
    }

    void Update()
    {
        if (Shot)
        {
            Vector2 dir = (Vector2)transform.position - previousLocation;
            RaycastHit2D[] hits = Physics2D.BoxCastAll(transform.position, GetComponent<Renderer>().bounds.size, transform.rotation.eulerAngles.z, dir.normalized, dir.magnitude);

            foreach (RaycastHit2D hit in hits)
            {
                PlayerInfo playerInfo = hit.collider.gameObject.GetComponent<PlayerInfo>();
                if (playerInfo && !playerInfo.Equals(Owner))
                {
                    PunchPlayer(playerInfo);
                }
            }
        }

        previousLocation = transform.position;
    }

    public void PunchPlayer(PlayerInfo playerInfo)
    {
        if (!punchedPlayers.Contains(playerInfo.gameObject.GetInstanceID()))
        {
            float punchForce = Mathf.Clamp(rb2d.velocity.magnitude * ForceScale, MinPushForce, MaxPushForce);

            playerInfo.GetComponent<Rigidbody2D>().AddForce(ShotDirection.normalized * punchForce);
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

            if (!source.isPlaying)
            {
                source.PlayOneShot(hurtSound, 0.6f);
            }

            punchedPlayers.Add(playerInfo.gameObject.GetInstanceID());
            playerInfo.ShakeController(0.25f);
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
            gameObject.SetActive(false); 
        }

        punchedPlayers.Clear();
    }
}
