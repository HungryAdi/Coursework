using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Platform : MonoBehaviour {
    public enum Type {
        NORMAL,
        MOVING,
        SOLID,
        JUMP,
        PULSING,
        COUNT
    }

    public AudioClip whiteAndMoving;
    public AudioClip jump;
    public AudioClip black;
    public AudioClip shrink;
    private AudioSource source;

    public Type type;
    public float jumpForce;
    public float jumpMultiplier;
    public float speed;
    public BoxCollider2D bc;
    SpriteRenderer sr;
    int moveForward = 1;
    int grow = -1;
    float maxSize;
    // Use this for initialization
    void Start() {
        sr = GetComponent<SpriteRenderer>();
        maxSize = transform.localScale.x;
        switch (type) {
            case Type.NORMAL:
                sr.color = new Color(0.7f, 0.7f, 0.7f);
                break;
            case Type.JUMP:
                sr.color = Color.green;
                jumpForce *= jumpMultiplier;
                break;
            case Type.MOVING:
                sr.color = Color.yellow;
                break;
            case Type.PULSING:
                sr.color = Color.blue;
                break;
            case Type.SOLID:
                sr.color = Color.black;
                bc.usedByEffector = false;
                break;
        }
        source = gameObject.GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update() {
        //if(transform.position.y < Camera.main.ScreenToWorldPoint(new Vector3(Screen.height, 1)).y) {
        //          Destroy(gameObject);
        //      }
        if (type == Type.MOVING) {
            Vector3 screenPoint = Camera.main.WorldToScreenPoint(transform.position);
            if (screenPoint.x < 0) {
                moveForward = 1;
            }
            if (screenPoint.x > Screen.width) {
                moveForward = -1;
            }
            transform.Translate(new Vector3(speed * Time.deltaTime * moveForward, 0, 0));
        }
        if (type == Type.PULSING) {
            float xScale = transform.localScale.x;
            if (transform.localScale.x <= 1) {
                grow = 1;
            } else if (transform.localScale.x >= maxSize) {
                grow = -1;
            }
            transform.localScale = new Vector3(xScale + grow * speed * Time.deltaTime, transform.localScale.y, 1);
        }
    }

    void OnCollisionEnter2D(Collision2D col) {
        if (col.gameObject.CompareTag("Player")) {
            col.gameObject.SendMessage("Jump", jumpForce);
            if (col.gameObject.GetComponent<Player>().IsLocalPlayer) {
                if (source) {
                    switch (type) {
                        case Type.NORMAL:
                            source.PlayOneShot(whiteAndMoving);
                            break;
                        case Type.JUMP:
                            source.PlayOneShot(jump);
                            break;
                        case Type.MOVING:
                            source.PlayOneShot(whiteAndMoving);
                            break;
                        case Type.PULSING:
                            source.PlayOneShot(shrink);
                            break;
                        case Type.SOLID:
                            source.PlayOneShot(black);
                            break;
                    }
                }

            }


        }
    }

    public static Type GetRandomPlatformType() {
        return (Type)Random.Range(0, (int)Type.COUNT);
    }
}
