//The normal of the current vertex.
varying vec3 normal;
varying vec3 posv;
varying vec2 texCoord;

void main()
{
   // Pass color and normal data to the fragment shader.
   gl_FrontColor = gl_Color;
   
   //necessary to have this in .vert files
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  
   normal = vec3(gl_NormalMatrix * gl_Normal);
   posv = (gl_ModelViewMatrix * gl_Vertex).xyz;
   texCoord = vec2(gl_MultiTexCoord0);
}
