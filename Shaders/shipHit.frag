//Varying variables sent from fade.vert
varying float pixelAlpha;
varying vec4 wallColor;
varying vec4 vecta;

void main()
{
      //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
      vec4 fragColor = vec4(1.0, (abs(vecta.y) + abs(vecta.x)) / 5.0, abs(vecta.z), (abs(vecta.y) + abs(vecta.x)) / 5.0);
      //fragColor.a = pixelAlpha;
      gl_FragColor = fragColor;
}

