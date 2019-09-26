#ifndef ASSETSCENE_H
#define ASSETSCENE_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Object/ObjectBinaryTree/ObjectBinaryTree.h>
class AssetScene: public AbstractObject,
		          public ObjectBinaryTree
{
	public: AssetScene(void);
	public: virtual ~AssetScene(void);
	public: virtual void AddNodeEvent(MagicNode* _node);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};




#endif 