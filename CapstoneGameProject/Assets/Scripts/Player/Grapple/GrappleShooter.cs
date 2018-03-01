using UnityEngine;
using System.Collections;
[RequireComponent(typeof(Rigidbody2D))]
public class GrappleShooter : MonoBehaviour {
    public LayerMask platformMask;
    public float GrappleLength;
    public GameObject grappleGO;
    public Grapple grapple;
    public PlayerAI PlayerAIScript;
    private bool pullingToRockAI = false;
    private LineRenderer lineRenderer;

    private Vector2 destination;
    private Rigidbody2D rb2d;
    private AudioSource source;
    private AudioClip shootSound;
    private AudioClip hitSound;
    [HideInInspector]
    public GameObject hit;
    public Vector2 rockHitPos;
    public Aim aimGrapple;
    private Transform arrow;
    public  PlayerInfo playerInfo;

    public float grappleVelocity;
    public float minPullVelocity;
    public float pullAcceleration;
    public float swingForce;
    public float pullForce;
    public float maxVelocity;
    private Material mat;
    private Collider2D col;

    void Start() {
        if (grappleGO) {
            grappleGO.SetActive(false);
            grapple = grappleGO.GetComponent<Grapple>();
            Physics2D.IgnoreCollision(GetComponent<Collider2D>(), grappleGO.GetComponent<Collider2D>());
        }
        lineRenderer = GetComponentInChildren<LineRenderer>();
        rb2d = GetComponent<Rigidbody2D>();
        
        source = GetComponent<AudioSource>();
        col = GetComponent<Collider2D>();
        shootSound = Resources.Load<AudioClip>("Audio/SFX/ShootSound");
        arrow = transform.Find("Aim").GetChild(0);
        playerInfo = GetComponent<PlayerInfo>();
    }

    void Update() {
        if (hit && !hit.activeSelf) {
            Detach();
        }
        if (hit)
        {
            float dist = Vector2.Distance(transform.position, hit.transform.position);
            grapple.sj.distance = dist * 0.6f;

        }
        if (playerInfo.CanAct()) {
            if (GameInput.Shoot.WasPressed(playerInfo.PlayerNumber) && !hit) {
                Shoot();
            } else if (GameInput.Shoot.IsPressed(playerInfo.PlayerNumber) && hit || (PlayerAIScript && pullingToRockAI)) {
                //float x = GameInput.Horizontal.GetRaw(playerInfo.PlayerNumber);
                //Vector2 direction = GetDirection(hit.transform.position);
                //Vector3 swingDirection = Vector3.Cross(direction, x * Vector3.forward);
                //rb2d.AddForce(swingDirection * swingForce);
                //Vector2 v = new Vector2(GameInput.Horizontal.GetRaw(playerInfo.PlayerNumber), GameInput.Vertical.GetRawDelayed(playerInfo.PlayerNumber)).normalized;
                //float angle = Vector2.Angle(v, GetDirection(hit.transform.position));
                //Debug.Log(angle);
                //if(angle > 60f && angle > 120f) {
                //    rb2d.AddForce((v * Vector2.Distance(transform.position, hit.transform.position * swingForce)) + hit.GetComponent<Rigidbody2D>().velocity);
                //}
                //PullToTarget();
                Vector2 direction = new Vector2(GameInput.Horizontal.GetRaw(playerInfo.PlayerNumber), GameInput.Vertical.GetRaw(playerInfo.PlayerNumber)).normalized;
                //Debug.Log(direction);
                //Debug.Log(GetDirection(hit.transform.position));
                if(GetDirection(hit.transform.position).y > -0.85f) {
                    rb2d.AddForce(direction * pullForce);
                }
                if(rb2d.velocity.magnitude > maxVelocity) {
                    rb2d.velocity = maxVelocity * direction;
                }
            } else if (GameInput.Shoot.WasReleased(playerInfo.PlayerNumber)) {
                Detach();
            }
        }
        RenderLine();

        if (aimGrapple.GetRaw() == Vector2.zero)
        {
            aimGrapple.SetAimDirection(Vector2.up);
        }
    }

