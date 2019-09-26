
#include "RotatableObjectList.h"
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>




List<RotatableObject>::List(void)
{
	AddClassProperty(ROTATABLE_LIST_CLASS);
}



List<RotatableObject>::~List(void)
{ 
	Destroying();
}


void List<RotatableObject>::RotateItems(bool _right)
{
	Rotate(_right);
}


int32 List<RotatableObject>::GetValueType(void)const
{
	return ROTATABLE_OBJECT_TYPE;
}



bool List<RotatableObject>::IsListEmpty(void)const
{
	return IsEmpty();
}



void List<RotatableObject>::SwapIndex(int32 _index1, int32 _index2)
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


void List<RotatableObject>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<RotatableObject>*>(_list))));
	}
}


void List<RotatableObject>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<RotatableObject>*>(_list));
		}
	}
}


MagicNode::Address* List<RotatableObject>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<RotatableObject*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<RotatableObject*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<RotatableObject*>(_item));
		}
			
		default:
		{
			return AddTo(dynamic_cast<RotatableObject*>(_item), _index);
		}
	}
	return NIL;
}



void List<RotatableObject>::CopyTo(AbstractList* _list, int32 _copyMode)
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
						List<RotatableObject>* derivedList = dynamic_cast<List<RotatableObject>*>(_list);

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
							List<RotatableObject>* derivedList = dynamic_cast<List<RotatableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									RotatableObject* left = it1->GetNode();
									RotatableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetRotation(left->GetRotation());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					else if(_list->GetValueType() == QUATERNION_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<Quaternion>* derivedList = dynamic_cast<List<Quaternion>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<Quaternion>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									RotatableObject* left = it1->GetNode();
									Variable<Quaternion>* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetValue(left->GetRotation());
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
						List<RotatableObject>* derivedList = dynamic_cast<List<RotatableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								RotatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Quaternion>(right->GetRotation()));
								}
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == QUATERNION_TYPE)
					{
						List<Quaternion>* derivedList = dynamic_cast<List<Quaternion>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								RotatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Quaternion>(right->GetRotation()));
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


int32 List<RotatableObject>::GetAmountOfItems(void)const
{
	return GetSize();
}



AbstractObject* List<RotatableObject>::GetItem(int32 _index)
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


void List<RotatableObject>::SetValue(Quaternion _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetRotation(_value);
		
		it = it->GetRight();
	}
}


void List<RotatableObject>::SetItemValue(int32 _index, Quaternion _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				RotatableObject* item = GetHead();
				if(item) { item->SetRotation(_value); }
				break;
			}

			case TAIL:
			{
				RotatableObject* item = GetTail();
				if(item) { item->SetRotation(_value); }
				break;
			}

			case RANDOM:
			{
				RotatableObject* item = GetRandom();
				if(item) { item->SetRotation(_value); }
				break;
			}
				
			default:
			{
				RotatableObject* item = Get(_index);
				if(item) { item->SetRotation(_value); }
				break;
			}
		}
	}
}



void List<RotatableObject>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}




void List<RotatableObject>::InverseList(void)
{
	Inverse();
}


int32 List<RotatableObject>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<RotatableObject*>(_item));
}



void List<RotatableObject>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<RotatableObject*>(_item), (MagicContainer::ClearMode)_removeMode);
}


void List<RotatableObject>::RemoveIndex(int32 _index, int32 _removeMode)
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



void List<RotatableObject>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}



bool List<RotatableObject>::SaveToFile(StringANSI _path)
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



bool List<RotatableObject>::SaveToFile(File& _file)
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



bool List<RotatableObject>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<RotatableObject>::SaveAsToFile(File& _file, StringANSI _name)
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


void List<RotatableObject>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}


void List<RotatableObject>::SaveItemToFile(MagicList<RotatableObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		RotatableObject* object = _address->GetNode();
		bool isCommonAsset = object->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(isCommonAsset);
		int32 assetType = AssetLibrary::_GetAssetType(object->GetClassProperties());
		_file.Write(assetType);

		if(isCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(object->GetName(), assetType))
			{
				object->SaveToFile();
			}
			_file.Write(object->GetName().length());
			_file.WriteString(object->GetName());
		}
		else
		{
			object->SaveToFile(_file);
		}


		SaveItemToFile(_address->GetRight(), _file);
	}
}




void List<RotatableObject>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}


void List<RotatableObject>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	RotatableObject* object; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = dynamic_cast<RotatableObject*>(AssetLibrary::_LoadCommonAsset(name, type));
	}
	else
	{
		object = dynamic_cast<RotatableObject*>(AssetLibrary::_LoadPrivateAsset(_file, type));
	}

	if(object)
	{
		AddToTail(object);
	}
}

