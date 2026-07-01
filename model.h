#ifndef MODEL_H
#define MODEL_H

// model transformation for scene objects

// include 'cglm' vectors
#include "cglm/types.h"
#include "cglm/vec3.h"

// our model component / structure
typedef struct {
  // define the model's "attributes"
  vec3 position, rotation, scale;
} model;

// function signature --> returns default initialised model
model model_defaults();

#endif
