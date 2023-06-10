Sam Kitzerow, kitze012
Assignment #1 Text Rain

I first began this project by building a Rain class which stores object variables such as x-y positions, strings, and x-y velocities. I would then simulate the falling text using these
objects by incrementing the y position based on the y velocity. The x position remains constant thru the entire program (I planned on doing the same with the x as with the y position
but I ran out of time and had to shift my focus to other projects). Two lines of text are randomly selected from the file "quote.txt" and evenly spaced across the screen. The x positions
are not randomly generated but the y positions are randomly generated above the screen. This is to give it a more rain like feel but still spell out words as they come to rest on an
imaginary surface. I then focused on creating some support functions after creating the rain class. These functions and their purpose are listed below.

String[] extractFile()
Takes in no arguments but loads the lines of an input file ("quote.txt") into a string array. The array is then looped over to count the number of non-empty strings. All the non-empty 
strings are then copied over to a new string array which is returned.

Rain[] letterList(String myWords)
Takes in a string, counts all characters that aren't spaces, and creates an array of Rain objects. Each letter with equal spacing is loaded into a Rain object.

void mirror(PImage input)
Takes in an input image, calculates the x-y coordinates, and swaps the pixels at the corresponding points to mirror/flip the input image. The pixels are then updated.

void graying(PImage input)
Takes in an input image, calculates the RGB values, averages them, and converts the image to gray scale. The pixels are then updated.

color findThreshold(color pixel)
Note: The input color must be in gray scale to work. 
Takes in an input color and determines if it is above or below the threshold. The red value is taken and if it is below the threshold then the color black is returned otherwise white is
returned.

void debuggingMode(PImage input)
Note: The input image must be in gray scale to work.
Takes in an input image, determines where it lies on the threshold, and converts the pixels to black or white. It does not change the colors of the text. The pixels are then updated.