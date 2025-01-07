#include <FireGL/FireGL.h>

#include "CustomInputManager.h"
#include "CustomCamera.h"

// Uncomment if you'd like to use the fgl namespace directly for convenience
// using namespace fgl;

int main()
{
    // Initialize Asset Manager to manage and load assets
    fgl::AssetPathManager AssetManager(PROJECT_ROOT + std::string("Config.ini"));

    // Initialize main window
    fgl::BaseWindow MainWindow;
    MainWindow.Initialize(4, 1, "Test123", fgl::WindowType::Windowed, false, 800, 700);
    MainWindow.SetWindowIcon(AssetManager.GetPath("Troll"));

    // Initialize time manager for frame timing
    fgl::TimeManager MainTimer;
    MainTimer.Initialize();
    
    // Set up camera with perspective projection
    std::shared_ptr<CustomCamera> Camera = std::make_shared<CustomCamera>();
    Camera->SetPerspective(45.f, MainWindow.GetAspectRatio(), 0.1f, 1000);

    // Set up input manager to handle user input
    CustomInputManager Input( Camera );
    Input.Initialize();

    // Load shaders
    fgl::Shader LightingShader( AssetManager.GetPath("BaseLightingVertex"), AssetManager.GetPath("BaseLightingFragment") );
    fgl::Shader PointLightShader( AssetManager.GetPath("LightVertex"), AssetManager.GetPath("LightFragment") );
    fgl::Shader SkyBoxShader( AssetManager.GetPath("SkyboxVertex"), AssetManager.GetPath("SkyboxFragment") );

    // Load textures
    fgl::Texture WoodTexture;
    WoodTexture.LoadTexture(AssetManager.GetPath("Wood"));

    fgl::Texture SkyboxTexture;
    SkyboxTexture.LoadCubeMap({
        AssetManager.GetPath("Skybox1"), AssetManager.GetPath("Skybox2"), AssetManager.GetPath("Skybox3"), 
        AssetManager.GetPath("Skybox4"), AssetManager.GetPath("Skybox5"), AssetManager.GetPath("Skybox6") 
        });

    // Set up materials for lighting, point lights, and skyboxes
    std::shared_ptr<fgl::LightingMaterial> LightingMaterial = std::make_shared<fgl::LightingMaterial>(&LightingShader);
    LightingMaterial->SetTexture("normal", &WoodTexture);

    std::shared_ptr<fgl::Material> PointLightMaterial = std::make_shared<fgl::Material>(&PointLightShader);
    std::shared_ptr<fgl::SkyboxMaterial> SkyboxMaterial = std::make_shared<fgl::SkyboxMaterial>(&SkyBoxShader);
    SkyboxMaterial->SetTexture("skybox", &SkyboxTexture);

    // Create shape objects and set materials
    std::shared_ptr<fgl::Cube> SkyboxCube = std::make_shared<fgl::Cube>();
    SkyboxCube->SetMaterial(SkyboxMaterial);

    // Load imported model and set material
    std::shared_ptr<fgl::Model> BackpackModel = std::make_shared<fgl::Model>( AssetManager.GetPath("BackpackModel") );
    BackpackModel->SetupMaterial<fgl::LightingMaterial>(&LightingShader);

    // Create entities
    std::unique_ptr<fgl::Entity> BackpackEntity = std::make_unique<fgl::Entity>(BackpackModel);
    std::unique_ptr<fgl::SkyboxEntity> SkyboxEntity = std::make_unique<fgl::SkyboxEntity>(SkyboxCube);

    // Initialize the scene with the active camera
    fgl::Scene MainScene(Camera);

    for (int i = 0; i < 15; i++)
    {
        std::unique_ptr<fgl::Cube> PointLightCube = std::make_unique<fgl::Cube>();
        PointLightCube->SetMaterial(PointLightMaterial);
        PointLightCube->GetTransform().SetPosition(i * 2, 0, 0);
        MainScene.AddObject(std::move(PointLightCube));
    }
    MainScene.AddObject(std::move(SkyboxEntity));
    MainScene.AddObject(std::move(BackpackEntity));

    // Renderer
    fgl::Renderer SceneRenderer(fgl::RenderingMode::Default);

    // Main Loop
    while (!MainWindow.ShouldClose())
    {
        MainTimer.Update();
        Input.ProcessInput();

        MainScene.Process();
        SceneRenderer.Render(&MainScene);

        Input.FinalizeInput();
    }
    
    MainWindow.Terminate();
    return 0;
}