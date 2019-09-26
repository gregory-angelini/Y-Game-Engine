
#include "Vector2List.h"
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>




List<Vector2>::List(void)
{
	AddClassProperty(VECTOR_2_LIST_CLASS);
}


List<Vector2>::~List(void)
{ 
	Destroying();
}



bool List<Vector2>::IsListEmpty(void)const
{
	return IsEmpty();
}



void List<Vector2>::RotateItems(bool _right)
{
	Rotate(_right);
}


int32 List<Vector2>::GetValueType(void)const
{
	return VECTOR_2_TYPE;
}


float List<Vector2>::GetLength(void)const
{
	if(GetSize() > 1)
	{
		float length = 0.0f;
		for(AddressDerived* it = GetHeadAddress(); it != GetTailAddress(); it = it->GetRight())
		{	
			length += Line::_GetLength(it->GetNode()->GetValue(), it->GetRight()->GetNode()->GetValue());
		}
		return length;
	}
	return 0.0f;
}



void List<Vector2>::SwapIndex(int32 _index1, int32 _index2)
{
	AbstractObject* it1 = GetItem(_index1);
	
	if(it1)
	{
		AbstractObject* it2 = GetItem(_index2);

		if(it2)
		{	
			_Swap(dynamic_cast<AddressDerived*>(it1->GetAddress(this)), dynamic_cast<AddressDerived*>(it2->GetAddress(this)));
		}
	}
}


void List<Vector2>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<Vector2>*>(_list))));
	}
}


void List<Vector2>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<Vector2>*>(_list));
		}
	}
}


MagicNode::Address* List<Vector2>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<Variable<Vector2>*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<Variable<Vector2>*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<Variable<Vector2>*>(_item));
		}
			
		default:
		{
			return AddTo(dynamic_cast<Variable<Vector2>*>(_item), _index);
		}
	}
	return NIL;
}



void List<Vector2>::CopyTo(AbstractList* _list, int32 _copyMode)
{
	if(_list)
	{
		if(IsNotEmpty())
		{
			switch(_copyMode)
			{
				case SHARE_ITEM:
				{
					if(GetValueType() == _list->GetValueType())
					{
						List<Vector2>* derivedList = dynamic_cast<List<Vector2>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{			
								derivedList->AddToTail(it->GetNode());
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == OBJECT_TYPE)
					{
						List<AbstractObject>* derivedList = dynamic_cast<List<AbstractObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{			
								derivedList->AddToTail(it->GetNode());
								it = it->GetRight();
							}
						}
					}
					break;
				}

				case COPY_ONLY_DATA:
				{
					if(GetValueType() == _list->GetValueType())
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<Vector2>* derivedList = dynamic_cast<List<Vector2>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									Variable<Vector2>* left = it1->GetNode();
									Variable<Vector2>* right = it2->GetNode();
												
									if(left && right)
									{ 
										right->SetValue(left->GetValue());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					break;
				}

				case COPY_IN_VARIABLE:
				{
					if(GetValueType() == _list->GetValueType())
					{
						List<Vector2>* derivedList = dynamic_cast<List<Vector2>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								Variable<Vector2>* right = it->GetNode();
											
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector2>(right->GetValue()));
								}
								it = it->GetRight();
							}
						}
					}
					break;
				}
			}
		}
	}
}



int32 List<Vector2>::GetAmountOfItems(void)const
{
	return GetSize();
}



AbstractObject* List<Vector2>::GetItem(int32 _index)
{
	if(IsNotEmpty())
	{
		switch(_index)
		{
			case HEAD:
			{
				return GetHead();
			}

			case TAIL:
			{
				return GetTail();
			}

			case RANDOM:
			{
				return GetRandom();
			}
				
			default:
			{
				return Get(_index);
			}
		}
	}
	return NIL;
}


void List<Vector2>::SetValue(Vector2 _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetValue(_value);
		
		it = it->GetRight();
	}
}


void List<Vector2>::SetItemValue(int32 _index, Vector2 _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				Variable<Vector2>* item = GetHead();
				if(item) { item->SetValue(_value); }
				break;
			}

			case TAIL:
			{
				Variable<Vector2>* item = GetTail();
				if(item) { item->SetValue(_value); }
				break;
			}

			case RANDOM:
			{
				Variable<Vector2>* item = GetRandom();
				if(item) { item->SetValue(_value); }
				break;
			}
				
			default:
			{
				Variable<Vector2>* item = Get(_index);
				if(item) { item->SetValue(_value); }
				break;
			}
		}
	}
}



void List<Vector2>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}


void List<Vector2>::InverseList(void)
{
	Inverse();
}



int32 List<Vector2>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<Variable<Vector2>*>(_item));
}


void List<Vector2>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<Variable<Vector2>*>(_item), (MagicContainer::ClearMode)_removeMode);
}



void List<Vector2>::RemoveIndex(int32 _index, int32 _removeMode)
{
	if(IsNotEmpty()) 
	{
		if(_removeMode != MagicContainer::REMOVE_NODE)
		{
			switch(_index)
			{
				case HEAD:
				{
					Remove(GetHead(), (MagicContainer::ClearMode)_removeMode);
					break;
				}

				case TAIL:
				{
					Remove(GetTail(), (MagicContainer::ClearMode)_removeMode); 
					break;
				}

				case RANDOM:
				{
					Remove(GetRandom(), (MagicContainer::ClearMode)_removeMode);
					break;
				}
					
				default:
				{
					Remove(Get(_index), (MagicContainer::ClearMode)_removeMode);
					break;
				}
			}
		}
	}
}


void List<Vector2>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}




bool List<Vector2>::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + listsDir + GetName() + "." + listsSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<Vector2>::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(GetName().length());
		_file.WriteString(GetName());

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



bool List<Vector2>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<Vector2>::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetValueType());
		_file.Write(_name.length());
		_file.WriteString(_name);

		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}


void List<Vector2>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}


void List<Vector2>::SaveItemToFile(MagicList<Variable<Vector2>>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		Variable<Vector2>* variable = _address->GetNode();
		bool isCommonAsset = variable->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(isCommonAsset);

		if(isCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(variable->GetName(), AssetLibrary::VARIABLE_ASSET))
			{
				variable->SaveToFile();
			}
			_file.Write(variable->GetName().length());
			_file.WriteString(variable->GetName());
		}
		else
		{
			variable->SaveToFile(_file);
		}

		SaveItemToFile(_address->GetRight(), _file);
	}
}




void List<Vector2>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 variableListSize;
	_file.Read(variableListSize);
	for(int32 i = 0; i < variableListSize; ++i)
	{
		 LoadItemFromFile(_file);
	}
}



void List<Vector2>:: LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	Variable<Vector2>* variable;
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		variable = dynamic_cast<Variable<Vector2>*>(AssetLibrary::_LoadCommonAsset<AbstractVariable>(name));
	}
	else
	{
		variable = dynamic_cast<Variable<Vector2>*>(AssetLibrary::_LoadPrivateAsset<AbstractVariable>(_file));
	}

	if(variable)
	{
		AddToTail(variable);
	}
}

