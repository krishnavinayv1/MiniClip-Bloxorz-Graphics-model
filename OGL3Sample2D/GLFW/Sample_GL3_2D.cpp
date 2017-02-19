#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define out1(x)cout<<#x<<" is "<<x<<endl
using namespace std;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;

};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint MatrixID;
} Matrices;
struct fuse {
    VAO *vv;
    float xcentre;
    float ycentre;
    float zcentre;
    float xlen;
    float ylen;
    float zlen;
    float x_tra;
    float y_tra;
    float z_tra;
    float rot;
    float xr,yr,zr;
};
typedef struct fuse fuse;
fuse cube;
int level_flag=0,k=0;
int main_flag=0;int flagforrot=0; int rotflag=0;
float erro=0;
fuse plate[10][15][6];
float posy[10][15][6];
GLuint programID;
float xcm=0,zcm=0,xcm_pre=0,zcm_pre=0;
int anyer=0;
int flag[11];
float hel_rot=0.0f;
float xview=-20,yview=60,zview=40;
float xtarget=0,ytarget=0,ztarget=0;
int number_of_step=0;
/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}


int a[10][15][6];
void init(float e,float y)
{
    cube.x_tra=e;
    cube.z_tra=y;
    cube.y_tra=0;
    cube.xr=0;
    cube.zr=0;
    xcm=e;
    zcm=y;
    main_flag=0;
    flag[0]=0;
    flag[1]=0;
    flag[2]=0;
    flag[3]=0;
}

