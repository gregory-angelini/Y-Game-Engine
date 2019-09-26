#ifndef MYTREE_H
#define MYTREE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
//#include "Node.h"
// CLASS --------------------------------------------------------------------------------------------


// бинарное магическое дерево
class MyTree: public SimpleBinaryTree<int32>
{
	// DATA
	private: float minimumAngleBetweenChilds; // минимально допустимый угол между потомками одного узла 
	private: Node* selected;// выделенный узел
	private: Node* marked;// отмеченный узел


	// METHODS
	// конструктор по умолчанию
	public: MyTree(void);




	// выделить узел _node
	// примечание: выделенный узел будет иметь зеленый цвет
	// примечание: выделение с предыдущего узла будет снято, таким образом, в один момент времени может быть выделен только один узел
	public: void Select(Node* _node);


	// возвращает выделенный узел
	public: Node* Selected(void)const;


	// отметить узел _node
	// примечание: отмеченный узел будет иметь желтый цвет
	// примечание: метка с предыдущего узла будет снято, таким образом, в один момент времени может быть отмечен только один узел
	public: void SetMark(Node* _node);


	// возвращает отмеченный узел
	public: Node* Marked(void)const;


	// удалить узел _node из дерева
	// примечание: если удаляемый узел отмечен и выделен, то метка и выделение будут сняты
	public: void Del(Node* _node);



	// визуализирует бинарное дерево 
    public: void Draw(void);



	// рекурсивная функция, которая осуществляет визуализацию дерева
    private: void Draw(Node* _node, Vector3D _parentPos, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength);
};

#endif
