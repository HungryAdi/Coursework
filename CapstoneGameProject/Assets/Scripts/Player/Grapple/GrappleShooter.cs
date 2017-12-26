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
    public AimGrapple aimGrapple;
    private Transform arrow;
    public  PlayerInfo playerInfo;

    public float grappleVelocity;
    public float pullVelocity;
    public float pullAcceleration;
    public float ySwingForce = 0;
    public float xSwingForce = 0;
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
        arrow = transform.GetChild(0).GetChild(0);
        playerInfo = GetComponent<PlayerInfo>();
    }

    void Update() {
        if (hit && !hit.activeSelf) {
            Detach();
        }
        if (hit)
        {
            float overallDist = Vector2.Distance(transform.position, hit.transform.position);
            float closeDist = transform.position.y - hit.transform.position.y;
            if(closeDist < 2f && closeDist < 0 && rb2d.velocity.y < 2f)
            {
                overallDist = .005f;
            }
            else
            {
                if(overallDist < 1)
                {
                    overallDist = 2;
                }
            }
                float dist = overallDist / 1.5f;
            grapple.GetComponent<SpringJoint2D>().distance = dist;

        }
        if (playerInfo.CanAct()) {
            if (GameInput.Shoot.WasPressed(playerInfo.PlayerNumber) && !hit) {
                Shoot();
            } else if (GameInput.Shoot.IsPressed(playerInfo.PlayerNumber) && hit || (PlayerAIScript && pullingToRockAI)) {
                Vector2 v = new Vector2(GameInput.Horizontal.GetRaw(playerInfo.PlayerNumber), GameInput.Vertical.GetRawDelayed(playerInfo.PlayerNumber));
                //PullToTarget();
                if (Vector2.Distance(hit.transform.position, transform.position) <= 5f)
                {
                    //grapple.GetComponent<SpringJoint2D>().enabled = false;
                }
                else
                {
                    //grapple.GetComponent<SpringJoint2D>().enabled = true;
                }
                v = v.normalized;
                v.x *= xSwingForce;
                v.y *= ySwingForce;
                //if (v.y > 0)
                //v.y *= ySwingForce/2;
                rb2d.AddForce((v / (Vector2.Distance(transform.position, hit.transform.position))) + hit.GetComponent<Rigidbody2D>().velocity);

            } else if (GameInput.Shoot.WasReleased(playerInfo.PlayerNumber)) {
                Detach();
            }
        }
        RenderLine();
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
        rb2d.velocity = direction * Vector2.Distance(transform.position,hit.transform.position) * 2;
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
    }

    public void PullToTarget() {
        Vector2 dest = GetDestination();
        if (Vector2.Distance(dest, transform.position) <= 5f) {
            grapple.GetComponent<SpringJoint2D>().distance = 20.0f;
        } else {
            grapple.GetComponent<SpringJoint2D>().distance = .1f;
            Vector2 direction = GetDirection(dest);
            if (hit.CompareTag("Player")) {
                rb2d.AddForce(direction * pullAcceleration * Time.deltaTime / 2f);
                hit.GetComponent<Rigidbody2D>().AddForce(-direction * pullAcceleration * Time.deltaTime / 2f);
            } else {
                rb2d.AddForce(direction * pullAcceleration * Time.deltaTime);
            }
            //Minimum pull speed.
            if (rb2d.velocity.magnitude < pullVelocity) {
                rb2d.velocity = direction * pullVelocity;
            }
        }
    }

    public void Shoot() {
        if (!source.isPlaying) {
            source.PlayOneShot(shootSound,0.6f);
        }
        // Set hand rotation to aim rotation.
        grappleGO.transform.rotation = Quaternion.Euler(0, 0, aimGrapple.transform.localEulerAngles.z);
        grapple.attached = false;
        grappleGO.transform.position = arrow.transform.position;
        grappleGO.SetActive(true);
        destination = aimGrapple.GetAimDirection() * GrappleLength;
        grappleGO.GetComponent<Rigidbody2D>().velocity = destination.normalized * grappleVelocity;
    }

}
