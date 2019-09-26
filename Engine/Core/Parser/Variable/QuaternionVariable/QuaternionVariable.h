#ifndef QUATERNIONVARIABLE_H
#define QUATERNIONVARIABLE_H
#include <Engine/Core/Parser/Variable/Variable.h>
template<>
class Variable<Quaternion>: public AbstractVariable,
	                        public RotatableObject
{
	public: enum: int64 
	{
		QUATERNION_VARIABLE_CLASS = 8589934592
	};
	friend class AbstractVariable;
	private: Quaternion value;
	public: Variable(void);
	public: Variable(Quaternion _value);
	public: virtual ~Variable(void);
	public: virtual int32 GetValueType(void)const;
	public: Quaternion GetValue(void)const;
	public: virtual void SetValue(Quaternion _value);
	public: virtual void SetRotation(Quaternion _rotation);
	public: virtual void DisplaceRotation(Quaternion _rotation);
	public: virtual Quaternion GetRotation(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: bool operator == (const Variable& _node);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void LoadDerivedFromFile(File& _file);
};


#endif 