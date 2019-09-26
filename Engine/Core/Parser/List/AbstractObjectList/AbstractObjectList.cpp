
#include "AbstractObjectList.h"
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>



List<AbstractObject>::List(void)
{
	AddClassProperty(OBJECT_LIST_CLASS);
}


List<AbstractObject>::~List(void)
{ 
	Destroying();
}



int32 List<AbstractObject>::GetValueType(void)const
{
	return OBJECT_TYPE;
}



void List<AbstractObject>::SwapIndex(int32 _index1, int32 _index2)
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


void List<AbstractObject>::SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2)
{
	if(_list && _item1 && _item2)
	{
		_Swap(dynamic_cast<AddressDerived*>(_item1->GetAddress(this)), dynamic_cast<AddressDerived*>(_item2->GetAddress(dynamic_cast<List<AbstractObject>*>(_list))));
	}
}


void List<AbstractObject>::SwapList(AbstractList* _list)
{
	if(_list)
	{
		if(GetValueType() == _list->GetValueType())
		{
			Swap(dynamic_cast<List<AbstractObject>*>(_list));
		}
	}
}



bool List<AbstractObject>::IsListEmpty(void)const
{
	return IsEmpty();
}



MagicNode::Address* List<AbstractObject>::AddItem(AbstractObject* _item, int32 _index)
{
	switch(_index)
	{
		case HEAD:
		{
			return AddToHead(_item);
		}

		case TAIL:
		{
			return AddToTail(_item);
		}

		case RANDOM:
		{
			return AddRandom(_item);
		}
			
		default:
		{
			return AddTo(_item, _index);
		}
	}
	return NIL;
}



void List<AbstractObject>::CopyTo(AbstractList* _list, int32 _copyMode)
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
					if(_list->GetValueType() == STRING_TYPE)
					{
						if(GetAmountOfItems() == _list->GetAmountOfItems())
						{
							List<StringANSI>* derivedList = dynamic_cast<List<StringANSI>*>(_list);

							if(derivedList) 
							{
								AddressDerived* it1 = GetHeadAddress();
								List<StringANSI>::AddressDerived* it2 = derivedList->GetHeadAddress();

								while(it1 && it2)
								{
									AbstractObject* left = it1->GetNode();
									Variable<StringANSI>* right = it2->GetNode();
												
									if(left && right)
									{ 
										right->SetValue(left->GetName());
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
					if(_list->GetValueType() == STRING_TYPE)
					{
						List<StringANSI>* derivedList = dynamic_cast<List<StringANSI>*>(_list);

						if(derivedList) 
						{
							AddressDerived* it = GetHeadAddress();

							while(it)
							{
								AbstractObject* right = it->GetNode();
											
								if(right)
								{ 
									derivedList->AddToTail(new Variable<StringANSI>(right->GetName()));
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



int32 List<AbstractObject>::GetAmountOfItems(void)const
{
	return GetSize();
}




AbstractObject* List<AbstractObject>::GetItem(int32 _index)
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


void List<AbstractObject>::RotateItems(bool _right)
{
	Rotate(_right);
}



void List<AbstractObject>::RandomizeItems(int32 _repetitions)
{
	RandomSwap(_repetitions);
}



void List<AbstractObject>::InverseList(void)
{
	Inverse();
}



int32 List<AbstractObject>::GetItemIndex(AbstractObject* _item)
{
	return GetIndexOf(_item);
}


void List<AbstractObject>::RemoveItem(AbstractObject* _item, int32 _removeMode)
{
	Remove(_item, (MagicContainer::ClearMode)_removeMode);
}


void List<AbstractObject>::RemoveIndex(int32 _index, int32 _removeMode)
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



void List<AbstractObject>::ClearList(int32 _removeMode)
{
	Clear((MagicContainer::ClearMode)_removeMode);
}



bool List<AbstractObject>::SaveToFile(StringANSI _path)
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



bool List<AbstractObject>::SaveToFile(File& _file)
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



bool List<AbstractObject>::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + _name + "." + AbstractList::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool List<AbstractObject>::SaveAsToFile(File& _file, StringANSI _name)
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




void List<AbstractObject>::SaveDerivedToFile(File& _file)
{
	_file.Write(GetSize());
	SaveItemToFile(GetHeadAddress(), _file);
}



void List<AbstractObject>::SaveItemToFile(MagicList<AbstractObject>::AddressDerived* _address, File& _file)
{ 
	if(_address)
	{
		AbstractObject* object = _address->GetNode();
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



void List<AbstractObject>::LoadDerivedFromFile(File& _file)
{
	Clear();
	int32 listSize;
	_file.Read(listSize);
	for(int32 i = 0; i < listSize; ++i)
	{
		LoadItemFromFile(_file);
	}
}


void List<AbstractObject>::LoadItemFromFile(File& _file)
{
	bool isCommonAsset;
	_file.Read(isCommonAsset);
	int32 type;
	_file.Read(type);
	AbstractObject* object; 
			
	if(isCommonAsset)
	{
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);

		object = AssetLibrary::_LoadCommonAsset(name, type);
	}
	else
	{
		object = AssetLibrary::_LoadPrivateAsset(_file, type);
	}

	if(object)
	{
		AddToTail(object);
	}
}