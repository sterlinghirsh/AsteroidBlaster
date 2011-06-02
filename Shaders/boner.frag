void main()
{
   gl_FragData[0] = vec4(0.0, 0.0, 0.0, 1.0);
   gl_FragData[1] = vec4(0.0, 0.0, 0.0, 1.0);
   gl_FragData[2] = gl_Color;
   gl_FragData[3] = gl_Color;
}
