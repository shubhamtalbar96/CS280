#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <opencv2/opencv.hpp>

#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512

using namespace cv;
using namespace std;

int x[5000], y[5000], i=0, j=0;
//float r, g, b, x,  y;
float r=1.0, g=0.0, b=0.0;
bool check = true;
Mat img, src, mod_img;

int brushThickness = 5;

void init(void)
{
   //Put OpenGL Initializing Code here
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void mouse_click(int button, int state, int x, int y)
{
    switch (button) {
       case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN)
            {
                check = true;
            }
            else
            {
                check = false;
            }
          break;
        case GLUT_MIDDLE_BUTTON:
            break;
        case GLUT_RIGHT_BUTTON:
            break;
    }
}

void mouse_motion(int mousex, int mousey)
{
    j = i;
    if(check)
    {
        x[i] = mousex;
        y[i] = WINDOW_HEIGHT-mousey;
        i++;
    }
    glutPostRedisplay();
}


void display(void)
{
    glColor3f (r, g, b);
    glPointSize (brushThickness);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    
    if(check)
    {
        glBegin(GL_POINTS);
        for(int k=j; k<=i; k++)
        {
            glVertex2i(x[k], y[k]);
        }
        glEnd();
    }
    glFlush();
}

void displayCustomImage()
{
    GLubyte checkImage[WINDOW_HEIGHT][WINDOW_WIDTH][3];
    src = imread("/Users/shubhamtalbar/Desktop/cs280/Hello OpenGL/test_image.png");
    
    resize(src, src, Size(WINDOW_WIDTH, WINDOW_HEIGHT), INTER_LINEAR);
    flip(src, img, -1);
    
    for(int i=0; i<WINDOW_HEIGHT; i++)
    {
        for(int j=0; j<WINDOW_WIDTH; j++)
        {
            checkImage[i][j][0] = img.at<cv::Vec3b>(i,j)[2];
            checkImage[i][j][1] = img.at<cv::Vec3b>(i,j)[1];
            checkImage[i][j][2] = img.at<cv::Vec3b>(i,j)[0];
        }
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
    glFlush();
}

void saveCustomImage()
{
    GLubyte modifiedImage[WINDOW_HEIGHT][WINDOW_WIDTH][3];
    glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, modifiedImage);
    mod_img = img;
    
    for(int i=0; i<WINDOW_HEIGHT; i++)
    {
        for(int j=0; j<WINDOW_WIDTH; j++)
        {
            mod_img.at<cv::Vec3b>(i,j)[2] = modifiedImage[i][j][0];
            mod_img.at<cv::Vec3b>(i,j)[1] = modifiedImage[i][j][1];
            mod_img.at<cv::Vec3b>(i,j)[0] = modifiedImage[i][j][2];
        }
    }
    cv::flip(mod_img, mod_img, -1);
    imwrite("saved_image.jpg", mod_img);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case '0':
           r=1.0; g=0.0; b=0.0;
           break;
       case '1':
           r=0.0; g=1.0; b=0.0;
           break;
       case '2':
           r=0.0; g=0.0; b=1.0;
           break;
       case '3':
           r=1.0; g=0.0; b=1.0;
           break;
       case '4':
           r=0.0; g=1.0; b=1.0;
           break;
       case '5':
           r=1.0; g=1.0; b=0.0;
           break;
       case '6':
           r=1.0; g=1.0; b=1.0;
           break;
       case '7':
           r=0.7; g=0.6; b=0.8;
           break;
       case '8':
           r=0.5; g=0.7; b=0.4;
           break;
       case '9':
           r=1.0; g=0.6; b=0.4;
           break;
      case 32:
           init();
           glutPostRedisplay();
           break;
      case 'r':
           displayCustomImage();
           break;
      case 'w':
           saveCustomImage();
           break;
      case 'Q':
         exit(0);
         break;
      case '+':
           brushThickness += 2;
           break;
      case '=':
           brushThickness += 2;
           break;
      case '-':
           if(brushThickness > 2)
               brushThickness -= 2;
           break;
   }
}

int main(int argc, char** argv)
{
    glutInit (&argc, argv);

    //Configure Window
    //Define the size of the window
    glutInitWindowSize (WINDOW_HEIGHT, WINDOW_WIDTH);
    //Define the window position on the screen
    glutInitWindowPosition (10, 10);

    //Select Pixel Format Attributes
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

    //Create the Window and Set up Rendering Context
    glutCreateWindow ("Canvas");

    //Configure Rendering Context
    init ();

    //Connect callback functions that will respond to events
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(NULL);

    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(NULL);
    glutMainLoop ();
    
     
    return 0;
}

