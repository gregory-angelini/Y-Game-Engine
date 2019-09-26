// Console.cpp: определяет точку входа для консольного приложения.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/MagicCaller/MagicCaller.h>
#include <Engine/Core/LogicScene/LogicScene.h>



// пользователький объект
class MyLogicObject: public LogicObject,//базовые свойства объекта
	                 public MessageHandler<int32>::Receiver// получатель сообщений
{
	// METHODS
	// конструктор по умолчанию
	public: MyLogicObject(void)
	{}

	// виртуальный деструктор
	public: virtual ~MyLogicObject(void)
	{
		printf("~object %s\n", GetName().c_str());
		Destroying();// объект начинается уничтожаться
	}
				
			
	// реакция на переименование объекта
	public: void RenamedObjectEvent(Object::ObjectRenameInfo* _info)
	{
		printf("old name: %s, new name: %s\n", _info->oldName.c_str(), _info->newName.c_str());
	}
};

// пользовательская сцена
class MyLogicScene: public MessageHandler<int32>::Receiver,// получатель сообщений
	                public LogicScene// т.к. сцена реагирует на саму себя (подключается к своим же событиям), то деструктор LogicScene (исключение объектов сцены) должен быть вызван до деструктора Receiver (реакции подписчика), иначе объекты сцены будут исключены без всякой реакции на это исключение (подписчик будет уничтожаться раньше чем происходить событие, на которое он должен реагировать); в связи с тем, что деструкторы вызываются в порядке: от родительских классов к базовым, то самым верхний родителем является самый последний унаследованный класс
{
	// METHODS
	// виртуальный деструктор
	public: virtual ~MyLogicScene(void)
	{
		printf("~scene %s\n", GetName().c_str());
		//Destroying();// объект начинается уничтожаться
	}

	// реакция на добавление объекта в сцену
	public: void AddedLogicObjectEvent(LogicObject* _object)
	{
		printf("%s.added: %s\n", GetName().c_str(), _object->GetName().c_str());
	}

				
	// реакция на исключение объекта из сцены
	public: void RemovedLogicObjectEvent(LogicObject* _object)
	{
		printf("%s.removed: %s\n", GetName().c_str(), _object->GetName().c_str());
	}
};


// отображает имя логического объекта _object
void PrintLogicObjectName(LogicObject* _object)
{
	printf("object name: %s\n", _object->GetName().c_str());
}


// отображает информацию о логической сцене
void PrintLogicScene(MyLogicScene* _scene)
{
	printf("scene %s...\n", _scene->GetName().c_str());
	printf("scene size: %d\n", _scene->AmountOfLogicObjects());
	_scene->GetUpdateList().PrefixTraverse(MagicCaller<>(&PrintLogicObjectName));
	
}

int main()
{
	MyLogicScene* scene1 = new MyLogicScene();// создать сцену 1
	scene1->Rename("scene1");
	scene1->Connect(LogicScene::ADD_LOGIC_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyLogicScene::AddedLogicObjectEvent));// подписаться на событие добавления объекта в сцену
	scene1->Connect(LogicScene::REMOVE_LOGIC_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyLogicScene::RemovedLogicObjectEvent));// подписаться на событие исключения объекта из сцены


	MyLogicObject* object1 = new MyLogicObject();// создать объект 1
	object1->Rename("object1");
	object1->Connect(Object::RENAME_OBJECT_MESSAGE, object1, MagicCaller<>(object1, &MyLogicObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	scene1->AddObject(object1);
	scene1->AddLogicObject(object1);// добавить объект в сцену



	MyLogicObject* object2 = new MyLogicObject();// создать объект 2
	object2->Rename("object2");
	object2->Connect(Object::RENAME_OBJECT_MESSAGE, object2, MagicCaller<>(object2, &MyLogicObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	scene1->AddObject(object2);
	scene1->AddLogicObject(object2);// добавить объект в сцену


	MyLogicScene* scene2 = new MyLogicScene();// создать сцену 2
	scene2->Rename("scene2");
	scene2->Connect(LogicScene::ADD_LOGIC_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyLogicScene::AddedLogicObjectEvent));// подписаться на событие добавления объекта в сцену
	scene2->Connect(LogicScene::REMOVE_LOGIC_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyLogicScene::RemovedLogicObjectEvent));// подписаться на событие исключения объекта из сцены


	MyLogicObject* object3 = new MyLogicObject();// создать объект 3
	object3->Rename("object3");
	object3->Connect(Object::RENAME_OBJECT_MESSAGE, object3, MagicCaller<>(object3, &MyLogicObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	
	scene2->AddObject(object3);
	scene2->AddLogicObject(object3);// добавить объект в сцену

	scene1->AddObject(scene2);
	scene1->AddLogicObject(scene2);// добавить объект в сцену
	
	scene1->AddObject(object3);
	scene1->AddLogicObject(object3);// добавить объект в сцену

	
	// --------------------------------------- отобразить состояние сцен

	PrintLogicScene(scene1);
	PrintLogicScene(scene2);
	printf("\n");

		
	// --------------------------------------- переименовать объект 1
	
	printf("object renaming...\n");
	object1->Rename("object0");
	object2->Rename("object0");// переименование будет отклонено, т.к. такое имя уже занято
	
	PrintLogicScene(scene1);
	PrintLogicScene(scene2);
	printf("\n");
	// --------------------------------------- исключение объектов из сцены


	printf("remove object0...\n");
	delete object1->GetAddress(&scene1->GetUpdateList());// будет сгенерировано событие исключения из scene1 объекта object1
	
	object1->Rename("object10");// сцена scene1 больше не интересуется новым именем объекта

	printf("delete object10...\n");
	delete object1;

	printf("remove object2...\n");
	scene1->GetUpdateList().Remove(object2, MagicContainer::REMOVE_NODE);// будет сгенерировано событие исключения из scene1 объекта object2


	printf("scene1.Clear()...\n");
	scene1->Destroy();// будет сгенерировано событие исключения из scene2 объекта object3


	getch();
	return 0;
}