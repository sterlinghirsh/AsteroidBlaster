varying vec4 vecta;

uniform float zBlue;

void main()
	{	
	   float thisZ = zBlue;
	   vec4 color = vec4(0.0, 0.0, 0.0, 0.3);
	   float edgeColor = 0.0;
	   float otherEdge = 0.0;
      if(vecta.y > -.015 && vecta.y < .015) {//abs(vecta.x) > .9 && abs(vecta.y) > .9 && abs(vecta.z) > .9) {
         color = vec4(0.0, 0.0, 1.0, 1.0);
      }
      if(vecta.x > -.015 && vecta.x < .015) {//abs(vecta.x) > .9 && abs(vecta.y) > .9 && abs(vecta.z) > .9) {
         color = vec4(0.0, 1.0, .0, 1);
      }
      if(vecta.z > -.015 && vecta.z < .015) {//abs(vecta.x) > .9 && abs(vecta.y) > .9 && abs(vecta.z) > .9) {
         color = vec4(1.0, 1.0, 1.0, 1.0);
      }
      if(abs(vecta.y) + .015 > zBlue && abs(vecta.y) - .015 < zBlue) {
         color = vec4(0.0, 0.0, 1.0, 1.0);
      }

	   //vec4 color = vec4(0.0, 1.0, 0.0, 1);

		gl_FragColor = color;	
	}
