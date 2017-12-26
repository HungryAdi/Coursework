using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Pickup : MonoBehaviour {

    public PlayerInfo Owner;
    public PickupType PickupType;
    public PickupUse PickupUse;

    public abstract void Use();

}
