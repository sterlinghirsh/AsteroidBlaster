// Shader to make the particle sprite always face the camera.
uniform float size;
varying vec2 texture_coordinate;
varying vec4 color;
void main() {
   // Remove the top left 3x3 of the ModelView matrix.
   // Note: this is column-major.
   mat4 noRotateMVMatrix =
    mat4(0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0);

   // Make a new matrix to handle scale.
   mat4 scaleMatrix = mat4(size);
   mat4 newMV = matrixCompMult(gl_ModelViewMatrix, noRotateMVMatrix) + scaleMatrix;
   gl_Position = gl_ProjectionMatrix * newMV * gl_Vertex;

   texture_coordinate = vec2(gl_MultiTexCoord0);
   color = gl_Color;
}
