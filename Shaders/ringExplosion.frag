//Varying variables sent from fade.vert
varying float otherRatio;
varying vec4 myColor;
varying vec4 vecta;
uniform float ratio;

void main()
{
   vec4 thisColor;
   float thisAlpha;
   float thisRatio;
   thisRatio = otherRatio * 2.0;
   thisColor = myColor;
   thisAlpha = (sqrt(vecta.x * vecta.x + vecta.y * vecta.y + vecta.z * vecta.z) - .1) / .5;
   //thisAlpha = 0.0;
   thisColor.a = thisColor.a * thisAlpha;
   //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
   vec4 fragColor = vec4(thisColor);//abs(sin(vecta.z * 3.0)) / 5.0

   gl_FragColor = fragColor;
}

