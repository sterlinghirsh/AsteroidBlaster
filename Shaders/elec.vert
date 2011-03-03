//rate of flash passed to frag shader
varying float flashRate;

//variable passed in from ElectricityShot.cpp
uniform float flash;

void main()
	{	
		//Transform the vertex (ModelViewProj matrix)
		//gl_Position = ftransform();
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		flashRate = flash;
	}
