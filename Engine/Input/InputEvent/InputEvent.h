#ifndef INPUTEVENT_H
#define INPUTEVENT_H
#include <Engine/Core/Container/MagicNode.h>
#include <Engine/Output/File/File.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
class InputEvent: public MagicNode,
	              public MessageHandler<int32>
{
	friend class EventList;
	friend class PlayerControl;
	public: InputEvent(void);
	public: virtual ~InputEvent(void);
	public: virtual void Destroying(void);
	public: virtual int32 GetEventType(void)const = 0;
	public: virtual int32 GetDeviceType(void)const = 0;
	public: virtual int32 GetMessageId(void)const = 0;
	public: virtual bool SaveToFile(File& _file) = 0;
	public: virtual bool LoadFromFile(File& _file) = 0;
};


#endif 