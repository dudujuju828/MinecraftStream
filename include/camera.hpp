
#pragma once

// REVIEW: include path uses "Chunk.hpp" with capital C but the file is "chunk.hpp".
// This works on Windows (case-insensitive filesystem) but will break on Linux/macOS.
#include "math/vector_math.hpp"
#include "../include/Chunk.hpp"
#include <GLFW/glfw3.h>

class Camera {
   private:
   vecmath::Matrix44 perspective_matrix;
   vecmath::Matrix44 view_matrix;
   vecmath::Vector3 camera_position;
   // REVIEW: 'position' is never used anywhere — only 'camera_position' is.
   // Remove this to avoid confusion.
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
   bool firstMouse;
   bool is_fps;
   bool in_jump;
   float jump_speed;
   float default_jump_speed;
   float gravity;
   float default_height;
    
   // REVIEW(BUG): see camera.cpp — this function takes 6 parameters but completely
   // ignores them, using the member variables instead. Either use the params or remove them.
   void set_perspective(float n_p, float f_p, float l_x, float r_x, float b_y, float t_y);
   //void set_position(vecmath::Vector3 position);
   //void set_view(vecmath::Vector3 posi);

   public: 
   Camera(vecmath::Vector3);
   vecmath::Matrix44& get_perspective();
   vecmath::Vector3 get_position();
   vecmath::Matrix44& get_view();
   // REVIEW: emitRay only takes a single chunk — in main.cpp it's hardcoded to world[0].
   // This should iterate over all chunks or accept the full world vector to support
   // raycasting across the entire world.
   void emitRay(Chunk& chunk);
   void update(GLFWwindow * window);
   void poll_input(GLFWwindow * window);
 };