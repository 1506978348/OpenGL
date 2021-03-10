//你好 窗口
#include "main.hpp"
#include "Shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

////顶点着色器代码
//const char *vertexShaderSource="#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"gl_Position =vec4(aPos,1.0);\n"
//"ourColor=aColor;\n"
//"}\n";
////片元着色器代码
//const char *fragmentShaderSource="#version 330 core\n"
//"out vec4 FragColor;"
//"uniform vec4 unifColor;"
//"in vec3 ourColor;\n"
//"void main()\n"
//"{FragColor =vec4(ourColor,1.0f);}";


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //创建完窗口通知GLFW窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //注册函数告诉GLFW每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glad: load all OpenGL function pointers
    // 在调用任何OpenGL的函数之前我们需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //    //    开始渲染流程
    //    //     build and compile our shader program
    //    //     vertex shader
    //    //    创建顶点着色器 把着色器源码附加的着色器上 然后编译
    //    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    //    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //    glCompileShader(vertexShader);
    //    // check for shader compile errors
    //    int success;
    //    char infoLog[512];
    //    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    //    if(!success)
    //    {
    //        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
    //        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    //    }
    //    //    创建片元着色器 把着色器源码附加的着色器上 然后编译
    //    unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    //    glad_glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    //    glad_glCompileShader(fragmentShader);
    //    //       check for shader compile errors
    //    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    //    if(!success)
    //    {
    //        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
    //        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    //    }
    //
    //    //link shaders  生成着色器程序 组合连接着色器
    //    unsigned int shaderPorgram=glad_glCreateProgram();
    //    glad_glAttachShader(shaderPorgram,vertexShader);
    //    glad_glAttachShader(shaderPorgram,fragmentShader);
    //    glad_glLinkProgram(shaderPorgram);
    //    //check for linking errors
    //    glad_glGetProgramiv(shaderPorgram,GL_LINK_STATUS,&success);
    //    if(!success)
    //    {
    //        glad_glGetProgramInfoLog(shaderPorgram,512,NULL,infoLog);
    //         std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infoLog<<std::endl;
    //    }
    //    //生成着色器程序后 删除顶点和片元着色器
    //    glad_glDeleteShader(vertexShader);
    //    glad_glDeleteShader(fragmentShader);
    
    Shader ourShader("Shader/shader.vs","Shader/shader.fs");
    
    float vertices[]={
        //位置          //颜色
        0.5f,0.5f,0.0f,   1.0f,0.0f,0.0f,  //右上角
        0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,//右下角
        -0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,//左下角
        -0.5f,0.5f,0.0f,  1.0f,1.0f,0.0f//左上角
    };
    
    //opengl 右手坐标系
    unsigned int indices[]={
        0,1,3,
        1,2,3
    };
    
    unsigned int VBO,VAO,EBO;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);
    glad_glGenBuffers(1,&EBO);
    
    //     bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glad_glBindVertexArray(VAO);
    //复制顶点数组到缓冲中
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    //复制索引数据到索引缓冲中
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //设置顶点属性指针
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    //颜色属性
    glad_glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3* sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    //      note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glad_glBindBuffer(GL_ARRAY_BUFFER,0);
    //     You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //     VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glad_glBindVertexArray(0);
    
    // uncomment this call to draw in wireframe polygons.
    //渲染模式 GL_LINE 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    
    // render loop
    // 函数在我们每次循环的开始前检查一次GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //      激活着色器
        // glad_glUseProgram(shaderPorgram);
        
        ourShader.use();
        
        //        //更新uniform  outColor颜色
        //        float timeValue=glfwGetTime();
        //        float greenValue=sin(timeValue)/2.0f+0.5f;
        //        int vertexColorLocation=glad_glGetUniformLocation(shaderPorgram,"unifColor");
        //        glad_glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        
        glad_glBindVertexArray(VAO);//seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //        glad_glDrawArrays(GL_TRIANGLES,0,3);
        glad_glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // 函数会交换颜色缓冲区
        glfwSwapBuffers(window);
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）
        glfwPollEvents();
    }
    
    glad_glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);
    glad_glDeleteBuffers(1,&EBO);
    //  glad_glDeleteProgram(shaderPorgram);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // 渲染循环结束后需要正确释放、删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
