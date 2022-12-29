#include <iostream>
// масштабировать, затем вращать и в конце сдвигать


// GLEW
#define GLEW_STATIC
#include <glew.h>
#include"SOIL2/SOIL2/SOIL2.h"
// GLFW
#include <glfw3.h>


#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define SPEED 0.1



class Square;
void Activate_tranform(Square& other);

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
   // if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        
    
}






// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

class Square
{

    float x0 = 0.0f;

    float Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
    float position = -2.0f;
    float speed = -0.01f;
    float Speed =  0.01f;
    GLuint VBO, VAO ,EBO;
   
    GLuint shaderProgram = glCreateProgram();

    GLuint texture1;

    // Shaders
    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec3 position;"
        "layout(location = 1) in vec3 color;"
        "layout(location = 2) in vec2 texCoord;"

        "out vec3 ourColor;"
        "out vec2 TexCoord;"
        "/*uniform mat4 transform;*/uniform mat4 model; uniform mat4 view; uniform mat4 projection; "

        "void main()"
        "{"
        "/*gl_Position = transform * vec4(position, 1.0f);*/"
        "gl_Position = projection * view * model * vec4(position, 1.0f);"
        " ourColor = color;"
        "TexCoord = vec2(texCoord.x ,texCoord.y);"
        " }\0";

    // Fragment shader
    const GLchar* fragmentShaderSource = "#version 330 core\n"
        "in vec3 ourColor;"
        "in vec2 TexCoord;"

        "out vec4 color;"

        "uniform sampler2D ourTexture;"

        "void main()"
        "{"
        " color =texture(ourTexture,TexCoord)* vec4(ourColor, 1.0f);"
        "}\0";


public:

    Square(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy)
    {
        this->Ax = Ax;
        this->Ay = Ay;
        this->Bx = Bx;
        this->By = By;
        this->Cx = Cx;
        this->Cy = Cy;
        this->Dx = Dx;
        this->Dy = Dy;
          
        // Build and compile our shader program

        // Vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Check for compile time errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
      

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Check for compile time errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        // Link shaders
       
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }


        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            this->Ax, this->Ay, 0.0f,      1.0f, 0.0f, 0.0f,    3.0f, 3.0f,//  
            this->Bx, this->By, 0.0f,      0.0f, 1.0f, 0.0f,    3.0f, 0.0f,//  
            this->Cx, this->Cy, 0.0f,      0.0f ,0.0f, 1.0f,    0.0f, 0.0f,//   
            this->Dx, this->Dy, 0.0f,      1.0f, 1.0f, 1.0f,    0.0f, 3.0f
        };

        GLuint indices[] = {  // Помните, что мы начинаем с 0!
        0, 1, 2,   // Первый треугольник
        0, 2, 3    // Второй треугольник
        };

       
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);*/

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

   

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
        
        // CREATE TEXTURES
        
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // texture filtration
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Load, create texture and generate mipmaps
        int width, height;
        unsigned char* image = SOIL_load_image("blocks.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);


        

    }
   

    void rotate()
    {
        GLfloat timeValue = glfwGetTime();
        
        GLfloat change_posY  = (sin(timeValue) * 0.3);
        GLfloat change_posX = (cos(timeValue) * 0.3);
        
        // Build and compile our shader program  

        
        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
             this->Ax * cos(timeValue) - this->Ay * sin(timeValue), this->Ax * sin(timeValue) + this->Ay * cos(timeValue), 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //  
             this->Bx * cos(timeValue) - this->By * sin(timeValue), this->Bx * sin(timeValue) + this->By * cos(timeValue), 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,//  
             this->Cx * cos(timeValue) - this->Cy * sin(timeValue), this->Cx * sin(timeValue) + this->Cy * cos(timeValue), 0.0f,    0.0f ,0.0f, 1.0f,   0.0f, 0.0f,//   
             this->Dx * cos(timeValue) - this->Dy * sin(timeValue), this->Dx * sin(timeValue) + this->Dy * cos(timeValue), 0.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f
        };

        GLuint indices[] = {  // Помните, что мы начинаем с 0!
        0, 1, 2,   // Первый треугольник
        0, 2, 3    // Второй треугольник
        };


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);*/

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        

        
    }

    float keys(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            this->position += (float)glfwGetTime() * SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            this->position -= (float)glfwGetTime() * SPEED;
        }
        return this->position;
    }
   
    void transform(GLFWwindow* window)
    {
        //glm::mat4 transform = glm::mat4(1.0f);
        //transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
        //transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime() * glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));


        //// Get matrix's uniform location and set matrix
        //unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        float pos = keys(window);


        
        // Create transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime()* glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, pos));
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
       //  Get their uniform location
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        //Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }



    void Show_Square()
    {
             
        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

        // Draw our square
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    ~Square()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};



// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    
    
    
    // Set all the required options for GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

   

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    Square a(0.6f, 0.6f, 0.6f, -0.6f, -0.6f, -0.6f, -0.6f, 0.6f);
    
    Square b(0.5f, 0.5f, 0.5f, -0.5f, -0.6f, -0.6f, -0.6f, 0.6f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
       
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
       
        a.transform(window);
        a.Show_Square();
        
        b.transform(window);
        b.Show_Square();
        

        //Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
   
    glfwTerminate();
    return 0;
}

