//Varying variables sent from fade.vert
varying float pixelAlpha;
varying vec4 myColor;
varying vec4 vecta;

void main()
{
   vec4 thisColor;
   float thisAlpha;
   thisColor = myColor;
   thisAlpha = (abs(vecta.x) + abs(vecta.z)) * thisColor.a;

   //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
   vec4 fragColor = vec4(abs(cos(vecta.x)),  abs(sin(vecta.x * 3.0)), 0.0, thisAlpha);//abs(sin(vecta.z * 3.0)) / 5.0

   gl_FragColor = fragColor;
}

