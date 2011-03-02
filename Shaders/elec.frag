////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// simple fragment shader
varying float poop;
uniform float spin;

void main()
	{	
	      float pee = sin(poop);
	      //float some = cos(poop);
	      //float other = tan(poop);
	      pee = abs(pee);
	      //some = abs(some);
	      //other = abs(other);
	      vec4 color = vec4(0, 0, 1, 1);
	      color = vec4(pee, pee, 1, 1.0);
		gl_FragColor = color;	
	}
