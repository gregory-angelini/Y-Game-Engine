#ifndef LOGICSCENE_H
#define LOGICSCENE_H
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Scene/LogicScene/UpdateList/UpdateList.h>
class LogicScene: public virtual ObjectScene,
	              public virtual LogicObject
{
	public: enum
	{
		ADD_TO_UPDATE_LIST_MESSAGE = 16,
		REMOVE_FROM_UPDATE_LIST_MESSAGE = 76
	};
	public: enum: int64
	{
		LOGIC_SCENE_CLASS = 64
	};
	friend class LogicObject;
	friend class UpdateList;
	friend class Scene;
	private: UpdateList updateList;
	public: LogicScene(void);
	public: virtual ~LogicScene(void);
    public: virtual void SetDeltaScale(float _scale);
	private: void UpdateDeltaScale(void);
	private: void UpdateDeltaScale(MagicList<LogicObject>::AddressDerived* _address)const;
	public: virtual UpdateList::AddressDerived* AddToUpdateList(LogicObject* _object);
	public: virtual void Destroy(void);
	public: virtual void RemoveFromUpdateList(LogicObject* _object);
	public: virtual void Update(float _delta = 0.0f);
	public: int32 GetAmountOfLogicObjects(void)const;
	private: void GetAmountOfLogicObjects(MagicList<LogicObject>::AddressDerived* _address, int32& _size)const;
	public: UpdateList& GetUpdateList(void);
	public: void TotalPolygonsDivide4(void);
	private: void TotalPolygonsDivide4(MagicBinaryTree<AbstractObject>::AddressDerived* _address);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: void SaveLogicObjectToFile(MagicList<LogicObject>::AddressDerived* _address, File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
	protected: void LoadLogicObjectFromFile(File& _file);
};


#endif 