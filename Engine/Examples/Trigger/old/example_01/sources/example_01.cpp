// Console.cpp: определяет точку входа для консольного приложения.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include <Engine/Core/Object/Object.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



int main()
{
	// ---------------------------------------- автоматический захват переменных триггером
	// trigger1
	Trigger* trigger1 = new Trigger();
	trigger1->Rename("trigger1");
	trigger1->SetEventExpr("event1");// задать условие активации
	trigger1->SetActionExpr("action1");// задать реакцию 
	AssetLibrary::_GetAssets().AddObject(trigger1);// добавить в библиотеку ассетов
	


	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());
	


	Variable<StringANSI>* var1 = new Variable<StringANSI>("event1");
	var1->Rename("event1Var");
	AssetLibrary::_GetAssets().AddObject(var1);
 


	// создать событие
	printf("\ncreate event1(event1 from trigger1)...\n");
	IEvent* event1 = Trigger::_CreateEvent(Object::SEND_MESSAGE, "#event1Var", true, "#trigger1");
	event1->Rename("event1");
	printf("\nadd event1 to trigger1...\n");
	AssetLibrary::_GetAssets().AddObject(event1);
	


	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());
	

	printf("--------------------------------------------------------\n");
	
	


	// ---------------------------------------- автоматический захват действий триггером + эффект overlap (деактивация условия способ 1) + ручная активация условия (способ 1)

	// создать действие
	printf("\ncreate action1(event3 to trigger3)...\n");

	
	Variable<StringANSI>* var2 = new Variable<StringANSI>("event3");
	var2->Rename("event3Var");
	AssetLibrary::_GetAssets().AddObject(var2);



	IAction* action1 = Trigger::_CreateAction(Object::SEND_MESSAGE, "#event3Var", "#trigger3");
	action1->Rename("action1");
	printf("\nadd action1 to trigger1...\n");
	AssetLibrary::_GetAssets().AddObject(action1);



	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());



	printf("\ntrigger1.send(event1)...\n");
	trigger1->SendPackage(IMessageHandler::SEND_MESSAGE, StringANSI("event1"));
	
		

	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());



 
	// trigger3
	Trigger* trigger3 = new Trigger();
	trigger3->Rename("trigger3");
	trigger3->SetEventExpr("event3");// задать логическое выражение
	trigger3->SetActionExpr("action3");// задать реакцию
	AssetLibrary::_GetAssets().AddObject(trigger3);// добавить в библиотеку ассетов
	
	
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());


	// создать событие
	printf("\ncreate event3(event3 from trigger3)...\n");
	IEvent* event3 = Trigger::_CreateEvent(Object::SEND_MESSAGE, "#event3Var", false, "#trigger3");
	event3->Rename("event3");
	printf("\nadd event3 to trigger3...\n");
	AssetLibrary::_GetAssets().AddObject(event3);

	
	
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());


	Variable<StringANSI>* var3 = new Variable<StringANSI>("event2");
	var3->Rename("event2Var");
	AssetLibrary::_GetAssets().AddObject(var3);
	

	// создать действие
	printf("\ncreate action3(event2 to trigger1)...\n");
	IAction* action3 = Trigger::_CreateAction(Object::SEND_MESSAGE, "#event2Var", "#trigger1");// деактивирует trigger1 за счет эффекта overlap
	action3->Rename("action3");
	printf("\nadd action3 to trigger3...\n");
	AssetLibrary::_GetAssets().AddObject(action3);  
	
	
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());


	printf("\ntrigger::execute...\n");
	Trigger::_ExecuteActions();// выполнить активированные действия


	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());


	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());



	printf("--------------------------------------------------------\n");
	


	// -------------------------------------------------- триггер без условий + сложные условия активации + ручная активация условия (способ 2) + ручная деактивация условий (способ 2) 

	/*
	// второй триггер
	Trigger* trigger2 = new Trigger();
	trigger2->Rename("trigger2");
	trigger2->SetEventExpr("trigger1 & event2");// задать логическое выражение
	AssetLibrary::_GetAssets().AddObject(trigger2);// добавить в библиотеку ассетов
	


	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());

  
	// создать событие
	printf("\ncreate event2(event2 from trigger3)...\n");
	IEvent* event2 = Trigger::_CreateEvent(Object::SEND_MESSAGE, "#event2", true, "trigger3");
	event2->Rename("event2");  
	printf("\nadd event2 to trigger2...\n");
	AssetLibrary::_GetAssets().AddObject(event2);

	

	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());

 

	printf("\ntrigger1.setEventExpr()...\n");
	trigger1->SetEventExpr("");// убрать условия активации
	

	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());



	printf("\nevent2.activate...\n");
	event2->SetValue(true);// активировать событие
	

	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());

 

	

	printf("\ntrigger1.deactivate...\n");
	trigger1->SetValue(false);// деактивировать 
	

	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());

	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());
 
   
	printf("--------------------------------------------------------\n");


	// ----------------------------------------- удаление переменной, являющейся частью условия активации
		

	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());



	printf("\ndelete event3...\n");
	delete event3;// удалить аргумент для триггера trigger3

	
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());
	
	

	printf("--------------------------------------------------------\n");
	
	// ----------------------------------------- переименование условий + активация условия, общего для нескольких триггеров + изменение логического выражения (условий активации)

 
	event1->Rename("event3");// после этого event3 автоматически будет захвачен триггером trigger3 т.к. событие в ходе переименования будет повторно добавлено в библиотеку ассетов под новым именем
	printf("\nevent1.rename(event3)...\n");
	
		
	printf("\ntrigger1.setEventExpr(event3)...\n"); 
	trigger1->SetEventExpr("event3");


	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());
 

	printf("\nevent1.activate...\n");
	event1->SetValue(true);
  

	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());

	
	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());

	

	printf("\nevent3.rename(event4)...\n");
	event1->Rename("event4");
	

	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());


	
	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());
	 

	
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());
	 

		

	// ----------------------------------------- удаление действия + переименование действия

	printf("--------------------------------------------------------\n"); 

	printf("\ndelete action1...\n");
	delete action1;
 
	printf("\naction3.rename(action0)...\n");
	action3->Rename("action0");



	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());

		
	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());
		
	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());

	// удалить оставшиеся события и действия
	delete action3;
	delete event1;
	delete event2;

 
	printf("--------------------------------------------------------\n"); 

	
	// ----------------------------------------------- добавление действий в активированный триггер + уничтожение триггеров

	/*
	// trigger1...
	// создать действие
	printf("\ncreate action2(event1 to trigger3)...\n");
	IAction* action2 = Trigger::_CreateAction(Object::SEND_MESSAGE, "#event1", "trigger3");
	action2->Rename("action2");
	AssetLibrary::_GetAssets().AddObject(action2);

	// создать событие
	printf("\ncreate event1(event1 from trigger1)...\n");
	event1 = Trigger::_CreateEvent(Object::SEND_MESSAGE, "#event1", true, "trigger1");
	event1->Rename("event1");
	AssetLibrary::_GetAssets().AddObject(event1);
	
	

	printf("\ntrigger1.Destroy...\n");
	trigger1->Destroy();
	trigger1->SetEventExpr("event1");// задать условие активации
	trigger1->SetActionExpr("action2");// задать реакцию  



	// trigger2...
	// создать событие
	printf("\ncreate event2(event2 from trigger3)...\n");
	event2 = Trigger::_CreateEvent(Object::SEND_MESSAGE, "#event2", true, "trigger3");
	event2->Rename("event2");  
	AssetLibrary::_GetAssets().AddObject(event2);


	printf("\ntrigger2.Destroy...\n");
	trigger2->Destroy();
	trigger2->SetEventExpr("event2");// задать условие активации

	
	// trigger3...
	printf("\ntrigger3.Destroy...\n");
	trigger3->Destroy();
	trigger3->SetEventExpr("");// задать условие активации
	


	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());

	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());

	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());

	 
	// создать действие
	printf("\ncreate action1(event2 to trigger3)...\n");
	action1 = Trigger::_CreateAction(Object::SEND_MESSAGE, "#event2", "trigger3");
	action1->Rename("action1");
	AssetLibrary::_GetAssets().AddObject(action1);

	printf("\ntrigger3.setActionExpr(action1, action3)...\n");
	trigger3->SetActionExpr("action1, action3");// задать реакцию 

	// создать действие
	printf("\ncreate action3(event1 to trigger1)...\n");
	action3 = Trigger::_CreateAction(Object::SEND_MESSAGE, "#event1", "trigger1");
	action3->Rename("action3");
	AssetLibrary::_GetAssets().AddObject(action3);


	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());

		

	printf("\ntrigger::execute...\n");
	Trigger::_ExecuteActions();// выполнить активированные действия
	// сначала выполняется action1, который активирует trigger2
	// затем выполняется action3 который активирует trigger1
	// затем trigger1 дезактивирует trigger2
	
	

	printf("\ntrigger1.EventExpr: %s\n", trigger1->GetEventExpr().c_str());
	printf("trigger1.ActionExpr: %s\n", trigger1->GetActionExpr().c_str());
	printf("trigger1.IsActivated: %d\n", trigger1->GetValue());
	printf("trigger1.defined actions: %d\n", trigger1->GetAmountOfDefinedActions());
	printf("trigger1.undefined actions: %d\n", trigger1->GetAmountOfUndefinedActions());
	printf("trigger1.defined variables: %d\n", trigger1->GetAmountOfDefinedVariables());
	printf("trigger1.undefined variables: %d\n", trigger1->GetAmountOfUndefinedVariables());

	printf("\ntrigger2.EventExpr: %s\n", trigger2->GetEventExpr().c_str());
	printf("trigger2.ActionExpr: %s\n", trigger2->GetActionExpr().c_str());
	printf("trigger2.IsActivated: %d\n", trigger2->GetValue());
	printf("trigger2.defined actions: %d\n", trigger2->GetAmountOfDefinedActions());
	printf("trigger2.undefined actions: %d\n", trigger2->GetAmountOfUndefinedActions());
	printf("trigger2.defined variables: %d\n", trigger2->GetAmountOfDefinedVariables());
	printf("trigger2.undefined variables: %d\n", trigger2->GetAmountOfUndefinedVariables());


	printf("\ntrigger3.EventExpr: %s\n", trigger3->GetEventExpr().c_str());
	printf("trigger3.ActionExpr: %s\n", trigger3->GetActionExpr().c_str());
	printf("trigger3.IsActivated: %d\n", trigger3->GetValue());
	printf("trigger3.defined actions: %d\n", trigger3->GetAmountOfDefinedActions());
	printf("trigger3.undefined actions: %d\n", trigger3->GetAmountOfUndefinedActions());
	printf("trigger3.defined variables: %d\n", trigger3->GetAmountOfDefinedVariables());
	printf("trigger3.undefined variables: %d\n", trigger3->GetAmountOfUndefinedVariables());

	

	// trigger3...
	printf("\ntrigger3.Destroy...\n");
	trigger3->Destroy();
	trigger3->Destroy();
	

	printf("--------------------------------------------------------\n"); 

	*/ 

	getch();
	return 0;
}