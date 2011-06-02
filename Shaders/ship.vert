//Varying variables pass between vertex and fragment shaders
//These are the alpha we assign to each pixel and the initial color of the wall
varying vec4 sideColor;
varying vec4 vecta;

varying vec3 normal;
varying vec3 vertex_to_light;
varying vec3 eye_to_vertex;
varying vec4 pos;

uniform vec3 LightPos;
uniform vec3 EyePos;

void main()
{
   //assign the wertex passed in to vertex v
   vec4 v = vec4(gl_Vertex);
   vecta = v;

   pos = gl_ModelViewMatrix*gl_Vertex;
   normal = gl_NormalMatrix*gl_Normal;

   vertex_to_light = vec3(LightPos.x-pos.x, LightPos.y-pos.y, LightPos.z-pos.z);
   eye_to_vertex = vec3(EyePos.x - pos.x, EyePos.x - pos.y, EyePos.z - pos.z);
   
   //necessary to have this in .vert files
   gl_Position = gl_ModelViewProjectionMatrix * v;

   gl_FrontColor = gl_Color;

   /*
   if( abs(v.x) + 1.3 * abs(v.z) < .10) {
     darkness = 1.0;
   } else {
    was close: darkness = (abs(v.x) * abs(v.x)) / (1.8 - v.z);
   }
   darkness = abs(v.x) + abs(v.y) + .5 - v.z;
   pixelAlpha gets a higher value further from the center
   pixelAlpha = (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) - 40.0) / 30.0;
   darkness = .1 - abs(v.x);// + abs(v.y) + (1.3 - v.z);//abs(v.x) + abs(v.y);
   This makes the lowest alpha possible .1
   pixelAlpha *= pixelAlpha;
   pixelAlpha = (pixelAlpha * .85) + .15;
   if (abs(v.z) > 1.29) {
   darkness = abs(v.x) / .6 + abs(v.y) / .6 + (1.6 - abs(v.z) / 3.0);
   }
   else {
      darkness = 0.0;
   }
   */
}

