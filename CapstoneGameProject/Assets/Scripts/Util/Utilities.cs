using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public  class Utilities : MonoBehaviour {

    public static int CountTotalAI()
    {
        int count = 0;
        for (int i = 0; i < 5; i++)
        {
            count += PlayerPrefs.GetInt("IsAI" + i, 0);
        }
        //Debug.Log(count);
        return count;
    }

    public static Color GetColorWithAlpha(Color c, float alpha)
    {
        return new Color(c.r, c.g, c.b, alpha);
    }

    public static float GetAngle(Vector2 v)
    {
        float angle = Vector2.Angle(Vector2.up, v);

        if (v.x > 0)
        {
            angle *= -1;
        }

        return angle;
    }
}
