//Varying variables pass between vertex and fragment shaders
//These are the alpha we assign to each pixel and the initial color of the wall
varying vec4 sideColor;
varying vec4 vecta;

void main()
{	
      //assign the wertex passed in to vertex v
      vec4 v = vec4(gl_Vertex);		
      vecta = v;
	//necessary to have this in .vert files
	gl_Position = gl_ModelViewProjectionMatrix * v;

      //assign the initial color of the wall to our varying vec4
	sideColor = gl_Color;
   
   //if( abs(v.x) + 1.3 * abs(v.z) < .10) {
    //  darkness = 1.0;
   //} else {
    // was close: darkness = (abs(v.x) * abs(v.x)) / (1.8 - v.z);
   //}
   //darkness = abs(v.x) + abs(v.y) + .5 - v.z;
      //pixelAlpha gets a higher value further from the center
      //pixelAlpha = (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) - 40.0) / 30.0;
   //darkness = .1 - abs(v.x);// + abs(v.y) + (1.3 - v.z);//abs(v.x) + abs(v.y);
      //This makes the lowest alpha possible .1
      //pixelAlpha *= pixelAlpha;
      //pixelAlpha = (pixelAlpha * .85) + .15;
   //if (abs(v.z) > 1.29) {
      //darkness = abs(v.x) / .6 + abs(v.y) / .6 + (1.6 - abs(v.z) / 3.0);
   //}
   //else {
   //   darkness = 0.0;
   //}
}

