/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


// Remember in C++, the .h file list all the functions and member variables that are part of the class!
// Look there first to understand what is part of the CarSoccer class, then look below to see how each
// function is implemented.


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // If you are having trouble driving the car with the keybaord, you can set this to true to use
    // the mouse instead.  The mouse controls are based on the postion of the mouse cursor on the window.
    // There is a "dead zone" in the middle of the window, and if you move the mouse up/down or left/right
    // outside of that zone, it is like pushing the up/down and/or left/right keys on the keyboard
    use_mouse_ = false;
        
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


void CarSoccer::OnMouseMove(const Point2& pos, const Vector2& delta)
{
    mouse_pos_ = PixelsToNormalizedDeviceCoords(pos);
}
//================================================================================================================================================
// RELAUNCHING BALL
///===============================================================================================================================================
void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        std::cout << "SPACE PRESSED" << std::endl;
        ball_.Reset();
    }
}
///============================================================================================================================================
/// This is a little utility function that is helpful.  It treats the arrow keys like a joystick or D-pad on a game controller
/// and returns the direction you are pressing as a 2D vector, taking into account the fact that you might be holding
/// down more than one key at a time.
Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;

    if (use_mouse_) {
        // threshold defines the size of the "dead zone" in the middle of the screen
        // if the mouse's x,y position falls outside of this, then it is like pushing
        // the corresponding key on the keyboard
        const float threshold = 0.2f;
        dir[0] = 0;
        if (mouse_pos_[0] < -threshold) {
            dir[0] = -1;
        }
        else if (mouse_pos_[0] > threshold) {
            dir[0] = 1;
        }
        dir[1] = 0;
        if (mouse_pos_[1] < -threshold) {
            dir[1] = -1;
        }
        else if (mouse_pos_[1] > threshold) {
            dir[1] = 1;
        }
    }
    else {
        // the default user interface is to use the arrow keys on the keyboard.
        // like a D-pad on a game controller, you can hold more than one key down at a time if you want.
        if (IsKeyDown(GLFW_KEY_LEFT))
            dir[0]--;
        if (IsKeyDown(GLFW_KEY_RIGHT))
            dir[0]++;
        if (IsKeyDown(GLFW_KEY_UP))
            dir[1]++;
        if (IsKeyDown(GLFW_KEY_DOWN))
            dir[1]--;
    }

    return dir;
}
//================================================================================================================================================
// UPDATES POSITION
//================================================================================================================================================
// dt is for "Delta Time", the elapsed time in seconds since the last frame
void CarSoccer::UpdateSimulation(double dt) {
    Vector2 dpad_dir = joystick_direction();
    std::cout << "D-Pad Direction: " << dpad_dir << std::endl;

    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball based on the elapsed time and checking for collisions.  Filling
    // in this routine is the main part of the assignment.

    //dp = v*dt + 1/2*a*dt^2
    ball_.set_position(ball_.position() + ball_.velocity() * dt); //Moving the ball       

    //BALL COLLISIONS-----------------------------------------------------------------------------------------------------
    //Ball collides with ground
    if (ball_.position()[1] < ball_.radius()) { //Ball is bellow the ground, needs correction
        ball_.set_position(Point3(ball_.position()[0], ball_.radius(), ball_.position()[2])); //Raising ball above ground
        Matrix4 N = Matrix4::Scale(Vector3(0.8, -0.8, 0.8)); //Reflection matrix
        ball_.set_velocity(N * ball_.velocity());
    }
    else if (ball_.position()[1] == ball_.radius()) { //Ball is touching the ground
        Matrix4 N = Matrix4::Scale(Vector3(0.8, -0.8, 0.8)); //Reflection matrix
        ball_.set_velocity(N * ball_.velocity());
    }
    
    //Ball collides with front or back wall---------------------------------------------------------------------------------------
    if ((ball_.position()[2] < (-50.0 + ball_.radius())) || (ball_.position()[2] > (50.0 - ball_.radius()))) { 
        if ((ball_.position()[0] >= -10.0) && (ball_.position()[0] <= 10.0) && (ball_.position()[1] <= 10.0)) { //Goal
            ball_.Reset();
            car_.Reset();
        }
        else if(ball_.position()[2] < -50.0 + ball_.radius()){ //In back wall
            ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], (-50.0 + ball_.radius()))); //Moving ball off wall
            Matrix4 N = Matrix4::Scale(Vector3(0.8, 0.8, -0.8)); //Reflection matrix
            ball_.set_velocity(N * ball_.velocity());
        }
        else if (ball_.position()[2] > 50.0 - ball_.radius()) { //In front wall
            ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], (50.0 - ball_.radius()))); //Moving ball off wall
            Matrix4 N = Matrix4::Scale(Vector3(0.8, 0.8, -0.8)); //Reflection matrix
            ball_.set_velocity(N * ball_.velocity());
        }
    }
    else if ((ball_.position()[2] == (-50.0 + ball_.radius())) || (ball_.position()[2] == (50.0 - ball_.radius()))) { //Touching wall
        if (ball_.position()[0] >= -10.0 && ball_.position()[0] <= 10.0 && ball_.position()[1] <= 10.0) { //Goal
            ball_.Reset();
            car_.Reset();
        }
        else { //In back wall
            Matrix4 N = Matrix4::Scale(Vector3(0.8, 0.8, -0.8)); //Reflection matrix
            ball_.set_velocity(N * ball_.velocity());
        }        
    }
    
    //Ball collides with left or right wall----------------------------------------------------------------------------
    if ((ball_.position()[0] < (-40.0 + ball_.radius())) || (ball_.position()[0] > (40.0 - ball_.radius()))) {
        if (ball_.position()[0] < -40.0 + ball_.radius()) { //In left wall
            ball_.set_position(Point3((-40.0 + ball_.radius()), ball_.position()[1], ball_.position()[2])); //Moving ball off wall
            Matrix4 N = Matrix4::Scale(Vector3(-0.8, 0.8, 0.8)); //Reflection matrix
            ball_.set_velocity(N * ball_.velocity());
        }
        else if (ball_.position()[0] > 40.0 - ball_.radius()) { //In right wall
            ball_.set_position(Point3((40.0 - ball_.radius()), ball_.position()[1], ball_.position()[2])); //Moving ball off wall
            Matrix4 N = Matrix4::Scale(Vector3(-0.8, 0.8, 0.8)); //Reflection matrix
            ball_.set_velocity(N * ball_.velocity());
        }
    }
    else if ((ball_.position()[0] == (-40.0 + ball_.radius())) || (ball_.position()[0] == (40.0 - ball_.radius()))) { //Touching wall
        Matrix4 N = Matrix4::Scale(Vector3(0.8, 0.8, -0.8)); //Reflection matrix
        ball_.set_velocity(N * ball_.velocity());
    }
    
    //Ball collides with ceiling-------------------------------------------------------------------------------------------------
    if (ball_.position()[1] > 35.0 - ball_.radius()) { //Ball is above the ceiling, needs correction
        ball_.set_position(Point3(ball_.position()[0], 35.0 - ball_.radius(), ball_.position()[2])); //Lowering ball below ceiling
        Matrix4 N = Matrix4::Scale(Vector3(0.8, -0.8, 0.8)); //Reflection matrix
        ball_.set_velocity(N * ball_.velocity());
    }
    else if (ball_.position()[1] == 35.0 - ball_.radius()) { //Ball is touching the ceiling
        Matrix4 N = Matrix4::Scale(Vector3(0.8, -0.8, 0.8)); //Reflection matrix
        ball_.set_velocity(N * ball_.velocity());
    }

    ball_.set_velocity(ball_.velocity() + Vector3(0, -20.0, 0) * dt); //dv = v + a*dt, updating velocity w/ acceleration applied

    //Ball collides with car------------------------------------------------------------------------------------------------------
    Vector3 carAndBall = Vector3(ball_.position() - car_.position());
    if (carAndBall.Length() < car_.collision_radius() + ball_.radius()) { //Ball is inside car
        Vector3 hit = Vector3(ball_.position() - car_.position()); //Finding vector from car to ball
        hit.Normalize(); //Normalizing vector

        Vector3 move = (ball_.radius() + car_.collision_radius()) * hit;
        ball_.set_position(Point3(car_.position() + move)); //Moving ball outside of car

        Vector3 r = ball_.velocity() - car_.velocity(); //Relative velocity
        Matrix4 N = Matrix4::Scale(Vector3((-1.0) * hit)); //Reflection matrix

        if (ball_.position()[2] <= car_.position()[2]) { //Ball is in front of car
            ball_.set_velocity((N * r) + car_.velocity()); //Updating ball's velocity
        }
        else if (ball_.position()[2] > car_.position()[2]) { //Ball is behind car
            ball_.set_velocity((N * r) - car_.velocity()); //Updating ball's velocity
        }
    }
    else if (carAndBall.Length() == car_.collision_radius() + ball_.radius()) {
        Vector3 hit = Vector3(ball_.position() - car_.position()); //Finding vector from car to ball
        hit.Normalize(); //Normalizing vector

        Vector3 r = ball_.velocity() - car_.velocity(); //Relative velocity
        Matrix4 N = Matrix4::Scale(Vector3((-1.0) * hit)); //Reflection matrix
        
        if (ball_.position()[2] <= car_.position()[2]) { //Ball is in front of car
            ball_.set_velocity((N * r) + car_.velocity()); //Updating ball's velocity
        }
        else if (ball_.position()[2] > car_.position()[2]) { //Ball is behind car
            ball_.set_velocity((N * r) - car_.velocity()); //Updating ball's velocity
        }
    }
    //---------------------------------------------------------------------------------------------
    // Example: This is not the "correct way" to drive the car, but this code
    // will at least move the car around for testing
    float metersPerSec = 30.0f;
    car_.set_speed(30.0);
    car_.set_position(car_.position() + car_.speed() * Vector3(dpad_dir[0], 0, -dpad_dir[1]) * dt);

    //CAR COLLISIONS----------------------------------------------------------------------------------------
    if (car_.position()[0] < -40.0 + car_.collision_radius()) { //Inside left wall
        car_.set_position(Point3(-40.0 + car_.collision_radius(), car_.position()[1], car_.position()[2]));
    }
    else if (car_.position()[0] > 40.0 - car_.collision_radius()) { //Inside right wall
        car_.set_position(Point3(40.0 - car_.collision_radius(), car_.position()[1], car_.position()[2]));
    }
    if (car_.position()[2] < -50.0 + car_.collision_radius()) { //Inside back wall
        car_.set_position(Point3(car_.position()[0], car_.position()[1], -50.0 + car_.collision_radius()));
    }
    else if (car_.position()[2] > 50.0 - car_.collision_radius()) { //Inside front wall
        car_.set_position(Point3(car_.position()[0], car_.position()[1], 50.0 - car_.collision_radius()));
    }
}
//================================================================================================================================================

