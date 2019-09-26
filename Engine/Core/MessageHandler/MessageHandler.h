#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/Caller/Caller.h>
template<typename T> class MessageHandler
{};
template<> class MessageHandler<int32>
{
	public: enum
	{
		SEND_MESSAGE = 20
	};
	private: enum
	{
		AMOUNT_OF_HANDLERS = 350
	};
	private: class Handler;
	public: class Receiver
	{
		friend class MessageHandler;
	    private: class Contact: public MagicNode
		{
			friend class MessageHandler;
			private: Caller<> caller;
		    private: Handler* handler;
			public: Contact(void);
			public: Contact(Handler* _handler, const Caller<>& _caller);
			public: virtual ~Contact(void);
			public: Handler* GetHandler(void);
		};
		private: MagicBinaryTree<Contact> contacts;
		public: Receiver(void);
		public: virtual ~Receiver(void);
		private: Contact* CreateContact(Handler* _handler, const Caller<>& _caller);
		public: void DisconnectFromAllMessages(void);
		public: int32 GetAmountOfContacts(void)const;
	};
	private: class Handler: public MagicList<typename Receiver::Contact>,
				            public MagicNode
	{
		friend class Receiver::Contact;
		private: int32 message;
    	private: bool isDestroying;
		private: MessageHandler* parent;
		public: Handler(void);
		public: Handler(int32 _message, MessageHandler* _parent);
		public: virtual ~Handler(void);
		public: int32 GetMsg(void);
		public: bool IsDestroying(void)const;
	    public: bool IsConnected(Receiver* _receiver);
		public: bool AddReceiver(Receiver* _receiver, const Caller<>& _caller);
		public: void RemoveReceiver(Receiver* _receiver);
	};
	friend class Handler;
	friend class Receiver::Contact;
	private: Handler* handlers[AMOUNT_OF_HANDLERS];
	private: int32 amountOfHandlers;
	public: MessageHandler(void);
	public: virtual ~MessageHandler(void);
	public: bool IsReceiversExist(void)const;
	public: bool IsConnected(int32 _message, Receiver* _receiver);
	public: int32 GetAmountOfMessages(void)const;
	public: bool IsReceiversExist(int32 _message)const;
	public: bool Connect(int32 _message, Receiver* _receiver, const Caller<>& _caller);
	public: void Disconnect(int32 _message, Receiver* _receiver);
	public: void DisconnectAllReceiversFromMessage(int32 _message);
	public: void DisconnectAllReceivers(void);
	public: void SendMessage(int32 _message)
	{
		if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
		{
			if(handlers[_message])
			{ 
				MagicList<typename Receiver::Contact> receivers;
				handlers[_message]->Share(receivers);
				Receiver::Contact* contact;
				while(receivers.IsNotEmpty())
				{ 
					contact = receivers.GetHead();
					delete receivers.GetHeadAddress();
					contact->caller();
				}
			}
		}
	}
	public: template<typename N> void SendPackage(int32 _message, N _package)
    {
		if(_message > 0 && _message < AMOUNT_OF_HANDLERS)
		{
			if(handlers[_message])
			{ 
				MagicList<typename Receiver::Contact> receivers;
				handlers[_message]->Share(receivers);
				Receiver::Contact* contact;
				while(receivers.IsNotEmpty())
				{ 
					contact = receivers.GetHead();
					delete receivers.GetHeadAddress();
					contact->caller(_package);
				}
			}
		}
	}
};


#endif
