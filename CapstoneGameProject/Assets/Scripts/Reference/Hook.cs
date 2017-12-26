using UnityEngine;
using System.Collections;
[RequireComponent(typeof(LineRenderer))]
[RequireComponent(typeof(Rigidbody2D))]
public class Hook : MonoBehaviour {
    public GameObject hookPrefab;
    public GameObject ropeNode;
    public float pullForce;
    //public float moveForce;
    public float spacing;
    public LayerMask platformMask;
    public float GrappleLength;
    private LineRenderer lineRenderer;
    private float distance;
    private Vector2 destination;
    private GameObject hook;
    private GameObject lastNode;
    private Rigidbody2D rb;
	private AudioSource source;
	private AudioClip shootSound;
	private AudioClip hitSound;
    private PlayerMovement pm;
    private GameObject rockHit;
    private AimGrapple aimGrapple;
    void Start() {
        lineRenderer = GetComponent<LineRenderer>();
        rb = GetComponent<Rigidbody2D>();
        pm = GetComponent<PlayerMovement>();
		source = GetComponent<AudioSource>();
        aimGrapple = GetComponentInChildren<AimGrapple>();
		shootSound = Resources.Load<AudioClip>("Audio/SFX/shoot");
		hitSound = Resources.Load<AudioClip>("Audio/SFX/RockHit1");
    }
    void Update() {
        // left click to fire grappling hook
        if (GameInput.Shoot.WasPressed() && !hook) {
            lineRenderer.enabled = true;
            destination = aimGrapple.GetAimDirection() * GrappleLength;
            RaycastHit2D hit = Physics2D.Linecast(transform.position, destination, platformMask);
            if (hit) {
                hook = Instantiate(hookPrefab, destination, Quaternion.identity);
                rockHit = hit.collider.gameObject;
                //rockHit.GetComponent<Rock>().hook = hook;
                //hook.transform.SetParent(hit.transform);
				source.PlayOneShot (shootSound);
				source.PlayOneShot (hitSound);
                BuildJoints(transform.position, destination);
                pm.SetMovementState(MovementState.Hanging);
            }
        } else if (GameInput.Shoot.IsPressed() && hook) { // right click and hold to pull self towards hook
            rb.AddForce((destination - new Vector2(transform.position.x, transform.position.y)).normalized * pullForce);
            BuildJoints(transform.position, destination);
        }
        // destroy hook (for testing purposes)
        if (Input.GetKeyDown(KeyCode.X) && hook) {
            ClearJoints();
            Destroy(hook);
            lineRenderer.enabled = false;
            pm.SetMovementState(MovementState.Airborne);
        }
        // lets the player swing back and forth for now
        // rb.AddForce(Input.GetAxis("Horizontal") * new Vector2(moveForce, 0));
        if (hook) {
            destination = rockHit.transform.position;
            hook.transform.position = destination;
        }
        RenderLine();
    }

    //build all of the joints (not that kind of joint Brad)
    void BuildJoints(Vector2 start, Vector2 end) {
        ClearJoints();
        lastNode = hook; //first node is the hook itself
        distance = Vector2.Distance(start, end);
        Vector2 direction = start - end;
        Vector2 normDir = direction.normalized;
        int numNodes = Mathf.FloorToInt(distance/spacing);
        for(int i = 0; i < numNodes; ++i) {
            GameObject node = Instantiate(ropeNode, lastNode.transform.position + new Vector3(normDir.x * spacing, normDir.y * spacing, 0), Quaternion.identity);
            node.transform.SetParent(hook.transform);
            lastNode.GetComponent<HingeJoint2D>().connectedBody = node.GetComponent<Rigidbody2D>();
            lastNode = node;
        }
        lastNode.GetComponent<HingeJoint2D>().connectedBody = rb; // last node is the player
    }

    //clear all of the joints (not that kind of joint Brad)
    void ClearJoints() {
        foreach(Transform t in hook.transform) {
            Destroy(t.gameObject);
        }
    }

    //render the LineRenderer (called each frame)
    void RenderLine() {
        lineRenderer.positionCount = 0;
        if (hook) {
            lineRenderer.positionCount = hook.transform.childCount + 2; // each of the rope nodes, plus the hook and the player
            lineRenderer.SetPosition(0, hook.transform.position); // hook is initial point
            int i = 1;
            foreach (Transform t in hook.transform) {
                lineRenderer.SetPosition(i++, t.position);
            }
            lineRenderer.SetPosition(i, transform.position); // player is last point
        }
    }
}
