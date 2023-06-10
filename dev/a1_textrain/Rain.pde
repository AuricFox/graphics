//Creating a letter object to track the raining text attributes
public class Rain{
  int x;
  int y;
  String l;
  int velX;
  int velY;
  
  public Rain(char l, int x){
    this.x = x; //Set in draw method
    this.y = (int)random(-200,-10); //Randomly position text above screen to appear to fall naturally
    this.l = String.valueOf(l); //Tracks letter variable
    
    //X and Y velocities
    this.velY = (int)random(2,5);
    this.velX = 0; //Could change for pooling off a surface
  }
  //=================================================================================
  public void rain(){ //Increments x and/or y positions that are hard coded
    int ivelY = 0; //Starting y velocity
    boolean advanceY = false; //Determines whether the y position is correct
    
    while(!advanceY){
      if(y < 0){ //Y position is above the screen
        ivelY++;
      }
      else if((y >= 0) && (y < height-1)){ //Y position is inside the screen
        //color c = findThreshold(mirrorImage.pixels[(x + width * y)]); //Stores pixel color for comparison
        
        if(color(0,0,0) == (findThreshold(mirrorImage.pixels[(x + width * y)]))){ //Y position is in the black region
          --y;
        }
        else if((y < height-2) && (color(0,0,0) == (findThreshold(mirrorImage.pixels[(x + width * (1+y))])))){ //Next layer is black, stop advancing
          advanceY = true;
        }
        else{ //These aren't the pixels we're looking for, Move along
          ivelY++;
        }
      }
      else{ //Y position is below the screen
        y = (int)random(-200, -10);
        advanceY = true;
      }
      if (ivelY == velY){ //The desired velocity has been reached
        y += velY;
        advanceY = true;
      }
    }    
  }
}
//====================================================================================================
//Function loads in a text file, removes empty strings, and returns an array of strings to be used for rain
String[] extractFile(){
  String[] myStrings = loadStrings("quote.txt"); //Loads text file into the program
  
  int count = 0;
  for(int i = 0; i < myStrings.length; i++){ //Counts the number of strings in the array
    if(!myStrings[i].equals("")) count++;
  }
  
  if(count == 0){ //Checks for an empty array
    return myStrings;
  }
  
  String[] newStrings = new String[count];
  int index = 0;
  
  for(int i = 0; i < myStrings.length; i++){ //Adding the string to a new array without empty strings
    if(!myStrings[i].equals("")){
      newStrings[index] = myStrings[i];
      //println(myStrings[i]);
      index++;
    }
  }
  
  return newStrings;
}
//====================================================================================================
//Function that takes in a string and returns an array of letter objects
Rain[] letterList(String myWords){
  
  int count = 0;
  for(int i = 0; i < myWords.length(); i++){ //Counts the number of chars in the string
    if(myWords.charAt(i) != ' ') count ++;
  }
  
  Rain[] newList = new Rain[count];
  
  int index = 0;
  for(int i = 0; i < myWords.length(); i++){ //Adds chars to the array with no spaces
    if(myWords.charAt(i) != ' '){
      newList[index] = new Rain(myWords.charAt(i), i*(20));
      index++;
    }
  }
  return newList;
}
//=====================================================================================================
//Function that manipulates the copy of inputImage (mirrorImage) and flips the pixels to give it a mirror affect
// i = x + width * y
// x = i % width
// y = i / width
void mirror(PImage input){
  for(int i = 0; i < input.pixels.length; i++){
    int posX = i % input.width; // x position
    int posX2 = input.width - posX - 1; // x position to swap with
    int posY = i / input.width; // y position
    
    //Swaps pixels at x1 and x2 positions
    if(posX < input.width/2){ //Prevents the pixels from swapping back
      int temp = input.pixels[posY * input.width + posX2];
      input.pixels[posY * input.width + posX2] = input.pixels[i];
      input.pixels[i] = temp;
    }    
  }
  input.updatePixels(); //Udates the pixels to implement the change
}
//=======================================================================================================
//Function calculates the average of the RGB values and converts the pixel to gray scale
//Gray scale, RGB values are the same
void graying(PImage input){
  for(int i = 0; i < input.pixels.length; i++){
    int r = (input.pixels[i] >> 16) & 0xFF; //Red value
    int g = (input.pixels[i] >> 8) & 0xFF; //Green value
    int b = (input.pixels[i] >> 0) & 0xFF; //Blue value
    
    int gray = (r+g+b)/3; //Gray value, average of RGB values
    input.pixels[i] = color(gray, gray, gray); //Converting to gray scale
  }
  input.updatePixels();
}
//======================================================================================================
//Function determines if the input color is above or below a given threshold and returns black or white
color findThreshold(color pixel){
  int c = (pixel >> 16) & 0xff;
  
  if (c < threshold){ //Pixel color is less than threshold, return black
    return color(0,0,0); 
  }
  else{ //Pixel color is greater than or equal to threshold, return white
    return color(255,255,255);
  }
}
//=======================================================================================================
//When active, foreground pixels will be converted to black and background pixels will be converted to white colors
void debuggingMode(PImage input){
  for(int i = 0; i < input.pixels.length; i++){
    input.pixels[i] = findThreshold(input.pixels[i]); //Calling method to convert color
  }
  input.updatePixels();
}