void level1(){
    init(0,0);
    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            posy[i][j][0]=-60;
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            a[i][j][0]=1;
    for(int i=0;i<2;i++)
        for(int j=0;j<10;j++)
            a[i][j][0]=0;
    for(int i=9;i>=8;i--)
        for(int  j=0;j<10;j++)
            a[i][j][0]=0;
    a[6][7][0]=0;
    a[6][0][0]=0;
    a[6][2][0]=0;
    a[6][1][0]=0;
    a[6][3][0]=0;
    a[6][4][0]=0;
    int i=7;
    for(int j=0;j<6;j++)
        a[i][j][0]=0;
    a[i][9][0]=0;
    a[5][0][0]=0;
    i=2;
    for(int j=3;j<10;j++)
        a[i][j][0]=0;
    i=3;
    for(int j=6;j<10;j++)
        a[i][j][0]=0;
    a[4][9][0]=0;
    for(int i=0;i<10;i++)
        for(int j=10;j<15;j++)
            a[i][j][0]=0;
    a[6][7][0]=4;
    a[4][5][0]=5;
    a[5][6][0]=5;
    a[4][4][0]=1;
}
void level2(){
    init(-15,-5);
    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            posy[i][j][1]=-60;

    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            a[i][j][1]=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<15;j++)
            a[i][j][1]=0;
    for(int i=8;i<10;i++)
        for(int j=0;j<15;j++)
            a[i][j][1]=0;
    for(int i=3;i<8;i++){
        a[i][4][1]=0;
        a[i][5][1]=0;
    }
    int i=2;
    for(int j=6;j<15;j++)
        a[i][j][1]=1;
    a[2][10][1]=0;
    a[2][11][1]=0;
    for(int i=3;i<8;i++)
        for(int j=10;j<12;j++)
            a[i][j][1]=0;
    a[7][12][1]=0;
    a[7][13][1]=0;
    a[7][14][1]=0;
    a[3][13][1]=4;
    a[4][2][1]=2;
    a[2][7][1]=5;
    a[3][8][1]=3;
    a[6][4][1]=5;
    a[6][5][1]=5;
    // a[4][4][1]=1;
    // a[4][5][1]=1;
}
void level3(){
    init(-15,10);
    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            posy[i][j][2]=-60;
    for(int i=0;i<10;i++){
        for(int j=0;j<15;j++)
            a[i][j][2]=1;
    }
    for(int i=0;i<4;i++)
        for(int j=0;j<15;j++)
            a[i][j][2]=0;
    for(int i=8;i<10;i++)
        for(int j=0;j<15;j++)
            a[i][j][2]=0;
    for(int i=6;i<8;i++)
        for(int j=4;j<11;j++)
            a[i][j][2]=0;
    a[7][11][2]=0;
    int i=3;
    for(int j=6;j<15;j++)
        a[i][j][2]=1;
    a[4][4][2]=0;
    a[4][5][2]=0;
    a[4][9][2]=0;
    a[4][10][2]=0;
    a[5][9][2]=0;
    a[5][10][2]=0;
    a[3][13][2]=0;
    a[3][14][2]=0;
    a[4][13][2]=0;
    a[4][14][2]=0;
    a[6][13][2]=4;
    a[7][11][2]=1;
    a[8][13][2]=1;
    a[8][14][2]=1;
    a[8][12][2]=1;
}
void level4(){
    init(-15,10);
    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            posy[i][j][3]=-60;
    for(int i=0;i<10;i++){
        for(int j=0;j<15;j++)
            a[i][j][3]=1;
    }
    int i=0;
    for(int j=0;j<15;j++)
        a[i][j][3]=0;
    for(int i=1;i<3;i++){
        for(int j=0;j<3;j++)
            a[i][j][3]=0;
        for(int j=13;j<15;j++)
            a[i][j][3]=0;
    }
    for(int i=3;i<6;i++)
        for(int j=4;j<9;j++)
            a[i][j][3]=0;
    a[4][3][3]=0;
    a[4][9][3]=0;
    a[5][3][3]=0;
    a[5][9][3]=0;
    a[6][3][3]=0;
    a[6][4][3]=0;
    a[7][3][3]=0;
    a[7][4][3]=0;

    for(int i=8;i<10;i++)
        for(int j=0;j<5;j++)
            a[i][j][3]=0;
    for(int i=8;i<10;i++)
        for(int j=8;j<10;j++)
            a[i][j][3]=0;

    a[8][6][3]=4;
    a[8][13][3]=2;
    a[6][8][3]=5;
    a[6][9][3]=5;
    a[7][8][3]=0;
    a[7][9][3]=0;
}
void level5(){
  init(-20,-5);
    for(int i=0;i<10;i++)
        for(int j=0;j<15;j++)
            posy[i][j][4]=-60;
  for(int i=0;i<10;i++){
    for(int j=0;j<15;j++)
      a[i][j][4]=1;
  }
  int i;
  
  for(int i=0;i<3;i++){
    for(int j=0;j<5;j++)
      a[i][j][4]=0;
  }
  a[1][6][4]=0;
  a[1][7][4]=0;
  a[2][6][4]=0;
  a[2][7][4]=0;
  a[3][6][4]=0;
  for(int i=3;i<6;i++){
    for(int j=7;j<11;j++)
      a[i][j][4]=0;
  }
  a[6][8][4]=0;
  a[6][7][4]=0;
  a[5][11][4]=0;
  for(i=6;i<9;i++)
  for(int j=11;j<15;j++)
    a[i][j][4]=0;
  for(i=0;i<2;i++)
  for(int j=11;j<15;j++)
    a[i][j][4]=0;
  a[2][13][4]=0,a[2][14][4]=0;
  for(i=4;i<9;i++)
  for(int j=0;j<4;j++)
    a[i][j][4]=0;
  for(i=6;i<9;i++)
  for(int j=4;j<6;j++)
    a[i][j][4]=0;
  a[9][6][4]=0;
  a[9][10][4]=0;
  a[4][13][4]=4;
  for(int j=0;j<6;j++)
    a[9][j][4]=0;
  for(int j=10;j<15;j++)
    a[9][j][4]=0;
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    //    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
            0,                  // attribute 0. Vertices
            3,                  // size (x,y,z)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
            1,                  // attribute 1. Color
            3,                  // size (r,g,b)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/

float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_C:
                rectangle_rot_status = !rectangle_rot_status;
                break;
            case GLFW_KEY_P:
                triangle_rot_status = !triangle_rot_status;
                break;
            case GLFW_KEY_X:
                // do something ..
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
                // case GLFW_KEY_UP:
                //     if(main_flag==1)
                //       {
                //          cube.zr+=90;
                //         // cube.xr=0;
                //         // main_flag=2;

                //       }
                //     if(main_flag==2)
                //     {
                //       // cube.xr=90;
                //       // cube.zr=0;
                //       // main_flag=1;

                //     }
                //     if(main_flag==3)
                //     {

                //     }
                //     break;
                // case GLFW_KEY_DOWN:
                //  if(main_flag==1)
                //       {
                //         cube.rot+=90;
                //         cube.xr=0;
                //         cube.yr=0;
                //         cube.zr=1;
                //       }
                //     if(main_flag==2)
                //     {

                //     }
                //     if(main_flag==3)
                //     {

                //     }
                //     break;
                // case GLFW_KEY_LEFT:
                //  if(main_flag==1)
                //       {
                //         cube.rot+=90;
                //         cube.xr=0;
                //         cube.yr=0;
                //         cube.zr=1;
                //       }
                //     if(main_flag==2)
                //     {

                //     }
                //     if(main_flag==3)
                //     {

                //     }
                //     break;
                // case GLFW_KEY_RIGHT:
                //  if(main_flag==1)
                //       {
                //         cube.rot+=90;
                //         cube.xr=0;
                //         cube.yr=0;
                //         cube.zr=1;
                //       }
                //     if(main_flag==2)
                //     {

                //     }
                //     if(main_flag==3)
                //     {

                //     }
                //     break;
                // default:
                //     break;
            case GLFW_KEY_RIGHT:
              if(flagforrot==0 and rotflag==0){
                if(main_flag==0)
                {
                    //xtr+=0.5;
                    xcm+=7.5;
                    cube.x_tra+=5;
                    cube.zr-=90;
                    main_flag=1;
                }
                else if(main_flag==1)
                {
                    //xtr+=1;
                    cube.x_tra+=10;
                    xcm+=7.5;
                    cube.zr=0;
                    main_flag=0;
                }
                else if(main_flag==2)
                {
                    cube.x_tra+=5;
                    xcm+=5;
                }
                rotflag=1;
              }
              number_of_step+=1;
                break;
            case GLFW_KEY_LEFT:
               if(flagforrot==0 and rotflag==0){
                if(main_flag==0)
                {
                    cube.x_tra-=10;
                    xcm-=7.5;
                    cube.zr-=90;
                    main_flag=1;
                }
                else if(main_flag==1)
                {
                    xcm-=7.5;
                    cube.x_tra-=5;
                    cube.zr=0;
                    main_flag=0;
                }
                else if(main_flag==2)
                {
                    xcm-=5;
                    cube.x_tra-=5;
                }
                rotflag=1;
              }
              number_of_step+=1;
                break;
            case GLFW_KEY_UP:
             if(flagforrot==0 and rotflag==0){
                if(main_flag==0)
                {
                    //xtr+=0.5;
                    cube.z_tra-=5;
                    zcm-=7.5;
                    cube.xr-=90;
                    main_flag=2;
                }
                else if(main_flag==1)
                {
                    //xtr+=1;
                    cube.z_tra-=5;
                    zcm-=5;
                    //cube.zangle=0;
                    //main_flag=0;
                }
                else if(main_flag==2)
                {
                    //cube.x+=0.5;
                    cube.xr=0;
                    zcm-=7.5;
                    cube.z_tra-=10;
                    main_flag=0;
                }
              rotflag=1;
              }
              number_of_step+=1;
                break;
            case GLFW_KEY_DOWN:
             if(flagforrot==0 and rotflag==0){
                if(main_flag==0)
                {
                    //xtr+=0.5;
                    cube.z_tra+=10;
                    cube.xr-=90;
                    main_flag=2;
                    zcm+=7.5;
                }
                else if(main_flag==1)
                {
                    //xtr+=1;
                    cube.z_tra+=5;
                    zcm+=5;
                    //cube.zangle=0;
                    //main_flag=0;
                }
                else if(main_flag==2)
                {
                    //cube.x+=0.5;
                    cube.xr=0;
                    cube.z_tra+=5;
                    main_flag=0;
                    zcm+=7.5;
                }
                rotflag=1;
              }
              number_of_step+=1;
                break;
            case GLFW_KEY_T:
              flag[6]=1;
              flag[4]=0;
              flag[5]=0;
              flag[7]=0;
              flag[8]=0;
              flag[9]=0;
              xview=-20;
              yview=60;
              zview=40;
              xtarget=0;
              ytarget=0;
              ztarget=0;
              hel_rot=0;
              break;
            case GLFW_KEY_U:
              xview=0;
              yview=70;
              zview=1;
              xtarget=0;
              ytarget=0;
              ztarget=0;
              flag[5]=1;
              flag[4]=0;
              flag[6]=0;
              flag[7]=0;
              flag[8]=0;
              flag[9]=0;
              hel_rot=0;
              break;
            case GLFW_KEY_F:
              flag[4]=1;
              flag[5]=0;
              flag[6]=0;
              flag[7]=0;
              flag[8]=0;
              flag[9]=0;
              hel_rot=0;
              break;
            case GLFW_KEY_B:
              flag[4]=0;
              flag[5]=0;
              flag[6]=0;
              flag[7]=1;
              flag[8]=0;
              flag[9]=0;
              hel_rot=0;
              break;
            case GLFW_KEY_A:
              flag[4]=0;
              flag[5]=0;
              flag[6]=0;
              flag[7]=0;
              flag[8]=1;
              flag[9]=0;
              yview=60;
              xview=40*cos(hel_rot*(M_PI/180.0));
              zview=40*sin(hel_rot*(M_PI/180.0));
              xtarget=0;
              ytarget=0;
              ztarget=0;
              hel_rot+=10; 
              break;
            case GLFW_KEY_D:
              flag[4]=0;
              flag[5]=0;
              flag[6]=0;
              flag[7]=0;
              flag[8]=0;
              flag[9]=1;
              yview=60;
              xview=40*cos(hel_rot*(M_PI/180.0));
              zview=40*sin(hel_rot*(M_PI/180.0));
              xtarget=0;
              ytarget=0;
              ztarget=0;
              hel_rot-=10;
              break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
    switch (key) {
        case 'Q':
        case 'q':
            quit(window);
            break;
        default:
            break;
    }
}
int mouseleftpressed=0;
int mouseleftfirstpressed=0;
int mouserightpressed=0;
int mouserightfirstpressed=0;
double mouse_x,mouse_y;
double mouse_x_old,mouse_y_old;
int moveredbox=0;
int movegreenbox=0;
int movecannonbox=0;
void mouseright(GLFWwindow* window){
    if(mouserightpressed){
     // out1("camehere");
        glfwGetCursorPos(window,&mouse_x,&mouse_y);
        if(!mouserightfirstpressed){
            mouse_x_old=mouse_x;
            mouse_y_old=mouse_y;
            mouserightfirstpressed=1;
        }
        out1(mouse_x);
        out1(mouse_x-mouse_x_old);
        hel_rot+=(-1*(mouse_x-mouse_x_old)+200)/200.0;
              xview=40*cos(hel_rot*(M_PI/180.0));
              zview=40*sin(hel_rot*(M_PI/180.0));
        

    }
}
/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            
            if (action != GLFW_RELEASE){
                
                mouseleftpressed=1;

            }

            if(action == GLFW_RELEASE){
                moveredbox=0;
                movegreenbox=0;
                mouseleftfirstpressed=0;
                mouseleftpressed=0;
                movecannonbox=0;
            }

            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS){
                mouserightpressed=1;



            }
            if (action == GLFW_RELEASE) {
                mouserightpressed=0;
            }
            break;
        default:
            break;
    }
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
       is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    GLfloat fov = 0.9f;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
       glLoadIdentity ();
       gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    // Matrices.projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f);
    Matrices.projection = glm::perspective(fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);
}

