#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <chrono>

//you can change the bottom left corner coordinates of the front-facing side of the cube 
#define X_POS -0.6
#define Y_POS -0.6
#define Z_POS 1.5
#define LENGHT_OF_CUBE 0.4

#define WILL_THE_CUBE_ROTATE 1
#define ROTATION_POINT_X -0.4
#define ROTATION_POINT_Y -0.4
#define ROTATION_PERIOD 2.0

#define WRITE_THE_POSITION 1


using namespace std;

void shader_debugger(int, int, int);
int shader_handler();
float* create_cube(float x, float y, float z, float lenght);
float* create_square(string axis, float x, float y, float z, float lenght);
void key_function(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_function(GLFWwindow* window, double xpos, double ypos);

float z_key_offset = 0.0;
float x_key_offset = 0.0;
float y_key_offset = 0.0;
float y_rotation_angle = 0.0;
float x_rotation_angle = 0.0;
int write_the_position = WRITE_THE_POSITION;

int main(){
      
  GLFWwindow* window;

  if(!glfwInit()){
    return -1;
  }

  window = glfwCreateWindow(640,640,"Hello World",NULL,NULL);
  if(!window){
	glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  
  if(glewInit() != GLEW_OK){
    cout << "Error while initializing glew!!!\n";
  }

  float* vertexPositions = create_cube(X_POS, Y_POS, Z_POS, LENGHT_OF_CUBE);

  float colors[]{
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,

    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0,

    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,      
  };


  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 108*sizeof(float), vertexPositions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  unsigned int buffer2;
  glGenBuffers(1, &buffer2);
  glBindBuffer(GL_ARRAY_BUFFER, buffer2);
  glBufferData(GL_ARRAY_BUFFER, 144*sizeof(float), colors, GL_STATIC_DRAW);


  int program = shader_handler();

  int uniform_location = glGetUniformLocation(program, "time");
  int uniform_location2 = glGetUniformLocation(program, "rotation_point_x");
  int uniform_location3 = glGetUniformLocation(program, "rotation_point_y");
  int uniform_location4 = glGetUniformLocation(program, "period");
  int uniform_location5 = glGetUniformLocation(program, "z_key_offset");
  int uniform_location6 = glGetUniformLocation(program, "x_key_offset");
  int uniform_location7 = glGetUniformLocation(program, "y_rotation_angle");
  int uniform_location8 = glGetUniformLocation(program, "x_rotation_angle");
  int uniform_location9 = glGetUniformLocation(program, "y_key_offset");
  int uniform_location10 = glGetUniformLocation(program, "will_the_cube_rotate");
  

  glUseProgram(program);
  glUniform1f(uniform_location2, ROTATION_POINT_X);
  glUniform1f(uniform_location3, ROTATION_POINT_Y);
  glUniform1f(uniform_location4, ROTATION_PERIOD);
  glUniform1i(uniform_location10, WILL_THE_CUBE_ROTATE);
  glUseProgram(0);


  glEnable(GL_DEPTH_TEST);


  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, buffer2);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);


  glfwSetKeyCallback(window, &key_function);

  glfwSetCursorPosCallback(window, &mouse_function);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  while(!glfwWindowShouldClose(window)){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long int temp = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    float temp2 = (float)temp / 1000;
    glUniform1f(uniform_location, temp2);
    glUniform1f(uniform_location5, z_key_offset);
    glUniform1f(uniform_location6, x_key_offset);
    glUniform1f(uniform_location9, y_key_offset);
    glUniform1f(uniform_location7, y_rotation_angle);
    glUniform1f(uniform_location8, x_rotation_angle);


    glDrawArrays(GL_TRIANGLES, 0, 36);    
    
    glUseProgram(0);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

