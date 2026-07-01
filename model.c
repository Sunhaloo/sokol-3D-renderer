#include "model.h"

// model component and intialisation helpers

// create some defaults for the model
model model_defaults() {
  // create our model
  model m = {0};

  // update the scaling of the model
  glm_vec3_add(m.scale, (vec3){1, 1, 1}, m.scale);

  // return the default for the model
  return m;
}
