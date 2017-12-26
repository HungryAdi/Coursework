using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Players in game should have this attached.
// Container for their pickups and allows them to use pickups as well.
public class PickupHolder : MonoBehaviour {

    public List<GameObject> AvailablePickups = new List<GameObject>();
    public int MaxPickupAmount = 1;

	void Update ()
    {
	    // TODO check if they pressed the use pickup button.
        if (Input.GetKey(KeyCode.H))
        {
            UsePickup();
        }
	}

    public void UsePickup()
    {
        if (AvailablePickups.Count > 0)
        {
            UsePickup(AvailablePickups[0]);
            AvailablePickups.RemoveAt(0);
        }
    }

    public void AddPickup(GameObject pickup)
    {
        if (AvailablePickups.Count < MaxPickupAmount)
        {
            Pickup pickedUp = pickup.GetComponent<Pickup>();
            if (pickedUp.PickupUse == PickupUse.InstantUse)
            {
                UsePickup(pickedUp.gameObject);
            }
            else
            {
                AvailablePickups.Add(pickup);
            }
        }
    }

    private void UsePickup(GameObject toUse)
    {
        Pickup pickup = Instantiate(AvailablePickups[0], transform.position, Quaternion.identity).GetComponent<Pickup>();
        pickup.Owner = GetComponent<PlayerInfo>();
        pickup.Use();
    }

}
