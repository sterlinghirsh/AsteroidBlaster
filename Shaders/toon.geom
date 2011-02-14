#version 120 
#extension GL_EXT_geometry_shader4 : enable

	void main(void)
	{
		//increment variable
		int i;
		vec4 newpt;

		/////////////////////////////////////////////////////////////
		//This example has two parts
		//	step a) draw the primitive pushed down the pipeline
		//		 there are gl_Vertices # of vertices
		//		 put the vertex value into gl_Position
		//		 use EmitVertex => 'create' a new vertex
		// 		use EndPrimitive to signal that you are done creating a primitive!
		//	step b) create a new piece of geometry (I.E. WHY WE ARE USING A GEOMETRY SHADER!)
		//		I just do the same loop, but swizzle the x and y values
		//	result => the line we want to draw, and the same line, but along the other axis

/*  and a note from nvidia:
	varying in vec4 gl_PositionIn[gl_VerticesIn];
	
    The built-in varying variable gl_PositionIn[] holds the per-vertex
    position, as written by the vertex shader to its output variable
    gl_Position. Note that writing to gl_Position from either the vertex or
    fragment shader is optional.
*/

		//Pass-thru!
/*		
		for(i=0; i< gl_VerticesIn; i++){
			gl_Position = gl_PositionIn[i];
			EmitVertex();
		}
		EndPrimitive();	
*/

#define n 0.1;


		for(i=0; i< gl_VerticesIn; i++){


// lets make each vertex into a square
			gl_Position = gl_PositionIn[i]; // xyzw
			gl_Position.x = gl_Position.x-n;
			gl_Position.y = gl_Position.y-n;
			EmitVertex();
			
			gl_Position.x = gl_Position.x+2*n;
			EmitVertex();

			gl_Position.y = gl_Position.y+2*n;
			EmitVertex();			

			gl_Position.x = gl_Position.x-2*n;
			EmitVertex();	

			gl_Position.y = gl_Position.y-2*n;
			EmitVertex();
							
				
			EndPrimitive();	
		}	
		
	}
	
/*
#version 120
#extension GL_EXT_geometry_shader4 : enable
 
void main() {
  for(int i = 0; i < gl_VerticesIn; ++i) {
    gl_FrontColor = gl_FrontColorIn[i];
    gl_Position = gl_PositionIn[i];
    EmitVertex();
  }
}

*/

// geometry shader from http://cirl.missouri.edu/gpu/glsl_lessons/glsl_geometry_shader/index.html
