
#pragma once

#include "math/vector_math.hpp"
#include <GLFW/glfw3.h>

class Camera {
   private:
   vecmath::Matrix44 perspective_matrix;
   vecmath::Matrix44 view_matrix;
   vecmath::Vector3 camera_position;
   vecmath::Vector3 position;
   vecmath::Vector3 front;
   vecmath::Vector3 up;
   vecmath::Vector3 world_up;
   vecmath::Vector3 right;

   float projection_near;
   float projection_far;
   float projection_left;
   float projection_right;
   float projection_top;
   float projection_bottom;
   double lastX;
   double lastY;
   float pitch;
   float yaw;
   float mouse_sensitivity;
   float fly_speed;
   float lastTime;
   float deltaTime;
    
   void set_perspective(float n_p, float f_p, float l_x, float r_x, float b_y, float t_y);
   //void set_position(vecmath::Vector3 position);
   //void set_view(vecmath::Vector3 posi);

   public: 
   Camera(vecmath::Vector3);
   vecmath::Matrix44& get_perspective();
   vecmath::Vector3& get_position();
   vecmath::Matrix44& get_view();
   void update(GLFWwindow * window);
   void poll_input(GLFWwindow * window);
 };