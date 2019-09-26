#ifndef VECTOR2VARIABLE_H
#define VECTOR2VARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<Vector2>: public AbstractVariable
{
	public: enum: int64 
	{
		VECTOR_2_VARIABLE_CLASS = 17179869184
	};
	public: enum
	{
		GET_VECTOR_2_COMPONENT_MESSAGE = 262,
		CHANGE_VECTOR_2_COMPONENT_MESSAGE = 263,
		GET_NORMALIZED_VECTOR_2_MESSAGE = 265,
		GET_VECTOR_2_ANGLE_MESSAGE = 273,
		GET_VECTOR_2_LENGTH_MESSAGE = 272,
		VECTOR_2_TO_SPHERICAL_MESSAGE = 312
	};
	friend class AbstractVariable;
	private: Vector2 value;
	public: Variable(void);
	public: Variable(Vector2 _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: Vector2 GetValue(void)const;
	public: virtual void SetValue(Vector2 _value);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};



#endif 