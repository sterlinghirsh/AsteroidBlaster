//Varying variables sent from fade.vert
varying float pixelAlpha;
varying vec4 screenPosition;
varying vec4 vecta;

void main()
{
      //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
      vec4 fragColor = vec4((abs(vecta.y) + abs(vecta.x)) / 5.0, abs(vecta.z), 1.0, abs(vecta.z) / 1.5);//(sqrt(vecta.y * vecta.y + vecta.x * vecta.x) - .1) / 3.0);
      //vec4 fragColor = vec4(1.0, noise1(vecta.x) + 1.0 / 2.0, noise1(vecta.y) / 2.0, (sqrt(vecta.y * vecta.y + vecta.x * vecta.x) - .1) / 3.0);
      //fragColor.a = pixelAlpha;
      gl_FragColor = fragColor;
}

