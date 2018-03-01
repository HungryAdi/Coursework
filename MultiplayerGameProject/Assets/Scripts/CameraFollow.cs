using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour {

	public GameObject player;

	private float currentHeight;
	private float heightDamping;
	private float playerMaxVert;

	// Use this for initialization
	void Start () {
		heightDamping = 2.0f;

        if (player)
        {
            currentHeight = transform.position.y;
            playerMaxVert = player.transform.position.y;
        }

	}

	void LateUpdate () {
        if (player)
        {
            if (player.transform.position.y > playerMaxVert)
            {
                playerMaxVert = player.transform.position.y;
            }

            currentHeight = Mathf.Lerp(currentHeight, playerMaxVert, heightDamping * Time.deltaTime);
            Vector3 position = new Vector3(transform.position.x, currentHeight, transform.position.z);
            transform.position = position;
        }

	}
}
