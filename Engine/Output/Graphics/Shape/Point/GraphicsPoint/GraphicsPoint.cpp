#include "GraphicsPoint.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>



GraphicsPoint::GraphicsPoint(void)
{}



GraphicsPoint::~GraphicsPoint(void)
{
	Destroying();
}


void GraphicsPoint::Draw(void)
{
	if(showEnable)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
		glBegin(GL_POINTS);

		glColor4fv(GetColorRGBA().GetData());
		glVertex3fv(GetPosition().GetData()); 

		glEnd();

		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


void GraphicsPoint::_Draw(Vector3 _position, Vector3 _color, float _alpha)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
	glBegin(GL_POINTS);

	glColor4fv(Vector4(_color, _alpha).GetData());
	glVertex3fv(_position.GetData());

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



void GraphicsPoint::_Draw(Vector3 _position, Vector4 _color)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
	glBegin(GL_POINTS);

	glColor4fv(_color.GetData());
	glVertex3fv(_position.GetData());

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



void GraphicsPoint::_Draw(Vector3 _position, RGBA _color)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
	if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
	glBegin(GL_POINTS);

	glColor4ubv(_color.GetData());
	glVertex3fv(_position.GetData());

	glEnd();

	if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}