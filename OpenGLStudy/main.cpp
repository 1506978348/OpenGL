//你好 窗口
#include "main.hpp"
#include "Shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    
    
    Shader ourShader("Shader/shader.vs","Shader/shader.fs");
    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f,1.0f  // top left
    };
    //OpenGL右手坐标系
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    unsigned int VBO,VAO,EBO,texture1,texture2;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);
    glad_glGenBuffers(1,&EBO);
    glad_glGenTextures(1,&texture1);
    
    //     bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glad_glBindVertexArray(VAO);
    //复制顶点数组到缓冲中
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    //复制索引数据到索引缓冲中
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //在绑定纹理前激活纹理单元
    glad_glActiveTexture(GL_TEXTURE0);
    //绑定纹理
    glad_glBindTexture(GL_TEXTURE_2D,texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);
    
    //生成纹理
    int width,height,nrChannels;
    unsigned char *data=stbi_load("Texture/wall.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture1"<<std::endl;
    }
    stbi_image_free(data);
    
    glad_glGenTextures(1,&texture2);
    glad_glActiveTexture(GL_TEXTURE1);
    glad_glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data=stbi_load("Texture/face1.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture2"<<std::endl;
    }
    //释放图像内存
    stbi_image_free(data);
    
    
    //设置顶点属性指针
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);
    
    //颜色属性
    glad_glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3* sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    
    //纹理属性
    glad_glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6* sizeof(float)));
    glad_glEnableVertexAttribArray(2);
    
    //      note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glad_glBindBuffer(GL_ARRAY_BUFFER,0);
    //     You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //     VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glad_glBindVertexArray(0);
    
    // uncomment this call to draw in wireframe polygons.
    //渲染模式 GL_LINE 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ourShader.use();
    ourShader.setInt("texture1",0);
    ourShader.setInt("texture2",1);
//        //创建向量
//       //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//       //glm0.9.9及以上版本 初始化矩阵 mat4 4*4单位矩阵
//       glm::mat4 trans = glm::mat4(1.0f);
//       //glm::translate返回一个平移矩阵
//       trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
//    //   trans= glm::rotate(trans,glm::radians(90.0f) ,glm::vec3(0.0f,0.0f,1.0f));
//       trans=glm::scale(trans,glm::vec3(0.5,0.5,0.5));
    
    //获取shader uniform变量 transform 的地址
     unsigned int transformLoc=glad_glGetUniformLocation(ourShader.ID,"transform");
    
    // render loop
    // 函数在我们每次循环的开始前检查一次GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans=glm::scale(trans,glm::vec3(0.5,0.5,1));
        trans= glm::rotate(trans,(float)glfwGetTime() ,glm::vec3(0.0f,0.0f,1.0f));
        glad_glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        ourShader.use();
        
        glad_glBindVertexArray(VAO);//seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //        glad_glDrawArrays(GL_TRIANGLES,0,3);
        glad_glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glm::mat4 trans1 = glm::mat4(1.0f);
        trans1 = glm::translate(trans1, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scalnum=glm::sin((float)glfwGetTime());
        scalnum=abs(scalnum);
        trans1=glm::scale(trans1,glm::vec3(scalnum,scalnum,1));
        glad_glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans1));
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
