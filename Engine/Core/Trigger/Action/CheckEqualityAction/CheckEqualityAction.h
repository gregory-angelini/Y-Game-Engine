#ifndef CHECKEQUALITYACTION_H
#define CHECKEQUALITYACTION_H


// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Trigger/EqualityExprParserEx/EqualityExprParserEx.h>
// CLASS --------------------------------------------------------------------------------------------


// класс, реализующий действие CHECK_EQUALITY_MESSAGE
class CheckEqualityAction: public AbstractAction// базовые свойства действия
{
	// дружественные классы
	friend class Trigger;
	friend class AbstractAction;


	// DATA
	private: StringANSI arg1Name;// имя аргумента
    private: EqualityExprParserEx expr;// выражение, в котором закодировано имя аргумента 
    private: MagicPointer<Variable<bool>> arg1;// аргумент действия

	private: StringANSI arg2Name;// имя аргумента
    private: StringExprParserEx templateArg2;// выражение, в котором закодировано имя аргумента 
    private: MagicPointer<AbstractAction> arg2;// аргумент действия

	private: StringANSI arg3Name;// имя аргумента
    private: StringExprParserEx templateArg3;// выражение, в котором закодировано имя аргумента 
    private: MagicPointer<AbstractAction> arg3;// аргумент действия


	// METHODS
	// конструктор без параметров
	private: CheckEqualityAction(void);

		
	// инициализирующий конструктор 
	private: CheckEqualityAction(int32 _type);


	// деструктор
	public: virtual ~CheckEqualityAction(void);


	// АРГУМЕНТЫ...
	// задает аргумент-переменную _name для аргумента _index
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);


	// задает шаблонный аргумент _expr для аргумента _index
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);


	// задает шаблонный аргумент _expr для аргумента _index
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);


	// возвращает шаблонный аргумент _index
	public: virtual StringANSI GetTemplateArg(int32 _index);


	// возвращает шаблонный аргумент _index
	public: virtual StringANSI GetExpressionArg(int32 _index);


	// возвращает переменную-аргумент _index
	public: virtual StringANSI GetVariableArg(int32 _index);


	// АКТИВАЦИЯ...
	// выполняет действие
	// примечание: если действие не определено, то функция будет прервана
	public: virtual void operator () (void);


	// ОБРАБОТКА...
	// реакция на потерю одной из переменных выражения для Arg1
	private: void Arg1IsLost(void);


	// реакция на появление переменных выражения для Arg2
	private: void TemplateArg2IsCompleted(void);


	// реакция на потерю одной из переменных выражения для Arg2
	private: void Arg2IsLost(void);


	// реакция на изменение одной из переменных выражения для Arg2
	private: void TemplateArg2IsChanged(void);


	// реакция на появление переменных выражения для Arg3
	private: void TemplateArg3IsCompleted(void);


	// реакция на потерю одной из переменных выражения для Arg3
	private: void Arg3IsLost(void);


	// реакция на изменение одной из переменных выражения для Arg3
	private: void TemplateArg3IsChanged(void);


	// реакция на уничтожение объекта
	private: void ObjectIsDestroyed(AbstractObject* _object);


    // реакция на переименование аргумента действия
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);


    // реакция на переименование аргумента действия
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);


    // реакция на переименование аргумента действия
	private: void Arg3IsRenamed(AbstractObject::NameInfo* _info);


	// реакция на появление объекта
	private: void ObjectIsCreated(AbstractObject* _object);


	// обновляет валидность действия
	private: void UpdateValid(void);


	// СОХРАНЕНИЕ...
	// сохраняет действие по пути _path, и возвращает true в случае успеха
	// примечание: если _path содержит пустую строку, то действие будет сохранено по имени, в соответствующую директорию 
	public: virtual bool SaveToFile(StringANSI _path = "");


	// сохраняет действие в конец файла _file, и возвращает true в случае успеха
	public: virtual bool SaveToFile(File& _file);

				
	// загружает действие из файла _file, и возвращает на него указатель в случае успеха
	private: static AbstractAction* _LoadFromFile(File& _file);
};



#endif 