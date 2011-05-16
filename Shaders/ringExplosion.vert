//Varying variables pass between vertex and fragment shaders
//These are the alpha we assign to each pixel and the initial color of the wall
varying float otherRatio;
varying vec4 myColor;
varying vec4 vecta;
uniform float ratio;

void main()
{	
   //assign the wertex passed in to vertex v
   vec4 v = vec4(gl_Vertex);		
   vecta = v;
	//necessary to have this in .vert files
	gl_Position = gl_ModelViewProjectionMatrix * v;
	otherRatio = ratio;
	myColor = gl_Color;
}

