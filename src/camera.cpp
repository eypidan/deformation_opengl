#include "pch.h"

double mouseX, mouseY;

glm::vec3 Camera::createRay() {
    glm::mat4 view = this->GetViewMatrix();
    glm::mat4 proj = this->GetPerspectiveMatrix();
    glm::mat4 invVP = glm::inverse(proj * view);
    double trueMouseY = SCR_HEIGHT / 1.0f - mouseY;
    glm::vec4 screenPos = glm::vec4(mouseX, trueMouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}
