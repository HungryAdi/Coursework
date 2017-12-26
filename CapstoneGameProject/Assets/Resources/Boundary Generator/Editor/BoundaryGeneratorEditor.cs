using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(BoundaryGenerator))]
[CanEditMultipleObjects]
public class BoundaryGeneratorEditor : Editor {
    SerializedProperty enumProp;

    private void OnEnable() {
        enumProp = serializedObject.FindProperty("shape");
    }
    public override void OnInspectorGUI() {
        BoundaryGenerator boundaryGenerator = (BoundaryGenerator)target;
        boundaryGenerator.edge = boundaryGenerator.gameObject.GetComponent<EdgeCollider2D>();
        serializedObject.Update();
        EditorGUILayout.PropertyField(enumProp);
        switch (boundaryGenerator.shape) {
            case BoundaryGenerator.Shape.CIRCLE:
            case BoundaryGenerator.Shape.SEMICIRCLE:
                boundaryGenerator.radius = EditorGUILayout.FloatField("Radius", boundaryGenerator.radius);
                break;
            case BoundaryGenerator.Shape.SQUARE:
                boundaryGenerator.width = EditorGUILayout.FloatField("Width", boundaryGenerator.width);
                break;
            case BoundaryGenerator.Shape.RECTANGLE:
                boundaryGenerator.width = EditorGUILayout.FloatField("Width", boundaryGenerator.width);
                boundaryGenerator.height = EditorGUILayout.FloatField("Height", boundaryGenerator.height);
                break;
        }
        if (GUILayout.Button("Generate")) {
            boundaryGenerator.Generate();
        }
        serializedObject.ApplyModifiedProperties();


    }
}
