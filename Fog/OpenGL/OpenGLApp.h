#ifndef _OPENGLAPP_
#define _OPENGLAPP_

#define PI 3.14159
#define TWO_PI PI*2.0
#define HALF_PI PI/2.0

//extension function
typedef void (APIENTRY *PFNGLFOGCOORDFPROC) (GLfloat);

//constants
#define GL_FOG_COORD_SRC    0x8450
#define GL_FOG_COORD        0x8451
#define GL_FRAGMENT_DEPTH   0x8452

const int TERRAIN_SIZE = 65;
class OpenGLApp
{
private:
	PFNGLFOGCOORDFPROC myglFogCoordf;
	GLubyte heightmap[TERRAIN_SIZE * TERRAIN_SIZE];
	int m_windowWidth;
	int m_windowHeight;
	float m_angle;


public:
	OpenGLApp();
	~OpenGLApp();

	bool Initialize(); // initialize your opengl scene
	bool Shutdown();  // shut down your scene

	// used to setup your opengl camera and drawing area
	void SetupProjection(int width, int height);

	void Prepare(float dt);
	void Render();

	void DrawTerrain();
	void DrawLines();


};

#endif