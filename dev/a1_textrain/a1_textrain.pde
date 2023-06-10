/*
 * Samuel Kitzerow, kitze012
 * CSci-4611 Assignment #1 Text Rain
 */

/* Note: if Processing's video library does not support your particular combination of webcam and
   operating system, then the Sketch may hang in the setup() routine when the list of available
   image capture devices is requestd with "Capture.list()".  If this happens, you can skip all of
   the camera initilization code and just run in movie mode by setting the following global 
   variable to true. 
   
   If you having trouble accessing the cameara on a mac laptop due to a security issue.  Some
   students found this post helpful: 
   https://github.com/processing/processing-video/issues/134#issuecomment-664778394
 */
boolean forceMovieMode = false;

// Global vars used to access video frames from either a live camera or a prerecorded movie file
import processing.video.*;
String[] cameraModes;
Capture cameraDevice;
Movie inputMovie;
boolean initialized = false;


// Both modes of input (live camera and movie) will update this same variable with the lastest
// pixel data each frame.  Use this variable to access the new pixel data each frame!
PImage inputImage;
PImage mirrorImage;

// My Global variable

Rain[] Q1; //Creating an array of letters from the first quote segment
Rain[] Q2; //Creating an array of letters from the second quote segment

// Global threshold and debugging values
int threshold = 128;
boolean debugging = false;

//=================================================================================================
// Called automatically by Processing, once when the program starts up
void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB); //Original image
  mirrorImage = createImage(width, height, RGB); //Mirror image
  
  String[] quoteList = extractFile(); //Reads text file that contains the letters
  String quote1 = quoteList[(int) random(0, quoteList.length)];
  String quote2 = quoteList[(int) random(quoteList.length)];
  
  Q1 = letterList(quote1); //Creating an array of letters from the first quote segment
  Q2 = letterList(quote2); //Creating an array of letters from the second quote segment
  
  PFont font = loadFont("Calibri-24.vlw"); //Loading font
  textFont(font, 24);
  
  if (!forceMovieMode) {
    println("Querying avaialble camera modes.");
    cameraModes = Capture.list();
    println("Found " + cameraModes.length + " camera modes.");
    for (int i=0; i<cameraModes.length; i++) {
      println(" " + i + ". " + cameraModes[i]); 
    }
    // if no cameras were detected, then run in offline mode
    if (cameraModes.length == 0) {
      println("Starting movie mode automatically since no cameras were detected.");
      initializeMovieMode(); 
    }
    else {
      println("Press a number key in the Processing window to select the desired camera mode.");
    }
  }
}
//====================================================================================================
// Called automatically by Processing, once per frame
void draw() {
  // start each frame by clearing the screen
  background(0);
    
  if (!initialized) {
    // IF NOT INITIALIZED, DRAW THE INPUT SELECTION MENU
    drawMenuScreen();      
  }
  else {
    // IF WE REACH THIS POINT, WE'RE PAST THE MENU AND THE INPUT MODE HAS BEEN INITIALIZED
    // GET THE NEXT FRAME OF INPUT DATA FROM LIVE CAMERA OR MOVIE  
    if ((cameraDevice != null) && (cameraDevice.available())) {
      // Get image data from cameara and copy it over to the inputImage variable
      cameraDevice.read();
      inputImage.copy(cameraDevice, 0,0,cameraDevice.width,cameraDevice.height, 0,0,inputImage.width,inputImage.height);
    }
    else if ((inputMovie != null) && (inputMovie.available())) {
      // Get image data from the movie file and copy it over to the inputImage variable
      inputMovie.read();
      inputImage.copy(inputMovie, 0,0,inputMovie.width,inputMovie.height, 0,0,inputImage.width,inputImage.height);
    }

    // DRAW THE INPUTIMAGE ACROSS THE ENTIRE SCREEN
    // Note, this is like clearing the screen with an image.  It will cover up anything drawn before this point.
    // So, draw your text rain after this!
    
    mirrorImage.copy(inputImage, 0,0,inputImage.width,inputImage.height, 0,0,mirrorImage.width,mirrorImage.height); //Creating a copy image to be mirrored
    mirror(mirrorImage); //Creates a mirrored image
    graying(mirrorImage); //Converts the image to gray scale
    
    if(debugging == true) debuggingMode(mirrorImage);
    
    set(0, 0, mirrorImage);    

    //------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------
    // DRAW THE TEXT RAIN, ETC.
    // TODO: Much of your implementation code should go here.  At this point, the latest pixel data from the
    // live camera or movie file will have been copied over to the inputImage variable.  So, if you access
    // the pixel data from the inputImage variable, your code should always work, no matter which mode you run in.
    
    //----------------------------------
    //First quote segment
    fill(255,0,0);
    for(int i = 0; i < Q1.length; i++){
      text(Q1[i].l, Q1[i].x, Q1[i].y);
      Q1[i].rain();
    }
    //--------------------------------------
    //Second quote segment
    fill(255,255,0);
    for(int i = 0; i < Q2.length; i++){
      text(Q2[i].l, Q2[i].x, Q2[i].y);
      Q2[i].rain();
    }

  }
}
//========================================================================================================================
// Called automatically by Processing once per frame
void keyPressed() {
  if (!initialized) {
    // CHECK FOR A NUMBER KEY PRESS ON THE MENU SCREEN    
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        initializeMovieMode();
      }
      else if ((input >= 1) && (input <= 9)) {
        initializeLiveCameraMode(input);
      }
    }
  }
  else {
    // CHECK FOR KEYPRESSES DURING NORMAL OPERATION
    // TODO: Fill in your code to handle keypresses here..
    if (key == CODED) {
      if (keyCode == UP) {
        // up arrow key pressed
        threshold ++;
        println("New Threshold: " + threshold);
      }
      else if (keyCode == DOWN) {
        // down arrow key pressed
        -- threshold;
        println("New Threshold: " + threshold);
      }
    }
    else if (key == ' ') {
      // spacebar pressed
      println("Entering Debugging Mode");
      if(debugging == false) debugging = true; //Enter debugging mode
      else debugging = false; //Exit debugging mode
    } 
  }
}
//============================================================================================
// Loads a movie from a file to simulate camera input.
void initializeMovieMode() {
  String movieFile = "TextRainInput.mov";
  println("Simulating camera input using movie file: " + movieFile);
  inputMovie = new Movie(this, movieFile);
  inputMovie.loop();
  initialized = true;
}
//=============================================================================================
// Starts up a webcam to use for input.
void initializeLiveCameraMode(int cameraMode) {
  println("Activating camera mode #" + cameraMode + ": " + cameraModes[cameraMode-1]);
  cameraDevice = new Capture(this, cameraModes[cameraMode-1]);
  cameraDevice.start();
  initialized = true;
}
//=====================================================================================================
// Draws a quick text-based menu to the screen
void drawMenuScreen() {
  int y=10;
  text("Press a number key to select an input mode", 20, y);
  y += 40;
  text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
  y += 40; 
  for (int i = 0; i < min(9,cameraModes.length); i++) {
    text(i+1 + ": " + cameraModes[i], 20, y);
    y += 40;
  }  
}
