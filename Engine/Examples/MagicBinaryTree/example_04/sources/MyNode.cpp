// INCLUDES -----------------------------------------------------------------------------------------
#include "MyNode.h"
#include "MyTree.h"
// CLASS --------------------------------------------------------------------------------------------


// статические данные
ColorRGBA MyNode::DEFAULT_COLOR = ColorRGBA(ColorRGBA::WHITE_COLOR);
ColorRGBA MyNode::SELECT_COLOR = ColorRGBA(ColorRGBA::GREEN_COLOR);
ColorRGBA MyNode::MARK_COLOR = ColorRGBA(ColorRGBA::YELLOW_COLOR);
ColorRGBA MyNode::LEFT_COLOR = ColorRGBA(ColorRGBA::BLUE_COLOR);
ColorRGBA MyNode::RIGHT_COLOR = ColorRGBA(ColorRGBA::RED_COLOR);
ColorRGBA MyNode::EXCESS_COLOR = ColorRGBA(150, 0, 0, 0);

MyNode::MyNode(void): key(0)
{
	SetColor(DEFAULT_COLOR);// цвет узла
}


MyNode::MyNode(int32 _key): key(_key)
{
	SetColor(DEFAULT_COLOR);// цвет узла
}



int32 MyNode::Key(void)
{
	return key;
}


