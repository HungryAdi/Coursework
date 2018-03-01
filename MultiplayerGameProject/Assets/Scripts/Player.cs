using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Player : MonoBehaviour {

    public float MaxSpeed = 5f;
    public float jumpCooldown;
    private Rigidbody2D rb2d;
    private new Camera camera;
    private Animator anim;
    private SpriteRenderer render;
    private SpriteRenderer indicator;
    private bool dead = false;
    public bool gameOver = false;
    float jumpTimer;
    private Text scoreText;
    private int score;
    private bool canMove = true;

    public bool IsLocalPlayer = true;
    public float LocalPlayerHorizontal;

    public float RemotePlayerHorizontal;

    void Start() {

        jumpTimer = jumpCooldown;
        rb2d = GetComponentInChildren<Rigidbody2D>();
        camera = FindObjectOfType<Camera>();
        anim = GetComponentInChildren<Animator>();
        render = transform.Find("Model").GetComponent<SpriteRenderer>();
        indicator = transform.Find("Indicator").GetComponent<SpriteRenderer>();
        score = 0;
        scoreText = GameObject.Find("ScoreText").GetComponent<Text>();
    }

    void Update() {
        if (canMove) {
            jumpTimer -= Time.deltaTime;
            if (!render.isVisible) // For some reason returns false at the beginning for two frames
            {
                Vector2 screenPoint = camera.WorldToScreenPoint(rb2d.position);

                if (screenPoint.x < -render.bounds.size.x) // Player is fully off screen to the left, moving to the right of the screen..
                {
                    rb2d.MovePosition(camera.ScreenToWorldPoint(new Vector2(Screen.width, screenPoint.y))); // Sets player to the right side of the screen with half of the player already in the screen.
                } else if (screenPoint.x > Screen.width + render.bounds.size.x) // Player is fully off screen to the right, move to left of screen.
                {
                    rb2d.MovePosition(camera.ScreenToWorldPoint(new Vector2(0, screenPoint.y))); // Sets player to left side of the screen with half of the player already in the screen.
                } else if (!gameOver && screenPoint.y < Camera.main.ScreenToWorldPoint(new Vector3(Screen.height, 1)).y && IsLocalPlayer) {
                    dead = true;
                    //StartCoroutine(Death(1.5f));
                }
            }
            if (IsLocalPlayer) {
                if (dead) {
                    dead = false;
                    rb2d.isKinematic = true;
                    rb2d.velocity = Vector3.zero;
                    transform.rotation = Quaternion.identity;
                    if (GameHost.Instance) {
                        GameHost.Instance.dead = true;
                    } else {
                        GameClient.Instance.dead = true;
                    }
                    this.gameObject.SetActive(false);
                }
                rb2d.velocity = new Vector2(MaxSpeed * LocalPlayerHorizontal, rb2d.velocity.y);
                score = Mathf.Max(score, (int)transform.position.y);
                scoreText.text = "" + score;
                //rb2d.velocity = new Vector2(MaxSpeed * Input.GetAxis("Horizontal"), rb2d.velocity.y);
            } else {
                rb2d.velocity = new Vector2(MaxSpeed * RemotePlayerHorizontal, rb2d.velocity.y);
            }
        }
    }
    public void SetIndicator(Sprite sprite) {
        indicator = transform.Find("Indicator").GetComponent<SpriteRenderer>();
        indicator.sprite = sprite;
    }
    public IEnumerator Death(float deathTime) {
        if (anim) {
            anim.SetBool("Dead", true);
        }
        yield return new WaitForSeconds(deathTime);
        anim.enabled = false;
        canMove = false;
    }


    void Jump(int amount) {
        if (jumpTimer < 0) {
            rb2d.velocity = Vector2.zero;
            rb2d.AddForce(Vector2.up * amount);
            jumpTimer = jumpCooldown;
        }

    }


    void OnDrawGizmos() {
        Gizmos.color = Color.black;
        Gizmos.DrawCube(new Vector3(0, (int)Camera.main.ScreenToWorldPoint(new Vector3(Screen.height, 1)).y, transform.position.z), new Vector3(1, 1, 1));
    }
}