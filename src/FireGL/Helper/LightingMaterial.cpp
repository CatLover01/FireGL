#include <FireGL/Helper/LightingMaterial.h>
#include <FireGL/Renderer/Shader.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/BaseCamera.h>

#include <External/glm/vec3.hpp>
#include <External/glm/mat4x4.hpp>

namespace fgl
{

    LightingMaterial::LightingMaterial(Shader* Shader)
        : Material(Shader)
    {
    }

    void LightingMaterial::ApplyUniforms()
    {
        glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };

        const Shader* LightingShader = GetShader();
        SceneObject* SceneObject = GetSceneObject();
        std::shared_ptr<BaseCamera> Camera = SceneObject->GetScene()->GetActiveCamera();

        LightingShader->SetVec3("viewPos", Camera->GetCameraTransform().GetPosition());
        LightingShader->SetFloat("material.shininess", 32.0f);

        // directional light
        LightingShader->SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        LightingShader->SetVec3("dirLight.ambient", glm::vec3(0.01f, 0.01f, 0.01f));
        LightingShader->SetVec3("dirLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
        LightingShader->SetVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // point light 1
        LightingShader->SetVec3("pointLights[0].position", pointLightPositions[0]);
        LightingShader->SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        LightingShader->SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        LightingShader->SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetFloat("pointLights[0].constant", 1.0f);
        LightingShader->SetFloat("pointLights[0].linear", 0.09f);
        LightingShader->SetFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        LightingShader->SetVec3("pointLights[1].position", pointLightPositions[1]);
        LightingShader->SetVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        LightingShader->SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        LightingShader->SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetFloat("pointLights[1].constant", 1.0f);
        LightingShader->SetFloat("pointLights[1].linear", 0.09f);
        LightingShader->SetFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        LightingShader->SetVec3("pointLights[2].position", pointLightPositions[2]);
        LightingShader->SetVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        LightingShader->SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        LightingShader->SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetFloat("pointLights[2].constant", 1.0f);
        LightingShader->SetFloat("pointLights[2].linear", 0.09f);
        LightingShader->SetFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        LightingShader->SetVec3("pointLights[3].position", pointLightPositions[3]);
        LightingShader->SetVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        LightingShader->SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        LightingShader->SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetFloat("pointLights[3].constant", 1.0f);
        LightingShader->SetFloat("pointLights[3].linear", 0.09f);
        LightingShader->SetFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        LightingShader->SetVec3("spotLight.position", Camera->GetCameraTransform().GetPosition());
        LightingShader->SetVec3("spotLight.direction", Camera->GetFrontVector());
        LightingShader->SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        LightingShader->SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        LightingShader->SetFloat("spotLight.constant", 1.0f);
        LightingShader->SetFloat("spotLight.linear", 0.09f);
        LightingShader->SetFloat("spotLight.quadratic", 0.032f);
        LightingShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        LightingShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    }

} // namespace fgl