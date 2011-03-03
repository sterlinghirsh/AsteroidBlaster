//rate of flash passed from vert shader
varying float flashRate;

//variable passed in from ElectricityShot.cpp
uniform float flash;

void main()
	{	
	      float flashColor = sin(flashRate);

	      flashColor = abs(flashColor);

	      vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
	      color = vec4(flashColor, flashColor, 1.0, 1.0);
		gl_FragColor = color;	
	}
