#ifndef MYNODE_H
#define MYNODE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/ColorRGBA/ColorRGBA.h> 
#include <Engine/Core/Container/MagicNode.h>
// CLASS --------------------------------------------------------------------------------------------


// узел бинарного дерева
class MyNode: public GraphicsPoint<Vector3D, ColorRGBA> // графическая точка
{
	// DATA
	private: int32 key;

	public: static ColorRGBA DEFAULT_COLOR;// цвет обычных вершин
	public: static ColorRGBA SELECT_COLOR; // цвет выделенных узлов 
	public: static ColorRGBA MARK_COLOR; // цвет отмеченных узлов 
    public: static ColorRGBA LEFT_COLOR; // цвет левого потомка
	public: static ColorRGBA RIGHT_COLOR; // цвет правого потомка
	public: static ColorRGBA EXCESS_COLOR;// цвет для узла на лишнем уровне высоты

	// METHODS
	// конструктор по умолчанию
	public: MyNode(void);


	// инициализирующий конструктор
	public: MyNode(int32 _key);


	// возвращает ключ данного узла
	public: int32 Key(void);
};




#endif
