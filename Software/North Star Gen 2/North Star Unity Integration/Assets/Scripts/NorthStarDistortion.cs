using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;
#endif


[RequireComponent(typeof(Camera))]
[ExecuteInEditMode]
public class NorthStarDistortion : MonoBehaviour {
    public Material _distortionMaterial;
    public Vector4 _offsetScaling;

    float[]  left_uv_to_rect_x = { -0.7530364531010308f,  0.8806592947908687f, -0.8357813137161849f,   0.3013989721607643f,  0.9991764544369446f, -0.2578159567698274f,  0.3278667335649757f, -0.4602577277109663f,  -0.23980700925448195f, -0.056891370605734376f, -0.1248008903440144f,  0.7641381600051023f,   0.20935445281014292f, -0.06256983016261788f,  0.25844580123833516f, -0.5098143951663658f };
    float[]  left_uv_to_rect_y = {  0.5612597403791647f, -1.1899589356849427f,  0.4652815794139322f,  -0.2737933233160801f,  0.3774305703820774f, -0.8110333901413378f,  1.2705775357104372f, -0.7461290557575936f,  -0.19222925521894155f,  0.936404121235537f,    -1.7109388784623627f,  0.9147182510080394f,   0.33073407860855586f, -1.1463700238163494f,   1.4965795269835196f,  -0.7090919632511286f };
    float[] right_uv_to_rect_x = { -0.2117125319456463f, -0.432262579698108f,   0.41675063901331316f, -0.14650788483832153f, 1.0941580384494245f, -0.30628109185189906f, 0.109119134429531f,   0.11642874201014344f, -0.2761527408488216f,  -0.4335709010559027f,    0.9626491769528918f, -0.5572405188216735f,   0.18342869894719088f,  0.37981945016058366f, -0.8718621504058989f,   0.5218968716935535f };
    float[] right_uv_to_rect_y = {  1.0129568069314265f, -2.110976542118192f,   1.4108474581893895f,  -0.7746290913232183f, -0.746419837008027f,   1.747642287758405f, - 1.5753294007072252f,  0.7143402603200871f,   0.5607717274125551f,  -1.5019493985594772f,    1.2539128525783017f, -0.42999735712430215f, -0.21517910830152714f,  0.5965062719847273f,  -0.5664205050494074f,   0.18545738302854597f};
    
    /// <summary> Open the Calibration.json and set the distortion parameters. </summary>
    protected void UpdateDistortion() {
        bool isLeft = transform.localPosition.x < 0f;
        _distortionMaterial.SetFloatArray("uv_to_rect_x", isLeft ? left_uv_to_rect_x : right_uv_to_rect_x);
        _distortionMaterial.SetFloatArray("uv_to_rect_y", isLeft ? left_uv_to_rect_y : right_uv_to_rect_y);
        _distortionMaterial.SetMatrix    ("camera_matrix", GetComponent<Camera>().projectionMatrix);
        _distortionMaterial.SetVector    ("offset_scaling", _offsetScaling);
        _distortionMaterial.SetFloat     ("is_left", isLeft ? 0.0f : 1.0f);
    }

    void OnEnable() { UpdateDistortion(); }


    public void OnRenderImage(RenderTexture source, RenderTexture destination) {
        Graphics.Blit(source, destination, _distortionMaterial);
    }

    void Update() {
        //If the Application is fullscreen, you don't need this... but if it isn't...
#if UNITY_EDITOR
        EditorWindow[] windows = (EditorWindow[])Resources.FindObjectsOfTypeAll(System.Type.GetType("UnityEditor.GameView,UnityEditor"));
        foreach(EditorWindow window in windows) {
            var serializedObject = new SerializedObject(window);
            var targetDisplay = serializedObject.FindProperty("m_TargetDisplay");

            if (targetDisplay.intValue == 1){//window.name.Equals("NorthStar Preview")) {
                Rect pos = window.position;

                // Correct for the Titlebar+Tab and Monitor offset
                pos = new Rect(pos.x - 1920f, pos.y + 44, pos.width, pos.height - 44);


                // Pass the fullscreen UV bottom left corner of each viewport in
                // Pass the fullscreen UV scale of each viewport in...
                // Remap the UVs inside the shader... GO!

                bool isLeft = transform.localPosition.x < 0f;

                // WORKING (FOR 1080P MAIN MONITOR UVS)
                //_offsetScaling = new Vector4((pos.x + (isLeft ? pos.width/2f : 0f)) / (pos.width/2f), (pos.y + pos.height) / pos.height,
                //                              pos.width/1920f,                                        (pos.height/1080f)/2f);

                // WORKING FOR 2880x1600 NORTH STAR UVS, OFFSET 1920 TO THE RIGHT
                _offsetScaling = new Vector4((pos.x + (isLeft ? pos.width / 2f : 0f)) / (pos.width / 2f), (pos.y + pos.height) / pos.height,
                                              pos.width / 2880f,                                                  (pos.height / 1600f) / 2f);
            }
        }
#endif
        UpdateDistortion();
    }
}
