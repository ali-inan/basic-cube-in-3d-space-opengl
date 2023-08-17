#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

using namespace std;

extern float z_key_offset;
extern float x_key_offset;
extern float y_key_offset;
extern float y_rotation_angle;
extern float x_rotation_angle;
extern int write_the_position;

float* create_square(string st, float x, float y, float z, float lenght){
  float* ptr = new float[18];
  if(st == "xz"){
    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;
    ptr[3] = x;
    ptr[4] = y;
    ptr[5] = z + lenght;
    ptr[6] = x + lenght;
    ptr[7] = y;
    ptr[8] = z;

    ptr[9] = x + lenght;
    ptr[10] = y;
    ptr[11] = z;
    ptr[12] = x;
    ptr[13] = y;
    ptr[14] = z + lenght;
    ptr[15] = x + lenght;
    ptr[16] = y;
    ptr[17] = z + lenght; 
  }

  else if(st == "xy"){
    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;
    ptr[3] = x;
    ptr[4] = y + lenght;
    ptr[5] = z;
    ptr[6] = x + lenght;
    ptr[7] = y;
    ptr[8] = z;

    ptr[9] = x + lenght;
    ptr[10] = y;
    ptr[11] = z;
    ptr[12] = x;
    ptr[13] = y + lenght;
    ptr[14] = z;
    ptr[15] = x + lenght;
    ptr[16] = y + lenght;
    ptr[17] = z;
  }

  else if(st == "yz"){
    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;
    ptr[3] = x;
    ptr[4] = y + lenght;
    ptr[5] = z;
    ptr[6] = x;
    ptr[7] = y;
    ptr[8] = z + lenght;

    ptr[9] = x;
    ptr[10] = y;
    ptr[11] = z +lenght;
    ptr[12] = x;
    ptr[13] = y + lenght;
    ptr[14] = z;
    ptr[15] = x;
    ptr[16] = y + lenght;
    ptr[17] = z + lenght;
  }
  return ptr;
}

float* create_cube(float x, float y, float z, float lenght){
  float* ptr = new float[108];
  float* ptr2 = create_square("xy", x, y, z, lenght);
  float* ptr3 = create_square("xz", x, y, z, lenght);
  float* ptr4 = create_square("yz", x, y, z, lenght);
  float* ptr5 = create_square("xy", x, y, z + lenght, lenght);
  float* ptr6 = create_square("xz", x, y + lenght, z, lenght);
  float* ptr7 = create_square("yz", x + lenght, y, z, lenght);

  for(int i=0; i<18; i++){
    ptr[i] = ptr2[i];
  }
  for(int i=0; i<18; i++){
    ptr[i+18] = ptr3[15 + i%3 - 3*(i/3)];
  }
  for(int i=0; i<18; i++){
    ptr[i+36] = ptr4[15 + i%3 - 3*(i/3)];
  }
  for(int i=0; i<18; i++){
    ptr[i+54] = ptr5[15 + i%3 - 3*(i/3)];
  }
  for(int i=0; i<18; i++){
    ptr[i+72] = ptr6[i];
  }
  for(int i=0; i<18; i++){
    ptr[i+90] = ptr7[i];
  }
  delete[] ptr2;
  delete[] ptr3;
  delete[] ptr4;
  delete[] ptr5;
  delete[] ptr6;
  delete[] ptr7;

  return ptr;
}

void shader_debugger(int vertex_shader, int fragment_shader, int program){
  int status1;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status1);
  if (status1 == GL_FALSE)
  {
      GLint infoLogLength;
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
      
      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(vertex_shader, infoLogLength, NULL, strInfoLog);
      
      const char *strShaderType = NULL;
      switch(GL_VERTEX_SHADER)
      {
      case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
      }
      
      fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
      delete[] strInfoLog;
  }

  int status2;
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status2);
  if (status2 == GL_FALSE)
  {
      GLint infoLogLength;
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
      
      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(fragment_shader, infoLogLength, NULL, strInfoLog);
      
      const char *strShaderType = NULL;
      switch(GL_FRAGMENT_SHADER)
      {
      case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
      }
      
      fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
      delete[] strInfoLog;
  }
  
  int status3;
  glGetProgramiv(program, GL_LINK_STATUS, &status3);
  if (status3 == GL_FALSE)
  {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;
  }

}

int shader_handler(){
  string str1;
  string str2;
  FILE* ptr = fopen("vertex_shader.txt","r");
  int ch = fgetc(ptr);
  while(ch != EOF){
    str1 = str1 + (char)ch;
    ch = fgetc(ptr);
  }
  fclose(ptr);
  ptr = fopen("fragment_shader.txt", "r");
  ch = fgetc(ptr);
  while(ch != EOF){
    str2 = str2 + (char)ch;
    ch = fgetc(ptr);
  }
  fclose(ptr);

  
  int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char *strFileData  = str1.c_str();
  glShaderSource(vertex_shader, 1, &strFileData, NULL);
  glCompileShader(vertex_shader);
  
  int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *strFileData2  = str2.c_str();
  glShaderSource(fragment_shader, 1, &strFileData2, NULL);
  glCompileShader(fragment_shader);
  
  int program = glCreateProgram();   
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  shader_debugger(vertex_shader, fragment_shader, program);
  return program;
}

void key_function(GLFWwindow* window, int key, int scancode, int action, int mods){
  double xpos;
  double ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  y_rotation_angle = -3.14159/1280 * ypos + 3.14159/4;
  x_rotation_angle =  3.14159/1280 * xpos - 3.14159/4;

  float y = 0.2 * sin(y_rotation_angle);    
  float x = 0.2 * cos(y_rotation_angle) * cos(3.14159/2 - x_rotation_angle);
  float z = 0.2 * cos(y_rotation_angle) * sin(3.14159/2 - x_rotation_angle); 
    
  float x_second = 0.2 * sin(3.14159/2 - x_rotation_angle);
  float z_second = 0.2 * cos(3.14159/2 - x_rotation_angle);
  
  if((key == GLFW_KEY_UP || key == GLFW_KEY_W) && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    x_key_offset -= x;
    y_key_offset -= y;
    z_key_offset -= z;
  }
  else if((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    x_key_offset += x;
    y_key_offset += y;
    z_key_offset += z;
  }
  else if((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    x_key_offset -= x_second;
    z_key_offset += z_second;
  }
  else if((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    x_key_offset += x_second;
    z_key_offset -= z_second;
  }
  if(write_the_position){
    cout << "x position: " << -x_key_offset << " ---- y position: " << -y_key_offset << " ---- z_position " << -z_key_offset << endl;
  }

}


void mouse_function(GLFWwindow* window, double xpos, double ypos){
  y_rotation_angle = -3.14159/1280 * ypos + 3.14159/4;
  x_rotation_angle =  3.14159/1280 * xpos - 3.14159/4;
}
