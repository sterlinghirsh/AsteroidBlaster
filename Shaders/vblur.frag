uniform sampler2D RTScene; // the texture with the scene you want to blur
 
const float blurSize = 1.0/512.0; // I've chosen this size because this will result in that every step will be one pixel wide if the RTScene texture is of size 512x512

const float sigma = 5.0;

const float pi = 3.14159265;

const float numBlurPixelsPerSide = 4.0;
const vec2  blurMultiplyVec      = vec2(0.0, 1.0);

void main() {

   // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
   vec3 incrementalGaussian;
   incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
   incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
   incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

   vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
   float coefficientSum = 0.0;

   // Take the central sample first...
   avgValue += texture2D(RTScene, gl_TexCoord[0].xy)
      * incrementalGaussian.x;
   coefficientSum += incrementalGaussian.x;
   incrementalGaussian.xy *= incrementalGaussian.yz;

   // Go through the remaining 8 vertical samples (4 on each side of the center)
   for (float i = 1.0; i <= numBlurPixelsPerSide; i++) {
      avgValue += texture2D(RTScene, gl_TexCoord[0].xy - i * blurSize *
            blurMultiplyVec) * incrementalGaussian.x;
      avgValue += texture2D(RTScene, gl_TexCoord[0].xy + i * blurSize *
            blurMultiplyVec) * incrementalGaussian.x;
      coefficientSum += 2.0 * incrementalGaussian.x;
      incrementalGaussian.xy *= incrementalGaussian.yz;
   }

   gl_FragColor = avgValue / coefficientSum;
}
