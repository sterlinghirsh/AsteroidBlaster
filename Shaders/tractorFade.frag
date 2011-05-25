varying vec4 vecta;

void main()
	{	
         float fade = vecta.z / 55.0;
	      vec4 color = vec4(0.0, 1.0, 0.0, fade);

		gl_FragColor = color;	
	}
