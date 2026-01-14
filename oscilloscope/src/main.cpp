#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

int main(void){
  const char *title = "Osilloscope";
  int x = 1920/2;
  int y = 1080/2;
  int w = 640;
  int h = 480;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << SDL_GetError();
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

// create window
  SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_OPENGL);
  SDL_GLContext  context =  SDL_GL_CreateContext(window);
// Allow GLEW to use modern OpenGL methods
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW";
        return -1;
    }

// signal
    std::vector<float> points;
    int samples = 1000;
    
    for (int i = 0; i < samples; i++){
      //time 0 to 1
      float t = (float)i / (samples - 1);
      // x axis
      float x = (t*2.0f) - 1.0f;

      // y 
      float y = sin(x*10.0f)*0.8f;

      float z = 0.0f;

      //push into vector
      points.push_back(x);
      points.push_back(y);
      points.push_back(z);
    }
//graphics allocation
  // 2. Create GPU Buffers
    unsigned int VBO, VAO;
    
    // Generate IDs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind (Select) the VAO first
    glBindVertexArray(VAO);

    // Bind the VBO and copy data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    // Tell OpenGL how to read the VBO
    // 0 = Layout location (we will use layout (location = 0) in shader later)
    // 3 = 3 floats per point (x, y, z)
    // GL_FLOAT = Type of data
    // GL_FALSE = Don't normalize
    // 3 * sizeof(float) = Stride (byte distance between points)
    // (void*)0 = Offset (start at beginning)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind (Good practice)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

// ------------------------------------------------------------------------
    // SHADER COMPILATION (Sanity Check)
    // ------------------------------------------------------------------------
    
    // 1. Vertex Shader: "Just put the point where I said"
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // 2. Fragment Shader: "Make everything Green"
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\n\0";

    // Compile Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link them into a Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up (we don't need the individual shaders anymore)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // ------------------------------------------------------------------------
//window
  bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        // A. Input: Check if the user pressed X or ESC
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }

// Clear to Black (Oscilloscope style)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Draw the Wave
        glBindVertexArray(VAO); // Select our "recipe"
        glDrawArrays(GL_LINE_STRIP, 0, samples); // Connect dots with lines
                                                 SDL_GL_SwapWindow(window);
    }
  SDL_GL_DeleteContext(context);
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 0;
}
