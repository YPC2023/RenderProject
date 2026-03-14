#include "pch.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& target,
    int width, int height, float fov, float zNear, float zFar)
{
    m_Position = pos;
    m_Up = up;
    m_Target = target;

    m_Fov = fov;
    m_Near = zNear;
    m_Far = zFar;
}

bool Camera::Initialize()
{
    CalculateView();
    CalculateProjection();
	return true;
}

void Camera::UnInitialize()
{

}

void Camera::Resize(int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;
    CalculateProjection();
}

void Camera::SetPosition(const glm::vec3 pos)
{
    m_Position = pos;
}

void Camera::SetUp(const glm::vec3 up)
{
    m_Up = up;
}

void Camera::SetTarget(const glm::vec3 target)
{
    m_Target = target;
}

const glm::mat4& Camera::GetView()
{
    return m_View; 
}

const glm::mat4& Camera::GetProjection()
{ 
    return m_Projection; 
}

void Camera::CalculateProjection()
{
    m_Projection = glm::perspective(glm::radians(m_Fov), ((float)m_nWidth / m_nHeight), m_Near, m_Far);
}

void Camera::CalculateView()
{
    m_View = glm::lookAt(m_Position, m_Target, m_Up);
}