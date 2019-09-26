
#include "MessageHandler.h"


MessageHandler<int32>::Handler::Handler(void): message(0), isDestroying(false), parent(NIL)
{ 
	SetRemoveMode(MagicContainer::DELETE_NODE);
}


MessageHandler<int32>::Handler::Handler(int32 _message, MessageHandler* _parent): message(_message), isDestroying(false), parent(_parent)
{ 
	SetRemoveMode(MagicContainer::DELETE_NODE);
}


MessageHandler<int32>::Handler::~Handler(void)
{
	isDestroying = true;
}


int32 MessageHandler<int32>::Handler::GetMsg(void)
{
	return message;
}



bool MessageHandler<int32>::Handler::IsDestroying(void)const
{
	return isDestroying;
}


bool MessageHandler<int32>::Handler::IsConnected(MessageHandler<int32>::Receiver* _receiver)
{
	if(_receiver)
	{
		return _receiver->contacts.Find(this, &Receiver::Contact::GetHandler) ? true : false;
	}
	return false;
}


bool MessageHandler<int32>::Handler::AddReceiver(MessageHandler<int32>::Receiver* _receiver, const Caller<>& _caller)
{  
	Receiver::Contact* contact = _receiver->CreateContact(this, _caller);
	if(AddToTail(contact)) 
	{
		return true;
	}
	return false;
}

 

void MessageHandler<int32>::Handler::RemoveReceiver(Receiver* _receiver)
{ 
	delete _receiver->contacts.Find(this, &Receiver::Contact::GetHandler);
}  

