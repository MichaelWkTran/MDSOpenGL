#include "Classes/Mesh.h"

void FramebufferSizeCallback(GLFWwindow* _pWindow, int _iWidth, int _iHeight);

stVertex stVertices[] =
{
    //Coordinates                              /**/    Normals                         /**/    Colours                         /**/    Texture Cordinate
    stVertex{glm::vec3(-1.0f, 0.0f,  1.0f),    /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(0.0f, 0.0f)},
    stVertex{glm::vec3(-1.0f, 0.0f, -1.0f),    /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(0.0f, 1.0f)},
    stVertex{glm::vec3(1.0f,  0.0f, -1.0f),    /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(1.0f, 1.0f)},
    stVertex{glm::vec3(1.0f,  0.0f,  1.0f),    /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(1.0f, 0.0f)}
};

GLuint GLuIndices[]
{
    0, 1, 2,
    0, 2, 3
};

stVertex stLightVertices[] =
{
    //Coordinates
    stVertex{glm::vec3(1.0f,  -1.0f, -1.0f) * 0.1f},
    stVertex{glm::vec3(-1.0f, -1.0f, -1.0f) * 0.1f},
    stVertex{glm::vec3(1.0f,  -1.0f,  1.0f) * 0.1f},
    stVertex{glm::vec3(-1.0f, -1.0f,  1.0f) * 0.1f},
    stVertex{glm::vec3(1.0f,   1.0f, -1.0f) * 0.1f},
    stVertex{glm::vec3(-1.0f,  1.0f, -1.0f) * 0.1f},
    stVertex{glm::vec3(1.0f,   1.0f,  1.0f) * 0.1f},
    stVertex{glm::vec3(-1.0f,  1.0f,  1.0f) * 0.1f} 
};

GLuint GLuLightIndices[] =
{
    0, 2, 1, //Top
    1, 2, 3,
    6, 4, 7, //Bottom
    7, 4, 5,
    2, 7, 3, //Front
    2, 6, 7,
    4, 1, 5, //Back
    4, 0, 1,
    7, 5, 3, //Right Side
    3, 5, 1,
    4, 6, 0, //Left Side
    0, 6, 2
};


unsigned int uViewPortW = 800, uViewPortH = 800;

int main()
{
    //Initialize and Configure GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //Set up Window
    GLFWwindow* pWindow = glfwCreateWindow(uViewPortW, uViewPortH, "LearnOpenGL", NULL, NULL);
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

    glEnable(GL_CULL_FACE);

    //Setup Window Viewport
    glViewport(0, 0, uViewPortW, uViewPortH);
    glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

    //Set up Textures
    CTexture Textures[]
    {
        CTexture("Textures/Planks.png", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        CTexture("Textures/PlanksSpecular.png", "Specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    
    //Set up Light Shader
    CShader ShaderLight("Shaders/Light.vert", "Shaders/Light.frag"); ShaderLight.Activate();
    std::vector<stVertex> vLightVerticies(stLightVertices, stLightVertices + sizeof(stLightVertices) / sizeof(stVertex));
    std::vector<GLuint> vLightIndicies(GLuLightIndices, GLuLightIndices + sizeof(GLuLightIndices) / sizeof(GLuint));
    std::vector<CTexture> vLightTextures(Textures, Textures + sizeof(Textures) / sizeof(CTexture));
    CMesh Light(vLightVerticies, vLightIndicies, vLightTextures, &ShaderLight);
    
    glm::vec4 v4LightColour(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 v3LightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 mat4LightModel = glm::mat4(1.0f);
    mat4LightModel = glm::translate(mat4LightModel, v3LightPos);
    
    glUniformMatrix4fv(glGetUniformLocation(ShaderLight.GetID(), "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(mat4LightModel));
    glUniform4f(glGetUniformLocation(ShaderLight.GetID(), "uni_v4LightColor"), v4LightColour.x, v4LightColour.y, v4LightColour.z, v4LightColour.w);
    
    //Set up Floor Shader
    CShader ShaderFloor("Shaders/Default.vert", "Shaders/Default.frag"); ShaderFloor.Activate();
    std::vector<stVertex> vVerticies(stVertices, stVertices + sizeof(stVertices) / sizeof(stVertex));
    std::vector<GLuint> vIndicies(GLuIndices, GLuIndices + sizeof(GLuIndices) / sizeof(GLuint));
    std::vector<CTexture> vTextures(Textures, Textures + sizeof(Textures) / sizeof(CTexture));
    CMesh Floor(vVerticies, vIndicies, vTextures, &ShaderFloor);
    
    glm::mat4 mat4FloorModel = glm::mat4(1.0f);
    mat4FloorModel = glm::translate(mat4FloorModel, glm::vec3(0.0f, 0.0f, 0.0f));
    mat4FloorModel = glm::rotate(mat4FloorModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
    glUniformMatrix4fv(glGetUniformLocation(ShaderFloor.GetID(), "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(mat4FloorModel));
    glUniform4f(glGetUniformLocation(ShaderFloor.GetID(), "uni_v4LightColor"), v4LightColour.x, v4LightColour.y, v4LightColour.z, v4LightColour.w);
    glUniform3f(glGetUniformLocation(ShaderFloor.GetID(), "uni_v3LightPosition"), v3LightPos.x, v3LightPos.y, v3LightPos.z);

    //Set up Textures
    glEnable(GL_DEPTH_TEST);

    //Set up camera
    CCamera Camera(&uViewPortW, &uViewPortH, true, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    //Render Loop
    while (!glfwWindowShouldClose(pWindow))
    {
        //Input
        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(pWindow, true);
        }

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Set up Camera
        Camera.Inputs(pWindow);
        Camera.Update();

        Floor.Draw(Camera);
        Light.Draw(Camera);

        //Check and call events and swap the buffers
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    ShaderFloor.Delete();
    ShaderLight.Delete();
    glfwDestroyWindow(pWindow);
    glfwTerminate();
    return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}