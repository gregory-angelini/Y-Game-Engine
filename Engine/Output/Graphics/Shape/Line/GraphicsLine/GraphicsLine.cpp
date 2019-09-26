#include "GraphicsLine.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>


GraphicsLine::GraphicsLine(void)
{}



GraphicsLine::~GraphicsLine(void)
{
	Destroying();
}



void GraphicsLine::Draw(void)
{
	if(showEnable)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
 
		glBegin(GL_LINES);

		glColor4fv(GetColorRGBA().GetData());

		glVertex3fv((GetTransformation() * origin).GetData()); 
		glVertex3fv((GetTransformation() * end).GetData()); 

		glEnd();
		
		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsLine::_Draw(Vector3 _origin, Vector3 _end, Vector3 _color, float _alpha)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
 
	glBegin(GL_LINES);

	glColor4fv(Vector4(_color, _alpha).GetData());

	glVertex3fv(_origin.GetData()); 
	glVertex3fv(_end.GetData()); 

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



void GraphicsLine::_Draw(Vector3 _origin, Vector3 _end, RGBA _color)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
 
	glBegin(GL_LINES);

	glColor4ubv(_color.GetData());

	glVertex3fv(_origin.GetData()); 
	glVertex3fv(_end.GetData()); 

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



void GraphicsLine::_Draw(Vector3 _origin, Vector3 _end, Vector4 _color)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
 
	glBegin(GL_LINES);

	glColor4fv(_color.GetData());

	glVertex3fv(_origin.GetData()); 
	glVertex3fv(_end.GetData()); 

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}
