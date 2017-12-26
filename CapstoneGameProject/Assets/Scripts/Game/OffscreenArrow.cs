using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OffscreenArrow : MonoBehaviour {
    public PlayerInfo pi;
    private bool colorSet;
	// Update is called once per frame
	void Update () {
		if(pi) {
            if (!pi || Game.GetScreenState(pi.gameObject) == Game.ScreenState.OnScreen || Game.instance.GameOver()) {
                Destroy(gameObject);
            }
            if (!colorSet) {
                colorSet = true;
                GetComponent<SpriteRenderer>().color = pi.color;
            }
            transform.position = new Vector3(pi.transform.position.x, transform.position.y, 0);
        }

	}
}
