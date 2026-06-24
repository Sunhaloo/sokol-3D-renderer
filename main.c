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

// state stucture for rendering
static struct {
  // action performed during a render pass
  sg_pass_action pass_action;
  // GPU bindings for drawing --> hold data for buffers, textures and more
  sg_bindings bindings;
} state;

// function related to `sapp_run` and `sapp_desc`
void init(void) {
  // function to handle initialisation for window system and GPU rendering

  // initialise graphics for rendering ==> preparing memory stuff and pipelines
  sg_setup(&(sg_desc){
      // setup the environment ==> see line 5006 in 'sokol_gfx.h' for `struct`
      .environment = sglue_environment()});

  // array to hold coordinates for triangle
  float vertices[] = {
      // x        y         z
      0.0f,  0.5f,  0.0f, // top coordinate
      0.5f,  -0.5f, 0.0f, // bottom right coordinate
      -0.5f, -0.5f, 0.0f  // bottom left coordinate
  };

  // GPU buffer containing `vertices` / vertex data
  state.bindings.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      // place / initialise that buffer with our actual data
      .data = SG_RANGE(vertices),
  });

  // update the state
  // INFO: again my formatter is really weird WTF is this?
  state.pass_action =
      (sg_pass_action){              // render pass to have colours
                       .colors[0] = {// clean the screen from whatever we have
                                     .load_action = SG_LOADACTION_CLEAR,
                                     // change the colour
                                     .clear_value = {
                                         // red colour
                                         0.35f,
                                         // green colour
                                         0.35f,
                                         // blue colour
                                         0.35f,
                                         // opacity
                                         1.0f,
                                     }}};
};

void frame(void) {
  // function to display at each render state ==> called once every frame

  // start the pass to display at each state
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

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
