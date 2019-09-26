#ifndef CALLER_H
#define CALLER_H
#include <Engine/Core/Numerical/Numerical.h>
#include <stdlib.h>
#include <string.h>
#include <Engine/Core/Memory/Memory.h>
template<typename R = void>
class Caller
{
	template<typename R = void,
			 typename T1 = void,
			 typename T2 = void,
			 typename T3 = void>
	class CallerBase
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual R operator () (T1 _arg1, T2 _arg2, T3 _arg3) = 0;
	};


	template<typename R,
			 typename T1,
			 typename T2>
	class CallerBase<R, T1, T2, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual R operator () (T1 _arg1, T2 _arg2) = 0;
	};


	template<typename R,
			 typename T1>
	class CallerBase<R, T1, void, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual R operator () (T1 _arg) = 0;
	};
	template<typename R>
	class CallerBase<R, void, void, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual R operator () (void) = 0;
	};
	class FakeClass
	{};
	template<typename C1,
			 typename C2,
			 typename R,
			 typename T1,
			 typename T2,
			 typename T3>
	class CallerDerivedM3: public CallerBase<R, T1, T2, T3>
	{
		private: C1* object;
		private: R(C2::*func)(T1, T2, T3);
		public: virtual ~CallerDerivedM3(void)
		{}
		public: CallerDerivedM3(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM3(C1* _object, R(C2::*_func)(T1, T2, T3)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM3(const CallerDerivedM3& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, R(C2::*_func)(T1, T2, T3))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM3& operator = (const CallerDerivedM3& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg1, T2 _arg2, T3 _arg3)
		{
			if(object && func)
			{
				return (static_cast<C2*>(object)->*func)(_arg1, _arg2, _arg3);
			}
		}
	};
	template<typename C1,
			 typename C2,
			 typename R,
			 typename T1,
			 typename T2>
	class CallerDerivedM2: public CallerBase<R, T1, T2>
	{
		private: C1* object;
		private: R(C2::*func)(T1, T2);
		public: virtual ~CallerDerivedM2(void)
		{}
		public: CallerDerivedM2(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM2(C1* _object, R(C2::*_func)(T1, T2)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM2(const CallerDerivedM2& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, R(C2::*_func)(T1, T2))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM2& operator = (const CallerDerivedM2& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg1, T2 _arg2)
		{
			if(object && func)
			{
				return (static_cast<C2*>(object)->*func)(_arg1, _arg2);
			}
		}
	};
	template<typename C1,
			 typename C2,
			 typename R,
			 typename T1>
	class CallerDerivedM1: public CallerBase<R, T1>
	{
		private: C1* object;
		private: R(C2::*func)(T1);
		public: virtual ~CallerDerivedM1(void)
		{}
		public: CallerDerivedM1(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM1(C1* _object, R(C2::*_func)(T1)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM1(const CallerDerivedM1& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, R(C2::*_func)(T1))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM1& operator = (const CallerDerivedM1& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg)
		{
			if(object && func)
			{
				return (static_cast<C2*>(object)->*func)(_arg);
			}
		}
	};
	template<typename C1,
			 typename C2,
			 typename R>
	class CallerDerivedM0: public CallerBase<R> 
	{
		private: C1* object;
		private: R(C2::*func)(void);
		public: virtual ~CallerDerivedM0(void)
		{}
		public: CallerDerivedM0(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM0(C1* _object, R(C2::*_func)(void)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM0(const CallerDerivedM0& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, R(C2::*_func)(void))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM0& operator = (const CallerDerivedM0& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (void)
		{
			if(object && func)
			{
				return (static_cast<C2*>(object)->*func)();
			}
		}
	};
	template<typename R,
			 typename T1,
			 typename T2,
			 typename T3>
	class CallerDerivedF3: public CallerBase<R, T1, T2, T3>
	{
		private: R(*func)(T1, T2, T3);
		public: virtual ~CallerDerivedF3(void)
		{}
		public: CallerDerivedF3(void): func(NIL)
		{}
		public: CallerDerivedF3(R(*_func)(T1, T2, T3)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF3(const CallerDerivedF3& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(R(*_func)(T1, T2, T3))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF3& operator = (const CallerDerivedF3& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg1, T2 _arg2, T3 _arg3)
		{
			if(func)
			{
				return (*func)(_arg1, _arg2, _arg3);
			}
		}
	};
	template<typename R,
			 typename T1,
			 typename T2>
	class CallerDerivedF2: public CallerBase<R, T1, T2>
	{
		private: R(*func)(T1, T2);
		public: virtual ~CallerDerivedF2(void)
		{}
		public: CallerDerivedF2(void): func(NIL)
		{}
		public: CallerDerivedF2(R(*_func)(T1, T2)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF2(const CallerDerivedF2& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(R(*_func)(T1, T2))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF2& operator = (const CallerDerivedF2& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg1, T2 _arg2)
		{
			if(func)
			{
				return (*func)(_arg1, _arg2);
			}
		}
	};
	template<typename R,
			 typename T1>
	class CallerDerivedF1: public CallerBase<R, T1>
	{
		private: R(*func)(T1);
		public: virtual ~CallerDerivedF1(void)
		{}
		public: CallerDerivedF1(void): func(NIL)
		{}
		public: CallerDerivedF1(R(*_func)(T1)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF1(const CallerDerivedF1& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(R(*_func)(T1))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF1& operator = (const CallerDerivedF1& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (T1 _arg)
		{
			if(func)
			{
				return (*func)(_arg);
			}
		}
	};
	template<typename R>
	class CallerDerivedF0: public CallerBase<R> 
	{
		private: R(*func)(void);
		public: virtual ~CallerDerivedF0(void)
		{}
		public: CallerDerivedF0(void): func(NIL)
		{}
		public: CallerDerivedF0(R(*_func)(void)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF0(const CallerDerivedF0& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(R(*_func)(void))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF0& operator = (const CallerDerivedF0& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: virtual R operator () (void)
		{
			if(func)
			{
				return (*func)();
			}
		}
	};
	private: void* caller;
    private: int32 sizeInByte;
	public: Caller(void): caller(NIL), sizeInByte(0)
	{}
	public: Caller(const Caller& _caller): caller(NIL), sizeInByte(0)
	{ 
		*this = _caller;
	}
	public: virtual ~Caller(void)
	{
		if(caller) { free(caller); }
	}
	public: template<typename C1, typename C2> Caller(C1* _object, R(C2::*_func)(void)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1> Caller(C1* _object, R(C2::*_func)(T1)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2> Caller(C1* _object, R(C2::*_func)(T1, T2)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> Caller(C1* _object, R(C2::*_func)(T1, T2, T3)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: Caller(R(*_func)(void)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1> Caller(R(*_func)(T1)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1, typename T2> Caller(R(*_func)(T1, T2)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1, typename T2, typename T3> Caller(R(*_func)(T1, T2, T3)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename C1, typename C2> Caller(C1* _object, R(C2::*_func)(void)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1> Caller(C1* _object, R(C2::*_func)(T1)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2> Caller(C1* _object, R(C2::*_func)(T1, T2)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> Caller(C1* _object, R(C2::*_func)(T1, T2, T3)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: bool IsValid(void)const
	{
		return (caller != NIL);
	}
	public: void Reset(void)
	{
		if(caller)
		{ 
			free(caller);
			caller = NIL;
			sizeInByte = 0;
		}
	}
	public: template<typename C1, typename C2> void Set(C1* _object, R(C2::*_func)(void))
	{ 
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM0<C1, C2, R> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM0<C1, C2, R>);
			caller = (CallerDerivedM0<C1, C2, R>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1> void Set(C1* _object, R(C2::*_func)(T1))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM1<C1, C2, R, T1> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM1<C1, C2, R, T1>);
			caller = (CallerDerivedM1<C1, C2, R, T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2> void Set(C1* _object, R(C2::*_func)(T1, T2))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM2<C1, C2, R, T1, T2> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM2<C1, C2, R, T1, T2>);
			caller = (CallerDerivedM2<C1, C2, R, T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> void Set(C1* _object, R(C2::*_func)(T1, T2, T3))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM3<C1, C2, R, T1, T2, T3> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM3<C1, C2, R, T1, T2, T3>);
			caller = (CallerDerivedM3<C1, C2, R, T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: void Set(R(*_func)(void))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF0<R> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF0<R>);
			caller = (CallerDerivedF0<R>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1> void Set(R(*_func)(T1))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF1<R, T1> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF1<R, T1>);
			caller = (CallerDerivedF1<R, T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1, typename T2> void Set(R(*_func)(T1, T2))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF2<R, T1, T2> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF2<R, T1, T2>);
			caller = (CallerDerivedF2<R, T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1, typename T2, typename T3> void Set(R(*_func)(T1, T2, T3))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF3<R, T1, T2, T3> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF3<R, T1, T2, T3>);
			caller = (CallerDerivedF3<R, T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2> void Set(C1* _object, R(C2::*_func)(void)const)
	{
		if(_object && _func)
		{ 
			if(caller) { free(caller); }
			
			CallerDerivedM0<C1, C2, R> newCaller(_object, (R(C2::*)(void))_func);
			sizeInByte = sizeof(CallerDerivedM0<C1, C2, R>);
			caller = (CallerDerivedM0<C1, C2, R>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1> void Set(C1* _object, R(C2::*_func)(T1)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM1<C1, C2, R, T1> newCaller(_object, (R(C2::*)(T1))_func);
			sizeInByte = sizeof(CallerDerivedM1<C1, C2, R, T1>);
			caller = (CallerDerivedM1<C1, C2, R, T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2> void Set(C1* _object, R(C2::*_func)(T1, T2)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM2<C1, C2, R, T1, T2> newCaller(_object, (R(C2::*)(T1, T2))_func);
			sizeInByte = sizeof(CallerDerivedM2<C1, C2, R, T1, T2>);
			caller = (CallerDerivedM2<C1, C2, R, T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> void Set(C1* _object, R(C2::*_func)(T1, T2, T3)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM3<C1, C2, R, T1, T2, T3> newCaller(_object, (R(C2::*)(T1, T2, T3))_func);
			sizeInByte = sizeof(CallerDerivedM3<C1, C2, R, T1, T2, T3>);
			caller = (CallerDerivedM3<C1, C2, R, T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: Caller& operator = (const Caller& _caller)
	{
		if(caller) { free(caller); }

		if(_caller.IsValid())
		{
			caller = malloc(_caller.sizeInByte);
			memcpy(caller, _caller.caller, _caller.sizeInByte);
		}
		else { caller = NIL; }

		sizeInByte = _caller.sizeInByte;
		return *this;
	}
	public: R operator () (void)
	{
		if(caller)
		{
			return (*(CallerBase<R>*)caller)();
		}
	}
	public: template<typename T1> R operator () (T1 _arg)
	{
		if(caller)
		{
			return (*(CallerBase<R, T1>*)caller)(_arg);
		}
	}
	public: template<typename T1, typename T2> R operator () (T1 _arg1, T2 _arg2)
	{
		if(caller)
		{
			return (*(CallerBase<R, T1, T2>*)caller)(_arg1, _arg2);
		}
	}
	public: template<typename T1, typename T2, typename T3> R operator () (T1 _arg1, T2 _arg2, T3 _arg3)
	{
		if(caller)
		{
			return (*(CallerBase<R, T1, T2, T3>*)caller)(_arg1, _arg2, _arg3);
		} 
	}
};
template<>
class Caller<void>
{
	template<typename R = void,
			 typename T1 = void,
			 typename T2 = void,
			 typename T3 = void>
	class CallerBase
	{};
	template<typename T1,
			 typename T2,
			 typename T3>
	class CallerBase<void, T1, T2, T3>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual void operator () (T1 _arg1, T2 _arg2, T3 _arg3) = 0;
	};
	template<typename T1,
			 typename T2>
	class CallerBase<void, T1, T2, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual void operator () (T1 _arg1, T2 _arg2) = 0;
	};
	template<typename T1>
	class CallerBase<void, T1, void, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual void operator () (T1 _arg) = 0;
	};
	template<>
	class CallerBase<void, void, void, void>
	{
		public: virtual ~CallerBase(void)
		{}
		public: virtual void operator () (void) = 0;
	};
	class FakeClass
	{};
	template<typename C1,
			 typename C2,
			 typename T1,
			 typename T2,
			 typename T3>
	class CallerDerivedM3: public CallerBase<void, T1, T2, T3>
	{
		private: C1* object;
		private: void(C2::*func)(T1, T2, T3);
		public: virtual ~CallerDerivedM3(void)
		{}
		public: CallerDerivedM3(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM3(C1* _object, void(C2::*_func)(T1, T2, T3)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM3(const CallerDerivedM3& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, void(C2::*_func)(T1, T2, T3))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM3& operator = (const CallerDerivedM3& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg1, T2 _arg2, T3 _arg3)
		{
			if(object && func)
			{
				(static_cast<C2*>(object)->*func)(_arg1, _arg2, _arg3);
			}
		}
	};
	template<typename C1,
			 typename C2,
			 typename T1,
			 typename T2>
	class CallerDerivedM2: public CallerBase<void, T1, T2>
	{
		private: C1* object;
		private: void(C2::*func)(T1, T2);
		public: virtual ~CallerDerivedM2(void)
		{}
		public: CallerDerivedM2(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM2(C1* _object, void(C2::*_func)(T1, T2)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM2(const CallerDerivedM2& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, void(C2::*_func)(T1, T2))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM2& operator = (const CallerDerivedM2& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg1, T2 _arg2)
		{
			if(object && func)
			{
				(static_cast<C2*>(object)->*func)(_arg1, _arg2);
			}
		}
	};
	template<typename C1,
			 typename C2,
			 typename T1>
	class CallerDerivedM1: public CallerBase<void, T1>
	{
		private: C1* object;
		private: void(C2::*func)(T1);
		public: virtual ~CallerDerivedM1(void)
		{}
		public: CallerDerivedM1(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM1(C1* _object, void(C2::*_func)(T1)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM1(const CallerDerivedM1& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, void(C2::*_func)(T1))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM1& operator = (const CallerDerivedM1& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg)
		{
			if(object && func)
			{
				(static_cast<C2*>(object)->*func)(_arg);
			}
		}
	};
	template<typename C1,
			 typename C2>
	class CallerDerivedM0: public CallerBase<>
	{
		private: C1* object;
		private: void(C2::*func)(void);
		public: virtual ~CallerDerivedM0(void)
		{}
		public: CallerDerivedM0(void): object(NIL), func(NIL)
		{}
		public: CallerDerivedM0(C1* _object, void(C2::*_func)(void)): object(NIL), func(NIL)
		{
			Set(_object, _func);
		}
		public: CallerDerivedM0(const CallerDerivedM0& _caller): object(NIL), func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(C1* _object, void(C2::*_func)(void))
		{
			if(_object && _func)
			{
				object = _object;
				func = _func;
			}
		}
		public: CallerDerivedM0& operator = (const CallerDerivedM0& _caller)
		{
			object = _caller.object;
			func = _caller.func; 
			return *this;
		}
		public: void operator () (void)
		{
			if(object && func)
			{
				(static_cast<C2*>(object)->*func)();
			}
		}
	};
	template<typename T1,
			 typename T2,
			 typename T3>
	class CallerDerivedF3: public CallerBase<void, T1, T2, T3>
	{
		private: void(*func)(T1, T2, T3);
		public: virtual ~CallerDerivedF3(void)
		{}
		public: CallerDerivedF3(void): func(NIL)
		{}
		public: CallerDerivedF3(void(*_func)(T1, T2, T3)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF3(const CallerDerivedF3& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(void(*_func)(T1, T2, T3))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF3& operator = (const CallerDerivedF3& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg1, T2 _arg2, T3 _arg3)
		{
			if(func)
			{
				(*func)(_arg1, _arg2, _arg3);
			}
		}
	};
	template<typename T1,
			 typename T2>
	class CallerDerivedF2: public CallerBase<void, T1, T2>
	{
		private: void(*func)(T1, T2);
		public: virtual ~CallerDerivedF2(void)
		{}
		public: CallerDerivedF2(void): func(NIL)
		{}
		public: CallerDerivedF2(void(*_func)(T1, T2)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF2(const CallerDerivedF2& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(void(*_func)(T1, T2))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF2& operator = (const CallerDerivedF2& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg1, T2 _arg2)
		{
			if(func)
			{
				(*func)(_arg1, _arg2);
			}
		}
	};
	template<typename T1>
	class CallerDerivedF1: public CallerBase<void, T1>
	{
		private: void(*func)(T1);
		public: virtual ~CallerDerivedF1(void)
		{}
		public: CallerDerivedF1(void): func(NIL)
		{}
		public: CallerDerivedF1(void(*_func)(T1)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF1(const CallerDerivedF1& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(void(*_func)(T1))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF1& operator = (const CallerDerivedF1& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: void operator () (T1 _arg)
		{
			if(func)
			{
				(*func)(_arg);
			}
		}
	};
	class CallerDerivedF0: public CallerBase<>
	{
		private: void(*func)(void);
		public: virtual ~CallerDerivedF0(void)
		{}
		public: CallerDerivedF0(void): func(NIL)
		{}
		public: CallerDerivedF0(void(*_func)(void)): func(NIL)
		{
			Set(_func);
		}
		public: CallerDerivedF0(const CallerDerivedF0& _caller): func(NIL)
		{ 
			*this = _caller;
		}
		public: void Set(void(*_func)(void))
		{
			if(_func)
			{
				func = _func;
			}
		}
		public: CallerDerivedF0& operator = (const CallerDerivedF0& _caller)
		{
			func = _caller.func; 
			return *this;
		}
		public: void operator () (void)
		{
			if(func)
			{
				(*func)();
			}
		}
	};
	private: void* caller;
    private: int32 sizeInByte;
	public: Caller(void): caller(NIL), sizeInByte(0)
	{}
	public: Caller(const Caller& _caller): caller(NIL), sizeInByte(0)
	{ 
		*this = _caller;
	}
	public: virtual ~Caller(void)
	{
		if(caller) { free(caller); }
	}
	public: template<typename C1, typename C2> Caller(C1* _object, void(C2::*_func)(void)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1> Caller(C1* _object, void(C2::*_func)(T1)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2> Caller(C1* _object, void(C2::*_func)(T1, T2)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> Caller(C1* _object, void(C2::*_func)(T1, T2, T3)): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: Caller(void(*_func)(void)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1> Caller(void(*_func)(T1)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1, typename T2> Caller(void(*_func)(T1, T2)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename T1, typename T2, typename T3> Caller(void(*_func)(T1, T2, T3)): caller(NIL), sizeInByte(0)
	{
		Set(_func);
	}
	public: template<typename C1, typename C2> Caller(C1* _object, void(C2::*_func)(void)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1> Caller(C1* _object, void(C2::*_func)(T1)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2> Caller(C1* _object, void(C2::*_func)(T1, T2)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> Caller(C1* _object, void(C2::*_func)(T1, T2, T3)const): caller(NIL), sizeInByte(0)
	{
		Set(_object, _func);
	}
	public: bool IsValid(void)const
	{
		return (caller != NIL);
	}
	public: void Reset(void)
	{
		if(caller)
		{ 
			free(caller);
			caller = NIL;
			sizeInByte = 0;
		}
	}
	public: template<typename C1, typename C2> void Set(C1* _object, void(C2::*_func)(void))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM0<C1, C2> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM0<C1, C2>);
			caller = (CallerDerivedM0<C1, C2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1> void Set(C1* _object, void(C2::*_func)(T1))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM1<C1, C2, T1> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM1<C1, C2, T1>);
			caller = (CallerDerivedM1<C1, C2, T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2> void Set(C1* _object, void(C2::*_func)(T1, T2))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM2<C1, C2, T1, T2> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM2<C1, C2, T1, T2>);
			caller = (CallerDerivedM2<C1, C2, T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> void Set(C1* _object, void(C2::*_func)(T1, T2, T3))
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM3<C1, C2, T1, T2, T3> newCaller(_object, _func);
			sizeInByte = sizeof(CallerDerivedM3<C1, C2, T1, T2, T3>);
			caller = (CallerDerivedM3<C1, C2, T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: void Set(void(*_func)(void))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF0 newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF0);
			caller = (CallerDerivedF0*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1> void Set(void(*_func)(T1))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF1<T1> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF1<T1>);
			caller = (CallerDerivedF1<T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1, typename T2> void Set(void(*_func)(T1, T2))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF2<T1, T2> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF2<T1, T2>);
			caller = (CallerDerivedF2<T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename T1, typename T2, typename T3> void Set(void(*_func)(T1, T2, T3))
	{
		if(_func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedF3<T1, T2, T3> newCaller(_func);
			sizeInByte = sizeof(CallerDerivedF3<T1, T2, T3>);
			caller = (CallerDerivedF3<T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2> void Set(C1* _object, void(C2::*_func)(void)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM0<C1, C2> newCaller(_object, (void(C2::*)(void))_func);
			sizeInByte = sizeof(CallerDerivedM0<C1, C2>);
			caller = (CallerDerivedM0<C1, C2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1> void Set(C1* _object, void(C2::*_func)(T1)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM1<C1, C2, T1> newCaller(_object, (void(C2::*)(T1))_func);
			sizeInByte = sizeof(CallerDerivedM1<C1, C2, T1>);
			caller = (CallerDerivedM1<C1, C2, T1>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2> void Set(C1* _object, void(C2::*_func)(T1, T2)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM2<C1, C2, T1, T2> newCaller(_object, (void(C2::*)(T1, T2))_func);
			sizeInByte = sizeof(CallerDerivedM2<C1, C2, T1, T2>);
			caller = (CallerDerivedM2<C1, C2, T1, T2>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: template<typename C1, typename C2, typename T1, typename T2, typename T3> void Set(C1* _object, void(C2::*_func)(T1, T2, T3)const)
	{
		if(_object && _func)
		{
			if(caller) { free(caller); }
			
			CallerDerivedM3<C1, C2, T1, T2, T3> newCaller(_object, (void(C2::*)(T1, T2, T3))_func);
			sizeInByte = sizeof(CallerDerivedM3<C1, C2, T1, T2, T3>);
			caller = (CallerDerivedM3<C1, C2, T1, T2, T3>*)malloc(sizeInByte);
			memcpy(caller, (const void*)&newCaller, sizeInByte);
		}
	}
	public: Caller& operator = (const Caller& _caller)
	{ 
		if(caller) { free(caller); }

		if(_caller.IsValid())
		{
			caller = malloc(_caller.sizeInByte);
			memcpy(caller, _caller.caller, _caller.sizeInByte);
		}
		else { caller = NIL; }

		sizeInByte = _caller.sizeInByte;
		return *this;
	}
	public: void operator () (void)
	{
		if(caller)
		{
			(*(CallerBase<>*)caller)();
		}
	}
	public: template<typename T1> void operator () (T1 _arg)
	{
		if(caller)
		{
			(*(CallerBase<void, T1>*)caller)(_arg);
		}
	}
	public: template<typename T1, typename T2> void operator () (T1 _arg1, T2 _arg2)
	{
		if(caller)
		{
			(*(CallerBase<void, T1, T2>*)caller)(_arg1, _arg2);
		}
	}
	public: template<typename T1, typename T2, typename T3> void operator () (T1 _arg1, T2 _arg2, T3 _arg3)
	{
		if(caller)
		{
			(*(CallerBase<void, T1, T2, T3>*)caller)(_arg1, _arg2, _arg3);
		}
	}
};

#endif
