
#pragma once

#include "math/vector3.hpp"

class Camera {
   public: 
   Camera(Vector3);
   Vector3 position;
   Vector3 front;
   Vector3 up;
   Vector3 right;
};