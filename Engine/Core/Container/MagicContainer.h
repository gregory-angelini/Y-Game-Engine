#ifndef MAGICCONTAINER_H
#define MAGICCONTAINER_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/MagicNode.h>
class MagicContainer
{
	public: enum ClearMode
	{ 
		REMOVE_NODE = 0,
		DELETE_NODE = 1,
		DELETE_UNIQUE_NODE = 2
	};
	public: enum Action
	{
		TRANSFER,
		SHARE
	};
	template<typename N> friend class MagicBinaryTree;
	template<typename N> friend class MagicList;
	template<typename N> friend class MagicPointer;
	template<typename N> friend class MagicListEx;
	private: ClearMode removeMode;
	public: MagicContainer(void);
	public: virtual ~MagicContainer(void);
	public: ClearMode GetRemoveMode(void)const;
	public: void SetRemoveMode(ClearMode _removeMode);
	public: virtual void RemoveNodeEvent(MagicNode* _node);
	public: virtual void AddNodeEvent(MagicNode* _node);
	public: virtual int32 GetSize(void)const;
};

#endif
