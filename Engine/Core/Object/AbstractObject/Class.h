#ifndef CLASS_H
#define CLASS_H
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
template<typename A>
class Class1: public A
{
	public: virtual ~Class1(void)
	{
		Destroying();
	}
};
template<typename A, typename B>
class Class2: public A,
			  public B
{
	public: virtual ~Class2(void)
	{
		Destroying();
	}
};
template<>
class Class2<GraphicsSurface, Timer>: public GraphicsSurface,
			                          public Timer 
{
	public: virtual ~Class2(void)
	{
		Destroying();
	}
	public: virtual bool SaveAsToFile(StringANSI _name)
	{
		File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

		if(!SaveAsToFile(file, _name))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name)
	{
		if(GraphicsSurface::SaveAsToFile(_file, _name))
		{
			Timer::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		if(_path.empty())
		{
			_path = AssetLibrary::_GetDir() + AbstractObject::_GetDir() + GetName() + "." + AbstractObject::_GetFileSuffix();
		} 

		File file(_path, File::REWRITE);

		if(!SaveToFile(file))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveToFile(File& _file)
	{
		if(GraphicsSurface::SaveToFile(_file))
		{
			Timer::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
	protected: virtual bool LoadFromFile(File& _file)
	{
		if(GraphicsSurface::LoadFromFile(_file))
		{
			Timer::LoadDerivedFromFile(_file);
			return true;
		} 
		return false;
	}
};
template<>
class Class2<TexturingSurface, Timer>: public TexturingSurface,
			                           public Timer
{
	public: virtual ~Class2(void)
	{
		Destroying();
	}
	public: virtual bool SaveAsToFile(StringANSI _name)
	{
		File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

		if(!SaveAsToFile(file, _name))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name)
	{
		if(TexturingSurface::SaveToFile(_file))
		{
			Timer::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		if(_path.empty())
		{
			_path = AssetLibrary::_GetDir() + AbstractObject::_GetDir() + GetName() + "." + AbstractObject::_GetFileSuffix();
		} 

		File file(_path, File::REWRITE);

		if(!SaveToFile(file))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveToFile(File& _file)
	{
		if(TexturingSurface::SaveToFile(_file))
		{
			Timer::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
	protected: virtual bool LoadFromFile(File& _file)
	{
		if(TexturingSurface::LoadFromFile(_file))
		{
			Timer::LoadDerivedFromFile(_file);
			return true;
		} 
		return false;
	}
};
template<>
class Class2<GraphicsSurface, Scene>: public GraphicsSurface,
			                          public Scene
{
	public: virtual ~Class2(void)
	{
		Destroying();
	}
	protected: virtual void ResetTransformationValid(void)
	{
		ModelView::ResetTransformationValid();
		for(MagicList<PolygonalSurface>::AddressDerived* it = childrens.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			it->GetNode()->ResetTransformationValid();
		}

		UpdateTransformation(renderList.GetHeadAddress());
	}
	public: virtual void Destroy(void)
	{
		GraphicsSurface::Destroy();
		Scene::Destroy();
	}
	public: virtual void Draw(void)
	{
		GraphicsSurface::Draw();
		Scene::Draw();
	}
	public: virtual bool SaveAsToFile(StringANSI _name)
	{
		File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

		if(!SaveAsToFile(file, _name))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name)
	{
		if(GraphicsSurface::SaveAsToFile(_file, _name))
		{ 
			Scene::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		if(_path.empty())
		{
			_path = AssetLibrary::_GetDir() + AbstractObject::_GetDir() + GetName() + "." + AbstractObject::_GetFileSuffix();
		} 

		File file(_path, File::REWRITE);

		if(!SaveToFile(file))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveToFile(File& _file)
	{
		if(GraphicsSurface::SaveToFile(_file))
		{
			Scene::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
	protected: virtual bool LoadFromFile(File& _file)
	{
		if(GraphicsSurface::LoadFromFile(_file))
		{
			Scene::LoadDerivedFromFile(_file);
			return true;
		}
		return false;
	}
};
template<>
class Class2<TexturingSurface, Scene>: public TexturingSurface,
			                           public Scene
{
	public: virtual ~Class2(void)
	{
		Destroying();
	}
	protected: virtual void ResetTransformationValid(void)
	{
		ModelView::ResetTransformationValid();
		for(MagicList<PolygonalSurface>::AddressDerived* it = childrens.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			it->GetNode()->ResetTransformationValid();
		}

		UpdateTransformation(renderList.GetHeadAddress());
	}
	public: virtual void Destroy(void)
	{
		TexturingSurface::Destroy();
		Scene::Destroy();
	}
	public: virtual void Draw(void)
	{
		TexturingSurface::Draw();
		Scene::Draw();
	}
	public: virtual bool SaveAsToFile(StringANSI _name)
	{
		File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

		if(!SaveAsToFile(file, _name))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name)
	{
		if(TexturingSurface::SaveAsToFile(_file, _name))
		{ 
			Scene::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		if(_path.empty())
		{
			_path = AssetLibrary::_GetDir() + AbstractObject::_GetDir() + GetName() + "." + AbstractObject::_GetFileSuffix();
		} 

		File file(_path, File::REWRITE);

		if(!SaveToFile(file))
		{
			file.Remove();
			return false;
		}
		return true; 
	}
	public: virtual bool SaveToFile(File& _file)
	{
		if(TexturingSurface::SaveToFile(_file))
		{
			Scene::SaveDerivedToFile(_file);
			return true;
		}
		return false;
	}
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
	protected: virtual bool LoadFromFile(File& _file)
	{
		if(TexturingSurface::LoadFromFile(_file))
		{
			Scene::LoadDerivedFromFile(_file);
			return true;
		}
		return false;
	}
};


#endif 