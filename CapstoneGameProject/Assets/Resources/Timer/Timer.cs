using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Timer : MonoBehaviour {
    public enum Mode {
        TIMER,
        STOPWATCH,
        ABILITY_COOLDOWN,
        CLOCK
    }
    public Mode mode;
    public bool render;
    public bool resetOnExpiration;
    public float duration;
    public Font font;
    public int fontSize;
    public Vector2 minAnchor;
    public Vector2 maxAnchor;
    Text timerText;
    GameObject timerImageGO;
    Image cooldownImage;
    Sprite cooldownSprite;
    float timeLeft;
    DateTime localDate;
    bool paused;
    GameObject canvas;
    Mode modeLastFrame;
    bool isGlobalCooldown = false;
    float globalCooldown;
    // Use this for initialization
    void Start() {
        paused = mode != Mode.CLOCK;
        if (render) {
            Canvas c = GetComponentInParent<Canvas>();
            if (!c) {
                canvas = UI.CreateCanvas(transform, Vector2.zero);
            } else {
                canvas = gameObject;
            }
            timerText = UI.CreateText("Timer Text", "", minAnchor, maxAnchor, canvas.transform, font, Color.white, fontSize, TextAnchor.MiddleCenter, HorizontalWrapMode.Wrap, VerticalWrapMode.Truncate, false).GetComponent<Text>();
            UpdateText();
        }
        Reset();
    }

    // Update is called once per frame
    void Update() {
        if (canvas) {
            canvas.SetActive(render);
        }
        
        if(mode != modeLastFrame) {
            Reset();
        }
        modeLastFrame = mode;
        CreateTimerImage();
        if (!Expired() && !paused) {
            UpdateVariables();
            UpdateText();

        }
        if(Expired() && resetOnExpiration) {
            Reset();
        }
    }

    void UpdateText() {
        if (render) {
            switch (mode) {
                case Mode.TIMER:
                    timerText.text = TimeToString(timeLeft);
                    break;
                case Mode.ABILITY_COOLDOWN:
                    timerText.text =timeLeft < 1 ? timeLeft.ToString("F1") : "" + Mathf.FloorToInt(timeLeft);
                    if (timerImageGO) {
                        cooldownImage.fillAmount = isGlobalCooldown? timeLeft / globalCooldown : timeLeft / duration;
                    }
                    break;
                case Mode.CLOCK:
                    localDate = DateTime.Now;
                    timerText.text = localDate.ToLongTimeString();
                    break;
                case Mode.STOPWATCH:
                    timerText.text = TimeToString(timeLeft);
                    break;
            }
        }

    }

    void UpdateVariables() {
        switch (mode) {
            case Mode.TIMER:
            case Mode.ABILITY_COOLDOWN:
                timeLeft -= Time.deltaTime;
                timeLeft = Mathf.Clamp(timeLeft, 0, timeLeft);
                break;
            case Mode.STOPWATCH:
                timeLeft += Time.deltaTime;
                break;
        }
    }

    void CreateTimerImage() {
        if (!timerImageGO && mode == Mode.ABILITY_COOLDOWN && render) {
            timerImageGO = new GameObject("Timer Overlay");
            cooldownSprite = Resources.Load<Sprite>("Timer/circle");
            cooldownImage = timerImageGO.AddComponent<Image>();
            cooldownImage.type = Image.Type.Filled;
            cooldownImage.fillMethod = Image.FillMethod.Radial360;
            cooldownImage.fillOrigin = (int)Image.Origin360.Top;
            cooldownImage.color = new Color(0, 0, 0, 0.3f);
            cooldownImage.sprite = cooldownSprite;
            RectTransform imageRect = timerImageGO.GetComponent<RectTransform>();
            timerImageGO.transform.SetParent(canvas.transform, false);
            imageRect.anchorMin = minAnchor;
            imageRect.anchorMax = maxAnchor;
            imageRect.offsetMin = Vector2.zero;
            imageRect.offsetMax = Vector2.zero;
        }
        if (timerImageGO) {
            timerImageGO.SetActive(mode == Mode.ABILITY_COOLDOWN && timeLeft != duration);
        }
        
    }
    string TimeToString(float time) {
        // turns float in seconds to string in format 'hh:mm:ss.s'
        int hours = Mathf.FloorToInt(time / 3600f);
        int minutes = Mathf.FloorToInt(time / 60f) - hours * 60;
        float seconds = time - minutes * 60f - hours * 3600f;
        string secondsStr = (seconds < 10 ? "0" : "") + seconds.ToString("F1");
        string hoursStr = (hours > 0 ? hours + ":" : "");
        return hoursStr + (minutes < 10 ? "0" : "") + minutes + ":" + secondsStr;
    }

    public void StartTimer() {
        paused = false;
    }

    public float GetTimeLeft() {
        return timeLeft;
    }

    public void Pause() {
        paused = true;
    }
    public bool IsRunning() {
        return !paused;
    }

    public void GlobalCooldown(float gcd) {
        if(mode == Mode.ABILITY_COOLDOWN && paused) {
            StartCoroutine(GCD(gcd));
        }
    }

    IEnumerator GCD(float gcd) {
        timeLeft = gcd;
        globalCooldown = gcd;
        paused = false;
        isGlobalCooldown = true;
        yield return new WaitForSeconds(gcd);
        isGlobalCooldown = false;
        Reset();
    }

    public bool Expired() {
        switch (mode) {
            case Mode.TIMER:
            case Mode.ABILITY_COOLDOWN:
                return timeLeft <= 0;
            case Mode.STOPWATCH:
                if(duration < 0) {
                    return false;
                }
                return timeLeft >= duration;
            default:
                return false;
        }
    }

    public void Reset() {
        switch (mode) {
            case Mode.TIMER:
            case Mode.ABILITY_COOLDOWN:
                timeLeft = duration;
                paused = true;
                break;
            case Mode.STOPWATCH:
                timeLeft = 0;
                paused = true;
                break;
            default:
                paused = false;
                break;
        }
        UpdateText();
    }
}