VAO *triangle, *rectangle;

// Creates the triangle object used in this sample code
void createTriangle ()
{
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    static const GLfloat vertex_buffer_data [] = {
        0, 1,0, // vertex 0
        -1,-1,0, // vertex 1
        1,-1,0, // vertex 2
    };

    static const GLfloat color_buffer_data [] = {
        1,0,0, // color 0
        0,1,0, // color 1
        0,0,1, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
}


int score=1000;
int createRectangle (GLfloat *vertex_buffer_data,float a,float b,float l,float h,float tilt,int cnt)
  {
    vertex_buffer_data[cnt]=a;
    cnt++;
    vertex_buffer_data[cnt]=b;
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    vertex_buffer_data[cnt]=a+l*cos(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=b+l*sin(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    vertex_buffer_data[cnt]=a+h*sin(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=b+h*cos(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    vertex_buffer_data[cnt]=a+h*sin(tilt*(M_PI/180))+l*cos(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=b+h*cos(tilt*(M_PI/180))+l*sin(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    vertex_buffer_data[cnt]=a+l*cos(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=b+l*sin(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    vertex_buffer_data[cnt]=a+h*sin(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=b+h*cos(tilt*(M_PI/180));
    cnt++;
    vertex_buffer_data[cnt]=0;
    cnt++;
    return cnt;
  }


VAO *segments[10];
void Numbers()
    {
      GLfloat vertex_buffer_data0[500],color_buffer_data0[500];
      GLfloat vertex_buffer_data1[500],color_buffer_data1[500];
      GLfloat vertex_buffer_data2[500],color_buffer_data2[500];
      GLfloat vertex_buffer_data3[500],color_buffer_data3[500];
      GLfloat vertex_buffer_data4[500],color_buffer_data4[500];
      GLfloat vertex_buffer_data5[500],color_buffer_data5[500];
      GLfloat vertex_buffer_data6[500],color_buffer_data6[500];
      GLfloat vertex_buffer_data7[500],color_buffer_data7[500];
      GLfloat vertex_buffer_data8[500],color_buffer_data8[500];
      GLfloat vertex_buffer_data9[500],color_buffer_data9[500];

      int cnt=0,i,m=1,l;
      cnt=0;
      for(i=0;i<=9;i++)
      {
        cnt=0;
        if(i==0) 
        {  
          cnt=createRectangle(vertex_buffer_data0,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data0,2,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data0,2,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data0,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data0,2.45,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data0,2,2.05,0.5,0.05,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data0[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data0, color_buffer_data0, GL_FILL);
      //    printf("segments-%d\n",i);
        }
        if(i==1)
        {
          cnt=createRectangle(vertex_buffer_data1,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data1,2.45,2.05,0.05,0.5,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data1[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data1, color_buffer_data1, GL_FILL);

        }
        if(i==2)
        {
          cnt=createRectangle(vertex_buffer_data2,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data2,2,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data2,2,2.5,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data2,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data2,2,2.05,0.5,0.05,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data2[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data2, color_buffer_data2, GL_FILL);

        }
        if(i==3)
        {
          cnt=createRectangle(vertex_buffer_data3,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data3,2,2.5,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data3,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data3,2.45,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data3,2,2.05,0.5,0.05,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data3[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data3, color_buffer_data3, GL_FILL);
        }
        if(i==4)
        {
          cnt=createRectangle(vertex_buffer_data4,2,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data4,2,2.5,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data4,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data4,2.45,2.05,0.05,0.5,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data4[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data4, color_buffer_data4, GL_FILL);

        }
        if(i==5)
        {
          cnt=createRectangle(vertex_buffer_data5,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data5,2,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data5,2,2.5,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data5,2.45,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data5,2,2.05,0.5,0.05,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data5[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data5, color_buffer_data5, GL_FILL);

        } 
        if(i==6)
        {
          cnt=createRectangle(vertex_buffer_data6,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data6,2,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data6,2,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data6,2,2.5,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data6,2.45,2.05,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data6,2,2.05,0.5,0.05,0,cnt);

          for(l=0;l<cnt;l++)
          {
            color_buffer_data6[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data6, color_buffer_data6, GL_FILL);

        } 
        if(i==7)
        {
          cnt=createRectangle(vertex_buffer_data7,2,2.95,0.5,0.05,0,cnt);
          cnt=createRectangle(vertex_buffer_data7,2.45,2.5,0.05,0.5,0,cnt);
          cnt=createRectangle(vertex_buffer_data7,2.45,2.05,0.05,0.5,0,cnt);
          for(l=0;l<cnt;l++)
          {
            color_buffer_data7[l]=0;
          }    
          segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data7, color_buffer_data7, GL_FILL);

        }
        if(i==8)
        {
         cnt=createRectangle(vertex_buffer_data8,2,2.95,0.5,0.05,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2,2.5,0.05,0.5,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2,2.05,0.05,0.5,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2,2.5,0.5,0.05,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2.45,2.5,0.05,0.5,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2.45,2.05,0.05,0.5,0,cnt);
         cnt=createRectangle(vertex_buffer_data8,2,2.05,0.5,0.05,0,cnt);

         for(l=0;l<cnt;l++)
         {
          color_buffer_data8[l]=0;
        }    
        segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data8, color_buffer_data8, GL_FILL);

      } 
      if(i==9)
      {
        cnt=createRectangle(vertex_buffer_data9,2,2.95,0.5,0.05,0,cnt);
        cnt=createRectangle(vertex_buffer_data9,2,2.5,0.05,0.5,0,cnt);
        cnt=createRectangle(vertex_buffer_data9,2,2.5,0.5,0.05,0,cnt);
        cnt=createRectangle(vertex_buffer_data9,2.45,2.5,0.05,0.5,0,cnt);
        cnt=createRectangle(vertex_buffer_data9,2.45,2.05,0.05,0.5,0,cnt);
        cnt=createRectangle(vertex_buffer_data9,2,2.05,0.5,0.05,0,cnt);
        for(l=0;l<cnt;l++)
        {
          color_buffer_data9[l]=0;
        }    
        segments[i]=create3DObject(GL_TRIANGLES, cnt/3, vertex_buffer_data9, color_buffer_data9, GL_FILL);

      }   
    }
    /*
    0-cnt=createRectangle(vertex_buffer_data,2,2.95,0.5,0.05,0,cnt);
    5-cnt=createRectangle(vertex_buffer_data,2,2.5,0.05,0.5,0,cnt);
    4-cnt=createRectangle(vertex_buffer_data,2,2.05,0.05,0.5,0,cnt);
    6-cnt=createRectangle(vertex_buffer_data,2,2.5,0.5,0.05,0,cnt);
    1-cnt=createRectangle(vertex_buffer_data,2.45,2.5,0.05,0.5,0,cnt);
    2-cnt=createRectangle(vertex_buffer_data,2.45,2.05,0.05,0.5,0,cnt);
    3-cnt=createRectangle(vertex_buffer_data,2,2.05,0.5,0.05,0,cnt);
    */

    //}


  }
// Creates the rectangle object used in this sample code

VAO *createCube(float x,float y,float z,float w,float h,float q,int color_id,int counter,int br_flag,int er,int er1,int er2)
{
    VAO *rt;
    float x1=x+(w/2.0f);float y1=y+(h/2.0f);float x2=x-(w/2.0f);float y2=y-(h/2.0f);float z1=z+(q/2.0f);
    float z2=z-(q/2.0f);
    static GLfloat vertex_buffer_data[110];
    //1st face
    vertex_buffer_data[0]=x2;vertex_buffer_data[1]=y2;vertex_buffer_data[2]=z2;
    vertex_buffer_data[3]=x1;vertex_buffer_data[4]=y2;vertex_buffer_data[5]=z2;
    vertex_buffer_data[6]=x1;vertex_buffer_data[7]=y2;vertex_buffer_data[8]=z1;
    vertex_buffer_data[9]=x1;vertex_buffer_data[10]=y2;vertex_buffer_data[11]=z1;
    vertex_buffer_data[12]=x2;vertex_buffer_data[13]=y2;vertex_buffer_data[14]=z1;
    vertex_buffer_data[15]=x2;vertex_buffer_data[16]=y2;vertex_buffer_data[17]=z2;
    //4th face
    vertex_buffer_data[18]=x2;vertex_buffer_data[19]=y2;vertex_buffer_data[20]=z2;
    vertex_buffer_data[21]=x2;vertex_buffer_data[22]=y2;vertex_buffer_data[23]=z1;
    vertex_buffer_data[24]=x2;vertex_buffer_data[25]=y1;vertex_buffer_data[26]=z1;
    vertex_buffer_data[27]=x2;vertex_buffer_data[28]=y1;vertex_buffer_data[29]=z1;
    vertex_buffer_data[30]=x2;vertex_buffer_data[31]=y1;vertex_buffer_data[32]=z2;
    vertex_buffer_data[33]=x2;vertex_buffer_data[34]=y2;vertex_buffer_data[35]=z2;
    //6th face
    vertex_buffer_data[36]=x2;vertex_buffer_data[37]=y1;vertex_buffer_data[38]=z2;
    vertex_buffer_data[39]=x2;vertex_buffer_data[40]=y1;vertex_buffer_data[41]=z1;
    vertex_buffer_data[42]=x1;vertex_buffer_data[43]=y1;vertex_buffer_data[44]=z1;
    vertex_buffer_data[45]=x1;vertex_buffer_data[46]=y1;vertex_buffer_data[47]=z1;
    vertex_buffer_data[48]=x1;vertex_buffer_data[49]=y1;vertex_buffer_data[50]=z2;
    vertex_buffer_data[51]=x2;vertex_buffer_data[52]=y1;vertex_buffer_data[53]=z2;
    //5th face
    vertex_buffer_data[54]=x2;vertex_buffer_data[55]=y2;vertex_buffer_data[56]=z2;
    vertex_buffer_data[57]=x1;vertex_buffer_data[58]=y2;vertex_buffer_data[59]=z2;
    vertex_buffer_data[60]=x1;vertex_buffer_data[61]=y1;vertex_buffer_data[62]=z2;
    vertex_buffer_data[63]=x1;vertex_buffer_data[64]=y1;vertex_buffer_data[65]=z2;
    vertex_buffer_data[66]=x2;vertex_buffer_data[67]=y1;vertex_buffer_data[68]=z2;
    vertex_buffer_data[69]=x2;vertex_buffer_data[70]=y2;vertex_buffer_data[71]=z2;
    //2nd face
    vertex_buffer_data[72]=x1;vertex_buffer_data[73]=y2;vertex_buffer_data[74]=z2;
    vertex_buffer_data[75]=x1;vertex_buffer_data[76]=y1;vertex_buffer_data[77]=z2;
    vertex_buffer_data[78]=x1;vertex_buffer_data[79]=y1;vertex_buffer_data[80]=z1;
    vertex_buffer_data[81]=x1;vertex_buffer_data[82]=y1;vertex_buffer_data[83]=z1;
    vertex_buffer_data[84]=x1;vertex_buffer_data[85]=y2;vertex_buffer_data[86]=z1;
    vertex_buffer_data[87]=x1;vertex_buffer_data[88]=y2;vertex_buffer_data[89]=z2;
    //3rd face
    vertex_buffer_data[90]=x2;vertex_buffer_data[91]=y2;vertex_buffer_data[92]=z1;
    vertex_buffer_data[93]=x2;vertex_buffer_data[94]=y1;vertex_buffer_data[95]=z1;
    vertex_buffer_data[96]=x1;vertex_buffer_data[97]=y1;vertex_buffer_data[98]=z1;
    vertex_buffer_data[99]=x1;vertex_buffer_data[100]=y1;vertex_buffer_data[101]=z1;
    vertex_buffer_data[102]=x1;vertex_buffer_data[103]=y2;vertex_buffer_data[104]=z1;
    vertex_buffer_data[105]=x2;vertex_buffer_data[106]=y2;vertex_buffer_data[107]=z1;
    // if(br_flag==1){
    //     bricks[counter].c1=x;bricks[counter].c2=y;bricks[counter].high=h;bricks[counter].wid=w;
    //     bricks[counter].id=color_id;
    // }

    // static  GLfloat color_buffer_data[109]={0};
    GLfloat color_buffer_data1[12*3*3];
    GLfloat color_buffer_data2[12*3*3];
    GLfloat color_buffer_data3[12*3*3];
    GLfloat color_buffer_data4[12*3*3];
    GLfloat color_buffer_data5[12*3*3];
    GLfloat color_buffer_data6[12*3*3];
    GLfloat color_buffer_data7[12*3*3];


    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data1[3*v+0] = 0.8;
        color_buffer_data1[3*v+1] = 0.8;
        color_buffer_data1[3*v+2] = 0.6;
    }
    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data2[3*v+0] = 1;
        color_buffer_data2[3*v+1] = 1;
        color_buffer_data2[3*v+2] = 1;
    }
    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data3[3*v+0] = 0.6;
        color_buffer_data3[3*v+1] = 0.36;
        color_buffer_data3[3*v+2] = 0.26;
    }
    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data4[3*v+0] = 0;
        color_buffer_data4[3*v+1] = 1;
        color_buffer_data4[3*v+2] = 0;
    }
    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data5[3*v+0] = 1;
        color_buffer_data5[3*v+1] = 0;
        color_buffer_data5[3*v+2] = 0;
    }
    for (int v = 0; v < 12*3 ; v++){
        color_buffer_data6[3*v+0] = 0;
        color_buffer_data6[3*v+1] = 0;
        color_buffer_data6[3*v+2] = 0;
    }
     for (int v = 0; v < 12*3 ; v++){
        color_buffer_data7[3*v+0] = 1;
        color_buffer_data7[3*v+1] = 0.5;
        color_buffer_data7[3*v+2] = 0;
    }
    if(color_id==0 and (a[er][er1][er2]==1 or a[er][er1][er2]==0)){
        if((er+er1)%2==0)
            rt= create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data1, GL_FILL);
        else
            rt= create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data2, GL_FILL);
    } 
    else if(color_id==1)
    {
        rt = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data4, GL_FILL);
    }
    else if(a[er][er1][er2]==2)
    {
        rt= create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data3, GL_FILL);
    }
    else if(a[er][er1][er2]==3)
    {
        rt= create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data5, GL_FILL);
    }
    else if(a[er][er1][er2]==4)
    {
        rt= create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data6, GL_FILL);
    }
    else if(a[er][er1][er2]==5)
    {
      rt = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data7, GL_FILL);
    }
    return rt;
}

void helping_fun(VAO *ert,float t_x,float t_y,float t_z,float rotation,float a1,float a2,float a3,float b1,float b2,float b3)
{
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model
    Matrices.model = glm::mat4(1.0f);
     glm::mat4 scaleTriangle1 = glm::scale (glm::vec3(b1, b2, b3));
    glm::mat4 translate_box1_rec = glm::translate (glm::vec3(t_x,t_y,t_z)); // glTranslatef
    glm::mat4 rotate_box1_rec = glm::rotate((float)rotation, glm::vec3(a1,a2,a3));  // rotate about vector (1,0,0)
    glm::mat4 box1_rec_Transform = translate_box1_rec*rotate_box1_rec*scaleTriangle1;
    Matrices.model *=box1_rec_Transform;
    MVP = VP * Matrices.model; // MVP = p * V * M
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(ert);
}
void helping_fun1()
{
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateRectangle = glm::translate (glm::vec3(cube.x_tra,erro-2.5,cube.z_tra));        // glTranslatef
    glm::mat4 tra = glm::translate (glm::vec3(0, 2.5, 0));
    glm::mat4 rotateRectangle = glm::rotate((float)(cube.zr*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    glm::mat4 rotateRectangle2 = glm::rotate((float)(cube.xr*M_PI/180.0f), glm::vec3(1,0,0)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateRectangle*rotateRectangle2*rotateRectangle*tra);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(cube.vv);
}

float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
double last_update_time1=glfwGetTime();
double last_update_time2=glfwGetTime();
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw (GLFWwindow* window)
{
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    //  Don't change unless you are sure!!
    //Matrices.view = glm::lookAt(glm::vec3(2,2,3), glm::vec3(0,0,1), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
    Matrices.view = glm::lookAt(glm::vec3(xview,yview,zview), glm::vec3(xtarget,ytarget,ztarget), glm::vec3(0,1,0));
    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    //  Don't change unless you are sure!!
    if(flag[4]==1)
    {
              xview=xcm;
              yview=3;
              zview=zcm+25;
              xtarget=xcm;
              ytarget=5;
              ztarget=zcm;
    }
    if(flag[7]==1)
    {
      if(main_flag==0 or main_flag==2)
      {
        zview=zcm-2.5;
      }
      else
      {
        zview=zcm;
      }
      if( main_flag==2)
      {
        xview=xcm-7.5;
      }
      else
      {
        xview=xcm;
      }
             // xview=xcm;
              yview=5;
             // zview=zcm;
              xtarget=xview;
              ytarget=-6;
              ztarget=zcm-10;
            //  cout<<xview<<" "<<yview<<" "<<zview<<" "<<xtarget<<" "<<ytarget<<" "<<ztarget<<endl;
    }
    // if(flag[8]==1)
    // {
    //     hel_rot+=0.5;
    // }
    // if(flag[9]==1)
    // {
    //     hel_rot-=0.5;
    // }
    // if(flag[8]==1 or flag[9]==1)
    // {
    //     yview=60;
    //     xview=40*cos(hel_rot*(M_PI/180.0));
    //     zview=40*sin(hel_rot*(M_PI/180.0));
    //     xtarget=0;
    //     ytarget=0;
    //     ztarget=0;
    // }
    float r=1;

    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Matrices.model = glm::mat4(1.0f);
    // glm::mat4 translate_box1_rec = glm::translate (glm::vec3(cube.x_tra,0.6,cube.z_tra)); // glTranslatef
    // glm::mat4 translate_box1_rec1 = glm::translate (glm::vec3(0,5,0));
    // glm::mat4 rotate_box1_rec = glm::rotate((float)(cube.rot*M_PI/180.0f), glm::vec3(0,0,1));
    // glm::mat4 box1_rec_Transform = translate_box1_rec*rotate_box1_rec*translate_box1_rec1 ;
    // Matrices.model *=box1_rec_Transform;
    // MVP = VP * Matrices.model; // MVP = p * V * M
    // glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    anyer=0;;
    if(main_flag==0)
    {
        int ww=((xcm+25)/5.0)-1;
        int ww1=((zcm+25)/5.0)-1;
        if((a[ww1][ww][k]==4 ))
        {
         //  double current_time1=glfwGetTime();
           //   if((current_time1 - last_update_time1) >= 15){
               
               // while(erro>-45){
               //   erro-=0.00005;
               //   helping_fun1();
             //    last_update_time1=current_time1;
              
             // erro=0;
             //} 
           //  else    
            anyer=2;
            flagforrot=1;
          //  cout<<xcm<<" "<<zcm<<" "<<level_flag<<" "<<ww1<<ww<<anyer<<endl;
            //flag[0]=0;
        }
        else if(a[ww1][ww][k]==0 or a[ww1][ww][k]==5  or( ww1<0 or ww1>9 or ww>14 or ww<0))
        {
          anyer=1;
          flagforrot=1;
   //      cout<<xcm<<" "<<zcm<<" "<<level_flag<<" "<<ww1<<ww<<anyer<<endl;
        }
        else if((a[ww1][ww][k]==2) && ((xcm_pre!=xcm) || (zcm_pre!=zcm)))
        {
            if(flag[0]==0)
                flag[0]=1;
            else
                flag[0]=0;
            if(flag[2]==0)
                flag[2]=1;
            else
                flag[2]=0;
        }
         else if((a[ww1][ww][k]==3) && ((xcm_pre!=xcm) || (zcm_pre!=zcm)))
        {
            if(flag[1]==0)
                flag[1]=1;
            else
                flag[1]=0;
               
        }
        if(flag[0]==1)
        {
            a[6][4][1]=5;
            a[6][5][1]=5;
        }
        else{
            a[6][4][1]=0;
            a[6][5][1]=0;
          }
        if(flag[1]==1)
        {
          a[6][10][1]=1;
          a[6][11][1]=1;
        }
        else{
          a[6][10][1]=0;
          a[6][11][1]=0;
        }
        if(flag[2]==1)
        {
          a[6][8][3]=5;
          a[6][9][3]=5;
        }
        else
        {
          a[6][8][3]=0;
          a[6][9][3]=0;
        }
    }
    else if(main_flag==1)
    {
        int ww=((xcm+2.5+25)/5.0)-1;
        int ww1=((zcm+25)/5.0)-1;
        int ww2=((xcm-2.5+25)/5.0)-1;
        int ww3=((zcm+25)/5.0)-1;
        if(( a[ww1][ww][k]==0) or(a[ww3][ww2][k]==0 ) or (ww1<0 or ww1>9 or ww>14 or ww<0) or (ww3<0 or ww3>9 or ww2>14 or ww2<0) ){
           
            //  erro=0;
            anyer=1;
            flagforrot=1;
        }
        else if(((a[ww1][ww][k]==2 or a[ww3][ww2][k]==2)) and (((xcm_pre!=xcm) or (zcm_pre!=zcm))))
        {
            if(flag[0]==0)
                flag[0]=1;
            else
                flag[0]=0;
        }

        if(flag[0]==1)
        {
            a[6][4][1]=1;
            a[6][5][1]=1;
        }
        else{
            a[6][4][1]=0;
            a[6][5][1]=0;
        }

    }
    else
    {
        int ww=((xcm+25)/5.0)-1;
        int ww1=((zcm+2.5+25)/5.0)-1;
        int ww2=((xcm+25)/5.0)-1;
        int ww3=((zcm-2.5+25)/5.0)-1;
        if((( a[ww1][ww][k]==0) or( a[ww3][ww2][k]==0)) or (ww1<0 or ww1>9 or ww>14 or ww<0) or (ww3<0 or ww3>9 or ww2>14 or ww2<0) ){
           
            anyer=1;
            flagforrot=1;
        }
        else if((a[ww1][ww][k]==2 or a[ww3][ww2][k]==2) and ((xcm_pre!=xcm) or (zcm_pre!=zcm)))
        {
            if(flag[0]==0)
                flag[0]=1;
            else
                flag[0]=0;
        }
        if(flag[0]==1)
        {
            a[6][4][1]=1;
            a[6][5][1]=1;
        }
        else{
            a[6][4][1]=0;
            a[6][5][1]=0;
        }
    }
     double current_time2=glfwGetTime();

     if((current_time2 - last_update_time2) >= 0.05 and rotflag==1){
            last_update_time2=current_time2;
          //  if(cube.xr==90)
            rotflag=0;
            //cout<<"123456"<<" "<<erro<<endl;
          }
 double current_time1=glfwGetTime();
          
          if((current_time1 - last_update_time1) >= 0.05 and (anyer==1 or anyer==2)){
            last_update_time1=current_time1;
            erro-=0.85;
            //cout<<"123456"<<" "<<erro<<endl;
          }
    if (erro<-40)
    {
      //cout<<"safddf"<<endl;
    if(anyer==2)
    {
      number_of_step=0;
        if(level_flag==0){
            level2();
            level_flag=1;
        }
        else if(level_flag==1){
            level3();
            level_flag=2;
        }
        else if(level_flag==2){
            level_flag=3;
            level4();
        }
        else if(level_flag==3){
            level_flag=4;
            level5();
        }
        else{
          level5();
        }
    }
    if(anyer==1)
    {
      number_of_step=0;
        if(level_flag==0){
            level1();
        }
        else if(level_flag==1){
            level2();
        }
        else if(level_flag==2){
            level3();
        }
        else if(level_flag==3){
            level4();
        }
        else if(level_flag==4){
          level5();
        }
    }
    erro=0;
    flagforrot=0;
  }

    k=level_flag;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateRectangle = glm::translate (glm::vec3(cube.x_tra,erro-2.5,cube.z_tra));        // glTranslatef
    glm::mat4 tra = glm::translate (glm::vec3(0, 2.5, 0));
    glm::mat4 rotateRectangle = glm::rotate((float)(cube.zr*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    glm::mat4 rotateRectangle2 = glm::rotate((float)(cube.xr*M_PI/180.0f), glm::vec3(1,0,0)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateRectangle*rotateRectangle2*rotateRectangle*tra);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(cube.vv);
    xcm_pre=xcm;
    zcm_pre=zcm;
    // helping_fun(cube,r,0.5025f,2*r,(float)(1*(M_PI/180.0)),0.0f,0.0f,1.0f);
    // int k=0;

    for(int i=0;i<10;i++){
        for(int j=0;j<15;j++)
        {
            if(a[i][j][k]==1 ||a[i][j][k]==2 || a[i][j][k]==3 ||a[i][j][k]==4 || a[i][j][k]==5){
                posy[i][j][k]+=((i+j)/1.5);
                if(posy[i][j][k]>0)
                    posy[i][j][k]=0;
                helping_fun(plate[i][j][k].vv,(j+1)*5-25,posy[i][j][k]-5.6,(i+1)*5-25,0,0.0f,0.0f,1.0f,1.0,1.0,1.0);
            }
        }
        // Increment angles
        float increments = 1;

        // camera_rotation_angle++; // Simulating camera rotation
        triangle_rotation = triangle_rotation + increments*triangle_rot_dir*triangle_rot_status;
        rectangle_rotation = rectangle_rotation + increments*rectangle_rot_dir*rectangle_rot_status;
    }
     Matrices.view = glm::lookAt(glm::vec3(-20,60,40), glm::vec3(0,0,0), glm::vec3(0,1,0));
      // Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
     int temp1=level_flag+1;
 //helping_fun(segments[temp1],33.0,20.0f,10.0f,(float)(65*(M_PI/180.0)),0.0f,0.0f,1.0f,2.5,2.5,2.5);
      int temp=number_of_step;
//int ff=0;    if(temp<0)temp=-1*score,ff=1;
    Matrices.view = glm::lookAt(glm::vec3(-20,60,40), glm::vec3(0,0,0), glm::vec3(0,1,0));  
    float tr=0;
    if(temp==0)
    {
     helping_fun(segments[0],-30.0+tr,31.0f,-10.0f,(float)(-19*(M_PI/180.0)),0.0f,0.0f,1.0f,2.5,2.5,2.5);
    }
    helping_fun(segments[temp1],-30.0-8,31.0f-(-8/2.2)*(0.65f),-10.0f-(-8/2.2)*(0.1f),(float)(-19*(M_PI/180.0)),0.0f,0.0f,1.0f,2.5,2.5,2.5);
    while(temp>0)
    {
      int kk=temp%10;
      temp=temp/10;
      helping_fun(segments[kk],-30.0+tr,31.0f-(tr/2.2)*(0.75f),-10.0f-(tr/2.2)*(0.1f),(float)(-19*(M_PI/180.0)),0.0f,0.0f,1.0f,2.5,2.5,2.5);
      tr-=2.5;
    }
  //  if(ff==1);
     // helping_fun(uy,3.6+tr,3.3,0,0,0,0,1);
    mouseright(window);
}


/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        //        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
        //        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
       is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
  

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (0.5f, 1.0f, 1.0f, 0.0f); // R, G, B, A
    // glClearColor (1.0f, 0.9f, 0.4f, 0.0f);
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
    int width = 1500;
    int height = 800;

    GLFWwindow* window = initGLFW(width, height);

    initGL (window, width, height);
    level1();
    level2();
    level3();
    level4();
    level5();
    level1();
     Numbers();
    double last_update_time = glfwGetTime(), current_time;
    cube.vv = createCube(0,0,0,5,10.0,5,1,0,0,0,0,0);
    cube.xlen=5;
    cube.ylen=10;
    cube.zlen=5;cube.x_tra=0;cube.z_tra=0;cube.rot=0;
    cube.y_tra=0;cube.xr=0;cube.yr=0;cube.zr=0;
    for(int k=0;k<5;k++){
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<15;j++)
                plate[i][j][k].vv = createCube(0,0,0,5,1.2,5,0,0,0,i,j,k);
        }
    }
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // OpenGL Draw commands
        draw(window);

        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.005) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
         //    cout<<xcm<<" "<<zcm<<" "<<main_flag<<endl;
            // cout<<anyer<<" "<<level_flag<<" "<<cube.x_tra<<" "<<cube.z_tra<<endl;
            last_update_time = current_time;
        }
    }

    glfwTerminate();
    //    exit(EXIT_SUCCESS);
}
