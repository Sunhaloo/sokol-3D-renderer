/*
   NOTE: some simple notes

   - vertex shader is responsible for placing "data" / coordinates on screen
   - fragment shader is responsible for "colouring" our pixels ( inside the points of data )
   - each shader has its own entry point / `main` function
   - we need to tell the `sokol-shdc` that this is a complete shader program with `@program`
*/

// beginning of vertex shader
@vs vertex_shader

// vertex shader take IN a single set of x, y and z coordinates at A time
in vec3 pos;

// main entry point / main function for vertex shader
void main() {
  // no transformation for coordinates ==> simply "place" them
  // NOTE: `gl_Position` is of type `vec4` ==> need to convert `vec3` to `vec4`
  gl_Position = vec4(pos, 0.0f);
}

// end of vertex shader
@end

// beginning of fragment shader
@fs fragment_shader

// outputs 'RGBA' value
out vec4 frag_colour;

// main entry point / main function for vertex shader
void main() {
  // INFO: hardcode colour for now ( some type of green - I think )
  frag_colour = vec4(0.7f, 1.0f, 0.5f, 1.0);
}

// end of fragment shader
@end

// complete shader program with vertex and fragment shader
@program triangle vertex_shader fragment_shader
