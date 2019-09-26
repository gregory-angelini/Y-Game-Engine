#ifndef COMBINATOR_H
#define COMBINATOR_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Math/Math/Math.h>
class Combinator
{
	public: static int32 _AmountOfPermutations(int32 _n)
	{
		return Math::_Factorial(_n);
	}
	public: template<typename Type> static void _GetPermutations(SimpleList<Type> _in, SimpleList<SimpleList<Type>>& _out) 
	{
		_GetPermutations(_in, 0, _out);
	}
	private: template<typename Type> static void _GetPermutations(SimpleList<Type>& _in, int32 _p, SimpleList<SimpleList<Type>>& _out) 
	{
		int32 j; 
		if(_p == _in.GetSize())
		{
			_out.AddToTail(_in);
	    }
		else
	    {
			for(j = _p; j < _in.GetSize(); j++)
		    {
				_in.SwapKey(_p, j);
				_GetPermutations(_in, _p + 1, _out);
				_in.SwapKey(_p, j); 
			}
	    }
	}
	public: static int32 _AmountOfPermutationsWithRepetition(int32 _n, int32 _length)
	{
		if(_length == 0) 
		{
			return 1;
		}

		int32 total = 0;

		for(int32 i = 0; i < _n; i++) 
		{
			total += _AmountOfPermutationsWithRepetition(_n, _length - 1);
		}
		return total;
	}
	public: template<typename Type> static void _GetPermutationsWithRepetition(const SimpleList<Type>& _in, int32 _length, SimpleList<SimpleList<Type>>& _out)
	{
		SimpleList<Type> combine(_length, Type());
		_GetPermutationsWithRepetition(_in, _length, _out, _length, combine);
	}
	private: template<typename Type> static void _GetPermutationsWithRepetition(const SimpleList<Type>& _in, int32 _length, SimpleList<SimpleList<Type>>& _out, int32 _p, SimpleList<Type>& _combine)
	{
		if(_p == 0) 
		{
			_out.AddToTail(SimpleList<Type>());
			
			for(int32 i = _length - 1; i >= 0; i--)
			{ 
				_out.GetTail()->key.AddToTail(_combine[i]->key);
			}
			return;
		}
		for(int32 i = 0; i < _in.GetSize(); i++) 
		{
			_combine[_p - 1]->key = _in[i]->key;
			_GetPermutationsWithRepetition(_in, _length, _out, _p - 1, _combine);
		}
	}
	public: static int32 _AmoutOfCombinationsWithRepetition(int32 _n, int32 _length)
	{
		int32 count = 0;
		_AmoutOfCombinationsWithRepetition(_n, _length, 0, count);
		return count;
	}
	private: static int32 _AmoutOfCombinationsWithRepetition(int32 _n, int32 _length, int32 _j, int32& _index)
	{
		for(int32 i = 0; i < _n; ++i)
		{
			int32 combine = _j;

			if(combine < _length)
			{
				++combine;
				++_index;
				_AmoutOfCombinationsWithRepetition(_n, _length, combine, _index);
			}
		}
		return _index;
	}
	public: template<typename Type> static void _MoveCombinationsWithRepetitionToList(const SimpleList<Type>& _in, int32 _length, SimpleList<SimpleList<Type>>& _out)
	{
		SimpleList<Type> combine;
		_MoveCombinationsWithRepetitionToList(_in, _length, _out, combine);
	}
	private: template<typename Type> static void _MoveCombinationsWithRepetitionToList(const SimpleList<Type>& _in, int32 _length, SimpleList<SimpleList<Type>>& _out, SimpleList<Type>& _combine)
	{
		for(int32 i = 0; i < _in.GetSize(); ++i)
		{
			SimpleList<Type> combine = _combine;

			if(combine.GetSize() < _length)
			{
				combine.AddToTail(_in[i]->key);

				_out.AddToTail(combine);
				_MoveCombinationsWithRepetitionToList(_in, _length, _out, combine);
			}
		}
	}
	public: static int32 _GetCombinations(int32 _n)
	{
		return pow((double)2, (double)_n) - 1;
	}
	public: template<typename Type> static void _MoveCombinationsToList(SimpleList<Type>& _in, SimpleList<SimpleList<Type>>& _out)
	{
		SimpleList<Type> combine;
		_MoveCombinationsToList(combine, 0, _in, _out);
	}
	private: template<typename Type> static void _MoveCombinationsToList(SimpleList<Type> _combine, int32 _index, SimpleList<Type>& _in, SimpleList<SimpleList<Type>>& _out)
	{
		if(_index < _in.GetSize())
		{
			_MoveCombinationsToList(_combine, _index + 1, _in, _out);
			_combine += _in.Get(_index)->key;
			_MoveCombinationsToList(_combine, _index + 1, _in, _out);
			_out.AddToHead(_combine);
		}
	}
};






#endif
