//rate of flash passed from vert shader
varying float flashRate;

//variable passed in from ElectricityShot.cpp
uniform float flash;

void main()
	{	
	      float flashColor = sin(flashRate) * 0.5;
	      float flashColor2 = cos(flashRate) * 0.3;

	      flashColor = abs(flashColor);
	      flashColor2 = abs(flashColor2);

	      vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
	      //color = vec4(flashColor, flashColor, 1.0, 1.0);
	      color = vec4(flashColor + 0.5, flashColor + 0.5, 1.0 - flashColor2, 1.0);
	      //color = vec4(flashColor, flashColor, flashColor2, 1.0);
		gl_FragColor = color;	
	}
