
#include "MessageHandler.h"


MessageHandler<int32>::Receiver::Contact::Contact(void): handler(NIL)
{}


MessageHandler<int32>::Receiver::Contact:: Contact(MessageHandler<int32>::Handler* _handler, const Caller<>& _caller): handler(_handler), caller(_caller)
{}


MessageHandler<int32>::Receiver::Contact::~Contact(void)
{
	if(handler->GetSize() == 1 && !handler->IsDestroying())
	{ 

		handler->Remove(this, MagicContainer::REMOVE_NODE);
		handler->parent->handlers[handler->GetMsg()] = NIL;
		handler->parent->amountOfHandlers--;
		delete handler;
	}  
}


MessageHandler<int32>::Handler* MessageHandler<int32>::Receiver::Contact::GetHandler(void)
{
	return handler;
}