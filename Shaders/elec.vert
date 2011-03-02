////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// simple vertex shader
varying float poop;
uniform float spin;

void main()
	{	
		//Transform the vertex (ModelViewProj matrix)
		//gl_Position = ftransform();
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		poop = spin;
	}
