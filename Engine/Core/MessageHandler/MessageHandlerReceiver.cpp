
#include "MessageHandler.h"


MessageHandler<int32>::Receiver::Receiver(void)
{
	contacts.SetRemoveMode(MagicContainer::DELETE_NODE);
}


MessageHandler<int32>::Receiver::~Receiver(void)
{}
		


int32 MessageHandler<int32>::Receiver::GetAmountOfContacts(void)const
{
	return contacts.GetSize();
}


MessageHandler<int32>::Receiver::Contact* MessageHandler<int32>::Receiver::CreateContact(MessageHandler<int32>::Handler* _handler, const Caller<>& _caller)
{ 
	if(!contacts.Find(_handler, &Receiver::Contact::GetHandler))
	{
		Contact* contact = new Contact(_handler, _caller);
		if(contacts.Add(contact, &Receiver::Contact::GetHandler))
		{
			return contact;
		}
	}
	return NIL;
}


void MessageHandler<int32>::Receiver::DisconnectFromAllMessages(void)
{
	contacts.Clear();
}