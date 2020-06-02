#ifndef CAMERA_H
#define CAMERA_H

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()  {
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 GetPerspectiveMatrix() {
        return glm::perspective(Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
    }
    

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

//class Camera {
//public:
//    // Camera Attributes
//    Eigen::Vector3f Position;
//    Eigen::Vector3f Up;
//    Eigen::Vector3f Center;
//    Eigen::Matrix4f perspective;
//    // Constructor with vectors
//    Camera(Eigen::Vector3f position, Eigen::Vector3f up) {
//        Center = Eigen::Vector3f(0, 0, 0);
//        Position = position;
//        Up = up;
//    }
//
//
//    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
//    Eigen::Matrix4f GetViewMatrix() {
//
//        return glm::lookAt(Position, Position + Front, Up);
//        Eigen::Matrix4f mViewMatrix = Eigen::Matrix4f::Constant(0.0f);
//        Eigen::Matrix3f R;
//        R.col(2) = (Position - Center).normalized();
//        R.col(0) = Up.cross(R.col(2)).normalized();
//        R.col(1) = R.col(2).cross(R.col(0));
//        mViewMatrix.topLeftCorner<3, 3>() = R.transpose();
//        mViewMatrix.topRightCorner<3, 1>() = -R.transpose() * Position;
//        mViewMatrix(3, 3) = 1.0f;
//        return mViewMatrix;
//    }
//
//
//    Eigen::Matrix4f GetPersMatrix() {
//        float zFar = 150.0f;
//        float zNear = 0.1f;
//        float fovY = 10;
//        float aspect = SCR_WIDTH / SCR_HEIGHT;
//
//        Eigen::Matrix4f mProjectionMatrix = Eigen::Matrix4f::Constant(0.0f);
//        float theta = fovY * 0.5;
//        float range = zFar - zNear;
//        float invtan = 1. / tan(theta);
//
//        mProjectionMatrix(0, 0) = invtan / aspect;
//        mProjectionMatrix(1, 1) = invtan;
//        mProjectionMatrix(2, 2) = -(zNear + zFar) / range;
//        mProjectionMatrix(3, 2) = -1;
//        mProjectionMatrix(2, 3) = -2 * zNear * zFar / range;
//        mProjectionMatrix(3, 3) = 0;
//        return mProjectionMatrix;
//    }
// 
//};


#endif