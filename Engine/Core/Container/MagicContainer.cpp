
#include "MagicContainer.h"
#include <Engine/Core/Container/MagicNode.h>

MagicContainer::MagicContainer(void): removeMode(DELETE_UNIQUE_NODE)
{}


MagicContainer::~MagicContainer(void)
{}


void MagicContainer::RemoveNodeEvent(MagicNode* _node)
{}


void MagicContainer::AddNodeEvent(MagicNode* _node)
{}


MagicContainer::ClearMode MagicContainer::GetRemoveMode(void)const
{
	return removeMode;
}


void MagicContainer::SetRemoveMode(MagicContainer::ClearMode _removeMode)
{
	removeMode = _removeMode;
}


int32 MagicContainer::GetSize(void)const
{
	return 0;
}