using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

[RequireComponent(typeof(Selectable))]
public class ButtonSelected : MonoBehaviour, ISelectHandler, IDeselectHandler {
    private Selectable selectable;
    private RectTransform rt;
    // Use this for initialization
    void Start() {
        selectable = GetComponent<Selectable>();
        rt = GetComponent<RectTransform>();
    }

    public void OnSelect(BaseEventData eventData) {
        rt.localScale = new Vector3(1.25f, 1.25f, 1f);
        //Debug.Log("Selected");
    }

    public void OnDeselect(BaseEventData eventData) {
        rt.localScale = Vector3.one;
    }


}
