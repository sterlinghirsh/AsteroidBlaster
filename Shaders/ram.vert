
varying float fade;
varying vec4 thisColor;
varying float stupid;

uniform float poop;

void main()
	{	
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            fade = 1.2 - gl_Vertex.z * .8;

      thisColor = gl_Color;
	}
