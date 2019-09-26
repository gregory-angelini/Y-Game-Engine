#include "AssetScene.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


AssetScene::AssetScene(void)
{}



AssetScene::~AssetScene(void)
{
	Destroying();
}



void AssetScene::AddNodeEvent(MagicNode* _node)
{
	ObjectBinaryTree::AddNodeEvent(_node);

	if(IsReceiversExist(AssetLibrary::CREATE_OBJECT_MESSAGE)) { SendPackage(AssetLibrary::CREATE_OBJECT_MESSAGE, dynamic_cast<AbstractObject*>(_node)); }
}


bool AssetScene::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

				
bool AssetScene::LoadFromFile(File& _file)
{
	return false;
}


