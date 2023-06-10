# CSci-4611 Assignment 3:  Earthquake
Samuel Kitzerow, kitze012

I first started to build the mesh by using two for loops and incremented by radians (I did not know how to use the LatLongToPlane() function
at the time so I kept this format). Once the triangle mesh was finished I moved to texturing. This proved a little difficult at first but I
was eventually able to map the image to the texture. I looped through two triangles at a time to make the texturing more manageable. I believe
this eventually prevented me from forming an accurate globe. The noramls were correct but the indices were off when transitioning to a sphere.
A blue ring (from the oceans) would be rendered but in debugging mode all the veritices would line up perfectly. I adjusted the indices which 
caused the earth to be rendered but it was rotate 90 degrees and the plane wouldn't display. The earthquakes also appear to be displaying at
the proper locations. I used linear intperolation for color and size changes for quakes between magnitude 6 and 8. I ultimately decided to 
just stick with the plane and turn in what I had so I could move on to my other projects.

I did not attempt any wizard work (hopefully next time).
