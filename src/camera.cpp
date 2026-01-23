
#include <GLFW/glfw3.h>
#include "../include/camera.hpp"
#include "../include/math/vector_math.hpp"

#include <spdlog/spdlog.h>
#include <math.h>
#include <cmath>
#include <tuple>
#include <limits>

Camera::Camera(vecmath::Vector3 position_in) : 
camera_position{position_in}, 
default_height{position_in.y},
projection_near{1.0f},
projection_far{80.0f},
projection_left{-1.0f},
projection_right{1.0f},
projection_top{1.0f},
projection_bottom{-1.0f},
front{vecmath::Vector3(0.0f,0.0f,1.0f)},
world_up{vecmath::Vector3(0.0f,1.0f,0.0f)},
lastX{205.1},
lastY{200.1},
pitch{89.5f},
yaw{90.0f},
fly_speed{5.0f},
mouse_sensitivity{1.0f},
lastTime{0.0f},
deltaTime{0.0f},
firstMouse{true},
is_fps{true},
in_jump{false},
jump_speed{-1.21f},
default_jump_speed{-1.31f},
gravity{0.026f}
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

void Camera::update(GLFWwindow * window) {
    poll_input(window);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;


    /* calculate the deltas */
    double deltax = xpos - lastX;
    double deltay = ypos - lastY;

    /* update the last */
    lastX = xpos;
    lastY = ypos;

    if (!firstMouse) {
        pitch -= deltay * mouse_sensitivity;
        yaw   += deltax * mouse_sensitivity;
    }
    if (firstMouse) {
        firstMouse = false;
    }

    if (pitch < 1.0f) {
        pitch = 1.0f;
    }
    if (pitch > 179.0f) {
        pitch = 179.0f;
    }

    auto convertDegreesToRadians = [](float degree){
        return degree * (3.141592653/180.0f);
    };

    front.x = cosf(convertDegreesToRadians(yaw))*sinf(convertDegreesToRadians(pitch));
    front.y = cosf(convertDegreesToRadians(pitch));
    front.z = sinf(convertDegreesToRadians(yaw))*sinf(convertDegreesToRadians(pitch));
    front.normalize();

    right = front.cross(world_up);
    right.normalize();

    up = front.cross(right);
    up.normalize();

    view_matrix.set_column(vecmath::Vector4(right.x, up.x, front.x, 0.0f), 0);
    view_matrix.set_column(vecmath::Vector4(right.y, up.y, front.y, 0.0f), 1);
    view_matrix.set_column(vecmath::Vector4(right.z, up.z, front.z, 0.0f), 2);
    view_matrix.set_column(vecmath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 3);
    vecmath::Matrix44 translation_matrix;
    translation_matrix.set_column(vecmath::Vector4(-camera_position, 1.0f), 3);
    view_matrix *= translation_matrix;
    view_matrix.set_row(vecmath::Vector4(0.0f,0.0f,0.0f,1.0f),3);
}

vecmath::Matrix44& Camera::get_view() {
    return view_matrix;
}

vecmath::Vector3 Camera::get_position() {
    return camera_position;
}

void Camera::poll_input(GLFWwindow * window) {
    if (is_fps) {
         float speed = fly_speed * deltaTime;
         vecmath::Vector3 f_copy = front;
         f_copy.y = 0.0f;
         f_copy.normalize();
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_position += f_copy * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_position -= f_copy * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_position -= right * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_position += right * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (in_jump != true) {
                in_jump = true;
            }
        }
        if (in_jump) {
            jump_speed += gravity;
            camera_position.y += jump_speed * speed;
        }
        if (camera_position.y > default_height) {
                camera_position.y = default_height;
                in_jump = false;
                jump_speed = default_jump_speed;
            }
    } else {
        float speed = fly_speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_position += front * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_position -= front * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_position -= right * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_position += right * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera_position -= up * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera_position += up * speed;
        }
    }
}

vecmath::Vector3 Camera::emitRay() {

    // Obtain the float portion
    vecmath::Vector3 pos_minus_float_portion;
    float stepX, stepY, stepZ;

    if (camera_position.x < 0.0f) {
        pos_minus_float_portion.x = std::ceil(camera_position.x) - camera_position.x;
    } else {
        stepX = 1.0f;
        pos_minus_float_portion.x = camera_position.x - std::trunc(camera_position.x);
    }
    if (camera_position.y < 0.0f) {
        stepY = -1.0f;
        pos_minus_float_portion.y = std::ceil(camera_position.y) - camera_position.y;
    } else {
        stepY = 1.0f;
        pos_minus_float_portion.y = camera_position.y - std::trunc(camera_position.y);
    }
    if (camera_position.z < 0.0f) {
        stepZ = -1.0f;
        pos_minus_float_portion.z = std::ceil(camera_position.z) - camera_position.z;
    } else {
        stepZ = 1.0f;
        pos_minus_float_portion.z = camera_position.z - std::trunc(camera_position.z);
    }

    if (front.x < 0.0f) {
        stepX = -1.0f;
    } else {
        stepX = 1.0f;
    }
    if (front.y < 0.0f) {
        stepY = -1.0f;
    } else {
        stepY = 1.0f;
    }
    if (front.z < 0.0f) {
        stepZ = -1.0f;
    } else {
        stepZ = 1.0f;
    }
    // Copy the camera position
    vecmath::Vector3 ray_position = camera_position; 

    // Copy the camera front vector (off-by-one?)
    vecmath::Vector3 front_copy = front;
    float deltaX = front_copy.x == 0.0f ? std::numeric_limits<float>::infinity() :  1 / front_copy.x;
    float deltaY = front_copy.y == 0.0f ? std::numeric_limits<float>::infinity() :  1 / front_copy.y;
    float deltaZ = front_copy.z == 0.0f ? std::numeric_limits<float>::infinity() :  1 / front_copy.z;


    // pull us to an integer (wait - one at a time depending on deltaX)
    ray_position.x += pos_minus_float_portion.x;
    ray_position.y += pos_minus_float_portion.y;
    ray_position.z += pos_minus_float_portion.z;

    bool hit{false};
    while (!hit) {
        if (deltaX < deltaY) {
            if (deltaX < deltaZ) {

            } else {

            }
        } else {

        }
        if (deltaX < deltaZ) {

        } else {

        }
        if (deltaZ < deltaY) {

        } else {

        }

        ray_position.y += pos_minus_float_portion.y;
        hit = true;
    } 



    return {};
}

vecmath::Matrix44& Camera::get_perspective() {
    return perspective_matrix;
}