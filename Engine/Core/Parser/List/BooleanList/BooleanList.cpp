
#include "BooleanList.h"
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>


List<bool>::List(void)
{
	AddClassProperty(BOOLEAN_LIST_CLASS);
}


List<bool>::~List(void)
{ 
	Destroying();
}


int32 List<bool>::GetValueType(void)const
{
	return BOOLEAN_TYPE;
}



bool List<bool>::IsListEmpty(void)const
{
	return IsEmpty();
}


void List<bool>::RotateItems(bool _right)
{
	Rotate(_right);
}



void List<bool>::SwapIndex(int32 _index1, int32 _index2)
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


void List<bool>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<bool>*>(_list))));
	}
}


void List<bool>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<bool>*>(_list));
		}
	}
}



MagicNode::Address* List<bool>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<Variable<bool>*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<Variable<bool>*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<Variable<bool>*>(_item));
		}
				
		default:
		{
			return AddTo(dynamic_cast<Variable<bool>*>(_item), _index);
		}
	}
	return NIL;
}

 
void List<bool>::CopyTo(AbstractList* _list, int32 _copyMode)
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
						List<bool>* derivedList = dynamic_cast<List<bool>*>(_list);

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
							List<bool>* derivedList = dynamic_cast<List<bool>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									Variable<bool>* left = it1->GetNode();
									Variable<bool>* right = it2->GetNode();
												
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
						List<bool>* derivedList = dynamic_cast<List<bool>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								Variable<bool>* right = it->GetNode();
											
								if(right)
								{ 
									derivedList->AddToTail(new Variable<bool>(right->GetValue()));
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


int32 List<bool>::GetAmountOfItems(void)const
{
	return GetSize();
}


AbstractObject* List<bool>::GetItem(int32 _index)
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


void List<bool>::SetValue(bool _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetValue(_value);
		
		it = it->GetRight();
	}
}


void List<bool>::SetItemValue(int32 _index, bool _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				Variable<bool>* item = GetHead();
				if(item) { item->SetValue(_value); }
				break;
			}

			case TAIL:
			{
				Variable<bool>* item = GetTail();
				if(item) { item->SetValue(_value); }
				break;
			}

			case RANDOM:
			{
				Variable<bool>* item = GetRandom();
				if(item) { item->SetValue(_value); }
				break;
			}
				
			default:
			{
				Variable<bool>* item = Get(_index);
				if(item) { item->SetValue(_value); }
				break;
			}
		}
	}
}



void List<bool>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}


void List<bool>::InverseList(void)
{
	Inverse();
}


int32 List<bool>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<Variable<bool>*>(_item));
}


void List<bool>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<Variable<bool>*>(_item), (MagicContainer::ClearMode)_removeMode);
}


void List<bool>::RemoveIndex(int32 _index, int32 _removeMode)
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

void List<bool>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}




bool List<bool>::SaveToFile(StringANSI _path)
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



bool List<bool>::SaveToFile(File& _file)
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



bool List<bool>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<bool>::SaveAsToFile(File& _file, StringANSI _name)
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


  
void List<bool>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}


void List<bool>::SaveItemToFile(MagicList<Variable<bool>>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		Variable<bool>* variable = _address->GetNode();
		bool isCommonAsset = variable->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(isCommonAsset);
		int32 assetType = AssetLibrary::_GetAssetType(variable->GetClassProperties());
		_file.Write(assetType);

		if(isCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(variable->GetName(), assetType))
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



void List<bool>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}


void List<bool>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	Variable<bool>* variable; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		variable = dynamic_cast<Variable<bool>*>(AssetLibrary::_LoadCommonAsset(name, type));
	}
	else
	{
		variable = dynamic_cast<Variable<bool>*>(AssetLibrary::_LoadPrivateAsset(_file, type));
	}

	if(variable)
	{
		AddToTail(variable);
	}
}