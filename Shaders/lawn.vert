////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// simple vertex shader
varying float fade;
uniform float clr;

void main()
	{	
		//Transform the vertex (ModelViewProj matrix)
		//gl_Position = ftransform();
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		fade = clr;//abs(gl_Position.z);
	}
