varying vec4 thisColor;
varying vec4 vecta;

uniform float poop;
uniform float r1;
uniform float g1;
uniform float b1;
uniform float r2;
uniform float g2;
uniform float b2;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))));
}

void main()
{	
   float flashiness = (poop + 100.0) / 200.0;
   float oneVariable = 8.0;
   float flashColor = abs(cos(flashiness)) + .5;
   float otherColor = abs(sin(flashiness)) - .5;
   float fader = (sqrt(vecta.x * vecta.x + vecta.y * vecta.y + (vecta.z - .4) * (vecta.z - .4)) - .6);
   fader = fader * oneVariable;
   fader = floor(fader) / oneVariable;
   /*if (vecta.z == 1.5) {
      flashColor = abs(cos(flashiness)) + .5;
      otherColor = abs(sin(flashiness)) - .5;
   } else if (vecta.z == 7.5 * vecta.y || vecta.z == -7.5 * vecta.y) {
      flashColor = abs(cos(flashiness)) + .5;
      otherColor = abs(sin(flashiness)) - .5;
   } else if (vecta.z == 3.0 * vecta.x || vecta.z == -3.0 * vecta.x) {
      flashColor = abs(cos(flashiness)) + .5;
      otherColor = abs(sin(flashiness)) - .5;
   }*/

   //float fader = (vecta.x * vecta.x + .3) + vecta.y * vecta.y + (vecta.z -.75) * (vecta.z - .75);
   //flashColor = abs(flashColor) + .5;
   //otherColor = abs(otherColor) - .5;
   
   vec4 color = vec4(flashiness * r1 + (1.0 - flashiness) * r2, flashiness * g1 + (1.0 - flashiness) * g2, flashiness * b1 + (1.0 - flashiness) * b2, fader);

	gl_FragColor = color;	
}
