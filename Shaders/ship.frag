//Varying variables sent from fade.vert
varying vec4 sideColor;
varying vec4 vecta;

varying vec3 normal;
varying vec3 vertex_to_light;
varying vec3 eye_to_vertex;
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
   
   float darkness = 0.0;
   float otherd = 0.0;
   vec4 fragColor = vec4(sideColor);
   //fragColor.a = pixelAlpha;
   if(abs(vecta.z) > 1.0) {
      otherd = (vecta.z - 1.0) * 3.0;
      if((abs(vecta.x) + .03 > vecta.z / 6.5)){
         darkness = ((abs(vecta.x) + .03 - (vecta.z / 6.5)) * 90.0) / 3.0;
         darkness += otherd;
         fragColor.r *= darkness;
         fragColor.g *= darkness;
         fragColor.b *= darkness;
      } else {
         fragColor.r *= otherd;
         fragColor.g *= otherd;
         fragColor.b *= otherd;
      }
   } else if(abs(vecta.x) + .03 > vecta.z / 6.5) {
      darkness = ((abs(vecta.x) + .03 - (vecta.z / 6.5)) * 90.0) / 3.0;
      fragColor.r *= darkness;
      fragColor.g *= darkness;
      fragColor.b *= darkness;
   }

   
   if(!(abs(vecta.z) > 1.0) && !(abs(vecta.x) + .03 > vecta.z / 6.5)) {
      fragColor.r = 0.0;
      fragColor.g = 0.0;
      fragColor.b = 0.0;
   }
   //fragColor.r = 0.0;
   /*now add a specular contribution */
   vec3 reflectVec = normalize(reflect(-lightVec, normalVec));
   vec3 viewVec = normalize(eye_to_vertex);
   //float specComponent = max(dot(reflectVec, viewVec.xyz), 0.0);
   float specComponent = max(dot(reflectVec, viewVec), 0.0);
   //float shiny = 16.0;
   float shiny = 5.0;
   specComponent = pow(specComponent, shiny);
   
   const vec4 specMaterial = vec4(0.6, 0.6, 0.8, 1.0);
  
   fragColor += specComponent*specMaterial*1.0;
   clamp(fragColor, 0.0, 1.0);
   
   gl_FragColor = fragColor;
   //gl_FragColor = vec4(normalVec, 1.0);
   /*
   vec4 normVec = vec4(1.0, 0.0, 0.0, 1.0);
   */
   /*
   if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0) {
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
   } else {
      gl_FragColor = vec4(lightVec.rgb, 1.0);
   }
   */
}