void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen (don't worry if you get a depricated warning on this line in OSX)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}

//================================================================================================================================================
// SOCCER FIELD DISPLAY
//================================================================================================================================================
void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the car and the ball
    car_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    ball_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    
    // Draw the field with the field texture on it.
    Color col(16.0f/255.0f, 46.0f/255.0f, 9.0f/255.0f);
    Matrix4 M = Matrix4::Translation(Vector3(0.0f, -0.201f, 0.0f)) * Matrix4::Scale(Vector3(50.0f, 1.0f, 60.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f)) * Matrix4::Scale(Vector3(40.0f, 1.0f, 50.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    //------------------------------------------------------------------------------------------------------------------------------------
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    std::vector<Point3> lineLUC; // Left upper corner (-40, 35, -50);
    lineLUC.push_back(Point3(-40.0f, 0.0f, -50.0f)); //Bottom point
    lineLUC.push_back(Point3(-40.0f, 35.0f, -50.0f)); //Top point
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), lineLUC, QuickShapes::LinesType::LINE_LOOP, 0.05);

    std::vector<Point3> lineRUC; // Right upper corner (40, 35, -50);
    lineRUC.push_back(Point3(40.0f, 0.0f, -50.0f)); //Bottom point
    lineRUC.push_back(Point3(40.0f, 35.0f, -50.0f)); //Top point
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), lineRUC, QuickShapes::LinesType::LINE_LOOP, 0.05);

    std::vector<Point3> lineRLC; // Right lower corner (40, 35, 50);
    lineRLC.push_back(Point3(40.0f, 0.0f, 50.0f)); //Bottom point
    lineRLC.push_back(Point3(40.0f, 35.0f, 50.0f)); //Top point
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), lineRLC, QuickShapes::LinesType::LINE_LOOP, 0.05);

    std::vector<Point3> lineLLC; // Left lower corner (-40, 35, 50);
    lineLLC.push_back(Point3(-40.0f, 0.0f, 50.0f)); //Bottom point
    lineLLC.push_back(Point3(-40.0f, 35.0f, 50.0f)); //Top point
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), lineLLC, QuickShapes::LinesType::LINE_LOOP, 0.05);

    std::vector<Point3> topBoundry; // Top of the soccer field (-40, 35, -50);
    topBoundry.push_back(Point3(-40.0f, 35.0f, -50.0f)); //Top left corner
    topBoundry.push_back(Point3(40.0f, 35.0f, -50.0f)); //Top right corner
    topBoundry.push_back(Point3(40.0f, 35.0f, 50.0f)); //Bottom right corner
    topBoundry.push_back(Point3(-40.0f, 35.0f, 50.0f)); //Bottom left corner
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 1, 1), topBoundry, QuickShapes::LinesType::LINE_LOOP, 0.05);

    std::vector<Point3> goal1; // Top goal field (-10, 0, -50) <-> (10, 0, -50);
    goal1.push_back(Point3(-10.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 1.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 1.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 2.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 2.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 3.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 3.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 4.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 4.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 5.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 5.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 6.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 6.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 7.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 7.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 8.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 8.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 9.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 9.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(10.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(9.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(9.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(8.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(8.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(7.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(7.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(6.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(6.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(5.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(5.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(4.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(4.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(3.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(3.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(2.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(2.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(1.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(1.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(0.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(0.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-1.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-1.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-2.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-2.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-3.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-3.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-4.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-4.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-5.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-5.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-6.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-6.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-7.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-7.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-8.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-8.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-9.0f, 0.0f, -50.0f));
    goal1.push_back(Point3(-9.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 10.0f, -50.0f));
    goal1.push_back(Point3(-10.0f, 0.0f, -50.0f));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(0, 0, 1), goal1, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> goal2; // Bottom goal field (-10, 0, 50) <-> (10, 0, 50);
    goal2.push_back(Point3(-10.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 1.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 1.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 2.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 2.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 3.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 3.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 4.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 4.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 5.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 5.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 6.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 6.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 7.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 7.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 8.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 8.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 9.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 9.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(10.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(9.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(9.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(8.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(8.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(7.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(7.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(6.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(6.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(5.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(5.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(4.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(4.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(3.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(3.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(2.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(2.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(1.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(1.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(0.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(0.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-1.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-1.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-2.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-2.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-3.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-3.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-4.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-4.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-5.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-5.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-6.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-6.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-7.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-7.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-8.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-8.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-9.0f, 0.0f, 50.0f));
    goal2.push_back(Point3(-9.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 10.0f, 50.0f));
    goal2.push_back(Point3(-10.0f, 0.0f, 50.0f));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(1, 0, 0), goal2, QuickShapes::LinesType::LINE_LOOP, 0.1);
}
