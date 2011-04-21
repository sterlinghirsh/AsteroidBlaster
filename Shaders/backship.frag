//Varying variables sent from fade.vert
varying vec4 sideColor;
varying vec4 vecta;

void main()
{
      //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
      float darkness = 0.0;
      float otherd = 0.0;
      vec4 fragColor = vec4(sideColor);
      //fragColor.a = pixelAlpha;
      if(abs(vecta.x) + .02 > vecta.z / 6.5){ 
         darkness = ((abs(vecta.x) + .02 - (vecta.z / 6.5)) * 90.0) / 3.0;
         fragColor.g = darkness;
         fragColor.b = darkness;
      }
      if(abs(vecta.z) > 1.1) {
         otherd = (vecta.z - 1.1) * 3.0;
         fragColor.g = otherd;
         fragColor.b = otherd;
      }
      if (abs(vecta.z) > 1.1 && (abs(vecta.x) + .03 > vecta.z / 6.5)) {
         darkness += otherd;
         fragColor.g = darkness;
         fragColor.b = darkness;
      }
      fragColor.r = 0.0;
      gl_FragColor = fragColor;
}

