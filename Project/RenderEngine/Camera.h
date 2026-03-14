#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <CUtils.h>
#include <InitializeInterface.h>
#include "ICameraInterface.h"

class Camera : public InitializeInterface,
    public ICameraInterface
{
	friend class CameraManager;
protected:
	Camera(const glm::vec3& pos = glm::vec3(1.0f), 
        const glm::vec3& up = glm::vec3(1.0f), 
        const glm::vec3& target = glm::vec3(1.0f), 
        int width = 600, int height = 800, 
        float fov = 90.0f, float zNear = 0.1f, float zFar = 100.0f);
public:
    bool Initialize() override;
    void UnInitialize() override;
    void Resize(int width, int height);
public:
    void SetPosition(const glm::vec3 pos);
    void SetUp(const glm::vec3 pos);
    void SetTarget(const glm::vec3 pos);
    const glm::vec3& GetPosition() { return m_Position; }
    const glm::vec3& GetUp() { return m_Up; }
    const glm::vec3& GetTarget() { return m_Target; }
public:
    const glm::mat4& GetView() override;
    const glm::mat4& GetProjection() override;
private:
    void CalculateProjection();
    void CalculateView();
private:
    glm::vec3   m_Position;
    glm::vec3   m_Target;
    glm::vec3   m_Up;

    int     m_nWidth;
    int     m_nHeight;

    float m_Fov;
    float m_Near;
    float m_Far;
private:
    glm::mat4   m_View;
    glm::mat4   m_Projection;
};

