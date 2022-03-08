#include "Mesh.h"

void FramebufferSizeCallback(GLFWwindow* _pWindow, int _iWidth, int _iHeight);

stVertex stVertices[] =
{
    //Coordinates                               /**/    Normals                         /**/    Colours                         /**/    Texture Cordinate
    stVertex{glm::vec3(-1.0f, 0.0f,  1.0f),     /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(0.0f, 0.0f)},
    stVertex{glm::vec3(-1.0f, 0.0f, -1.0f),     /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(0.0f, 1.0f)},
    stVertex{glm::vec3(1.0f,  0.0f, -1.0f),     /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(1.0f, 1.0f)},
    stVertex{glm::vec3(1.0f,  0.0f,  1.0f),     /**/    glm::vec3(0.0f, 1.0f, 0.0f),    /**/    glm::vec3(1.0f, 1.0f, 1.0f),    /**/    glm::vec2(1.0f, 0.0f)}
};

GLuint GLuIndices[]
{
    0, 1, 2,
    0, 2, 3
};

stVertex stLightVertices[] =
{
    //Coordinates
    stVertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    stVertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    stVertex{glm::vec3(0.1f,  -0.1f, -0.1f)},
    stVertex{glm::vec3(0.1f,  -0.1f,  0.1f)},
    stVertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    stVertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    stVertex{glm::vec3(0.1f,   0.1f, -0.1f)},
    stVertex{glm::vec3(0.1f,   0.1f,  0.1f)}
};

GLuint GLuLightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};


const unsigned int uViewPortW = 800, uViewPortH = 800;

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

    //Setup Window Viewport
    glViewport(0, 0, uViewPortW, uViewPortH);
    glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

    //Set up Textures
    CTexture Textures[]
    {
        CTexture("Planks.png", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        CTexture("PlanksSpecular.png", "Specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    
    //Set up Shaders
    //Set up Light Shader
    CShader ShaderLight("Light.vert", "Light.frag");
    std::vector<stVertex> vLightVerticies(stLightVertices, stLightVertices + sizeof(stLightVertices) / sizeof(stVertex));
    std::vector<GLuint> vLightIndicies(GLuLightIndices, GLuLightIndices + sizeof(GLuLightIndices) / sizeof(GLuint));
    std::vector<CTexture> vLightTextures(Textures, Textures + sizeof(Textures) / sizeof(CTexture));
    CMesh Light(vLightVerticies, vLightIndicies, vLightTextures);
    
    glm::vec4 v4LightColour(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 v3LightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 mat4LightModel = glm::mat4(1.0f);
    mat4LightModel = glm::translate(mat4LightModel, v3LightPos);
    ShaderLight.Activate();
    glUniformMatrix4fv(glGetUniformLocation(ShaderLight.m_GLuID, "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(mat4LightModel));
    glUniform4f(glGetUniformLocation(ShaderLight.m_GLuID, "uni_v4LightColor"), v4LightColour.x, v4LightColour.y, v4LightColour.z, v4LightColour.w);
    
    //Set up Floor Shader
    CShader ShaderFloor("Default.vert", "Default.frag");
    std::vector<stVertex> vVerticies(stVertices, stVertices + sizeof(stVertices) / sizeof(stVertex));
    std::vector<GLuint> vIndicies(GLuIndices, GLuIndices + sizeof(GLuIndices) / sizeof(GLuint));
    std::vector<CTexture> vTextures(Textures, Textures + sizeof(Textures) / sizeof(CTexture));
    CMesh Floor(vVerticies, vIndicies, vTextures);
    
    glm::vec3 v3FloorPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 mat4FloorModel = glm::mat4(1.0f);
    mat4FloorModel = glm::translate(mat4FloorModel, v3FloorPos);
    ShaderFloor.Activate();
    glUniformMatrix4fv(glGetUniformLocation(ShaderFloor.m_GLuID, "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(mat4FloorModel));
    glUniform4f(glGetUniformLocation(ShaderFloor.m_GLuID, "uni_v4LightColor"), v4LightColour.x, v4LightColour.y, v4LightColour.z, v4LightColour.w);
    glUniform3f(glGetUniformLocation(ShaderFloor.m_GLuID, "uni_v3LightPosition"), v3LightPos.x, v3LightPos.y, v3LightPos.z);

    //Set up Textures
    glEnable(GL_DEPTH_TEST);

    //Set up camera
    CCamera Camera(uViewPortW, uViewPortH, glm::vec3(0.0f, 0.0f, 2.0f));

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
        Camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

        Floor.Draw(ShaderFloor, Camera);
        Light.Draw(ShaderLight, Camera);

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