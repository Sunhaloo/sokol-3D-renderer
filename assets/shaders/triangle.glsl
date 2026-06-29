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
// vertex shader take IN a single set of red, green and blue colour at A time
in vec3 colour;

// need to pass that colour to `fragment_shader` ==> `vertex_shader` now has an output
out vec3 out_vertex_shader_colour;

// main entry point / main function for vertex shader
void main() {
  // no transformation for coordinates ==> simply "place" them
  // NOTE: `gl_Position` is of type `vec4` ==> need to convert `vec3` to `vec4`
  gl_Position = vec4(pos, 1.0f);

  // return colour from the vertex_shader
  out_vertex_shader_colour = colour;
}

// end of vertex shader
@end

// beginning of fragment shader
@fs fragment_shader

// get the colour from our `vertex_shader`
in vec3 out_vertex_shader_colour;

// outputs 'RGBA' value
out vec4 frag_colour;

// main entry point / main function for vertex shader
void main() {
  // INFO: hardcode colour from `vertices` array from 'main.c'
  frag_colour = vec4(out_vertex_shader_colour, 1.0);
}

// end of fragment shader
@end

// complete shader program with vertex and fragment shader
@program triangle vertex_shader fragment_shader
