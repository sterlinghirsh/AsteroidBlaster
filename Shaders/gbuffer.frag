//Varying variable sent from gbuffer.vert
varying vec3 normal;
varying vec3 posv;
varying vec2 texCoord;

uniform float far;
uniform float spec;
uniform sampler2D texIn;
uniform sampler2D depth;

void main()
{
   //vec2 screen = (vec2(gl_FragCoord.x, gl_FragCoord.y) - viewport.xy) * viewport.zw;
   //vec2 screen = (vec2(gl_FragCoord.x, gl_FragCoord.y) - vec2(0, 0)) * vec2(600, 800);
   //float depth = texture2D(depth, screen).x;


   vec4 color = texture2D(texIn, texCoord);
   // Albedo and specular.
   gl_FragData[0] = gl_Color;
   /*
   if (color.a == 0.0) {
      //gl_FragData[0] = vec4(normal, 1.0);
   }
   */
   float depth = gl_FragCoord.z;
   depth = pow(depth, 128.0);
   if (normal.z > 0.0) {
      // Depth and normal (if not facing backwards).
      //gl_FragData[1] = vec4(normal, -posv.z / far);
      //normalize(normal);
      gl_FragData[1] = vec4(normal, depth);
   }
   // Color (for glow).
   gl_FragData[2] = gl_Color;
   gl_FragData[3] = vec4(0.0, 0.0, 0.0, 1.0);
}
