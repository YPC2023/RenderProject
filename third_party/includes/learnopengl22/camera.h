#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.01f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float   width;
    float   height;
    float   nearPlane;
    float   farPlane;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
        /*
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -30.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(cameraPos, cameraTarget, cameraUp);
        */
    }

    glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(Zoom), (width / height), nearPlane, farPlane);
    }

    glm::mat4 GetModelMatrix()
    {
        return model;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 89.0f)
            Zoom = 89.0f;
    }

    void setView(glm::mat4 view)
    {
        this->view = view;
    }
    glm::mat4 getView()
    {
        return this->view;
    }

    void setProjection(glm::mat4 projection)
    {
        this->projection = projection;
    }
    glm::mat4 getProjection()
    {
        return this->projection;
    }
    void setModel(glm::mat4 model)
    {
        this->model = model;
    }
    glm::mat4 getModel()
    {
        return this->model;
    }

    void setWidth(float width)
    {
        this->width = width;
    }
    float getWidth()
    {
        return this->width;
    }
    void setHeight(float height)
    {
        this->height = height;
    }
    float getHeight()
    {
        return this->height;
    }

    void setNearPlane(float val)
    {
        nearPlane = val;
    }

    float getNearPlane()
    {
        return nearPlane;
    }

    void setFarPlane(float val)
    {
        farPlane = val;
    }

    float getFarPlane()
    {
        return farPlane;
    }

    

    glm::vec3 getNearPlaneLeftTop()
    {
        float H = 2 * nearPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, H / 2, -nearPlane);
    }

    glm::vec3 getNearPlaneRightTop()
    {
        float H = 2 * nearPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, H / 2, -nearPlane);
    }

    glm::vec3 getNearPlaneRightBottom()
    {
        float H = 2 * nearPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, -H / 2, -nearPlane);
    }

    glm::vec3 getNearPlaneLeftBottom()
    {
        float H = 2 * nearPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, -H / 2, -nearPlane);
    }

    glm::vec3 getFarPlaneLeftTop()
    {
        float H = 2 * farPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, H / 2, -farPlane);
    }

    glm::vec3 getFarPlaneRightTop()
    {
        float H = 2 * farPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, H / 2, -farPlane);
    }

    glm::vec3 getFarPlaneRightBottom()
    {
        float H = 2 * farPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, -H / 2, -farPlane);
    }

    glm::vec3 getFarPlaneLeftBottom()
    {
        float H = 2 * farPlane * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, -H / 2, -farPlane);
    }


    glm::vec3 getPlaneLeftTop(float distance)
    {
        float H = 2 * distance * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, H / 2, -distance);
    }

    glm::vec3 getPlaneRightTop(float distance)
    {
        float H = 2 * distance * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, H / 2, -distance);
    }

    glm::vec3 getPlaneRightBottom(float distance)
    {
        float H = 2 * distance * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(W / 2, -H / 2, -distance);
    }

    glm::vec3 getPlaneLeftBottom(float distance)
    {
        float H = 2 * distance * tan(glm::radians(Zoom) / 2);
        float W = H * (width / height);

        // 计算摄像机坐标
        return glm::vec3(-W / 2, -H / 2, -distance);
    }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));

        width = 800;
        height = 600;
        nearPlane = 0.1f;
        farPlane = 100.0f;
        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
    }
};
#endif
