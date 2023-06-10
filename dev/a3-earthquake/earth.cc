/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    //const int nslices = 10;
    //const int nstacks = 10;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    // As a demo, we'll add a square with 2 triangles.
    std::vector<Point3> verticesPlane;
    std::vector<Vector3> normalPlane;

    std::vector<Point3> verticesSphere;
    std::vector<Vector3> normalSphere;

    std::vector<Point2> tex_coords;
    std::vector<unsigned int> indices;

    // four vertices
    // Top left corner      (-M_PI, M_PI_2, 0)
    // Bottom left corner   (-M_PI, -M_PI_2, 0)
    // Top right corner     (M_PI, M_PI_2, 0)
    // Bottom right corner  (M_PI, -M_PI_2, 0)

    const int n = 100; //nslices and nstacks
    double fn = 100.0;
    double x = -M_PI;
    //Forming Points
    for (int i = 0; i <= n; i ++) { //X axis
        double y = M_PI_2;

        for (int j = 0; j <= n; j++) { //Y axis
            //Plane Elements
            verticesPlane.push_back(Point3(x, y, 0)); //Adding vertices
            normalPlane.push_back(Vector3(0, 0, 1));

            //Sphere Elements
            Point3 p = LatLongToSphere(x, y);
            verticesSphere.push_back(p);
            normalSphere.push_back(Vector3(p[0],p[1],p[2]).ToUnit());

            tex_coords.push_back(Point2((float) i/fn, (float) j/fn)); //Adding texture
            //printf("Point(%g, %g, 0)\n", x, y);

            y -= (M_PI / (fn));
        }
        x += (M_PI / (fn / 2.0));
    }

    //Forming squares with two triangles per iteration
    for (int i = 0; i < n; i++) {
        for (int j = i * (n + 1); j < (i * (n + 1) + n); j++) {
            //First triangle
            indices.push_back(j + n + 2);
            indices.push_back(j + n + 1);
            indices.push_back(j);

            //Second triangle
            indices.push_back(j + 1);
            indices.push_back(j + n + 2);
            indices.push_back(j);
        }
    }
    

    // indices into the arrays above for the first triangle
    //indices.push_back(0);
    //indices.push_back(1);
    //indices.push_back(2);
    
    // indices for the second triangle, note some are reused
    //indices.push_back(0);
    //indices.push_back(2);
    //indices.push_back(3);
    
    earth_mesh_.SetVertices(verticesPlane); //Setting points
    earth_mesh_.SetNormals(normalPlane);

    //earth_mesh_.SetVertices(verticesSphere);
    //earth_mesh_.SetNormals(normalSphere);

    earth_mesh_.SetIndices(indices); //Setting triangles
    earth_mesh_.SetTexCoords(0, tex_coords); //Setting texture
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place
    double x = cos(latitude) * sin(longitude);
    double y = sin(latitude);
    double z = cos(latitude) * cos(longitude);
    //printf("Point(%g, %g, %g)\n", x, y, z);
    return Point3(x, y, z);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    //double lat = GfxMath::ToRadians(latitude);
    //double lon = GfxMath::ToRadians(longitude);
    double x = longitude;
    double y = latitude;
    //printf("Point(%g, %g, 0)\n", x, y);
    return Point3(x, y, 0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.read_triangle_indices_data(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.read_vertex_data(indices[0]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[1]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005f);
    }
}

