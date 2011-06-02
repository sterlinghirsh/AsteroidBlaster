//Varying variables sent from fade.vert
varying vec4 sideColor;
varying vec4 vecta;

varying vec3 normal;

void main()
{
   gl_FragData[0] = gl_Color;
   gl_FragData[1] = gl_Color;
}

