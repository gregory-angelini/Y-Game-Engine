#include "GraphicsCircle.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>



GraphicsCircle::GraphicsCircle(void): segments(3), wireframeEnable(false)
{}



GraphicsCircle::~GraphicsCircle(void)
{
	Destroying();
}


void GraphicsCircle::SetWireframeEnable(bool _enable)
{
	wireframeEnable = _enable;
}


bool GraphicsCircle::IsWireframeEnabled(void)const
{
	return wireframeEnable;
}


void GraphicsCircle::Draw(void)
{
	if(showEnable && radius > 0.0f && segments > 2)
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

			Vector3 vertex;
			for(int32 i = 0; i < segments + 1; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / segments;
				vertex.x = center.x + cosf(angle) * radius;
				vertex.y = center.y + sinf(angle) * radius;
				vertex.z = center.z;

				glVertex3fv((GetTransformation() * vertex).GetData()); 
			}

			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);
			
			Vector3 vertex;
			for(int32 i = 0; i < segments + 1; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / segments;
				vertex.x = center.x + cosf(angle) * radius;
				vertex.y = center.y + sinf(angle) * radius;
				vertex.z = center.z;

				glVertex3fv((GetTransformation() * vertex).GetData()); 
			}
			
			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


void GraphicsCircle::_Draw(Vector3 _position, float _radius, int32 _segments, Vector3 _color, float _alpha, bool _wireframe)
{
	if(_radius > 0.0f && _segments > 2)
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

			Vector3 vertex;
			for(int32 i = 0; i < _segments + 1; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);

			Vector3 vertex;
			for(int32 i = 0; i < _segments; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsCircle::_Draw(Vector3 _position, float _radius, int32 _segments, Vector4 _color, bool _wireframe)
{
	if(_radius > 0.0f && _segments > 2)
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

			Vector3 vertex;
			for(int32 i = 0; i < _segments + 1; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);

			Vector3 vertex;
			for(int32 i = 0; i < _segments; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsCircle::_Draw(Vector3 _position, float _radius, int32 _segments, RGBA _color, bool _wireframe)
{
	if(_radius > 0.0f && _segments > 2)
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

			Vector3 vertex;
			for(int32 i = 0; i < _segments + 1; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();

			if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		}
		else
		{
			glBegin(GL_POLYGON);

			Vector3 vertex;
			for(int32 i = 0; i < _segments; ++i)
			{
				float angle = Angle::DOUBLE_PI * (float)i / _segments;
				vertex.x = _position.x + cosf(angle) * _radius;
				vertex.y = _position.y + sinf(angle) * _radius;
				vertex.z = _position.z;

				glVertex3fv(vertex.GetData()); 
			}

			glEnd();
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}
