
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdio> //will use for reading heightmap file
#include "OpenGLApp.h"


const char heightmapFilename[] = "heightmap.raw";
const float MAX_HEIGHT = 10.0f;
const float MAX_FOG_HEIGHT = MAX_HEIGHT * 0.5f;
const float SCALE_FACTOR = 256.0f / MAX_HEIGHT;
const float WATER_HEIGHT = 0.8f;


OpenGLApp::OpenGLApp()
{
	myglFogCoordf = NULL;
}


OpenGLApp::~OpenGLApp()
{
}


bool OpenGLApp::Initialize() // initialize your opengl scene
{
	//myglFogCoordf is the function name we are going to use for the glFogCoordExt function
	myglFogCoordf = (PFNGLFOGCOORDFPROC)wglGetProcAddress("glFogCoordfEXT");
	//the following code is for loading in the heightmap color
	//values into the heightmap array

	FILE *pFile = fopen(heightmapFilename, "rb");

	if (!pFile)
		return false;

	//read the file all at once, load it into the array
	fread(&heightmap, //where in memory to store the data
		TERRAIN_SIZE*TERRAIN_SIZE, // HOW MUCH DATA, IN BYTES	
		1, //how many times to read? 1 because all at once
		pFile); //which file to read from
	fclose(pFile); //close the file, bra

	//specify how wide lines should be drawn
	glLineWidth(3.0f);

	glEnable(GL_DEPTH_TEST);
	return true;
} 

bool OpenGLApp::Shutdown() // shut down your scene
{
	return true;
}  

// used to setup your opengl camera and drawing area
void OpenGLApp::SetupProjection(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// you need 3 things to create a frustum
	// 1. Field of view (52.0f here)
	// 2. aspect ratio of screen (width/height of window)
	// 3. near and far plane (1, and 1000)
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_windowWidth = width;
	m_windowHeight = height;
}

void OpenGLApp::Prepare(float dt)
{

}

