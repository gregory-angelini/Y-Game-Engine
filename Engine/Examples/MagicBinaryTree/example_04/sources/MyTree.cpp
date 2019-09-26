// INCLUDES -----------------------------------------------------------------------------------------
#include "MyTree.h"
#include <Engine/Math/Angle/Angle.h> 
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
// CLASS --------------------------------------------------------------------------------------------


MyTree::MyTree(void): selected(NIL), marked(NIL), seqIndex(0), play(false), timer(0.0f), animFPS(1), height(0), excessHeight(0)
{	
	minimumAngleBetweenChilds = 10.0f; // при текущей длинне связей между узлами (55.0f), этого расстояния достаточно, чтобы потомки не пересекались
}


void MyTree::ClearAnimSeq(void)
{
	animSeq.Clear();
}


int32 MyTree::AnimSeqSize(void)const
{
	return animSeq.Size();
}


void MyTree::PlayAnim(void)
{
	if(animSeq.Size())
	{
		play = true;
	}
}


void MyTree::StopAnim(void)
{
	play = false;
}

void MyTree::SetAnimFPS(int32 _fps)
{
	animFPS = _fps;
}

int32 MyTree::AnimFPS(void)const
{
	return animFPS;
}


void MyTree::AnimSeqUpdate(float _delta)
{
	if(play && animFPS && animSeq.Size())// если проигрывание анимации разрешено, и FPS больше нуля, и очередь анимации не пуста
	{
		timer += _delta;// обновить таймер

		if(timer >= (1.0f / (float)animFPS))// выполнение с частотой FPS
		{
			timer = 0.0f; // сбросить состояние таймера

			// если анимация закончилась, то прервать функцию
			if(seqIndex >= animSeq.Size()) 
			{ 
				seqIndex = 0; // перейти в начало анимации
				StopAnim();// выключить проигрывание анимации
				return;// прервать функцию
			}
			// иначе анимация продолжается...
			Select(animSeq[seqIndex]->data);// выделить узел под индексом seqIndex
			seqIndex++; // увеличить индекс узла
		}
	}
}

bool MyTree::IsPlaying(void)const
{
	return play;
}



void MyTree::AddToAnimSeq(MyNode* _node)
{
	if(_node) { animSeq.AddToTail(_node); }
}

void MyTree::UpdateColors(MyNode* _node)
{
	if(_node)// если узел существует
	{
		if(_node == marked) { _node->SetColor(MyNode::MARK_COLOR); } // цвет для отмеченных узлов
		if(_node == selected) { _node->SetColor(MyNode::SELECT_COLOR); } // цвет для выделенных узлов
	}
}


void MyTree::Select(MyNode* _node)
{
	selected = _node;// обновить выделенный узел
}



MyNode* MyTree::Selected(void)const
{
	return selected;
}



void MyTree::SetMark(MyNode* _node)
{
	marked = _node;// обновить отмеченный узел
}



MyNode* MyTree::Marked(void)const
{
	return marked;
}

void MyTree::UpdateHeight(void)
{
	height = Height();
}

void MyTree::UpdateExcessHeight(void)
{
	excessHeight = ExcessHeight();
}


int32 MyTree::StaticHeight(void)const
{
	return height;
}


int32 MyTree::StaticExcessHeight(void)const
{
	return excessHeight;
}




void MyTree::Del(MyNode* _node)
{
	if(_node)// если есть выделенный узел
	{
		if(_node == marked) { SetMark(NIL); } // снять метку с удаляемого узла
		if(_node == selected) { Select(NIL); } // снять выделение с удаляемого узла

		//Remove(_node, 0);//исключить выделенный узел; левостороннее исключение
		delete _node; // удалить выделенный узел
	}
}



void MyTree::Draw(void)
{
	Draw(Root(), // точка отсчета визуализации 
		 1, // глубина пустого дерева
		 0.0f, // начальный угол линии для видимого узла 
		 1.0f, // коэффициент деления угла на каждом шаге рекурсии; для видимого узла (1.0f = паук, > 1.0f = дерево, растущее вверх)  
		 70.0f/100.0f); // начальная длина линии для видимого узла (50 оптимально)
}



void MyTree::Draw(MyNode* _node, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength)
{
	/* примечание: каждый узел посещается только один раз; узлы посещаются до того, как посещаются их потомки */ 
	if(_node) // если узел существует 
	{
		// определить цвет узла...
		if(_deep > height - excessHeight)// если узел находится на лишнем уровне высоты
		{
		_node->SetColor(MyNode::EXCESS_COLOR);
		}
		else { _node->SetColor(MyNode::DEFAULT_COLOR); }// цвет для остальных узлов
		UpdateColors(_node);// если узел выделен или отмечен
			

		if(ParentOf(_node))// если есть родитель, значит текущей узел не корень
		{
			ColorRGBA colorLine; // цвет линии, которая соединяет текущий узел с предыдущим 

			// расчет модификатора угла
			float nodeAngleRelativeToTheParent = 360.0f / pow(2.0f, _deep); // угол в градусах для текущего узла, относительно корня; делим окружность (360 градусов) на 2 в степени _deep
			 
			// поворот линии на нужный угол 
			if((nodeAngleRelativeToTheParent / _lineAngleDivisor) < minimumAngleBetweenChilds)// не даем модификатору спуститься ниже minimumAngleBetweenChilds
			{
				nodeAngleRelativeToTheParent = minimumAngleBetweenChilds;
			}
				
			nodeAngleRelativeToTheParent /= _lineAngleDivisor; 
				
	
			if(ChildOf(ParentOf(_node), 1) == _node)// если мы правый потомок 
			{
				_lineAngle += nodeAngleRelativeToTheParent;// итоговый угол для текущего узла
				colorLine = MyNode::RIGHT_COLOR; // цвет линии для правых узлов 
			}
			else// иначе мы левый потомок 
			{
				_lineAngle -= nodeAngleRelativeToTheParent; // итоговый угол для текущего узла 
				colorLine = MyNode::LEFT_COLOR; // цвет линии для левых узлов 
			}
				
			// вычислить координаты узла 
			_node->SetPosition(Vector3D(ParentOf(_node)->GetPosition().x + sinf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
				                        ParentOf(_node)->GetPosition().y + cosf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
								        0.0f));

			// вывод линии 
			GraphicsLine<Vector3D, ColorRGBA>::_Draw(ParentOf(_node)->GetPosition(), _node->GetPosition(), colorLine);
	
			_node->Draw(); // вывод узла на экран 
		}
		else // иначе текущий узел - корень дерева 
		{	
			// задать координаты вывода для корня 
			_node->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));
	
			_node->Draw(); // вывод узла на экран 
		}

		++_deep; // глубина следующего уровня

		// узлы посещаются до того, как посещаются их потомки
		Draw(ChildOf(_node, 0), _deep, _lineAngle, _lineAngleDivisor, _lineLength); 
		Draw(ChildOf(_node, 1), _deep, _lineAngle, _lineAngleDivisor, _lineLength);
	} 
}