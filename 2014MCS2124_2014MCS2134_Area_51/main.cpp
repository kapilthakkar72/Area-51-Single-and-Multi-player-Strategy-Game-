#include "header.h"

//void *startGraphics(void *arg);

int argcG;
char** argsG;

void runMainLoop( int val );

int main( int argc, char* args[] )
{
	XInitThreads();
	myIPAddress=getMyIPAdress();
	glutInit( &argc, args );

	//Create OpenGL 2.1 context
	glutInitContextVersion( 2, 1 );

	//Create Double Buffered Window
	//glutInitDisplayMode( GLUT_DOUBLE );
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutInitWindowPosition(0, 0);  
	glutCreateWindow( "Area-51" );

	//Do post window/context creation initialization
	if( !initGL() )
	{
		printf( "Unable to initialize graphics library!\n" );
		return 1;
	}

	//Set rendering function
	glutDisplayFunc( render );

	//Set Mouse Hndler
	glutMouseFunc(onMouseClick);
	
	//Set main loop
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop( int val )
{
    //Frame logic
    update();
    render();

    //Run frame one more time
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}