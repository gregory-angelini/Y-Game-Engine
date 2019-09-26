#include "GraphicsPolygon.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>



GraphicsPolygon::GraphicsPolygon(void): wireframeEnable(false)
{}



GraphicsPolygon::~GraphicsPolygon(void)
{
	Destroying();
}


void GraphicsPolygon::SetWireframeEnable(bool _enable)
{
	wireframeEnable = _enable;
}


bool GraphicsPolygon::IsWireframeEnabled(void)const
{
	return wireframeEnable;
}



void GraphicsPolygon::Draw(void)
{
	if(showEnable && vertices.GetSize() > 2)
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
			for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv((GetTransformation() * it->key).GetData()); 
			}

			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
			for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


void GraphicsPolygon::_Draw(SimpleList<Vector3>& _vertices, Vector3 _color, float _alpha, bool _wireframe)
{
	if(_vertices.GetSize() > 2)
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
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
			
			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsPolygon::_Draw(SimpleList<Vector3>& _vertices, Vector4 _color, bool _wireframe)
{
	if(_vertices.GetSize() > 2)
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
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
			
			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsPolygon::_Draw(SimpleList<Vector3>& _vertices, RGBA _color, bool _wireframe)
{ 
	if(_vertices.GetSize() > 2)
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
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
			
			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				glVertex3fv(it->key.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}
