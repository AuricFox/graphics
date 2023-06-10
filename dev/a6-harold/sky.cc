/** CSci-4611 Assignment 6: Harold
 */

#include "sky.h"


Sky::Sky() {
    
}


Sky::~Sky() {
    
}

void Sky::Init(ShaderProgram *stroke3d_shaderprog) {
    stroke3d_shaderprog_ = stroke3d_shaderprog;
}


/// Projects a 2D normalized screen point (e.g., the mouse position in normalized
/// device coordinates) to a 3D point on the "sky", which is really a huge sphere
/// (radius = 1500) that the viewer is inside.  This function should always return
/// true since any screen point can successfully be projected onto the sphere.
/// sky_point is set to the resulting 3D point.  Note, this function only checks
/// to see if the ray passing through the screen point intersects the sphere; it
/// does not check to see if the ray hits the ground or anything else first.
bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                        const Point2 &normalized_screen_pt, Point3 *sky_point)
{
    // TODO: Stitch together your worksheet implementation of this method
    Matrix4 camera_matrix = view_matrix.Inverse();
    Point3 eye = camera_matrix.ColumnToPoint3(3);
    Vector3 look = -camera_matrix.ColumnToVector3(2);
    
    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
    Vector3 dir = (mouseIn3d - eye);
    dir.Normalize();

    Ray eyeThroughMouse = mingfx::Ray::Ray(eye, dir);
    float time;

    return eyeThroughMouse.IntersectSphere(eye, 1500.0, &time, sky_point);
}




/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                       const Mesh &stroke2d_mesh, const Color &stroke_color)
{
    // TODO: Create a new SkyStroke and add it to the strokes_ array.
    Mesh m = stroke2d_mesh;
    std::vector<Point3> vList;
    for (int i = 0; i < m.num_vertices(); i++) { //Looping through the vertices, not the indices
        Point3 v = m.read_vertex_data(i);

        //Coverting 2D vertices to 3D sky points
        Point2 skyPoint = Point2(v.x(),v.y());
        ScreenPtHitsSky(view_matrix, proj_matrix, skyPoint, &v);

        vList.push_back(v); //Adding 3D vertex to lsit
    }

    if (vList.size() > 0) {
        m.SetVertices(vList); //Updating mesh

        SkyStroke stroke; //Initializing new stroke
        stroke.mesh = m;
        stroke.color = stroke_color;

        strokes_.push_back(stroke); //Saving new stroke to the array
    }
}


/// Draws all of the sky strokes
void Sky::Draw(const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {

    // Precompute matrices needed in the shader
    Matrix4 model_matrix; // identity
    Matrix4 modelview_matrix = view_matrix * model_matrix;
    
    // Draw sky meshes
    stroke3d_shaderprog_->UseProgram();
    stroke3d_shaderprog_->SetUniform("modelViewMatrix", modelview_matrix);
    stroke3d_shaderprog_->SetUniform("projectionMatrix", proj_matrix);
    for (int i=0; i<strokes_.size(); i++) {
        stroke3d_shaderprog_->SetUniform("strokeColor", strokes_[i].color);
        strokes_[i].mesh.Draw();
    }
    stroke3d_shaderprog_->StopProgram();
}

