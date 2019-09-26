#ifndef FLOATVARIABLE_H
#define FLOATVARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<float>: public AbstractVariable
{
	public: enum: int64 
	{
		FLOAT_VARIABLE_CLASS = 33554432
	};
	public: enum
	{
		CHANGE_FLOAT_RANGE_MESSAGE = 274
	};
	friend class AbstractVariable;
	private: float value;
	private: float lowerBound;
	private: float upperBound;
	public: Variable(void);
	public: Variable(float _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: float GetValue(void)const;
	public: virtual void SetValue(float _value);
	public: void DisplaceValue(float _offset);
	public: void SetLowerBound(float _lowerBound);
	public: float GetLowerBound(void)const;
	public: void SetUpperBound(float _upperBound);
	public: float GetUpperBound(void)const;
	public: void SetRange(float _lowerBound, float _upperBound);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};
 


#endif 