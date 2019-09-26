#ifndef STRINGVARIABLE_H
#define STRINGVARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<StringANSI>: public AbstractVariable
{
	public: enum: int64 
	{
		STRING_VARIABLE_CLASS = 65536
	};
	friend class AbstractVariable;
	private: StringANSI value;
	public: Variable(void);
	public: Variable(StringANSI _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: StringANSI GetValue(void)const;
	public: virtual void SetValue(StringANSI _value);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};


#endif 