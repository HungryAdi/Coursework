using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AOEStun : Pickup
{
    private bool used = false;

    public override void Use()
    {
        used = true;
    }

    private void Update()
    {
        if (used)
        {
            transform.localScale += Vector3.one * 5f * Time.deltaTime;
        }
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        PlayerInfo playerHit = other.GetComponent<PlayerInfo>();
        if (playerHit != null
            && playerHit != Owner)
        {
            playerHit.Stun();
            Destroy(gameObject);
        }
    }
}
