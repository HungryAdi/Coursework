using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(Timer))]
[CanEditMultipleObjects]
public class TimerEditor : Editor {
    SerializedProperty enumProp;
    SerializedProperty fontProp;
    SerializedProperty minVecProp;
    SerializedProperty maxVecProp;

    private void OnEnable() {
        enumProp = serializedObject.FindProperty("mode");
        fontProp = serializedObject.FindProperty("font");
        minVecProp = serializedObject.FindProperty("minAnchor");
        maxVecProp = serializedObject.FindProperty("maxAnchor");
    }
    public override void OnInspectorGUI() {
        Timer timer = (Timer)target;
        serializedObject.Update();
        EditorGUILayout.PropertyField(enumProp);
        timer.render = EditorGUILayout.Toggle("Render", timer.render);
        if (timer.render) {
            EditorGUILayout.PropertyField(fontProp);
            timer.fontSize = EditorGUILayout.IntField("Font Size", timer.fontSize);
            EditorGUILayout.PropertyField(minVecProp);
            EditorGUILayout.PropertyField(maxVecProp);
        }
        switch (timer.mode) {
            case Timer.Mode.TIMER:
            case Timer.Mode.ABILITY_COOLDOWN:
            case Timer.Mode.STOPWATCH:
                timer.duration = EditorGUILayout.FloatField("Duration", timer.duration);
                timer.resetOnExpiration = EditorGUILayout.Toggle("Reset On Expiration", timer.resetOnExpiration);
                if (GUILayout.Button("Start")) {
                    timer.StartTimer();
                }
                if (GUILayout.Button("Pause")) {
                    timer.Pause();
                }
                if (GUILayout.Button("Reset")) {
                    timer.Reset();
                }
                break;
        }
        serializedObject.ApplyModifiedProperties();


    }
}
