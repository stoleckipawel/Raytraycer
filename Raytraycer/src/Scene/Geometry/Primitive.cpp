#include "Primitive.h"

void Primitive::UpdateTransforms()
{
    UpdateWorldTransform();
    UpdateInvWorldTransform();

    /*
    WorldRotationMtx = glm::mat4(1.0f);
    WorldRotationMtx = glm::rotate(WorldRotationMtx, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    WorldRotationMtx = glm::rotate(WorldRotationMtx, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    WorldRotationMtx = glm::rotate(WorldRotationMtx, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    */

}

void Primitive::UpdateWorldTransform()
{
    Transform world_transform;
    world_transform.position = Position;
    world_transform.scale = Scale;
    world_transform.rotation = Rotation;
    WorldMtx = world_transform.GetMatrix();
}

void Primitive::UpdateInvWorldTransform()
{
    //To do check world transform dirty
    InvWorldMtx = glm::inverse(WorldMtx);
}
