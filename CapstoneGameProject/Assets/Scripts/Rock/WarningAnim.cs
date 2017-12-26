using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WarningAnim : MonoBehaviour {
    public Gradient gradient;
    public float duration;
    private float timer;
    private SpriteRenderer sr;
    private SpriteRenderer background;
    private SpriteRenderer[] spriteRenderers;
    private Rigidbody2D rb;

    void Start() {
        timer = 0;
        background = GetComponent<SpriteRenderer>();
        sr = transform.Find("Fire").GetComponent<SpriteRenderer>();
        rb = GetComponent<Rigidbody2D>();
        spriteRenderers = GetComponentsInChildren<SpriteRenderer>();
        Destroy(gameObject, duration);
    }
	
	// Update is called once per frame
	void Update () {
        timer += Time.deltaTime;
        for(int i = 0; i < spriteRenderers.Length; ++i) {
            if((int)timer >= i - 1 && spriteRenderers[i] != background) {
                spriteRenderers[i].color = gradient.Evaluate(timer / duration);
            }
        }
        transform.localScale = Vector3.one + Vector3.one * (timer / duration);
        if(rb.velocity.y < -0.1f) {
            rb.constraints = RigidbodyConstraints2D.FreezeAll;
        }
	}
}
