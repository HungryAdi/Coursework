using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// This gets created when a pickup is spawned.
// The most basic pickup should just fall until it hits the floor.
// Some special pickups might fly around or float, which may require inheriting from this class.
[RequireComponent(typeof(Collider2D))]
public class PickupSpawn : MonoBehaviour
{
    public GameObject OnUse;

    public virtual void OnTriggerEnter2D(Collider2D other)
    {
        PickupHolder pickupHolder = other.GetComponent<PickupHolder>();
        if (pickupHolder != null)
        {
            pickupHolder.AddPickup(OnUse);
            ConsumePickup();
        }
    }

    public virtual void ConsumePickup()
    {
        // TODO: Play pickup noise?
        Destroy(gameObject);
    }

}