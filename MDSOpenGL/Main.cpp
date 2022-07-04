#include "Classes/GameManager.h"
#include "Classes/LightManager.h"
#include "Classes/CubeSkybox.h"
#include "ExternVariables.h"
#include "GenerateMesh.h"
#include "Classes/UpdatedObject.h"
#include <iostream>

int main()
{
    #pragma region Setup GLFW, GLEW, and Viewport

    //Initialize and Configure GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Set up Window
    GLFWwindow* pWindow = glfwCreateWindow(e_uViewPortW, e_uViewPortH, "LearnOpenGL", NULL, NULL);
    if (pWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(pWindow);

    //Initializing GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Setup Window Viewport
    glViewport(0, 0, e_uViewPortW, e_uViewPortH);
    glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* _pWindow, int _iWidth, int _iHeight) {glViewport(0, 0, _iWidth, _iHeight); });

    #pragma endregion

    //Set up Culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //Set up Anti-Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    //Create Game Manager
    CGameManager GameManager;

    //Set up Shaders
    CShader Diffuse("Resources/Shaders/Diffuse.vert", "Resources/Shaders/Diffuse.frag");

    //Set up Cube Map
    CShader CubeMapShader("Resources/Shaders/CubeMap.vert", "Resources/Shaders/CubeMap.frag");
    const char* CubeMapDirectories[6]
    {
        "Resources/Textures/Cubemaps/MountainOutpost/Right.jpg",
        "Resources/Textures/Cubemaps/MountainOutpost/Left.jpg",
        "Resources/Textures/Cubemaps/MountainOutpost/Up.jpg",
        "Resources/Textures/Cubemaps/MountainOutpost/Down.jpg",
        "Resources/Textures/Cubemaps/MountainOutpost/Back.jpg",
        "Resources/Textures/Cubemaps/MountainOutpost/Front.jpg"
    };

    CCubeSkybox* CubeMap = GameManager.CreateObject<CCubeSkybox>();
    CubeMap->CreateSkybox(&CubeMapShader, 2000.0f, CubeMapDirectories);

    //Set up Camera
    CCamera* Camera = nullptr;
    {
        Camera = GameManager.CreateObject<CCamera>();
        Camera->m_Transform.SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        Camera->SetFarPlane(4000.0f);
        GameManager.m_pCamera = Camera;
    }

    CMesh<>* pCrate = GameManager.CreateObject<CMesh<>>();
    {
        pCrate->m_pTransform = new CTransform;
        pCrate->m_pTransform->SetPosition(glm::vec3(-0.2f, 0.0f, 0.0f));
    
        //Setup Mesh Component
        gm::GenerateFlatCube(*pCrate, glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);
        pCrate->m_pShader = &Diffuse;
        pCrate->m_vTextures.push_back(CTextureManager::Insert("Resources/Textures/Crate.png", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
        pCrate->m_vTextures.push_back(CTextureManager::Insert("Resources/Textures/CrateReflect.png", "Reflect", 1, GL_RED, GL_UNSIGNED_BYTE));
    }

    //Setup textures for the spheres and the plane
    CTexture* pDiffuse = CTextureManager::Insert("Resources/Textures/Planks.png", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    CTexture* pSpecular = CTextureManager::Insert("Resources/Textures/PlanksSpecular.png", "Specular", 1, GL_RED, GL_UNSIGNED_BYTE);

//    //Create Spheres
//    {
//        glm::ivec3 v3iGrouping(3, 3, 3);
//        float fSepparation = 0.4f;
//        glm::vec3 v2Position(-fSepparation, -fSepparation, -fSepparation);
//    
//        for (int x = 0; x < v3iGrouping.x; x++) for (int y = 0; y < v3iGrouping.y; y++) for (int z = 0; z < v3iGrouping.z; z++)
//        {
//            CTransform* pSphere = GameManager.CreateObject<CTransform>();
//            pSphere->SetPosition(glm::vec3(x, y, z) * fSepparation + v2Position);
//    
//            //Setup Mesh Component
//            CMesh<>* pMesh = pSphere->AddComponent<CMesh<>>();
//            gm::GenerateSphere(*pMesh, 0.05f, 20);
//            pMesh->m_pShader = &Diffuse;
//            pMesh->m_vTextures.push_back(pDiffuse);
//            pMesh->m_vTextures.push_back(pSpecular);
//        }
//    }
//
//    CTransform* pPlane = GameManager.CreateObject<CTransform>();
//    {
//        pPlane->SetPosition(glm::vec3(0.0f, -0.6f, 0.0f));
//
//        //Setup Mesh Component
//        {
//            CMesh<>* pMesh = pPlane->AddComponent<CMesh<>>();
//            gm::GeneratePlane(*pMesh, glm::vec3(5.0f, 5.0f, 5.0f));
//            pMesh->m_pShader = &Diffuse;
//            pMesh->m_vTextures.push_back(pDiffuse);
//            pMesh->m_vTextures.push_back(pSpecular);
//        }
//    }

    //Set up Lighting
    {
        CLightManager::m_v4AmbientColour = glm::vec4(155.0f/255.0f, 180.0f/255.0f, 204.0f/255.0f, 1.0f);

        CLightManager::m_vDirectionalLight.push_back(stDirectionalLight{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) });
        CLightManager::m_vPointLight.push_back(stPointLight{ glm::vec3(0.4f, 0.4f, 0.4f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) });
        CLightManager::m_vPointLight.push_back(stPointLight{ glm::vec3(-0.4f, -0.4f, -0.4f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) });
        CLightManager::m_vSpotLight.push_back(stSpotLight{ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.9f, 0.95f});

        CLightManager::UpdateDiffuseShader("Resources/Shaders/Diffuse.frag");
    }

    glClearColor(CLightManager::m_v4AmbientColour.x, CLightManager::m_v4AmbientColour.y, CLightManager::m_v4AmbientColour.z, 1.0f);
    
    //Render Loop
    while (!glfwWindowShouldClose(pWindow))
    {
        UpdateDeltatime();
        
        //Inputs
        {
            UpdateMousePosition(pWindow);
        }

        //Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        CLightManager::UpdateShaderUniforms(&Diffuse);
        //CubeMap.Draw(*Camera);
        GameManager.Update();

        //Check and call events and swap the buffers
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
    return 0;
}