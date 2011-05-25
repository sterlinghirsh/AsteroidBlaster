varying vec4 vecta;

void main()
	{	
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		vecta = vec4(gl_Vertex);
	}
