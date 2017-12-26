using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public static class UI {
    public static GameObject CreateButton(string name, string text, Vector2 minAnchor, Vector2 maxAnchor, Transform parent = null, Font font = null, Color fontColor = new Color(), Color buttonColor = new Color(), int fontSize = 10, Sprite sprite = null, Texture texture = null, UnityEngine.Events.UnityAction method = null) {
        GameObject buttonGO = new GameObject(name);
        buttonGO.transform.SetParent(parent, false);
        buttonGO.AddComponent<RectTransform>();
        if (texture) {
            RawImage rawImage = buttonGO.AddComponent<RawImage>();
            rawImage.texture = texture;
            rawImage.color = buttonColor;
        } else {
            Image image = buttonGO.AddComponent<Image>();
            image.sprite = sprite;
            image.color = buttonColor;
        }

        Button b = buttonGO.AddComponent<Button>();
        b.onClick.AddListener(method);
        RectTransform rectTransform = buttonGO.GetComponent<RectTransform>();
        rectTransform.anchorMin = minAnchor;
        rectTransform.anchorMax = maxAnchor;
        rectTransform.offsetMin = Vector2.zero;
        rectTransform.offsetMax = Vector2.zero;
        rectTransform.localScale = Vector3.one;
        rectTransform.localRotation = Quaternion.identity;
        if (text != "") {
            CreateText("Text", text, Vector2.zero, Vector2.one, buttonGO.transform, font, fontColor, fontSize);
        }
        return buttonGO;
    }
    public static void AddButton(GameObject gameObject, string text, Vector2 minAnchor, Vector2 maxAnchor, Font font = null, Color fontColor = new Color(), Color buttonColor = new Color(), int fontSize = 10, Sprite sprite = null, Texture texture = null, UnityEngine.Events.UnityAction method = null) {
        if (texture) {
            RawImage rawImage = gameObject.AddComponent<RawImage>();
            rawImage.texture = texture;
            rawImage.color = buttonColor;
        } else {
            Image image = gameObject.AddComponent<Image>();
            image.sprite = sprite;
            image.color = buttonColor;
        }

        Button b = gameObject.AddComponent<Button>();
        b.onClick.AddListener(method);
        RectTransform rectTransform = gameObject.GetComponent<RectTransform>();
        rectTransform.anchorMin = minAnchor;
        rectTransform.anchorMax = maxAnchor;
        rectTransform.offsetMin = Vector2.zero;
        rectTransform.offsetMax = Vector2.zero;
        rectTransform.localScale = Vector3.one;
        rectTransform.localRotation = Quaternion.identity;
        if(text != "") {
            CreateText("Text", text, Vector2.zero, Vector2.one, gameObject.transform, font, fontColor, fontSize);
        }
    }

    public static GameObject CreateText(string name, string text, Vector2 minAnchor, Vector2 maxAnchor, Transform parent, Font font = null, Color fontColor = new Color(), int fontSize = 10, TextAnchor alignment = TextAnchor.MiddleCenter, HorizontalWrapMode hwm = HorizontalWrapMode.Wrap, VerticalWrapMode vwm = VerticalWrapMode.Truncate, bool bestFit = true, int minSize = 1, int maxSize = 100) {
        GameObject textGO = new GameObject(name);
        textGO.transform.parent = parent;
        textGO.AddComponent<RectTransform>();
        RectTransform rectTransform = textGO.GetComponent<RectTransform>();
        rectTransform.anchorMin = minAnchor;
        rectTransform.anchorMax = maxAnchor;
        rectTransform.offsetMin = Vector2.zero;
        rectTransform.offsetMax = Vector2.zero;
        Text t = textGO.AddComponent<Text>();
        t.text = text;
        t.alignment = alignment;
        if (font) {
            t.font = font;
        }
        if(fontColor != new Color()) {
            t.color = fontColor;
        }
        t.fontSize = fontSize;
        t.resizeTextForBestFit = bestFit;
        t.resizeTextMinSize = minSize;
        t.resizeTextMaxSize = maxSize;
        t.verticalOverflow = vwm;
        t.horizontalOverflow = hwm;
        return textGO;
    }
    public static GameObject CreatePanel(string name, Vector2 minAnchor, Vector2 maxAnchor, Transform parent, Sprite sprite = null, Texture texture = null, Color color = new Color()) {
        GameObject panelGO = new GameObject(name);
        panelGO.transform.parent = parent;
        panelGO.AddComponent<RectTransform>();
        RectTransform rectTransform = panelGO.GetComponent<RectTransform>();
        rectTransform.anchorMin = minAnchor;
        rectTransform.anchorMax = maxAnchor;
        rectTransform.offsetMin = Vector2.zero;
        rectTransform.offsetMax = Vector2.zero;
        if (texture) {
            RawImage rawImage = panelGO.AddComponent<RawImage>();
            rawImage.texture = texture;
            rawImage.color = color;
        } else {
            Image image = panelGO.AddComponent<Image>();
            image.sprite = sprite;
            image.color = color;
        }
        return panelGO;
    }

    public static GameObject CreateCanvas(Transform parent, Vector2 offset, RenderMode renderMode = RenderMode.ScreenSpaceOverlay, float width = 100, float height = 100) {
        GameObject canvas = new GameObject("Canvas", typeof(Canvas), typeof(CanvasScaler), typeof(GraphicRaycaster));
        canvas.GetComponent<Canvas>().renderMode = renderMode;
        canvas.transform.SetParent(parent, false);
        if(renderMode == RenderMode.WorldSpace) {
            Canvas c = canvas.GetComponent<Canvas>();
            c.renderMode = RenderMode.WorldSpace;
            RectTransform crect = canvas.GetComponent<RectTransform>();
            crect.anchorMin = Vector2.zero;
            crect.anchorMax = Vector2.zero;
            crect.position = offset;
            crect.sizeDelta = new Vector2(width, height);
        }
        return canvas;
    }
}

