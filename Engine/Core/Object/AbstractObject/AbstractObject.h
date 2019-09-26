#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
#include <Engine/Output/File/File.h>
class AbstractObject: public virtual MagicNode,
	                  public virtual MessageHandler<int32>::Receiver,
	                  public virtual MessageHandler<int32>
{
	private: enum
	{
		CHECK_OBJECT_NAME_MESSAGE = 8
	};
	public: enum
	{
		DESTROY_OBJECT_MESSAGE = 9,
		CHANGE_NAME_MESSAGE = 10,
		GET_NAME_MESSAGE = 111,
		SAVE_TO_FILE_MESSAGE = 83,
		ENABLE_PROPERTY_MESSAGE = 333,
		CHECK_PROPERTY_MESSAGE = 334
	};
	public: enum: int64
	{
		ABSTRACT_OBJECT_CLASS = 16777216
	};
	private: class ObjectTypeProperty: public MagicNode
	{
		private: int64 type;
		private: MagicBinaryTree<ObjectTypeProperty> childProperties;
		public: ObjectTypeProperty(void);
		public: ObjectTypeProperty(int64 _type);
		public: int64 GetType(void)const;
		public: bool IsChild(ObjectTypeProperty* _property);
		private: bool IsChild(MagicBinaryTree<ObjectTypeProperty>::AddressDerived* _address, ObjectTypeProperty* _property);
		public: bool AddChildProperty(ObjectTypeProperty* _property);
	};
	private: class ObjectTypeProperties: public MagicBinaryTree<ObjectTypeProperty>
	{
		public: ObjectTypeProperties(void);
	};
	private: class RenameInfo
	{
	    public: StringANSI newName;
		public: bool reserved;
		public: RenameInfo(StringANSI _newName, bool _reserved): newName(_newName), reserved(_reserved)
		{}
	};
	public: class PropertyInfo
	{
	    public: int32 property;
		public: bool enable;
		public: PropertyInfo(int32 _property, bool _enable): property(_property), enable(_enable)
		{}
	};
	public: class NameInfo
	{
	    public: StringANSI oldName;
		public: StringANSI newName;
		public: AbstractObject* object;
		public: NameInfo(StringANSI _oldName, StringANSI _newName, AbstractObject* _object): oldName(_oldName), newName(_newName), object(_object)
		{}
	};
	public: class ObjectType
	{
		private: int64 properties;
		public: ObjectType(void);
		public: ObjectType(int64 _properties);
		public: int64 GetProperties(void)const;
		public: StringANSI ToString(void)const; 
	};
	friend class ObjectBinaryTree;
	friend class RenderList;
	friend class UpdateList;
	friend class ObjectTree;
	friend class DestroyObjectAction;
	friend class Plane;
    private: StringANSI name;
	private: bool renaming;
    private: int64 objectType;
	protected: int64 properties;
	private: static StringANSI logFileSuffix;
	private: static StringANSI logFileDir;
	
    private: static StringANSI objectsSuffix;
    private: static StringANSI objectsDir;
	
    private: static ObjectTypeProperties supportedProperties;
	public: AbstractObject(void);
	public: virtual ~AbstractObject(void);
	public: StringANSI GetName(void)const;
    public: virtual bool Rename(StringANSI _newName);
	protected: void AddClassProperty(int64 _property);
	public: int64 GetClassProperties(void);
	public: bool IsClassPropertyExist(int64 _property)const;
	public: StringANSI GetClassPropertiesString(void)const;
	public: void EnableProperty(int32 _property, bool _enable);
	public: bool IsPropertyExist(int32 _property)const;
	public: int64 GetProperties(void);
	public: void SetProperties(int64 _properties);
	public: virtual void Destroy(void);
	protected: virtual void Destroying(void);
	public: StringANSI GetLogFile(void);
	public: static StringANSI _GetLogFile(StringANSI _name);
	public: void RemoveLogFile(void);
	public: static void _RemoveLogFile(StringANSI _name);
	public: int32 WriteToLogFile(StringANSI _string, ...);
	public: static int32 _WriteToLogFile(StringANSI _fileName, StringANSI _string, ...);
	public: static void _SetLogFileDir(StringANSI _directory);
	public: static StringANSI _GetLogFileDir(void);
	public: static void _SetLogFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetLogFileSuffix(void);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static AbstractObject* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static AbstractObject* _LoadFromFile(File& _file);
	public: static bool _IsChild(int64 _parent, int64 _child);
	public: static bool _IsCompatible(int64 _property1, int64 _property2);
	public: static bool _IsSupported(int64 _properties);
	public: static ObjectType _CreateObjectType(int64 _properties);
	private: static bool _ToList(int64 _type, MagicList<ObjectTypeProperty>& _properties);
	private: static void _RemoveLowPriorityProperties(MagicList<ObjectTypeProperty>& _properties);
	private: static bool _IsLowPriority(ObjectTypeProperty* _property, MagicList<ObjectTypeProperty>& _properties);
	public: static AbstractObject* _CreateObject(ObjectType _objectType); 
};


#endif 