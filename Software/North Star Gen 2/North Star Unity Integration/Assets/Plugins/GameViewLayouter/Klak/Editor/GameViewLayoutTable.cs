using UnityEngine;
using UnityEditor;

namespace Klak
{
    class GameViewLayoutTable : ScriptableObject
    {
        [SerializeField, Range(1, 7)] public int viewCount = 2;

        // This array contains the display indices assigned to each view:
        // the (n)th view is to show the (viewTable[n])th display.
        [SerializeField] public int[] viewTable = { 0, 1, 2, 3, 4, 5, 6 };

        /*
        [MenuItem("Assets/Create/Game View Layout Table")]
        static void CreateAsset()
        {
            var table = ScriptableObject.CreateInstance<GameViewLayoutTable>();
            AssetDatabase.CreateAsset(table, "Assets/GameViewLayout.asset");
        }
        */
    }
}
