using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VerticalGameCamera : MonoBehaviour {

	public Vector2 LowestPosition;
	public Vector2 HighestPosition;
    public float CameraFollowBuffer;
	public float CameraZOffset;
	public float CameraLerpSpeed;
	public float TimeBeforeMoving;

	float orthoSize;
	Game game;

	void Start ()
	{
		orthoSize = GetComponent<Camera>().orthographicSize;
		game = FindObjectOfType<Game>();

		StartCoroutine(WaitBeforeMoving());
	}
	
	void Update ()
	{
		if (!game.GameOver())
		{
			PlayerInfo lowest = getLowestPlayer();

			if (lowest)
			{
				Renderer playerRend = lowest.GetComponent<Renderer>();

				//Add orthoSize to convert player loc to "camera loc" since camera location is centered.
				float bottomPlayerY = Mathf.Clamp((playerRend.bounds.center.y - (playerRend.bounds.size.y / 2)) + orthoSize - CameraFollowBuffer, LowestPosition.y, HighestPosition.y);

				transform.position = Vector3.Lerp(transform.position, new Vector3(0, bottomPlayerY, CameraZOffset), CameraLerpSpeed);
			}
		}
		else
		{
			transform.position = new Vector3(LowestPosition.x, LowestPosition.y, CameraZOffset);
		}
	}

	PlayerInfo getLowestPlayer()
	{
		PlayerInfo[] players = FindObjectsOfType<PlayerInfo>();
		PlayerInfo lowest = null;

		foreach (PlayerInfo player in players)
		{
			if (lowest == null
				|| (player.LivesLeft > 0 && player.transform.position.y < lowest.transform.position.y))
			{
				lowest = player;
			}
		}
		return lowest;
	}

	IEnumerator WaitBeforeMoving()
	{
		enabled = false;
		yield return new WaitForSeconds(TimeBeforeMoving);
		enabled = true;
	}
}
