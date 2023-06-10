# CSci-4611 Assignment 5: Artrender
Samuel Kitzerow, kitze012

I first began working on the gouraud shader demonstrated in class. Once I had that working I moved on to the Phong shader. This proved a little difficult
since the image constantly came out as a transparent rainbow shape. This was do to some spelling errors or incorrect syntax. The image evenually rendered
as a gouraud and not a phong shading after all the errors were solved. This was corrected by normalizing the provided normal vector. After completing the 
phong shader I moved onto the artsy shader. I was able to produce most of the color bands but the brightest one appeared as a phong specular. I realized 
that I needed to used the other input texture (specular_ramp) for specular lighting. The outline was also a little tricky since there would be a gap 
between the line of the silhouette edge and the object. This was fixed by performing the displacement of the vertex before converting it to a 4D vector.
