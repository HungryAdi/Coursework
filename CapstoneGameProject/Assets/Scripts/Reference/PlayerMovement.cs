using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerMovement : MonoBehaviour {
    public Vector3 respawnPoint;
    public Text deathText;
    public int deathCount; // maybe should move this to somewhere else later
    public float GroundControl = 10f;
    public float AirControl = 5f;
    public float HangControl = 7.5f;
    public float MaxVelocity = 1f;
    public MovementState MovementState = MovementState.Grounded;

    private Rigidbody2D rb2d;
    private float controlSpeed;

    void Start () {
        rb2d = GetComponent<Rigidbody2D>();
        controlSpeed = GroundControl;
        deathCount = 0;
        deathText.text = "Death Count: ";
	}
	
	void Update () {
        float forceScale = controlSpeed * GameInput.Horizontal.Get();

        rb2d.AddForce(new Vector2(forceScale, 0));
        
        // Ensure ground speed doesn't exceed max velocity.
        if (MovementState == MovementState.Grounded
            && Mathf.Abs(rb2d.velocity.x) > MaxVelocity)
        {
            rb2d.velocity = new Vector2(rb2d.velocity.x < 0 ? -MaxVelocity : MaxVelocity, rb2d.velocity.y);
        }
    }

    public void SetMovementState(MovementState movementState)
    {
        switch (movementState)
        {
            case MovementState.Grounded:
                controlSpeed = GroundControl;
                break;
            case MovementState.Airborne:
                controlSpeed = AirControl;
                break;
            case MovementState.Hanging:
                controlSpeed = HangControl;
                break;
        }

        MovementState = movementState;
    }

    void OnCollisionEnter2D(Collision2D col) {
        if (col.collider.CompareTag("Ground")) {
            transform.position = respawnPoint;
            deathCount++;
            deathText.text = "Death Count: " + deathCount;
        }
    }
}
