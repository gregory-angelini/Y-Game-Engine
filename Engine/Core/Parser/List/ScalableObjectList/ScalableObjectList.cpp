
#include "ScalableObjectList.h"
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>




List<ScalableObject>::List(void)
{
	AddClassProperty(SCALABLE_LIST_CLASS);
}



List<ScalableObject>::~List(void)
{ 
	Destroying();
}



int32 List<ScalableObject>::GetValueType(void)const
{
	return SCALABLE_OBJECT_TYPE;
}


void List<ScalableObject>::RotateItems(bool _right)
{
	Rotate(_right);
}



bool List<ScalableObject>::IsListEmpty(void)const
{
	return IsEmpty();
}


float List<ScalableObject>::GetLength(void)const
{
	if(GetSize() > 1)
	{
		float length = 0.0f;
		for(AddressDerived* it = GetHeadAddress(); it != GetTailAddress(); it = it->GetRight())
		{	
			length += Line::_GetLength(it->GetNode()->GetScale(), it->GetRight()->GetNode()->GetScale());
		}
		return length;
	}
	return 0.0f;
}



void List<ScalableObject>::SwapIndex(int32 _index1, int32 _index2)
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


void List<ScalableObject>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<ScalableObject>*>(_list))));
	}
}


void List<ScalableObject>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<ScalableObject>*>(_list));
		}
	}
}


MagicNode::Address* List<ScalableObject>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<ScalableObject*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<ScalableObject*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<ScalableObject*>(_item));
		}
			
		default:
		{
			return AddTo(dynamic_cast<ScalableObject*>(_item), _index);
		}
	}
	return NIL;
}




void List<ScalableObject>::CopyTo(AbstractList* _list, int32 _copyMode)
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
						List<ScalableObject>* derivedList = dynamic_cast<List<ScalableObject>*>(_list);

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
							List<ScalableObject>* derivedList = dynamic_cast<List<ScalableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									ScalableObject* left = it1->GetNode();
									ScalableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetScale(left->GetScale());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					else if(_list->GetValueType() == TRANSLATABLE_OBJECT_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<TranslatableObject>* derivedList = dynamic_cast<List<TranslatableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<TranslatableObject>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									ScalableObject* left = it1->GetNode();
									TranslatableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetPosition(left->GetScale());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					else if(_list->GetValueType() == COLORABLE_OBJECT_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<ColorableObject>* derivedList = dynamic_cast<List<ColorableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<ColorableObject>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									ScalableObject* left = it1->GetNode();
									ColorableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetColorRGB(left->GetScale());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					else if(_list->GetValueType() == VECTOR_3_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<Vector3>* derivedList = dynamic_cast<List<Vector3>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<Vector3>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									ScalableObject* left = it1->GetNode();
									Variable<Vector3>* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetValue(left->GetScale());
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
						List<ScalableObject>* derivedList = dynamic_cast<List<ScalableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								ScalableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetScale()));
								}
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == TRANSLATABLE_OBJECT_TYPE)
					{
						List<TranslatableObject>* derivedList = dynamic_cast<List<TranslatableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								ScalableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetScale()));
								}
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == COLORABLE_OBJECT_TYPE)
					{
						List<ColorableObject>* derivedList = dynamic_cast<List<ColorableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								ScalableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetScale()));
								}
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == VECTOR_3_TYPE)
					{
						List<Vector3>* derivedList = dynamic_cast<List<Vector3>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								ScalableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetScale()));
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



int32 List<ScalableObject>::GetAmountOfItems(void)const
{
	return GetSize();
}




AbstractObject* List<ScalableObject>::GetItem(int32 _index)
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



void List<ScalableObject>::SetValue(Vector3 _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetScale(_value);
		
		it = it->GetRight();
	}
}



void List<ScalableObject>::SetItemValue(int32 _index, Vector3 _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				ScalableObject* item = GetHead();
				if(item) { item->SetScale(_value); }
				break;
			}

			case TAIL:
			{
				ScalableObject* item = GetTail();
				if(item) { item->SetScale(_value); }
				break;
			}

			case RANDOM:
			{
				ScalableObject* item = GetRandom();
				if(item) { item->SetScale(_value); }
				break;
			}
				
			default:
			{
				ScalableObject* item = Get(_index);
				if(item) { item->SetScale(_value); }
				break;
			}
		}
	}
}



void List<ScalableObject>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}


void List<ScalableObject>::InverseList(void)
{
	Inverse();
}



int32 List<ScalableObject>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<ScalableObject*>(_item));
}



void List<ScalableObject>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<ScalableObject*>(_item), (MagicContainer::ClearMode)_removeMode);
}


void List<ScalableObject>::RemoveIndex(int32 _index, int32 _removeMode)
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


void List<ScalableObject>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}



bool List<ScalableObject>::SaveToFile(StringANSI _path)
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



bool List<ScalableObject>::SaveToFile(File& _file)
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



bool List<ScalableObject>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<ScalableObject>::SaveAsToFile(File& _file, StringANSI _name)
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



void List<ScalableObject>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}



void List<ScalableObject>::SaveItemToFile(MagicList<ScalableObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		ScalableObject* object = _address->GetNode();
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




void List<ScalableObject>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}


void List<ScalableObject>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	ScalableObject* object; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = dynamic_cast<ScalableObject*>(AssetLibrary::_LoadCommonAsset(name, type));
	}
	else
	{
		object = dynamic_cast<ScalableObject*>(AssetLibrary::_LoadPrivateAsset(_file, type));
	}

	if(object)
	{
		AddToTail(object);
	}
}
