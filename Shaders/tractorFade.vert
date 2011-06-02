
varying float fade;

void main()
	{	
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            fade = (gl_Vertex.z) / 55.0;
	}
