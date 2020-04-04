using UnityEditor;
using UnityEngine;
using System;
using System.Reflection;

namespace Klak
{
    class GameViewLayouter : EditorWindow
    {
        #region Editor resources

        static GUIContent _textViewCount = new GUIContent("Number of Screens");

        static GUIContent[] _viewLabels = {
            new GUIContent("Screen 2"),
            new GUIContent("Screen 3"), new GUIContent("Screen 4"),
            new GUIContent("Screen 5"), new GUIContent("Screen 6"),
            new GUIContent("Screen 7"), new GUIContent("Screen 8"),
        };

        static GUIContent[] _optionLabels = {
            new GUIContent("Display 1"), new GUIContent("Display 2"),
            new GUIContent("Display 3"), new GUIContent("Display 4"),
            new GUIContent("Display 5"), new GUIContent("Display 6"),
            new GUIContent("Display 7"), new GUIContent("Display 8"),
        };

        static int[] _optionValues = { 0, 1, 2, 3, 4, 5, 6, 7 };

        #endregion

        #region Private variables

        [SerializeField] GameViewLayoutTable _table;

        #endregion

        #region UI methods

        [MenuItem("Window/Game View Layouter")]
        static void OpenWindow()
        {
            EditorWindow.GetWindow<GameViewLayouter>("Layouter").Show();
        }

        void OnGUI()
        {
            var serializedTable = new UnityEditor.SerializedObject(_table);

            EditorGUILayout.BeginVertical();

            // Screen num box
            var viewCount = serializedTable.FindProperty("viewCount");
            EditorGUILayout.PropertyField(viewCount, _textViewCount);

            EditorGUILayout.Space();

            // View-display table
            var viewTable = serializedTable.FindProperty("viewTable");
            for (var i = 0; i < viewCount.intValue; i++)
            {
                EditorGUILayout.IntPopup(
                    viewTable.GetArrayElementAtIndex(i),
                    _optionLabels, _optionValues, _viewLabels[i]
                );
            }

            EditorGUILayout.Space();

            // Function buttons
            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.Space();
            if (GUILayout.Button("Layout")) LayoutViews();
            EditorGUILayout.Space();
            if (GUILayout.Button("Close All")) CloseAllViews();
            EditorGUILayout.Space();
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.EndVertical();

            //serializedTable.ApplyModifiedProperties();
        }

        #endregion

        #region Private properties and methods

        // Retrieve the hidden GameView type.
        static Type GameViewType {
            get { return System.Type.GetType("UnityEditor.GameView,UnityEditor"); }
        }

        // Change the target display of a game view.
        static void ChangeTargetDisplay(EditorWindow view, int displayIndex)
        {
            var serializedObject = new SerializedObject(view);
            var targetDisplay = serializedObject.FindProperty("m_TargetDisplay");
            targetDisplay.intValue = displayIndex;
            serializedObject.ApplyModifiedPropertiesWithoutUndo();
        }

        // Close all the game views.
        static void CloseAllViews()
        {
            foreach (EditorWindow view in Resources.FindObjectsOfTypeAll(GameViewType))
                view.Close();
        }

        // Send a game view to a given screen.
        void SendViewToScreen(EditorWindow view, int screenIndex) {
#if UNITY_2019_1_OR_NEWER
            const int kMenuHeight = 38;
#else
            const int kMenuHeight = 17;
#endif

            //Vector2 origin = new Vector2(1920, -38);
            //Vector2 size   = new Vector2(2880,               //Display.displays[screenIndex].systemWidth, 
            //                             1600 + kMenuHeight);//Display.displays[screenIndex].systemHeight + kMenuHeight);
            //view.position = newRect;
            //view.maxSize = new Vector2(8192, 8192); //view.minSize = size; //

            //view.minSize = new Vector2(2880, 1570); 

            Rect r = view.position;
            r.x = 1920;
            r.y = -38;//40;//-kMenuHeight;

            Debug.Log("Was " + r);
            if (r.width != 2880){// || r.height < 1565) {
                r.width  = 2880;
                r.height = 1638;
            }

            view.position = r;
            EditorApplication.delayCall += () => {
                view.position = r;
            };
        }

        // Instantiate and layout game views based on the setting.
        void LayoutViews() {
            var views = (EditorWindow[])Resources.FindObjectsOfTypeAll(GameViewType);
            if (views.Length != _table.viewCount) {
                CloseAllViews();

                for (var i = 0; i < _table.viewCount; i++) {
                    var view = (EditorWindow)ScriptableObject.CreateInstance(GameViewType);
                    view.Show(true);
                    ChangeTargetDisplay(view, _table.viewTable[i]);
                    SendViewToScreen(view, i + 1);
                }
            } else {
                for (var i = 0; i < views.Length; i++) {
                    //view.Show();
                    //ChangeTargetDisplay(views[i], _table.viewTable[i]);
                    SendViewToScreen(views[i], i + 1);
                }
            }


        }

        #endregion
    }
}
