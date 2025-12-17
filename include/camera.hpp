
#pragma once

#include "math/vector_math.hpp"

class Camera {
   private:
   float near;
   float far;
   float projection_left;
   float projection_right;
   float projection_top;
   float projection_bottom;

   public: 
   Camera(vecmath::Vector3);
   vecmath::Vector3 position;
   vecmath::Vector3 front;
   vecmath::Vector3 up;
   vecmath::Vector3 right;

};