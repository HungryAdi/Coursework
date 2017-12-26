using UnityEngine;
using System.Collections;

// Credit: Calle Erlandsson and John Francis Collins
public class Blob : MonoBehaviour {
    private class PropagateCollisions : MonoBehaviour {
        void OnCollisionEnter2D(Collision2D collision) {
            transform.parent.SendMessage("OnCollisionEnter2D", collision);
        }
    }

    public int width = 5;
    public int height = 5;
    public int referencePointsCount = 12;
    public float referencePointRadius = 0.25f;
    public float mappingDetail = 10;
    public float springDampingRatio = 0;
    public float springFrequency = 2;
    public PhysicsMaterial2D surfaceMaterial;

    GameObject[] referencePoints;
    int vertexCount;
    Vector3[] vertices;
    int[] triangles;
    Vector2[] uv;
    Vector3[,] offsets;
    float[,] weights;

    MaterialPropertyBlock mpb;
    Renderer rend;
    Vector4[] controls;
    int _Controls = Shader.PropertyToID("_Controls");

    void Start() {
        CreateReferencePoints();
        CreateMesh();
        MapVerticesToReferencePoints();

        mpb = new MaterialPropertyBlock();

        int num = vertexCount * referencePointsCount;
        Vector4[] offsetsSingle = new Vector4[num];
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < referencePointsCount; ++j) {
                Vector4 offs = offsets[i, j];
                offs.w = weights[i, j];
                offsetsSingle[j + i * referencePointsCount] = offs;
            }
        }

        mpb.SetVectorArray(Shader.PropertyToID("_Offsets"), offsetsSingle);
        mpb.SetFloat("_ControlCount", referencePointsCount + 0.5f);

        controls = new Vector4[referencePointsCount];
        rend = GetComponent<Renderer>();
        rend.material.SetInt("_ControlPoints", referencePointsCount);
    }

    public void Restart() {
        ClearAllReferencePoints();
        CreateReferencePoints();
        CreateMesh();
        MapVerticesToReferencePoints();

        mpb = new MaterialPropertyBlock();

        int num = vertexCount * referencePointsCount;
        Vector4[] offsetsSingle = new Vector4[num];
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < referencePointsCount; ++j) {
                Vector4 offs = offsets[i, j];
                offs.w = weights[i, j];
                offsetsSingle[j + i * referencePointsCount] = offs;
            }
        }

        mpb.SetVectorArray(Shader.PropertyToID("_Offsets"), offsetsSingle);
        mpb.SetFloat("_ControlCount", referencePointsCount + 0.5f);

        controls = new Vector4[referencePointsCount];
        rend = GetComponent<Renderer>();
        rend.material.SetInt("_ControlPoints", referencePointsCount);
    }

    public void CreateReferencePoints() {
        Rigidbody2D rigidbody = GetComponent<Rigidbody2D>();
        referencePoints = new GameObject[referencePointsCount];
        Vector3 offsetFromCenter = ((0.5f - referencePointRadius) * Vector3.up);
        float angle = 360.0f / referencePointsCount;
        PlayerInfo pi = GetComponent<PlayerInfo>();
        int playerNumber = 1;
        if (pi) {
            playerNumber = pi.PlayerNumber;
        }
        gameObject.layer = LayerMask.NameToLayer("Player " + playerNumber);
        for (int i = 0; i < referencePointsCount; i++) {
            referencePoints[i] = new GameObject("Collider " + (i + 1));
            referencePoints[i].tag = gameObject.tag;
            referencePoints[i].layer = gameObject.layer;
            referencePoints[i].AddComponent<PropagateCollisions>();
            referencePoints[i].transform.parent = transform;
            Quaternion rotation = Quaternion.AngleAxis(angle * (i - 1), Vector3.back);
            referencePoints[i].transform.localPosition = rotation * offsetFromCenter;

            Rigidbody2D body = referencePoints[i].AddComponent<Rigidbody2D>();
            body.freezeRotation = true;
            body.interpolation = rigidbody.interpolation;
            body.collisionDetectionMode = rigidbody.collisionDetectionMode;
            body.drag = rigidbody.drag;
            body.angularDrag = rigidbody.angularDrag;
            body.gravityScale = rigidbody.gravityScale;
            body.mass = 0;

            CircleCollider2D collider = referencePoints[i].AddComponent<CircleCollider2D>();
            collider.radius = referencePointRadius * transform.localScale.x;
            if (surfaceMaterial) {
                collider.sharedMaterial = surfaceMaterial;
            }

            AttachWithSpringJoint(referencePoints[i], gameObject);
            if (i > 0) {
                AttachWithSpringJoint(referencePoints[i], referencePoints[i - 1]);
            }
        }
        AttachWithSpringJoint(referencePoints[0], referencePoints[referencePointsCount - 1]);
        Physics2D.IgnoreLayerCollision(LayerMask.NameToLayer("Player " + playerNumber), LayerMask.NameToLayer("Player " + playerNumber));
    }

    public void ClearAllReferencePoints() {
        foreach(GameObject go in referencePoints) {
            if (go) {
                Destroy(go);
            }
        }
    }

    void AttachWithSpringJoint(GameObject referencePoint, GameObject connected) {
        SpringJoint2D springJoint = referencePoint.AddComponent<SpringJoint2D>();
        springJoint.connectedBody = connected.GetComponent<Rigidbody2D>();
        springJoint.connectedAnchor = LocalPosition(referencePoint) - LocalPosition(connected);
        springJoint.distance = 0;
        springJoint.dampingRatio = springDampingRatio;
        springJoint.frequency = springFrequency;
    }

    void CreateMesh() {
        vertexCount = (width + 1) * (height + 1);

        int trianglesCount = width * height * 6;
        vertices = new Vector3[vertexCount];
        triangles = new int[trianglesCount];
        uv = new Vector2[vertexCount];
        int t;

        for (int y = 0; y <= height; y++) {
            for (int x = 0; x <= width; x++) {
                int v = (width + 1) * y + x;
                vertices[v] = new Vector3(x / (float)width - 0.5f,
                        y / (float)height - 0.5f, 0);
                uv[v] = new Vector2(x / (float)width, y / (float)height);

                if (x < width && y < height) {
                    t = 3 * (2 * width * y + 2 * x);

                    triangles[t] = v;
                    triangles[++t] = v + width + 1;
                    triangles[++t] = v + width + 2;
                    triangles[++t] = v;
                    triangles[++t] = v + width + 2;
                    triangles[++t] = v + 1;
                }
            }
        }

        Mesh mesh = GetComponent<MeshFilter>().mesh;
        mesh.vertices = vertices;
        mesh.uv = uv;
        mesh.triangles = triangles;
        mesh.RecalculateBounds();
    }

    void MapVerticesToReferencePoints() {
        offsets = new Vector3[vertexCount, referencePointsCount];
        weights = new float[vertexCount, referencePointsCount];

        for (int i = 0; i < vertexCount; i++) {
            float totalWeight = 0;

            for (int j = 0; j < referencePointsCount; j++) {
                offsets[i, j] = vertices[i] - LocalPosition(referencePoints[j]);
                weights[i, j] = 1.0f / Mathf.Pow(offsets[i, j].magnitude, mappingDetail);
                totalWeight += weights[i, j];
            }

            for (int j = 0; j < referencePointsCount; j++) {
                weights[i, j] /= totalWeight;
            }
        }
    }

    void Update() {
        // update shader control points
        for (int i = 0; i < referencePointsCount; ++i) {
            controls[i] = LocalPosition(referencePoints[i]);
        }
        mpb.SetVectorArray(_Controls, controls);
        rend.SetPropertyBlock(mpb);
    }

    Vector3 LocalPosition(GameObject obj) {
        return transform.InverseTransformPoint(obj.transform.position);
    }
}
