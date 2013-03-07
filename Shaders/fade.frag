//Varying variables sent from fade.vert
varying float pixelAlpha;
varying vec4 wallColor;

void main()
{
      //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
      vec4 fragColor = vec4(wallColor);
      fragColor.a = pixelAlpha;
      gl_FragColor = fragColor;
}

