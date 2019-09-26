#ifndef SCENE_H
#define SCENE_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Scene/LogicScene/LogicScene.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
class Scene: public GraphicsScene,
	         public LogicScene
{
	public: enum: int64
	{
		SCENE_CLASS = 1048576
	};
	public: enum
	{
		PICK_SCENE_MESSAGE = 44
	};
    private: static Scene* pickedScene;
	public: Scene(void);
	public: virtual ~Scene(void);
	public: void Pick(void);
	public: bool IsPicked(void)const;
	public: static void _Unpick(void);
    public: static Scene* _GetPicked(void);
	public: virtual void Destroy(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: virtual bool LoadFromFile(File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: void LoadDerivedFromFile(File& _file);
};




#endif 