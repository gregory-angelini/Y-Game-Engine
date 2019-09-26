
#include "TranslatableObjectList.h"
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>


List<TranslatableObject>::List(void)
{
	AddClassProperty(TRANSLATABLE_LIST_CLASS);
}


List<TranslatableObject>::~List(void)
{ 
	Destroying();
}



int32 List<TranslatableObject>::GetValueType(void)const
{
	return TRANSLATABLE_OBJECT_TYPE;
}

	
void List<TranslatableObject>::RotateItems(bool _right)
{
	Rotate(_right);
}



bool List<TranslatableObject>::IsListEmpty(void)const
{
	return IsEmpty();
}


float List<TranslatableObject>::GetLength(void)const
{
	if(GetSize() > 1)
	{
		float length = 0.0f;
		for(AddressDerived* it = GetHeadAddress(); it != GetTailAddress(); it = it->GetRight())
		{	
			length += Line::_GetLength(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition());
		}
		return length;
	}
	return 0.0f;
}




void List<TranslatableObject>::SwapIndex(int32 _index1, int32 _index2)
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


void List<TranslatableObject>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<TranslatableObject>*>(_list))));
	}
}


void List<TranslatableObject>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<TranslatableObject>*>(_list));
		}
	}
}


MagicNode::Address* List<TranslatableObject>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<TranslatableObject*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<TranslatableObject*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<TranslatableObject*>(_item));
		}
			
		default:
		{
			return AddTo(dynamic_cast<TranslatableObject*>(_item), _index);
		}
	}
	return NIL;
}



void List<TranslatableObject>::CopyTo(AbstractList* _list, int32 _copyMode)
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
						List<TranslatableObject>* derivedList = dynamic_cast<List<TranslatableObject>*>(_list);

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
							List<TranslatableObject>* derivedList = dynamic_cast<List<TranslatableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									TranslatableObject* left = it1->GetNode();
									TranslatableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetPosition(left->GetPosition());
									}
									it1 = it1->GetRight();
									it2 = it2->GetRight();
								}
							}
						}
					}
					else if(_list->GetValueType() == SCALABLE_OBJECT_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<ScalableObject>* derivedList = dynamic_cast<List<ScalableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<ScalableObject>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									TranslatableObject* left = it1->GetNode();
									ScalableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetScale(left->GetPosition());
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
									TranslatableObject* left = it1->GetNode();
									ColorableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetColorRGB(left->GetPosition());
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
									TranslatableObject* left = it1->GetNode();
									Variable<Vector3>* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetValue(left->GetPosition());
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
						List<TranslatableObject>* derivedList = dynamic_cast<List<TranslatableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								TranslatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetPosition()));
								}
								it = it->GetRight();
							}
						}
					}
					else if(_list->GetValueType() == SCALABLE_OBJECT_TYPE)
					{
						List<ScalableObject>* derivedList = dynamic_cast<List<ScalableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								TranslatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetPosition()));
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
								TranslatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetPosition()));
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
								TranslatableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetPosition()));
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



int32 List<TranslatableObject>::GetAmountOfItems(void)const
{
	return GetSize();
}



AbstractObject* List<TranslatableObject>::GetItem(int32 _index)
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



void List<TranslatableObject>::SetValue(Vector3 _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetPosition(_value);
		
		it = it->GetRight();
	}
}


void List<TranslatableObject>::SetItemValue(int32 _index, Vector3 _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				TranslatableObject* item = GetHead();
				if(item) { item->SetPosition(_value); }
				break;
			}

			case TAIL:
			{
				TranslatableObject* item = GetTail();
				if(item) { item->SetPosition(_value); }
				break;
			}

			case RANDOM:
			{
				TranslatableObject* item = GetRandom();
				if(item) { item->SetPosition(_value); }
				break;
			}
				
			default:
			{
				TranslatableObject* item = Get(_index);
				if(item) { item->SetPosition(_value); }
				break;
			}
		}
	}
}



void List<TranslatableObject>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}



void List<TranslatableObject>::InverseList(void)
{
	Inverse();
}


int32 List<TranslatableObject>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<TranslatableObject*>(_item));
}


void List<TranslatableObject>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<TranslatableObject*>(_item), (MagicContainer::ClearMode)_removeMode);
}


void List<TranslatableObject>::RemoveIndex(int32 _index, int32 _removeMode)
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


void List<TranslatableObject>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}




bool List<TranslatableObject>::SaveToFile(StringANSI _path)
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



bool List<TranslatableObject>::SaveToFile(File& _file)
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




bool List<TranslatableObject>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<TranslatableObject>::SaveAsToFile(File& _file, StringANSI _name)
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



void List<TranslatableObject>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}



void List<TranslatableObject>::SaveItemToFile(MagicList<TranslatableObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		TranslatableObject* object = _address->GetNode();
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



void List<TranslatableObject>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}


void List<TranslatableObject>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	TranslatableObject* object; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = dynamic_cast<TranslatableObject*>(AssetLibrary::_LoadCommonAsset(name, type));
	}
	else
	{
		object = dynamic_cast<TranslatableObject*>(AssetLibrary::_LoadPrivateAsset(_file, type));
	}

	if(object)
	{
		AddToTail(object);
	}
}
