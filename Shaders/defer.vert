varying vec2 texCoord;
varying vec3 posv;

void main()
{
   //Transform the vertex (ModelViewProj matrix)
   //normalv = (gl_NormalMatrix * gl_Normal).xyz;
   //posv = (gl_ModelViewProjectionMatrix * gl_Vertex).xyz;
   posv = (gl_ModelViewMatrix * gl_Vertex).xyz;
   gl_Position = ftransform();
   texCoord = vec2(gl_MultiTexCoord0);
}
