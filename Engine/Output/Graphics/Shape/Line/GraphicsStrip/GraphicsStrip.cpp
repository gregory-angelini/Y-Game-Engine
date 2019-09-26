#include "GraphicsStrip.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



GraphicsStrip::GraphicsStrip(void)
{
	AddClassProperty(GRAPHICS_STRIP_CLASS);
}



GraphicsStrip::~GraphicsStrip(void)
{
	Destroying();
}



void GraphicsStrip::Draw(void)
{
	if(showEnable && vertices.GetSize() >= 2)
	{ 
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
		glBegin(GL_LINE_STRIP);

		glColor4fv(GetColorRGBA().GetData());

		for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			glVertex3fv((GetTransformation() * it->key).GetData()); 
		} 

		glEnd();
		
		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


void GraphicsStrip::_Draw(SimpleList<Vector3>& _vertices, Vector3 _color, float _alpha)
{
	if(_vertices.GetSize() >= 2)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
		glBegin(GL_LINE_STRIP);

		glColor4fv(Vector4(_color, _alpha).GetData());

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			glVertex3fv(it->key.GetData()); 
		}

		glEnd();

		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}




void GraphicsStrip::_Draw(SimpleList<Vector3>& _vertices, Vector4 _color)
{
	if(_vertices.GetSize() >= 2)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
		glBegin(GL_LINE_STRIP);

		glColor4fv(_color.GetData());

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			glVertex3fv(it->key.GetData()); 
		}

		glEnd();

		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void GraphicsStrip::_Draw(SimpleList<Vector3>& _vertices, RGBA _color)
{
	if(_vertices.GetSize() >= 2)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		bool zBufferEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE ? true : false;
		if(zBufferEnable) { glDisable(GL_DEPTH_TEST); }
	 
		glBegin(GL_LINE_STRIP);

		glColor4ubv(_color.GetData());

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			glVertex3fv(it->key.GetData()); 
		}

		glEnd();

		if(zBufferEnable) { glEnable(GL_DEPTH_TEST); }
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}


SimpleList<Vector3>& GraphicsStrip::GetVertices(void)
{
	return vertices;
}


void GraphicsStrip::Destroy(void)
{
	vertices.Clear();
}


bool GraphicsStrip::SaveToFile(File& _file)
{
	if(AbstractObject::SaveToFile(_file))
	{ 
		GraphicsObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



bool GraphicsStrip::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool GraphicsStrip::SaveAsToFile(File& _file, StringANSI _name)
{
	if(AbstractObject::SaveAsToFile(_file, _name))
	{ 
		GraphicsObject::SaveDerivedToFile(_file);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void GraphicsStrip::SaveDerivedToFile(File& _file)
{
	_file.Write(vertices.IsNotEmpty());

	if(vertices.IsNotEmpty())
	{
		_file.Write(vertices.GetSize());
		for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
	} 
}



bool GraphicsStrip::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		GraphicsObject::LoadDerivedFromFile(_file);

		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}



void GraphicsStrip::LoadDerivedFromFile(File& _file)
{ 
	Destroy();
	bool verticesIsExist;
	_file.Read(verticesIsExist);

	if(verticesIsExist)
	{
		int32 amountOfVertices;
		_file.Read(amountOfVertices);

		Vector3 vertex;
		for(int32 i = 0; i < amountOfVertices; i++)
		{
			_file.Read(vertex);
			vertices.AddToTail(vertex);
		}
	}
}