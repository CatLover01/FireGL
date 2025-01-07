#include <FireGL/Helper/Skybox/SkyboxMaterial.h>
#include <FireGL/Renderer/Shader.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/BaseCamera.h>

#include <External/glm/glm.hpp> 

namespace fgl
{

    SkyboxMaterial::SkyboxMaterial(Shader* Shader)
        : Material(Shader)
    {
    }

    void SkyboxMaterial::ApplyUniforms()
    {
        const Shader* SkyboxShader = GetShader();
        SceneObject* SceneObject = GetSceneObject();

        // The skybox is treated as infinite, so we manually set the projection and view matrices 
        // without applying the model transformation to ensure the skybox is rendered correctly.        
        SkyboxShader->SetMat4("projection", SceneObject->GetScene()->GetActiveCamera()->GetProjectionMatrix());
        SkyboxShader->SetMat4("view", glm::mat4(glm::mat3(SceneObject->GetScene()->GetActiveCamera()->GetViewMatrix())));
    }

} // namespace fgl