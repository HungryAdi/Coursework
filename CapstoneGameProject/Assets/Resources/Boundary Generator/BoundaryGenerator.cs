using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[RequireComponent(typeof(EdgeCollider2D))]
public class BoundaryGenerator : MonoBehaviour {
    public enum Shape {
        CIRCLE,
        RECTANGLE,
        SQUARE,
        SEMICIRCLE
    }
    public Shape shape;
    public int points = 30;
    public float radius = 30f;
    public float width = 30;
    public float height = 50;
    public EdgeCollider2D edge;

    void Start() {
        edge = GetComponent<EdgeCollider2D>();
        Generate();
    }

    public void Generate() {
        switch (shape) {
            case Shape.CIRCLE:
                GenerateCircle(radius);
                break;
            case Shape.SQUARE:
                GenerateRectangle(width, width);
                break;
            case Shape.RECTANGLE:
                GenerateRectangle(width, height);
                break;
            case Shape.SEMICIRCLE:
                GenerateSemicircle(radius);
                break;
        }
    }
    public void GenerateCircle(float radius) {

        List<Vector2> ps = new List<Vector2>();

        for (int i = 0; i <= points; i++) {
            float angle = (float)i / points * Mathf.PI * 2f;
            Vector2 v = new Vector2(-Mathf.Sin(angle), Mathf.Cos(angle)) * radius;
            ps.Add(v);
        }
        edge.points = ps.ToArray();
    }

    public void GenerateRectangle(float width, float height) {
        List<Vector2> ps = new List<Vector2>();
        ps.Add(new Vector2(-width, height));
        ps.Add(new Vector2(width, height));
        ps.Add(new Vector2(width, -height));
        ps.Add(new Vector2(-width, -height));
        ps.Add(new Vector2(-width, height));
        edge.points = ps.ToArray();
    }

    public void GenerateSemicircle(float radius) {
        List<Vector2> ps = new List<Vector2>();

        for (int i = 0; i <= points; i++) {
            float angle = i / (points * 2f) * Mathf.PI * 2f;
            Vector2 v = new Vector2(Mathf.Cos(angle), Mathf.Sin(angle)) * radius;
            ps.Add(v);
        }
        edge.points = ps.ToArray();
    }

}
