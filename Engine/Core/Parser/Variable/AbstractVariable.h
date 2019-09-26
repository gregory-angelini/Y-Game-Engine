#ifndef ABSTRACTVARIABLE_H
#define ABSTRACTVARIABLE_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Quaternion/Quaternion.h>
class AbstractVariable: public virtual AbstractObject
{
	public: enum: int64 
	{
		ABSTRACT_VARIABLE_CLASS = 2
	};
	public: enum
	{
		INTEGER_TYPE = 1,
		FLOAT_TYPE = 2,
		BOOLEAN_TYPE = 3,
		STRING_TYPE = 4,
		VECTOR_3_TYPE = 5,
		QUATERNION_TYPE = 6,
		VECTOR_2_TYPE = 7
	};
	public: enum
	{
			CREATE_BOOLEAN_VARIABLE_MESSAGE = 63,
			CREATE_INTEGER_VARIABLE_MESSAGE = 48,
			CREATE_FLOAT_VARIABLE_MESSAGE = 158,
			CREATE_STRING_VARIABLE_MESSAGE = 159,
			CREATE_VECTOR_2_VARIABLE_MESSAGE = 160,
			CREATE_VECTOR_3_VARIABLE_MESSAGE = 161,
			CREATE_QUATERNION_VARIABLE_MESSAGE = 162,
		CHANGE_VALUE_MESSAGE = 165,
			CHANGE_INTEGER_VALUE_MESSAGE = 128,
			CHANGE_FLOAT_VALUE_MESSAGE = 122,
			CHANGE_BOOLEAN_VALUE_MESSAGE = 123,
			CHANGE_STRING_VALUE_MESSAGE = 124,
			CHANGE_VECTOR_2_VALUE_MESSAGE = 129,
			CHANGE_VECTOR_3_VALUE_MESSAGE = 130,
			CHANGE_QUATERNION_VALUE_MESSAGE = 131,
		CHECK_EQUALITY_MESSAGE = 253
	};
	template<typename Type> friend class Variable;
	private: static StringANSI variablesSuffix;
    private: static StringANSI variablesDir;
	public: AbstractVariable(void);
	public: virtual ~AbstractVariable(void);
	public: virtual int32 GetValueType(void)const = 0;
	public: template<typename Type>
	static int32 _GetValueType(void)
	{
		return 0;
	}
	public: template<>
	static int32 _GetValueType<int32>(void)
	{
		return INTEGER_TYPE;
	}
	public: template<>
	static int32 _GetValueType<float>(void)
	{
		return FLOAT_TYPE;
	}
	public: template<>
	static int32 _GetValueType<bool>(void)
	{
		return BOOLEAN_TYPE;
	}
	public: template<>
	static int32 _GetValueType<StringANSI>(void)
	{
		return STRING_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Vector3>(void)
	{
		return VECTOR_3_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Quaternion>(void)
	{
		return QUATERNION_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Vector2>(void)
	{
		return VECTOR_2_TYPE;
	}
    public: virtual bool Rename(StringANSI _name);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
    public: static AbstractVariable* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static AbstractVariable* _LoadFromFile(File& _file);
};





#endif 