void OpenGLApp::Render()
{
	glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set up the view
	glLoadIdentity();
	gluLookAt(TERRAIN_SIZE / 3.0f, MAX_HEIGHT + 1.0f, TERRAIN_SIZE / 3.0f,
				0.0f, 0.0f, 0.0f, 
				0.0f, 1.0f, 0.0f);
	//use gray for all fog
	GLfloat fogColorGray[] = { 0.5f, 0.5f, 0.5f };
	glFogfv(GL_FOG_COLOR, fogColorGray);


	//TOP 1
	glViewport(0, m_windowHeight / 2, 
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//TOP 2
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_START, 10.0f);
	glFogi(GL_FOG_END, 60.0f);
	//change the viewport to different location (upper right), then draw terrain
	glViewport(m_windowWidth / 4, m_windowHeight / 2,
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();
	

	//TOP 3
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogi(GL_FOG_START, 10.0f);
	glFogi(GL_FOG_END, 60.0f);
	//change the viewport to different location (upper right), then draw terrain
	glViewport(m_windowWidth / 2, m_windowHeight / 2,
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//TOP 4
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogi(GL_FOG_START, 10.0f);
	glFogi(GL_FOG_END, 60.0f);
	//change the viewport to different location (upper right), then draw terrain
	glViewport((m_windowWidth / 4) * 3, m_windowHeight / 2,
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//BOTTOM 1
	GLfloat fogColorBlue[] = { 0.0f, 0.0f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColorBlue);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_START, 15.0f);
	glFogi(GL_FOG_END, 70.0f);
	glViewport(0, 0, 
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//BOTTOM 2
	GLfloat fogColorOrange[] = { 1.0f, 0.5f, 0.0f };
	glFogfv(GL_FOG_COLOR, fogColorOrange);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.9f);
	glViewport(m_windowWidth / 4, 0, 
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//BOTTOM 3
	GLfloat fogColorPurple[] = { 1.0f, 0.0f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColorPurple);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.02f);
	glFogi(GL_FOG_START, 5.0f);
	glFogi(GL_FOG_END, 40.0f);
	glViewport(m_windowWidth / 2, 0,
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//BOTTOM 4
	GLfloat fogColorRed[] = { 1.0f, 0.0f, 0.0f };
	glFogfv(GL_FOG_COLOR, fogColorRed);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.04f);
	glFogi(GL_FOG_START, 30.0f);
	glFogi(GL_FOG_END, 60.0f);
	glViewport((m_windowWidth / 4) * 3, 0,
			   m_windowWidth / 4, m_windowHeight / 2);
	DrawTerrain();


	//before we leave render, switch to defaults
	//glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
	glDisable(GL_FOG);

	DrawLines();
}

float ComputeFogCoord(float height)
{
	//cap height min/max based on max_fog_height and water_height
	if (height > MAX_FOG_HEIGHT)
		height = MAX_FOG_HEIGHT;
	else if (height < WATER_HEIGHT)
		height = WATER_HEIGHT;

	//height is a value between max fog height and water height
	//scale it, so it will be a value between 0 and 1. The fog coord will be 0 at max and 1 at water

	height = height - WATER_HEIGHT;
	height = 1.0f - (height / (MAX_FOG_HEIGHT - WATER_HEIGHT));
	
	return height;
}

void OpenGLApp::DrawTerrain()
{
	for (int z = 0; z < TERRAIN_SIZE - 1; ++z)
	{
		// draw the terrain as a set of triangle strips
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < TERRAIN_SIZE; ++x)
		{
			float scaledHeight = heightmap[z * TERRAIN_SIZE + x] / SCALE_FACTOR;
			float nextScaledHeight = heightmap[(z + 1)* TERRAIN_SIZE + x] / SCALE_FACTOR;

			if (myglFogCoordf)
				myglFogCoordf(ComputeFogCoord(scaledHeight));
			glColor3f(0.1f, 0.5f + 0.5f * scaledHeight / MAX_HEIGHT, 0.1f);
			glVertex3f(static_cast<GLfloat>(x - TERRAIN_SIZE / 2), scaledHeight, static_cast<GLfloat>(z - TERRAIN_SIZE / 2));

			if (myglFogCoordf)
				myglFogCoordf(ComputeFogCoord(nextScaledHeight));
			glColor3f(0.1f, 0.5f + 0.5f * nextScaledHeight / MAX_HEIGHT, 0.1f);
			glVertex3f(static_cast<GLfloat>(x - TERRAIN_SIZE / 2), nextScaledHeight, static_cast<GLfloat>(z + 1 - TERRAIN_SIZE / 2));

		}
		glEnd();
	}

	// draw the water plane
	glColor3f(0.0, 0.0, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(-TERRAIN_SIZE / 2.1f, WATER_HEIGHT, TERRAIN_SIZE / 2.1f);
	glVertex3f(TERRAIN_SIZE / 2.1f, WATER_HEIGHT, TERRAIN_SIZE / 2.1f);
	glVertex3f(TERRAIN_SIZE / 2.1f, WATER_HEIGHT, -TERRAIN_SIZE / 2.1f);
	glVertex3f(-TERRAIN_SIZE / 2.1f, WATER_HEIGHT, -TERRAIN_SIZE / 2.1f);
	glEnd();
}

//draw the lines that separate the viewports
void OpenGLApp::DrawLines()
{
	glDisable(GL_DEPTH_TEST);

	//switch from perspective to ortho (2D) to draw the lines
	glMatrixMode(GL_PROJECTION); //store the perspective
	glPushMatrix();	 //push projection matrix
	glLoadIdentity();
	gluOrtho2D(0.0, 2.0, 0.0, 2.0);
	glViewport(0, 0, m_windowWidth / 2, m_windowHeight);
	//draw the lines
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//push model view matrix
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2i(2, 1); //horizontal line
		glVertex2i(0, 1);
		glVertex2i(1, 2); //vertical line
		glVertex2i(1, 0);
	glEnd();
	glPopMatrix(); //pop model view matrix


	//switch from perspective to ortho (2D) to draw the lines
	glMatrixMode(GL_PROJECTION); //store the perspective
	glPushMatrix();	 //push projection matrix
	glLoadIdentity();
	gluOrtho2D(0.0, 2.0, 0.0, 2.0);
	glViewport(m_windowWidth / 2, 0, m_windowWidth / 2, m_windowHeight);
	//draw the lines
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//push model view matrix
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2i(2, 1); //horizontal line
	glVertex2i(0, 1);
	glVertex2i(1, 2); //vertical line
	glVertex2i(1, 0);
	glEnd();
	glPopMatrix(); //pop model view matrix


	//switch from perspective to ortho (2D) to draw the lines
	glMatrixMode(GL_PROJECTION); //store the perspective
	glPushMatrix();	 //push projection matrix
	glLoadIdentity();
	gluOrtho2D(0.0, 2.0, 0.0, 2.0);
	glViewport(m_windowWidth / 4, 0, m_windowWidth / 2, m_windowHeight);
	//draw the lines
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//push model view matrix
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2i(2, 1); //horizontal line
	glVertex2i(0, 1);
	glVertex2i(1, 2); //vertical line
	glVertex2i(1, 0);
	glEnd();
	glPopMatrix(); //pop model view matrix


	glMatrixMode(GL_PROJECTION); //back to projection mode
	glPopMatrix(); //pop projection matrix

	glMatrixMode(GL_MODELVIEW); //go back to model view before we leave this function
	glEnable(GL_DEPTH_TEST);
}