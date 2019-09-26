
#include "ColorableObjectList.h"
#include <Engine/Core/Parser/List/TranslatableObjectList/TranslatableObjectList.h>
#include <Engine/Core/Parser/List/ScalableObjectList/ScalableObjectList.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>



List<ColorableObject>::List(void)
{
	AddClassProperty(COLORABLE_LIST_CLASS);
}



List<ColorableObject>::~List(void)
{ 
	Destroying();
}



int32 List<ColorableObject>::GetValueType(void)const
{
	return COLORABLE_OBJECT_TYPE;
}



bool List<ColorableObject>::IsListEmpty(void)const
{
	return IsEmpty();
}



void List<ColorableObject>::RotateItems(bool _right)
{
	Rotate(_right);
}


float List<ColorableObject>::GetLength(void)const
{
	if(GetSize() > 1)
	{
		float length = 0.0f;
		for(AddressDerived* it = GetHeadAddress(); it != GetTailAddress(); it = it->GetRight())
		{	
			length += Line::_GetLength(it->GetNode()->GetColorRGB(), it->GetRight()->GetNode()->GetColorRGB());
		}
		return length;
	}
	return 0.0f;
}



void List<ColorableObject>::SwapIndex(int32 _index1, int32 _index2)
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


void List<ColorableObject>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<ColorableObject>*>(_list))));
	}
}


void List<ColorableObject>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<ColorableObject>*>(_list));
		}
	}
}


MagicNode::Address* List<ColorableObject>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(dynamic_cast<ColorableObject*>(_item));
		}

		case TAIL:
		{
			return AddToTail(dynamic_cast<ColorableObject*>(_item));
		}

		case RANDOM:
		{
			return AddRandom(dynamic_cast<ColorableObject*>(_item));
		}
			
		default:
		{
			return AddTo(dynamic_cast<ColorableObject*>(_item), _index);
		}
	}
	return NIL;
}


void List<ColorableObject>::CopyTo(AbstractList* _list, int32 _copyMode)
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
						List<ColorableObject>* derivedList = dynamic_cast<List<ColorableObject>*>(_list);

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
							List<ColorableObject>* derivedList = dynamic_cast<List<ColorableObject>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									ColorableObject* left = it1->GetNode();
									ColorableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetColorRGBA(left->GetColorRGBA());
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
									ColorableObject* left = it1->GetNode();
									ScalableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetScale(left->GetColorRGB());
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
									ColorableObject* left = it1->GetNode();
									TranslatableObject* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetPosition(left->GetColorRGB());
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
									ColorableObject* left = it1->GetNode();
									Variable<Vector3>* right = it2->GetNode();
													
									if(left && right)
									{ 
										right->SetValue(left->GetColorRGB());
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
						List<ColorableObject>* derivedList = dynamic_cast<List<ColorableObject>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								ColorableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetColorRGB()));
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
								ColorableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetColorRGB()));
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
								ColorableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetColorRGB()));
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
								ColorableObject* right = it->GetNode();
												
								if(right)
								{ 
									derivedList->AddToTail(new Variable<Vector3>(right->GetColorRGB()));
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



int32 List<ColorableObject>::GetAmountOfItems(void)const
{
	return GetSize();
}




AbstractObject* List<ColorableObject>::GetItem(int32 _index)
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



void List<ColorableObject>::SetValue(Vector3 _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetColorRGB(_value);
		
		it = it->GetRight();
	}
}


void List<ColorableObject>::SetValue(Vector4 _value)
{
	AddressDerived* it = GetHeadAddress();

	while(it)
	{
		it->GetNode()->SetColorRGBA(_value);
		
		it = it->GetRight();
	}
}


void List<ColorableObject>::SetItemValue(int32 _index, Vector3 _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				ColorableObject* item = GetHead();
				if(item) { item->SetColorRGB(_value); }
				break;
			}

			case TAIL:
			{
				ColorableObject* item = GetTail();
				if(item) { item->SetColorRGB(_value); }
				break;
			}

			case RANDOM:
			{
				ColorableObject* item = GetRandom();
				if(item) { item->SetColorRGB(_value); }
				break;
			}
				
			default:
			{
				ColorableObject* item = Get(_index);
				if(item) { item->SetColorRGB(_value); }
				break;
			}
		}
	}
}



void List<ColorableObject>::SetItemValue(int32 _index, Vector4 _value)
{
	if(IsNotEmpty()) 
	{
		switch(_index)
		{
			case HEAD:
			{
				ColorableObject* item = GetHead();
				if(item) { item->SetColorRGBA(_value); }
				break;
			}

			case TAIL:
			{
				ColorableObject* item = GetTail();
				if(item) { item->SetColorRGBA(_value); }
				break;
			}

			case RANDOM:
			{
				ColorableObject* item = GetRandom();
				if(item) { item->SetColorRGBA(_value); }
				break;
			}
				
			default:
			{
				ColorableObject* item = Get(_index);
				if(item) { item->SetColorRGBA(_value); }
				break;
			}
		}
	}
}


void List<ColorableObject>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}





void List<ColorableObject>::InverseList(void)
{
	Inverse();
}



int32 List<ColorableObject>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(dynamic_cast<ColorableObject*>(_item));
}



void List<ColorableObject>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(dynamic_cast<ColorableObject*>(_item), (MagicContainer::ClearMode)_removeMode);
}



void List<ColorableObject>::RemoveIndex(int32 _index, int32 _removeMode)
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


void List<ColorableObject>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}





bool List<ColorableObject>::SaveToFile(StringANSI _path)
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



bool List<ColorableObject>::SaveToFile(File& _file)
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



bool List<ColorableObject>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<ColorableObject>::SaveAsToFile(File& _file, StringANSI _name)
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


void List<ColorableObject>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}




void List<ColorableObject>::SaveItemToFile(MagicList<ColorableObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		ColorableObject* object = _address->GetNode();
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



void List<ColorableObject>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}



void List<ColorableObject>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	ColorableObject* object; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = dynamic_cast<ColorableObject*>(AssetLibrary::_LoadCommonAsset(name, type));
	}
	else
	{
		object = dynamic_cast<ColorableObject*>(AssetLibrary::_LoadPrivateAsset(_file, type));
	}

	if(object)
	{
		AddToTail(object);
	}
}

