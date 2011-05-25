varying vec4 vecta;
varying vec4 thisColor;
varying float stupid;

uniform float poop;

void main()
	{	
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
      vecta = vec4(gl_Vertex);
            thisColor = gl_Color;
	}
