// example_01.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
//#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Template/MessageHandler/MessageHandler.h>
#include "conio.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;


// класс, реализующий событие
class Event
{
	// типы событий
	public: enum 
	{
		MOUSE,
		KEYBOARD,
		DESTROY
	};

	// DATA
	public: int32 type;// тип события
	public: string action;// действие


	// METHODS
	// инициализирующий конструктор
	public: Event(int32 _type, string _action): type(_type), action(_action)
	{}

	// возвращает тип события
	public: virtual int32 Type(void)const
	{
		return type;
	}
};



 
// класс, реализующий получателя
class Receiver: public MessageHandler<int32>::Receiver
{
	// METHODS
	// конструктор по умолчанию
	public: Receiver(void)
	{
		cout<<"Receiver constructor"<<endl;
	}

	// обработчик событий мыши
	public: void MouseMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::MOUSE)
		{
			cout<<"event type: "<<"MOUSE"<<endl;
			cout<<"event action: "<<_event.action.c_str()<<endl;
		}
	}
	

	// обработчик событий клавиатуры
	public: void KeyboardMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::KEYBOARD)
		{
			cout<<"event type: "<<"KEYBOARD"<<endl;
			cout<<"event action: "<<_event.action.c_str()<<endl;
		}
	}

	// обработчик событий уничтожения
	public: void DestroyMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::DESTROY)
		{
			cout<<"event type: "<<"DESTROY"<<endl;
		}
	}
				
	// деструктор
	public: ~Receiver(void)
	{
		cout<<"Receiver destructor"<<endl;
	}
};



int main()
{
	MessageHandler<int32> sender;// создать отправителя сообщений
	

	// добавить первого подписчика...
	Receiver* receiver1 = new Receiver;// создать подписчика
	cout<<"sender.Connect(Event::MOUSE, receiver1)..."<<endl;
	sender.Connect(Event::MOUSE, // сообщение, на которое должен реагировать подписчик
		           receiver1, // адрес подписчика
		           MagicCaller<>(receiver1, &Receiver::MouseMessage));// обработчик сообщений
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl<<endl;
	


	// отправить сообщения...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));

	
	// исключить подписчика...
	cout<<endl<<"sender.Disconnect(Event::MOUSE, receiver1)..."<<endl;
	//receiver1->DisconnectFromAll();
	sender.Disconnect(Event::MOUSE, receiver1);// отключить подписчика от события Event::MOUSE
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;
	
		

	// отправить сообщения...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));


	// добавить второго подписчика...
	Receiver* receiver2 = new Receiver;// создать подписчика; подпишем его сразу на два разных события: Event::KEYBOARD и Event::DESTROY
	cout<<"sender.Connect(Event::KEYBOARD, receiver2)..."<<endl;
	sender.Connect(Event::KEYBOARD, // сообщение, на которое должен реагировать подписчик
		           receiver2, // адрес подписчика
		           MagicCaller<>(receiver2, &Receiver::KeyboardMessage));// обработчик сообщений


	cout<<"sender.Connect(Event::DESTROY, receiver2)..."<<endl;
	sender.Connect(Event::DESTROY, // сообщение, на которое должен реагировать подписчик
		           receiver2, // адрес подписчика
		           MagicCaller<>(receiver2, &Receiver::DestroyMessage));// обработчик сообщений
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;

	cout<<endl<<"sender.Connect(Event::MOUSE, receiver1)..."<<endl;
	sender.Connect(Event::MOUSE, // сообщение, на которое должен реагировать подписчик
		           receiver1, // адрес подписчика
		           MagicCaller<>(receiver1, &Receiver::MouseMessage));// обработчик сообщений
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl<<endl;
	

	// отправить сообщения...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));
	 
	// удалить подписчика...
	cout<<endl<<"delete receiver2..."<<endl;
	delete receiver2;// удалить подписчика
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;
	
		
	// отправить сообщения...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));
	

	cout<<endl<<"sender.IsConnected(Event::MOUSE, receiver1): "<<sender.IsConnected(Event::MOUSE, receiver1)<<endl;

	// отменить всех подписчиков...
	cout<<endl<<"sender.DisconnectAll()..."<<endl;
	sender.DisconnectAll();// отменить всех подписчиков
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;

	// отправить сообщения...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));

	getch();
	return 0;
}