#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform()
	: scale(glm::vec3(1.0f, 1.0f, 1.0f)), 
	position(glm::vec3(0.0f, 0.0f, 0.0f)), 
	rotation(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

glm::mat4 Transform::GetMatrix()
{
    glm::mat4 model_mtx = glm::mat4(1.0f);

    // Apply translation
    model_mtx = glm::translate(model_mtx, position);

    // Apply rotation
    model_mtx = glm::rotate(model_mtx, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_mtx = glm::rotate(model_mtx, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_mtx = glm::rotate(model_mtx, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Apply scaling
    model_mtx = glm::scale(model_mtx, scale);

    return model_mtx;
}

glm::mat4 Transform::GetInverseMatrix()
{
    // Get the inverse of the transformation matrix
    glm::mat4 inverse_model_mtx = glm::inverse(GetMatrix());

    return inverse_model_mtx;
}
