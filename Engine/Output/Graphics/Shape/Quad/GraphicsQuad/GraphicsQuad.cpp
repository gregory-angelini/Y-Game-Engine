#include "GraphicsQuad.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>



GraphicsQuad::GraphicsQuad(void): wireframeEnable(false)
{}



GraphicsQuad::~GraphicsQuad(void)
{
	Destroying();
}


void GraphicsQuad::SetWireframeEnable(bool _enable)
{
	wireframeEnable = _enable;
}


bool GraphicsQuad::IsWireframeEnabled(void)const
{
	return wireframeEnable;
}



void GraphicsQuad::Draw(void)
{
	if(showEnable)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		
		glColor4fv(GetColorRGBA().GetData());

		if(wireframeEnable)
		{  
			bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
			if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }

			glBegin(GL_LINE_STRIP);
				glVertex3fv((GetTransformation() * GetVertex1()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex2()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex3()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex4()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex1()).GetData()); 
			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
				glVertex3fv((GetTransformation() * GetVertex4()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex3()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex2()).GetData()); 
				glVertex3fv((GetTransformation() * GetVertex1()).GetData()); 
			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


void GraphicsQuad::_Draw(Quad _quad, Vector3 _color, float _alpha, bool _wireframe)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
	glColor4fv(Vector4(_color, _alpha).GetData());
		
	if(_wireframe)
	{
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }

		glBegin(GL_LINE_STRIP);
			glVertex3fv(_quad.GetVertex1().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
		
		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	}
	else
	{
		glBegin(GL_POLYGON);
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

}



void GraphicsQuad::_Draw(Quad _quad, Vector4 _color, bool _wireframe)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
	glColor4fv(_color.GetData());

	if(_wireframe)
	{
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }

		glBegin(GL_LINE_STRIP);
			glVertex3fv(_quad.GetVertex1().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
		
		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	}
	else
	{
		glBegin(GL_POLYGON);
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



void GraphicsQuad::_Draw(Quad _quad, RGBA _color, bool _wireframe)
{ 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
	glColor4ubv(_color.GetData());
	
	if(_wireframe)
	{
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }

		glBegin(GL_LINE_STRIP);
			glVertex3fv(_quad.GetVertex1().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
		
		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
	}
	else
	{
		glBegin(GL_POLYGON);
			glVertex3fv(_quad.GetVertex4().GetData()); 
			glVertex3fv(_quad.GetVertex3().GetData()); 
			glVertex3fv(_quad.GetVertex2().GetData()); 
			glVertex3fv(_quad.GetVertex1().GetData()); 
		glEnd();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}
