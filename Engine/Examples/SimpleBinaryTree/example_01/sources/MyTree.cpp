// INCLUDES -----------------------------------------------------------------------------------------
#include "MyTree.h"
#include <Engine/Math/Angle/Angle.h> 
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
// CLASS --------------------------------------------------------------------------------------------


MyTree::MyTree(void): selected(NIL), marked(NIL)
{	
	minimumAngleBetweenChilds = 10.0f; // при текущей длинне связей между узлами (55.0f), этого расстояния достаточно, чтобы потомки не пересекались
}



void MyTree::Select(MyTree::Node* _node)
{
	selected = _node;// обновить выделенный узел
}



MyTree::Node* MyTree::Selected(void)const
{
	return selected;
}



void MyTree::SetMark(MyTree::Node* _node)
{
	marked = _node;// обновить отмеченный узел
}



MyTree::Node* MyTree::Marked(void)const
{
	return marked;
}


void MyTree::Del(MyTree::Node* _node)
{
	if(_node)// если есть выделенный узел
	{
		if(_node == marked) { SetMark(NIL); } // снять метку с удаляемого узла
		if(_node == selected) { Select(NIL); } // снять выделение с удаляемого узла

		Remove(_node);//исключить выделенный узел; левостороннее исключение
	}
}



void MyTree::Draw(void)
{
	Draw(Root(), // точка отсчета визуализации 
		 Vector3D(), 
		 1, // глубина пустого дерева
		 0.0f, // начальный угол линии для видимого узла 
		 1.0f, // коэффициент деления угла на каждом шаге рекурсии; для видимого узла (1.0f = паук, > 1.0f = дерево, растущее вверх)  
		 70.0f/100.0f); // начальная длина линии для видимого узла (50 оптимально)
}

 

void MyTree::Draw(MyTree::Node* _node, Vector3D _parentPos, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength)
{
	/* примечание: каждый узел посещается только один раз; узлы посещаются до того, как посещаются их потомки */ 
	if(_node) // если узел существует 
	{
		RGBA color = RGBA(RGBA::WHITE_COLOR); // цвет для узлов
		Vector3D nodePos;// координаты узла _node
		
		// обновить цвет узла
		if(_node)// если узел существует
		{
			if(_node == marked) { color = RGBA(RGBA::YELLOW_COLOR); } // цвет для отмеченных узлов
			if(_node == selected) { color = RGBA(RGBA::GREEN_COLOR); } // цвет для выделенных узлов
		}

		if(_node->Parent())// если есть родитель, значит текущей узел не корень
		{
			RGBA colorLine; // цвет линии, которая соединяет текущий узел с предыдущим 

			// расчет модификатора угла
			float nodeAngleRelativeToTheParent = 360.0f / pow(2.0f, _deep); // угол в градусах для текущего узла, относительно корня; делим окружность (360 градусов) на 2 в степени _deep
			 
			// поворот линии на нужный угол 
			if((nodeAngleRelativeToTheParent / _lineAngleDivisor) < minimumAngleBetweenChilds)// не даем модификатору спуститься ниже minimumAngleBetweenChilds
			{
				nodeAngleRelativeToTheParent = minimumAngleBetweenChilds;
			}
				
			nodeAngleRelativeToTheParent /= _lineAngleDivisor; 
				
	
			if(_node->Parent()->Child(1) == _node)// если мы правый потомок 
			{
				_lineAngle += nodeAngleRelativeToTheParent;// итоговый угол для текущего узла
				colorLine = RGBA(RGBA::RED_COLOR); // цвет линии для правых узлов 
			}
			else// иначе мы левый потомок 
			{
				_lineAngle -= nodeAngleRelativeToTheParent; // итоговый угол для текущего узла 
				colorLine =RGBA(RGBA::BLUE_COLOR); // цвет линии для левых узлов 
			}
				
			// вычислить координаты узла 
			nodePos = Vector3D(_parentPos.x + sinf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
				               _parentPos.y + cosf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
							   0.0f);

			// вывод линии 
			GraphicsLine<Vector3D>::_Draw(_parentPos, nodePos, colorLine);
	

			GraphicsPoint<Vector3D>::_Draw(nodePos, color);// вывод узла на экран 
		}
		else // иначе текущий узел - корень дерева 
		{	
			// задать координаты вывода для корня 
			nodePos = Vector3D(0.0f, 0.0f, 0.0f);
	
			GraphicsPoint<Vector3D>::_Draw(nodePos, color);// вывод узла на экран 
		}

		++_deep; // глубина следующего уровня

		// узлы посещаются до того, как посещаются их потомки
		Draw(_node->Child(0), nodePos, _deep, _lineAngle, _lineAngleDivisor, _lineLength); 
		Draw(_node->Child(1), nodePos, _deep, _lineAngle, _lineAngleDivisor, _lineLength);
	} 
}