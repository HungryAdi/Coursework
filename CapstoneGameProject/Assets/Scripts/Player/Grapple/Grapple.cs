using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grapple : MonoBehaviour {
    public GrappleShooter grappleShooter;
    public PlayerAI PlayerAIScript;
    private Rigidbody2D rb2d;
    public bool attached = false;
    float nudgeAngleThreshold = 45f;
    float nudgeAimForce = 500;
    //private Collider2D col;
    public SpringJoint2D sj { get; private set; }
    public FixedJoint2D fj { get; private set; }
    // Use this for initialization
    void Start() {
        rb2d = GetComponent<Rigidbody2D>();
        //col = GetComponent<Collider2D>();
        fj = GetComponent<FixedJoint2D>();
        fj.enabled = false;
        sj = GetComponent<SpringJoint2D>();
        sj.enabled = false;
    }

    // Update is called once per frame
    void Update() {
        if (!attached)
        {
            Vector2 nudgeAim = new Vector2(GameInput.Horizontal.Get(grappleShooter.playerInfo.PlayerNumber), GameInput.Vertical.Get(grappleShooter.playerInfo.PlayerNumber)).normalized;
            if (nudgeAim.x != 0 || nudgeAim.y != 0)
            {
                Vector2 velNorm = rb2d.velocity.normalized;

                if (Mathf.Abs(Vector2.Angle(nudgeAim, velNorm)) > nudgeAngleThreshold)
                {
                    rb2d.AddForce(nudgeAim * nudgeAimForce * Time.deltaTime);
                    //transform.rotation = Quaternion.AngleAxis(Utilities.GetAngle(nudgeAim), Vector3.forward);
                }
            }
        }
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.CompareTag("Platform")) {
            AttachToObject(other.gameObject);
        } else if (other.CompareTag("Ground") || other.CompareTag("Wall")) {
            grappleShooter.Detach();
        } else if (other.CompareTag("Player") && other.GetComponent<PlayerInfo>() && grappleShooter != other.GetComponent<GrappleShooter>()) {
            //other.GetComponent<PlayerInfo>().Stun();
            //other.GetComponent<Rigidbody2D>().velocity = Vector2.zero;
            //other.GetComponent<PlayerInfo>().Push(GetComponent<Rigidbody2D>().velocity);
            //other.GetComponent<QuickHook>().Detach();
            AttachToObject(other.gameObject);
        }
    }

    // attaches to the given game object (player or rock)
    private void AttachToObject(GameObject obj) {
        if (!fj.connectedBody) {
            if (PlayerAIScript) {
                PlayerAIScript.shot = true;
                PlayerAIScript.hooked = true;
            }
            attached = true;
            grappleShooter.Attach(obj);
            transform.position = obj.transform.position;
            rb2d.velocity = Vector2.zero;
            fj.enabled = true;
            Rigidbody2D objRb = obj.GetComponent<Rigidbody2D>();
            fj.connectedBody = objRb;
            sj.enabled = true;
            sj.connectedBody = grappleShooter.GetComponent<Rigidbody2D>();
            Rock r = obj.GetComponent<Rock>();
            if(r && r.type == Rock.Type.King) {
                Game.instance.AddToKingRock(grappleShooter.playerInfo);
            }
        }
    }
}
