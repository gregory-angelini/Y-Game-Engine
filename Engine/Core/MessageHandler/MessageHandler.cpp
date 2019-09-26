
#include "MessageHandler.h"


MessageHandler<int32>::MessageHandler(void): amountOfHandlers(0)
{
	for(int32 i = 0; i < AMOUNT_OF_HANDLERS; i++)
	{
		handlers[i] = NIL;
	}
}
    

MessageHandler<int32>::~MessageHandler(void)
{
	DisconnectAllReceivers();
}
   

bool MessageHandler<int32>::IsReceiversExist(int32 _message)const
{
	if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
	{
		return handlers[_message] != NIL;
	}
	return false;
}



bool MessageHandler<int32>::IsReceiversExist(void)const
{
	return amountOfHandlers > 0;
}



bool MessageHandler<int32>::IsConnected(int32 _message, MessageHandler<int32>::Receiver* _receiver)
{
	if(_receiver)
	{
		if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
		{
			Handler* handler = handlers[_message];

			if(handler) { return handler->IsConnected(_receiver); }
		}
	}
	return false;
}



int32 MessageHandler<int32>::GetAmountOfMessages(void)const
{
	return amountOfHandlers;
}



bool MessageHandler<int32>::Connect(int32 _message, Receiver* _receiver, const Caller<>& _caller)
{
	if(_receiver)
	{
		if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
		{
			Handler* handler = handlers[_message];

			if(handler == NIL)
			{ 
				handler = new Handler(_message, this);
				handlers[_message] = handler;
				amountOfHandlers++;
			}
			return handler->AddReceiver(_receiver, _caller);
		}
	}
	return false;
}
	 


void MessageHandler<int32>::Disconnect(int32 _message, MessageHandler<int32>::Receiver* _receiver)
{
	if(_receiver)
	{
		if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
		{
			Handler* handler = handlers[_message];

			if(handler)
			{ 
				handler->RemoveReceiver(_receiver);
			} 
		}
	}
}
 

void MessageHandler<int32>::DisconnectAllReceiversFromMessage(int32 _message)
{
	if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
	{
		if(handlers[_message])
		{
			delete handlers[_message];
			handlers[_message] = NIL;
			amountOfHandlers--;
		}
	}
}




void MessageHandler<int32>::DisconnectAllReceivers(void)
{  
	if(amountOfHandlers > 0)
	{
		for(int32 i = 0; i < AMOUNT_OF_HANDLERS; i++)
		{
			if(handlers[i])
			{
				delete handlers[i];
				handlers[i] = NIL;
			}
		}
		amountOfHandlers = 0;
	}
}

