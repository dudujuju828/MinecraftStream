
#include "../include/chunk.hpp"
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

void Camera::emitRay(Chunk& chunk) {

    // Amanatides & Woo DDA
    // X, Y- current grid indices
    // stepX, stepY - either -1 or 1 (along each axis)
    // tMaxX, tMaxY - the value of t, at which the ray crosses a ____ grid-line
    // tDeltaX, tDeltaY - how much tMaxX, tMaxY increase each time you step to the next voxel

    // Rules
    // tMaxX < tMaxY - you cross a vertical boundary first
    // tMaxX > tMaxY - you cross a horizontal boundary first
    // Warning: Voxel size is implicitly assumed to be 1
    // Warning: hard-fixed distance to 5 in all directions
    
    // Starting Voxel coordinates (based on the camera position)
    int X = camera_position.x / 1;
    int Y = (camera_position.y + 1) / 1;
    int Z = camera_position.z / 1;

    int rayDistance = 5;
    int X_STARTING = camera_position.x / 1;
    int Y_STARTING = camera_position.y / 1;
    int Z_STARTING = camera_position.z / 1;
    // Depending on the ray direction, set stepX and stepY so they allow jumps to integer voxel coordinates
    int stepX = (front.x < 0) ? -1 : 1;
    int stepY = (front.y < 0) ? -1 : 1;
    int stepZ = (front.z < 0) ? -1 : 1;

    // Depending on the step, calculate the next boundaries we expect the ray to intersect
    int nextBoundaryX = (front.x > 0) ? (X+1) : (X);
    int nextBoundaryY = (front.y > 0) ? (Y+1) : (Y);
    int nextBoundaryZ = (front.z > 0) ? (Z+1) : (Z);

    // The amount of t needed, to go from the current position to the next boundary
    const double INF = std::numeric_limits<double>::infinity();
    double tMaxX = (front.x != 0.0f) ? (nextBoundaryX - camera_position.x) / front.x : INF;
    double tMaxY = (front.y != 0.0f) ? (nextBoundaryY - camera_position.y) / front.y : INF;
    double tMaxZ = (front.z != 0.0f) ? (nextBoundaryZ - camera_position.z) / front.z : INF;

    // The amount of t needed to step one whole voxel (width/height/depth) in either direction
    double tDeltaX = (front.x != 0.0f) ? (1.0f / std::abs(front.x)) : INF;
    double tDeltaY = (front.y != 0.0f) ? (1.0f / std::abs(front.y)) : INF;
    double tDeltaZ = (front.z != 0.0f) ? (1.0f / std::abs(front.z)) : INF;

    // Change this condition
    while (true) {
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                tMaxX = tMaxX + tDeltaX;
                X = X + stepX;
            } else {
                tMaxZ = tMaxZ + tDeltaZ;
                Z = Z + stepZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                tMaxY = tMaxY + tDeltaY;
                Y = Y + stepY;
            } else {
                tMaxZ = tMaxZ + tDeltaZ;
                Z = Z + stepZ;
            }
        }
        // If we get here, and we have not hit a block (as we know we have not returned)
        // If we have traversed too much in either direction, return
        if (std::abs(X - X_STARTING) > rayDistance || std::abs(Y - Y_STARTING) > rayDistance ||std::abs(Z - Z_STARTING) > rayDistance) return;


        // We will have stepped +1/-1 in a single axis direction.
        // Obtain the current block
        BLOCK_TYPE current_block = chunk.getBlock(X,Y,Z);

        // Check if the current block is breakable
        // (Assuming all blocks are breakable)

        // Check if the current block is air
        // (In which case we continue the loop, nothing to do (UNLESS we are PLACING a block, then terminate))
        // (Assuming we are deleting from the chunk)

        if (bool destroying{true}; destroying && (current_block == BLOCK_TYPE::AIR)) continue;
        else {
            // Update the current block to air
            // This might be from (stone... or any other solid block)
            chunk.setBlock(BLOCK_TYPE::AIR, X,Y,Z);
            return;
        }

    } 

}

vecmath::Matrix44& Camera::get_perspective() {
    return perspective_matrix;
}