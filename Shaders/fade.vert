varying float flo;
varying vec4 clr;

void main()
	{	
	
	      vec4 v = vec4(gl_Vertex);		
		//v.z = 0.0;
		float cro = 40.0;
		gl_Position = gl_ModelViewProjectionMatrix * v;

		//Transform the vertex (ModelViewProj matrix)
		//gl_Position = ftransform();
		clr = gl_Color;
		// Clean up inaccuracies
            //vec2 Pos;
            //Pos = sign(gl_Vertex.xy);
            flo = abs(v.x)/40.0 * abs(v.y)/40.0 * abs(v.z)/40.0;
            flo = (flo * .9) + .1;
            flo *= flo;
            //flo = 0;
            //if(v.z == cro){
                  //clr = vec4(1.0, 0.0, 0.0, flo);
            //}
            /*else if(v.z == -cro && v.x != -cro && v.y != -cro && v.x != cro && v.y != cro){
                  clr = vec4(0.0, 1.0, 0.0, flo);
            }
            else if(v.x == -cro && v.z != cro && v.y != cro && v.z != cro && v.y != cro){
                  clr = vec4(0.0, 0.0, 1.0, flo);
            }
            else if(v.x == cro && v.z != cro && v.y != cro && v.z != cro && v.y != cro){
                  clr = vec4(1.0, 1.0, 0.0, flo);
            }
            else if(v.y == -cro && v.x != cro && v.z != cro && v.x != cro && v.z != cro){
                  clr = vec4(0.0, 1.0, 1.0, flo);
            }
            else if(v.y == cro && v.x != cro && v.z != cro && v.x != cro && v.z != cro){
                  clr = vec4(1.0, 0.0, 1.0, flo);
            }*/
            //flo = 1;
            //gl_Position = vec4(Pos, 0.0, 1.0);
            // Image-space
            //vTexCoord = Pos * 0.5 + 0.5;
		//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	}
	
/*varying vec2 vTexCoord;
varying float flo;

// remember that you should draw a screen aligned quad
void main(void)
{
  gl_Position = ftransform();;

  // Clean up inaccuracies
  vec2 Pos;
  Pos = sign(gl_Vertex.xy);
  flo = abs(Pos.x) + abs(Pos.y);
  flo = (flo - 40.0) / 80.0;
  gl_Position = vec4(Pos, 0.0, 1);
  // Image-space
  vTexCoord = Pos * .5 + .5;

  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}*/
