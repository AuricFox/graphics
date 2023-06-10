# CSCI-4611 Assignment 6: Harold
Samuel Kitzerow, kitze012

I first began working on the drawSky function. It was not too difficult except when storing the strokes. I could not figure out at first way
the strokes would not stay put and just dissapear. I quickly found out that I was not saving the strokes to the array which was a quick fix.
Next I worked on the drawGround function. This proved more tricky since I was not familiar with rendering graphics this way. First my hills would
not render. This was because I set my normal and projection points wrong. Then the hill would render as pointed teeth instead of smooth curves. 
This was a ray casting issue I made when setting up the ray. Finally the hills would render infront of me but if I turned around, then they
would not render at all. This was because I set the origin point on the plane as (0,0,0) instead of the starting or ending point (also inverting the
normal after taking the dot product with the look vector also fixed it).

I did not attempt any wizard work and have not detected any buggy behavior.