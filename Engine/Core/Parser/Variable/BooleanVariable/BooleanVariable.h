#ifndef BOOLEANVARIABLE_H
#define BOOLEANVARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<bool>: public AbstractVariable
{
	public: enum: int64 
	{
		BOOLEAN_VARIABLE_CLASS = 256
	};
	friend class AbstractVariable;
	protected: bool value;
	public: Variable(void);
	public: Variable(bool _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: bool GetValue(void)const;
	public: virtual void SetValue(bool _value);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};


#endif 