//Varying variable sent from defer.vert
varying vec2 texCoord;
varying vec3 posv;

uniform float far;
uniform float lightRad;
uniform vec3 lightPos;
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D bloom;
uniform sampler2D nolight;
uniform sampler2D hud;

void main()
{
   vec4 diffuse = texture2D(albedo, texCoord);
   vec4 bloomColor = texture2D(bloom, texCoord);
   vec4 nolightColor = texture2D(nolight, texCoord);
   vec4 hudColor = texture2D(hud, texCoord);
   vec4 normalDepthData = texture2D(normal, texCoord);
   vec4 normalColor = vec4(normalDepthData.rgb, 1.0);
   float depth = normalDepthData.a;
   vec4 color = diffuse + bloomColor + nolightColor;
   if (length(hudColor.rgb) > 0.0) {
      color = hudColor;
   }

   
   vec4 ex_persp_position = gl_FragCoord / gl_FragCoord.w;
   //vec2 texCoord = ex_persp_position.xy * 0.5 + vec2(0.5);
   //vec3 viewRay = vec3(posv.xy * (-far / depth), -far);
   //normalize(viewRay);
   vec3 viewRay = vec3(posv.xy * (-far / depth), 0.0);
   //normalize(viewRay);



   float lradius = 5.0;
   vec3 vscoord = viewRay * depth;
   float dlight = length( lightPos - vscoord );
   float factor = 1.0 - dlight/lradius;
   //if(dlight > lradius) discard;
   //gl_FragColor = vec4( color.rgb, factor );
   //gl_FragColor = vec4(dlight/lradius, dlight/lradius, dlight/lradius, 1.0);
   //gl_FragColor = vec4(vscoord, 1.0);
   //gl_FragColor = vec4(color.rgb, factor);
   //gl_FragColor = vec4(depth, depth, depth, 1.0);
   //if (depth < 1.0 && depth >= 0.0) {
      gl_FragColor = vec4(color.rgb, 1.0);
   //}


   //gl_FragColor = vec4(color.rgb, 1.0);
   //gl_FragColor = vec4(viewRay, 1.0);
   //gl_FragColor = vec4(posv, 1.0);
   //gl_FragColor = vec4(far, far, far, 1.0);
   //gl_FragColor = vec4(lightPos, 1.0);
   /*
   if (viewRay.r <= 0.9) {
      gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
   }
   if (viewRay.r >= 1.0) {
      gl_FragColor = vec4(0.2, 0.9, 0.2, 1.0);
   }
   if (viewRay.r <= 0.0) {
      gl_FragColor = vec4(0.2, 0.2, 0.9, 1.0);
   }
   */
}
