varying float red, green, blue;

void main()
{
      //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
      vec4 fragColor;
      fragColor.r = red;
      fragColor.g = green;
      fragColor.b = blue;
      fragColor.a = 1;
      gl_FragColor = fragColor;
}
