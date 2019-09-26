// Console.cpp: определяет точку входа для консольного приложения.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/MagicCaller/MagicCaller.h>
#include <Engine/Core/Object/Object.h>
#include <Engine/Core/ObjectScene/ObjectScene.h>



// пользователький объект
class MyObject: public Object,//базовые свойства объекта
	            public MessageHandler<int32>::Receiver// получатель сообщений
{
	// METHODS
	// конструктор по умолчанию
	public: MyObject(void)
	{}

	// виртуальный деструктор
	public: virtual ~MyObject(void)
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
class MyScene: public MessageHandler<int32>::Receiver,// получатель сообщений
	           public ObjectScene// объектная сцена; т.к. сцена реагирует на саму себя (подключается к своим же событиям), то деструктор ObjectScene (исключение объектов сцены) должен быть вызван до деструктора Receiver (реакции подписчика), иначе объекты сцены будут исключены без всякой реакции на это исключение (подписчик будет уничтожаться раньше чем происходить событие, на которое он должен реагировать); в связи с тем, что деструкторы вызываются в порядке: от родительских классов к базовым, то самым верхний родителем является самый последний унаследованный класс
{
	// METHODS
	// виртуальный деструктор
	public: virtual ~MyScene(void)
	{
		printf("~scene %s\n", GetName().c_str());
		//Destroying();// объект начинается уничтожаться
	}

	// реакция на добавление объекта в сцену
	public: void AddedObjectEvent(Object* _object)
	{
		printf("%s.added: %s\n", GetName().c_str(), _object->GetName().c_str());
	}

				
	// реакция на исключение объекта из сцены
	public: void RemovedObjectEvent(Object* _object)
	{
		printf("%s.removed: %s\n", GetName().c_str(), _object->GetName().c_str());
	}
};


// отображает имя объекта _object
void PrintObjectName(Object* _object)
{
	printf("object name: %s\n", _object->GetName().c_str());
}


// отображает информацию о сцене
void PrintScene(MyScene* _scene)
{
	printf("scene %s...\n", _scene->GetName().c_str());
	printf("scene size: %d\n", _scene->AmountOfObjects());
	_scene->GetObjectBinaryTree().InfixTraverse(MagicCaller<>(&PrintObjectName));
	
}

int main()
{
	MyScene* scene1 = new MyScene();// создать сцену 1
	scene1->Rename("scene1");
	scene1->Connect(ObjectScene::ADD_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyScene::AddedObjectEvent));// подписаться на событие добавления объекта в сцену
	scene1->Connect(ObjectScene::REMOVE_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyScene::RemovedObjectEvent));// подписаться на событие исключения объекта из сцены


	MyObject* object1 = new MyObject();// создать объект 1
	object1->Rename("object1");
	object1->WriteToLogFile("");
	object1->Connect(Object::RENAME_OBJECT_MESSAGE, object1, MagicCaller<>(object1, &MyObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	scene1->AddObject(object1);// добавить объект в сцену



	MyObject* object2 = new MyObject();// создать объект 2
	object2->Rename("object2");
	object2->WriteToLogFile("");
	object2->Connect(Object::RENAME_OBJECT_MESSAGE, object2, MagicCaller<>(object2, &MyObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	scene1->AddObject(object2);// добавить объект в сцену


	MyScene* scene2 = new MyScene();// создать сцену 2
	scene2->Rename("scene2");
	scene2->WriteToLogFile("");
	scene2->Connect(ObjectScene::ADD_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyScene::AddedObjectEvent));// подписаться на событие добавления объекта в сцену
	scene2->Connect(ObjectScene::REMOVE_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyScene::RemovedObjectEvent));// подписаться на событие исключения объекта из сцены


	MyObject* object3 = new MyObject();// создать объект 3
	object3->Rename("object3");
	object3->WriteToLogFile("");
	object3->Connect(Object::RENAME_OBJECT_MESSAGE, object3, MagicCaller<>(object3, &MyObject::RenamedObjectEvent));// подписаться на событие переименования объекта
	scene2->AddObject(object3);// добавить объект в сцену

	scene1->AddObject(scene2);// добавить сцену в другую сцену
	scene1->GetObjectBinaryTree().AddObject(object3);// добавить объект в сцену
	// --------------------------------------- отобразить состояние сцен

	PrintScene(scene1);
	PrintScene(scene2);
	printf("\n");

	// --------------------------------------- переименовать объект 1
	
	printf("object renaming...\n");
	object1->Rename("object0");
	object2->Rename("object0");// переименование будет отклонено, т.к. такое имя уже занято
	
	PrintScene(scene1);
	PrintScene(scene2);
	printf("\n");

	// --------------------------------------- исключение объектов из сцены

	printf("remove object1 from scene1...\n");
	delete object1->GetAddress(&scene1->GetObjectBinaryTree());// будет сгенерировано событие исключения из контейнера
	object1->Rename("object10");// сцена scene1 больше не интересуется новым именем объекта
	printf("delete object10...\n");
	delete object1;

	printf("remove object2...\n");
	scene1->GetObjectBinaryTree().Remove(object2, MagicContainer::REMOVE_NODE);// будет сгенерировано событие исключения из scene1 объекта object2


	printf("scene1.Clear()...\n");
	scene1->Destroy();// будет сгенерировано событие исключения из scene2 объекта object3


	getch();
	return 0;
}