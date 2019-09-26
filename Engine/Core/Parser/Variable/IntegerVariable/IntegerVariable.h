#ifndef INTEGERVARIABLE_H
#define INTEGERVARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<int32>: public AbstractVariable
{
	public: enum: int64 
	{
		INTEGER_VARIABLE_CLASS = 512
	};
	public: enum
	{
		CHANGE_INTEGER_RANGE_MESSAGE = 275
	};
	friend class AbstractVariable;
	private: int32 value;
	private: int32 lowerBound;
	private: int32 upperBound;
	public: Variable(void);
	public: Variable(int32 _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: int32 GetValue(void)const;
	public: virtual void SetValue(int32 _value);
	public: void DisplaceValue(int32 _offset);
	public: void SetLowerBound(int32 _lowerBound);
	public: int32 GetLowerBound(void)const;
	public: void SetUpperBound(int32 _upperBound);
	public: int32 GetUpperBound(void)const;
	public: void SetRange(int32 _lowerBound, int32 _upperBound);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};



#endif 