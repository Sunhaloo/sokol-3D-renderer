// own the function implementation found in sokol's header file
#define SOKOL_IMPL
// Linux: using OpenGL's API to communicate with GPU
#define SOKOL_GLCORE
// NOTE: please read line 1154 of "our" `sokol_app.h` header file
#define SOKOL_NO_ENTRY
// include the sokol header file --> windowing and events
#include "dependencies/sokol/sokol_app.h"
// include the sokol header file --> simple GPU API wrapper - pixels, rendering
#include "dependencies/sokol/sokol_gfx.h"
// include the sokol header file --> helper functions for 'sokol_gfx.h' file
#include "dependencies/sokol/sokol_glue.h"
// include 'clgm' higly optimsed math library for 2D and 3D stuff
#include "cglm/cglm.h"
// include our newly created 'model.h' file
#include "model.h"
// include our shader for our triangle
#include "triangle_shader.h"

// state stucture for rendering
static struct {
  // action performed during a render pass
  sg_pass_action pass_action;
  // GPU bindings for drawing --> hold data for buffers, textures and more
  sg_bindings bindings;
  // shader, vertex layout / positioning and render settings
  sg_pipeline pipeline;
  // define our model to be rendered --> triangle transformation for every frame
  model triangle;
} state;

// function related to `sapp_run` and `sapp_desc`
void init(void) {
  // function to handle initialisation for window system and GPU rendering

  // initialise graphics for rendering ==> preparing memory stuff and pipelines
  sg_setup(&(sg_desc){
      // setup the environment ==> see line 5006 in 'sokol_gfx.h' for `struct`
      .environment = sglue_environment()});

  // setup the triangle's model default positioning, rotation and scaling
  state.triangle = model_defaults();

  // array to hold coordinates for triangle
  float vertices[] = {
      // x        y         z         red       green     blue
      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top coordinate
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right coordinate
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left coordinate
  };

  // GPU buffer containing `vertices` / vertex data
  state.bindings.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      // place / initialise that buffer with our actual data
      .data = SG_RANGE(vertices),
  });

  // create the pipeline for applying the shaders
  state.pipeline = sg_make_pipeline(&(sg_pipeline_desc){
      // pass in our shader
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      // make the GPU understand our `vertices` vertex data
      .layout = {.attrs = {
                     // get the actual coordinate position
                     [ATTR_triangle_pos].format = SG_VERTEXFORMAT_FLOAT3,
                     // get the colour
                     [ATTR_triangle_colour].format = SG_VERTEXFORMAT_FLOAT3,
                 }}});

  // update the state
  // INFO: again my formatter is really weird WTF is this?
  state.pass_action =
      (sg_pass_action){              // render pass to have colours
                       .colors[0] = {// clean the screen from whatever we have
                                     .load_action = SG_LOADACTION_CLEAR,
                                     // change the colour
                                     .clear_value = {
                                         // red colour
                                         0.15f,
                                         // green colour
                                         0.15f,
                                         // blue colour
                                         0.15f,
                                         // opacity
                                         1.0f,
                                     }}};
};

void frame(void) {
  // function to display at each render state ==> called once every frame

  // move the triangle model back along the z-axis for the duration of the frame
  state.triangle.position[2] -= sapp_frame_duration();
  // rotate the triangle model along the y-axis for the duration of the frame
  state.triangle.rotation[1] -= sapp_frame_duration();
  // scale the triangle model along the x-axis for the duration of the frame
  state.triangle.scale[0] += 0.01f * sapp_frame_duration();

  // define our 4x4 matrices for 3D "rendering"
  mat4 model_matrix, view_matrix, proj_matrix;

  // place the object in the middle of our screen ==> no transformation,
  // rotation, or any sort of that stuff
  glm_mat4_identity(model_matrix);

  // move the model using the "data" from model's default function
  glm_translate(model_matrix, state.triangle.position);

  // setup the rotation for each of the axes that we have

  // x-axis
  glm_rotate(model_matrix, state.triangle.rotation[0],
             (vec3){1.0f, 0.0f, 0.0f});
  // y-axis
  glm_rotate(model_matrix, state.triangle.rotation[1],
             (vec3){0.0f, 1.0f, 0.0f});
  // z-axis
  glm_rotate(model_matrix, state.triangle.rotation[2],
             (vec3){0.0f, 0.0f, 1.0f});

  // set the model's scale using data from the model's default function
  glm_scale(model_matrix, state.triangle.scale);

  // similarly, we need to do the same thing for our view matrix ==> the camera
  glm_mat4_identity(view_matrix);

  // define the camera position ==> at the origin in the middle of our screen
  // INFO: see OpenGL's coordinate system to learn more
  vec3 eye = {0.0f, 0.0f, 0.0f};

  // define the place where the camera is going to be looking at
  /*
   * INFO: need to look at 'z' due to the right-handed nature of OpenGL
   *
   * Picture showing Right-Handed ( and Left Handed ):
   * https://perry.cz/articles/ProjectionMatrix.xhtml
   */
  vec3 center = {0.0f, 0.0f, -1.0f};

  // where does our y-axis is located
  // INFO: in this case its basically like in the image above ( see link )
  // additionally, Minecraft also places its y-axis like a normal human-being!
  vec3 up = {0.0f, 1.0f, 0.0f};

  // place the "camera" at set location on the screen by updating `viewr_matrix`
  glm_lookat(eye, center, up, view_matrix);

  // field of view for our "eye"
  float fov = glm_rad(100.0f);

  // get the aspect ratio of the "dynamic" / resize-able window
  float window_width = sapp_widthf();
  float window_height = sapp_heightf();

  float aspect_ratio = window_width / window_height;

  // how "near" a vertex can be before its not seen
  float near = 0.1f;

  // how "far" a vertex can be before its not seen
  float far = 100.0f;

  // convert the 3D world and project it on a 2D screen
  glm_perspective(fov, aspect_ratio, near, far, proj_matrix);

  // start the pass to display at each state
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

  // apply the pipeline that we created ==> so as to be able to render shader
  sg_apply_pipeline(state.pipeline);

  // bind the GPU buffer to handle these vertex data
  sg_apply_bindings(&state.bindings);

  // sokol-shdc generated triangle params through 'triangle.glsl' uniform
  triangle_params_t params = {0};

  // combine all the "populated" matrix into one final matrix to pass to shader
  // basically matrix multiplication is going to happen here
  // INFO: multiplies n number of matrices, given array of matrices of length n
  glm_mat4_mulN((mat4 *[]){&proj_matrix, &view_matrix, &model_matrix}, 3,
                params.mvp);

  // apply and use the uniforms so as to pass the data to the GPU
  sg_apply_uniforms(UB_triangle_params, &SG_RANGE(params));

  // actually render the thing on our screen
  sg_draw(

      // base element
      0,
      // number of items

      // NOTE: even though that we have 9 elements in total in our array
      // remember that we have x, y and z ==> 3 actual usable data

      3,
      // number of intances
      1);

  // finish recording commands for this pass
  sg_end_pass();

  // submit / "write" all command to the GPU
  sg_commit();
};

void cleanup(void) {
  // function to cleanup resources at the end of our program

  // shutdown / kill the instance of our sokol graphics
  sg_shutdown();
};

void event(const sapp_event *event) {};

// our main function
int main() {
  // initialise our application ==> windowing, GPU setup...
  // INFO: my formatter is formatting weirdly x_x
  sapp_run(&(sapp_desc){// setup main point
                        .init_cb = init,
                        .frame_cb = frame,
                        .cleanup_cb = cleanup,
                        .event_cb = event});

  return 0;
}
