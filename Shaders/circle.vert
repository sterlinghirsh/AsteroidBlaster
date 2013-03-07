varying float red, green, blue;

void main()
{	
      //assign the wertex passed in to vertex v
      vec4 v = vec4(gl_Vertex);		
      
      //the length from wall to wall
	//float wallMax = 40.0;
	
	//necessary to have this in .vert files
	gl_Position = gl_ModelViewProjectionMatrix * v;

      //assign the initial color of the wall to our varying vec4
	//wallColor = gl_Color;
      red = (70.0 - v.x) / 70.0;
      green = (70.0 - v.y) / 70.0;
      blue = (70.0 - v.z) / 70.0;
      //pixelAlpha gets a higher value further from the center
      //pixelAlpha = (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) - 40.0) / 30.0;
      
      //This makes the lowest alpha possible .1
      //pixelAlpha = (pixelAlpha * .9) + .25;
      //pixelAlpha *= pixelAlpha;
}
