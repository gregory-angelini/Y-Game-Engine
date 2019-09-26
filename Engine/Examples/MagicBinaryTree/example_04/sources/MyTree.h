#ifndef MYTREE_H
#define MYTREE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/ColorRGBA/ColorRGBA.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include "MyNode.h"
// CLASS --------------------------------------------------------------------------------------------


// бинарное магическое дерево
class MyTree: public MagicBinaryTree<MyNode, int32>
{
	// DATA
	private: float minimumAngleBetweenChilds; // минимально допустимый угол между потомками одного узла 
	private: MyNode* selected;// выделенный узел
	private: MyNode* marked;// отмеченный узел
	private: SimpleList<MyNode*> animSeq;// очередь анимации
	private: int32 seqIndex; // индекс узла, который анимируется в данный момент
	private: bool play;// если true, то разрешается анимация очереди
	private: float timer;//таймер, осуществляющий синхронизацию анимации со временем
	private: int32 animFPS;// скорость анимации (количество кадров в секунду)
	private: int32 height;// высота дерева 
	private: int32 excessHeight; // лишние уровни высоты дерева


	// METHODS
	// конструктор по умолчанию
	public: MyTree(void);


	// очистить очередь анимации
	public: void ClearAnimSeq(void);


	// добавляет узел _node в конец очереди анимации
	public: void AddToAnimSeq(MyNode* _node);



	// возвращает размер очереди анимации
	public: int32 AnimSeqSize(void)const;


	// анимирование очереди анимации
	// примечание: в качестве _delta передается промежуток времени (в секундах) между текущим и предыдущим вызовом функции кадра
	// примечание: очередь анимации не должна быть пустой
	// примечание: после проигрывания всей очереди, анимация будет преостановлена
	public: void AnimSeqUpdate(float _delta);


	// возвращает true если анимация проигрывается
	public: bool IsPlaying(void)const;


	// разрешает проигрывание очереди анимации
	// примечание: если очередь анимации пуста, то проигрывание будет отменено
	// примечание: по умолчанию анимация запрещена
	public: void PlayAnim(void);


	// запрещает проигрывание очереди анимации
	public: void StopAnim(void);


	// задает новую скорость анимации в FPS (количество кадров в секунду)
	public: void SetAnimFPS(int32 _fps);


	// возвращает высоту дерева
	public: int32 StaticHeight(void)const;

			
	// обновить высоту дерева
	public: void UpdateHeight(void);


	// возвращает количество лишних уровней высоты при текущем количестве узлов 
	public: int32 StaticExcessHeight(void)const;


	// обновить количество лишних уровней высоты при текущем количестве узлов 
	public: void UpdateExcessHeight(void);


	// возвращает скорость анимации в FPS (количество кадров в секунду)
	public: int32 AnimFPS(void)const;



	// обновляет цвета узла _node
	// примечание: выделенные узлы перекрывают отмеченные узлы по цвету; выделенные и отмеченные узлы перекрывают узлы на лишних уровнях дерева по цвету
	private: void UpdateColors(MyNode* _node);


	// выделить узел _node
	// примечание: выделенный узел будет иметь зеленый цвет
	// примечание: выделение с предыдущего узла будет снято, таким образом, в один момент времени может быть выделен только один узел
	public: void Select(MyNode* _node);


	// возвращает выделенный узел
	public: MyNode* Selected(void)const;


	// отметить узел _node
	// примечание: отмеченный узел будет иметь желтый цвет
	// примечание: метка с предыдущего узла будет снято, таким образом, в один момент времени может быть отмечен только один узел
	public: void SetMark(MyNode* _node);


	// возвращает отмеченный узел
	public: MyNode* Marked(void)const;


	// удалить узел _node из дерева
	// примечание: если удаляемый узел отмечен и выделен, то метка и выделение будут сняты
	public: void Del(MyNode* _node);



	// визуализирует бинарное дерево 
    public: void Draw(void);



	// рекурсивная функция, которая осуществляет визуализацию дерева
    private: void Draw(MyNode* _node, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength);
};

#endif
