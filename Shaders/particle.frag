varying vec2 texture_coordinate;
uniform sampler2D texture;
varying vec4 color;

void main() {
   gl_FragColor = texture2D(texture, texture_coordinate) * color;
}
