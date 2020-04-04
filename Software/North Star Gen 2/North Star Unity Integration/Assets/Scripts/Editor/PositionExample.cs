// The position of the window is displayed when it is
// external from Unity.

using UnityEngine;
using UnityEditor;

public class PositionExample : EditorWindow {
    Vector2Int p1;
    bool showBtn = true;

    [MenuItem("Examples/position")]
    static void Init() {
        GetWindow<PositionExample>("position");
    }

    void OnGUI() {
        var view = (EditorWindow)Resources.FindObjectsOfTypeAll(System.Type.GetType("UnityEditor.GameView,UnityEditor"))[0];
        Rect r = view.position;
        GUILayout.Label("Position: " + r.x + "x" + r.y);

        p1 = EditorGUILayout.Vector2IntField("Set the position:", p1);
        if (showBtn) {
            if (GUILayout.Button("Accept new position")) {

                r.x = p1.x;
                r.y = p1.y;

                //r = new Rect(r.position, new Vector2(2880, 1644)); //

                view.position = r;// new Rect(r.position, r.size);//new Vector2(2880, 1600 + 44));//r.size);// r;

                EditorApplication.delayCall += () => {
                    //view.position = r;// new Rect(r.position, view.position.size);

                    EditorApplication.delayCall += () => {
                        Debug.Log(view.position);
                        view.position = r;// new Rect(r.position, view.position.size);
                    };
                };
            }
        }
    }
}