//Varying variables sent from fade.vert
varying vec4 sideColor;
varying vec4 vecta;

varying vec3 normal;
varying vec3 vertex_to_light;
varying vec4 pos;

void main()
{
   //normalize the interpolated normal and light
   vec3 normalVec = normalize(normal);

   //has no camera transform on light fine for this example
   vec3 lightVec = normalize(vertex_to_light);

   //calculate the diffuse contribution
   float diffuse = clamp(dot(lightVec, normalVec), 0.0, 1.0);
   //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor

   //Create a modifiable vec4 to change the alpha and assign it to gl_FragColor
   float darkness = 0.0;
   float otherd = 0.0;
   vec4 fragColor = vec4(sideColor);
   //fragColor.a = pixelAlpha;
   if(abs(vecta.y) + .03 > vecta.z / 6.5){
      darkness = ((abs(vecta.y) + .03 - (vecta.z / 6.5)) * 90.0) / 3.0;
      fragColor.g = darkness;
      fragColor.b = darkness;
   }
   if(abs(vecta.z) > 1.0) {
      otherd = (vecta.z - 1.0) * 3.0;
      fragColor.g = otherd;
      fragColor.b = otherd;
   }
   if (abs(vecta.z) > 1.0 && (abs(vecta.y) + 0.03 > vecta.z / 6.5)) {
      darkness += otherd;
      fragColor.g = darkness;
      fragColor.b = darkness;
   }
   fragColor.r = 0.0;
   clamp(fragColor, 0.0, 1.0);
   
   gl_FragData[0] = fragColor;
   gl_FragData[1] = vec4(normalVec, 1);
   gl_FragData[2] = fragColor;
   gl_FragData[3] = vec4(0.0, 0.0, 0.0, 1.0);
}