    public LineRenderer GetLineRenderer() {
        return lineRenderer;
    }

    //render the LineRenderer (called each frame)
    void RenderLine() {
        if (grappleGO && grappleGO.activeSelf) {
            lineRenderer.enabled = true;
            lineRenderer.SetPosition(0, transform.position - (transform.position - grappleGO.transform.position).normalized * col.bounds.size.x);
            lineRenderer.SetPosition(1, grappleGO.GetComponent<Collider2D>().bounds.center);
        }
    }

    public Vector2 GetDestination() {
        if (hit) {
            return grappleGO.transform.position;
        }
        return Vector2.zero;
    }

    public Vector2 GetDirection(Vector2 destination) {
        return (destination - rb2d.position).normalized;
    }

    public void Attach(GameObject go) {
        hit = go;
        Vector2 dest = GetDestination();
        Vector2 direction = GetDirection(dest);
        double distanceModifier = Mathf.Log10(Vector2.Distance(transform.position, hit.transform.position));
        if(distanceModifier < .2)
            distanceModifier = .4;
        if (distanceModifier > .8)
            distanceModifier = .8;
        rb2d.velocity = direction * ((float)distanceModifier) * 3;
        grappleGO.transform.position = dest;
        //Leave open for now for other stuff like animations or sounds or whatever
        if (Random.value >= 0.5f) {
            hitSound = Resources.Load<AudioClip>("Audio/SFX/RockHit1");
        } else {
            hitSound = Resources.Load<AudioClip>("Audio/SFX/RockHit2");
        }
		if (!source.isPlaying) {
			source.PlayOneShot (hitSound, 0.6f);
		}

        if (PlayerAIScript) {
            pullingToRockAI = true;
            PlayerAIScript.hooked = true;
        }
    }

    public void Detach() {
        if (hit) {
            Rock r = hit.GetComponent<Rock>();
            if (r && r.type == Rock.Type.Breakable) {
                RockSpawner.instance.ReturnRock(hit);
                r.OnPool();
            } else if (r && r.type == Rock.Type.King) {
                Game.instance.RemoveFromKingRock(playerInfo);
            }
            FixedJoint2D fj = grapple.fj;
            if (fj) {
                fj.enabled = false;
                fj.connectedBody = null;
                grapple.sj.enabled = false;
                grapple.sj.connectedBody = null;
            }
            hit = null;
        }
        if (PlayerAIScript && pullingToRockAI) {
            pullingToRockAI = false;
        }
        grappleGO.transform.position = arrow.transform.position;
        lineRenderer.enabled = false;

        if (PlayerAIScript) {
            PlayerAIScript.shot = false;
            PlayerAIScript.hooked = false;
        }
        grappleGO.SetActive(false);
        playerInfo.pState = PlayerInfo.PlayerState.Idle;
    }

    public void PullToTarget() {
        Vector2 dest = GetDestination();
        if (Vector2.Distance(dest, transform.position) <= 0.5f) {
            Detach();
        } else {
            Vector2 direction = GetDirection(dest);
            if (hit.CompareTag("Player")) {
                rb2d.AddForce(direction * pullAcceleration * Time.deltaTime / 2f);
                hit.GetComponent<Rigidbody2D>().AddForce(-direction * pullAcceleration * Time.deltaTime / 2f);
            }
            //Minimum pull speed.
            if (rb2d.velocity.magnitude < minPullVelocity) {
                rb2d.velocity = direction * minPullVelocity;
            }
        }
    }

    public void Shoot() {
        if (!source.isPlaying) {
            source.PlayOneShot(shootSound,0.6f);
        }
        playerInfo.pState = PlayerInfo.PlayerState.Grappling;
        // Set hand rotation to aim rotation.
        grappleGO.transform.rotation = Quaternion.Euler(0, 0, aimGrapple.transform.localEulerAngles.z);
        grapple.attached = false;
        grappleGO.transform.position = arrow.transform.position;
        grappleGO.SetActive(true);
        destination = aimGrapple.GetAimDirection() * GrappleLength;
        grappleGO.GetComponent<Rigidbody2D>().velocity = destination.normalized * grappleVelocity;
    }

}
