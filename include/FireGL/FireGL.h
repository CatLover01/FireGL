// #Helper: Utility and support-related headers
#include <FireGL/Helper/LightingMaterial.h>
#include <FireGL/Helper/Skybox/SkyboxMaterial.h>
#include <FireGL/Helper/Skybox/SkyboxEntity.h>

// #Core: Core system management and essential utilities
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Core/BaseSingletonManager.h>
#include <FireGL/Core/InputManager.h>
#include <FireGL/Core/SystemManager.h>
#include <FireGL/Core/TimeManager.h>
#include <FireGL/Core/Window.h>
#include <FireGL/Core/AssetPathManager.h>

// #Renderer: Rendering-related headers and components
#include <FireGL/Renderer/Entity.h>
#include <FireGL/Renderer/Material.h>
#include <FireGL/Renderer/Mesh.h>
#include <FireGL/Renderer/Model.h>
#include <FireGL/Renderer/Renderer.h>
#include <FireGL/Renderer/Scene.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/Shader.h>
#include <FireGL/Renderer/Texture.h>
#include <FireGL/Renderer/Vertex.h>
#include <FireGL/Renderer/MatrixBuffer.h>
#include <FireGL/Renderer/Component.h>
#include <FireGL/Renderer/Transform.h>
#include <FireGL/Renderer/Shapes/Shape.h>
#include <FireGL/Renderer/Shapes/Cube.h>
#include <FireGL/Renderer/Shapes/Sphere.h>
#include <FireGL/Renderer/BaseCamera.h>