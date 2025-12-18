
#include <GLFW/glfw3.h>
#include "../include/camera.hpp"
#include "../include/math/vector_math.hpp"

Camera::Camera(vecmath::Vector3 position_in) : 
position{position_in}, 
projection_near{0.10f},
projection_far{5.0f},
projection_left{-1.0f},
projection_right{1.0f},
projection_top{1.0f},
projection_bottom{-1.0f}
{
    set_perspective(projection_near,projection_far,projection_left,projection_right,projection_bottom,projection_top);
}

void Camera::set_perspective(float n_p, float f_p, float l_x, float r_x, float b_y, float t_y) {
    perspective_matrix.set_column(vecmath::Vector4(2.0f*projection_near/(projection_right-projection_left), 0.0f,0.0f,0.0f),0);
    perspective_matrix.set_column(vecmath::Vector4(0.0f,2.0f*projection_near/(projection_top-projection_bottom),0.0f,0.0f),1);
    perspective_matrix.set_column(vecmath::Vector4((-projection_right-projection_left)/(projection_right - projection_left),
    (-projection_top-projection_bottom)/(projection_top-projection_bottom),
    (projection_far + projection_near)/(-projection_near + projection_far),1.0f),2);
    perspective_matrix.set_column(vecmath::Vector4(0.0f,0.0f,(2*projection_near*projection_far)/(-projection_far + projection_near),0.0f),3);
}

void Camera::set_view(vecmath::Vector3 position) {
    view_matrix.set_column(vecmath::Vector4(position,1.0f),3);
}

vecmath::Matrix44& Camera::get_view() {
    return view_matrix;
}

void Camera::set_position(vecmath::Vector3 position) {
    camera_position.x = position.x;
    camera_position.y = position.y;
    camera_position.z = position.z;
    set_view(camera_position);
}

vecmath::Vector3 &Camera::get_position() {
    return camera_position;
}

void Camera::poll_input(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_position.z -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_position.z += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_position.x += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_position.x -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera_position.y += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera_position.y -= 0.1f;
    }
    set_view(camera_position);
}

vecmath::Matrix44& Camera::get_perspective() {
    return perspective_matrix;
}