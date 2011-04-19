varying float pixelAlpha;
varying vec4 wallColor;

void main()
{	
      //assign the wertex passed in to vertex v
      vec4 v = vec4(gl_Vertex);		
      
      //the length from wall to wall
	float wallMax = 1.0;
	
	//necessary to have this in .vert files
	gl_Position = gl_ModelViewProjectionMatrix * v;

      //assign the initial color of the wall to our varying vec4
	wallColor = gl_Color;

      //pixelAlpha gets a higher value further from the center
      pixelAlpha = (1.0 - sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
      
      //This makes the lowest alpha possible .1
      //pixelAlpha *= pixelAlpha;
      //pixelAlpha = (pixelAlpha * .85) + .15;
}
