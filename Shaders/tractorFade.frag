
varying float fade;

void main()
	{	
            float fader = fade;
	      vec4 color = vec4(0.0, 1.0, 0.0, fader);

		gl_FragColor = color;	
	}
