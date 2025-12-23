
#include <GLFW/glfw3.h>
#include "../include/camera.hpp"
#include "../include/math/vector_math.hpp"

#include <spdlog/spdlog.h>
#include <math.h>

Camera::Camera(vecmath::Vector3 position_in) : 
camera_position{position_in}, 
projection_near{1.0f},
projection_far{50.0f},
projection_left{-1.0f},
projection_right{1.0f},
projection_top{1.0f},
projection_bottom{-1.0f},
front{vecmath::Vector3(0.0f,0.0f,1.0f)},
world_up{vecmath::Vector3(0.0f,1.0f,0.0f)},
lastX{0.0},
lastY{0.0},
pitch{0.0f},
yaw{0.0f},
mouse_sensitivity{3.0f}
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

void Camera::set_view(GLFWwindow * window) {
    poll_input(window);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    /* calculate the deltas */
    double deltax = xpos - lastX;
    double deltay = ypos - lastY;
    spdlog::info("ypos: {}  lasty: {}, deltay: {}", ypos, lastY, deltay);
    spdlog::info("xpos: {}  lastx: {}, deltax: {}", xpos, lastX, deltax);

    /* update the last */
    lastX = xpos;
    lastY = ypos;

    pitch += deltay * mouse_sensitivity;
    yaw   += deltax * mouse_sensitivity;

    auto convertDegreesToRadians = [](float degree){
        return degree * (3.14159265358979323846/180.0f);
    };

    front.x = cosf(convertDegreesToRadians(yaw))*sinf(convertDegreesToRadians(pitch));
    front.y = sinf(convertDegreesToRadians(pitch));
    front.z = sinf(convertDegreesToRadians(yaw))*sinf(convertDegreesToRadians(pitch));
    front.normalize();

    right = front.cross(world_up);
    right.normalize();

    up = front.cross(right);
    up.normalize();

    view_matrix.set_column(vecmath::Vector4(right.x, right.y, right.z, 0.0f), 0);
    view_matrix.set_column(vecmath::Vector4(up.x, up.y, up.z, 0.0f), 1);
    view_matrix.set_column(vecmath::Vector4(front.x,    front.y,    front.z, 0.0f), 2);
    view_matrix.set_column(vecmath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 3);
    vecmath::Matrix44 translation_matrix;
    translation_matrix.set_column(vecmath::Vector4(-camera_position, 1.0f), 3);
    view_matrix *= translation_matrix;

}

vecmath::Matrix44& Camera::get_view() {
    return view_matrix;
}

vecmath::Vector3 &Camera::get_position() {
    return camera_position;
}

void Camera::poll_input(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_position += front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_position -= front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_position -= right;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_position += right;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera_position -= up;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera_position += up;
    }
    //set_view(camera_position);
}

vecmath::Matrix44& Camera::get_perspective() {
    return perspective_matrix;
}