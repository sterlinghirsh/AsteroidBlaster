//Varying variables pass between vertex and fragment shaders
//These are the alpha we assign to each pixel and the initial color of the wall
varying float pixelAlpha;
varying vec4 wallColor;
varying vec4 vecta;

void main()
{	
      //assign the wertex passed in to vertex v
      vec4 v = vec4(gl_Vertex);		
      vecta = v;
      //the length from wall to wall
	//float wallMax = 40.0;
	
	//necessary to have this in .vert files
	gl_Position = gl_ModelViewProjectionMatrix * v;

      //assign the initial color of the wall to our varying vec4
	/*wallColor = gl_Color;

      //pixelAlpha gets a higher value further from the center
      pixelAlpha = (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) - 40.0) / 30.0;
      
      //This makes the lowest alpha possible .1
      //pixelAlpha *= pixelAlpha;
      pixelAlpha = (pixelAlpha * .85) + .15;*/
}

