using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WarningAnim : MonoBehaviour {
    public Gradient gradient;
    public Sprite chevron;
    [HideInInspector]
    public int level;
    public float duration;
    private float timer;
    private SpriteRenderer sr;
    private SpriteRenderer background;
    private SpriteRenderer[] spriteRenderers;
    private Rigidbody2D rb;
    private Vector3 origin;
    private Vector3 destination;
    private int chevronLevels;
    private int currentChevron;

    void Start() {
        timer = 0;
        currentChevron = 0;
        background = GetComponent<SpriteRenderer>();
        sr = transform.Find("Fire").GetComponent<SpriteRenderer>();
        rb = GetComponent<Rigidbody2D>();
        spriteRenderers = GetComponentsInChildren<SpriteRenderer>();
        origin = transform.position;
        switch (level) {
            case 0:
                chevronLevels = 3;
                break;
            case 1:
                chevronLevels = 5;
                break;
            default:
                chevronLevels = 7;
                break;
        }

        Destroy(gameObject, duration);
    }
	
	// Update is called once per frame
	void Update () {
        timer += Time.deltaTime;
        if (destination != null && timer > currentChevron * duration / chevronLevels) {
            GameObject go = new GameObject("Chevron " + currentChevron);
            go.transform.position = origin + Vector3.up * currentChevron * (destination.y - origin.y) / chevronLevels;
            go.transform.parent = transform;
            SpriteRenderer sr = go.AddComponent<SpriteRenderer>();
            sr.sprite = chevron;
            go.transform.parent = transform;
            Destroy(go, duration / chevronLevels);
            currentChevron++;
        }

        for (int i = 0; i < spriteRenderers.Length; ++i) {
            if((int)timer >= i - 1 && spriteRenderers[i] != background) {
                spriteRenderers[i].color = gradient.Evaluate(timer / duration);
            }
        }
        transform.localScale = Vector3.one + Vector3.one * (timer / duration);
        if(rb.velocity.y < -0.1f) {
            rb.constraints = RigidbodyConstraints2D.FreezeAll;
            destination = transform.position;
        }
	}
}
