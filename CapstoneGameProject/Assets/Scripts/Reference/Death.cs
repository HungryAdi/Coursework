using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Death : MonoBehaviour {
    private const string deathStr = "Death Count: ";
    public Vector3 respawnPoint;
    public Text deathText;
    public int deathCount; // maybe should move this to somewhere else later
    void Start () {
        deathCount = 0;
        deathText.text = deathStr + deathCount;
    }

    void OnCollisionEnter2D(Collision2D col) {
        if (col.collider.CompareTag("Ground")) {
            transform.position = respawnPoint;
            deathCount++;
            deathText.text = "Death Count: " + deathCount;
            GetComponent<GrappleShooter>().Detach();
        }
    }
}
