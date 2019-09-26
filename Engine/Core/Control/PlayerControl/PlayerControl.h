#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Window/Window.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
#include <Engine/Input/Mouse/Mouse.h>
#include <Engine/Input/Keyboard/Keyboard.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
class PlayerControl: public AbstractObject
{
	private: class EventList: public MagicList<InputEvent>,
				              public MessageHandler<int32>::Receiver
	{
		friend class PlayerControl;
	    private: PlayerControl* playerControl;
		public: EventList(void);
	    public: virtual ~EventList(void);
		public: virtual void RemoveNodeEvent(MagicNode* _node);
		private: void DestroyInputEvent(InputEvent* _event);
		private: void DisconnectFrom(InputEvent* _event);
		public: virtual void AddNodeEvent(MagicNode* _node);
	};
	public: enum ActivationMode
	{
		SINGLE,
		COMBO,
		SEQUENCE
	};
	public: enum
	{
		PLAYER_CONTROL_FINISH_MESSAGE = 25,
		ADD_INPUT_EVENT_MESSAGE = 316,
		CLEAR_INPUT_EVENTS_MESSAGE = 317,
		CREATE_PLAYER_CONTROL_MESSAGE = 318,
		CHANGE_INPUT_TIMING_MESSAGE = 319,
		SET_ONE_INPUT_EVENT_MESSAGE = 320
	};
	private: enum
	{
		DESTROY_PLAYER_CONTROL_FINISH_MESSAGE = 49
	};
	public: enum: int64
	{
		PLAYER_CONTROL_CLASS = 67108864
	};
	friend class InputEvent;
	private: EventList events;
	private: ActivationMode activationMode;
	private: AsyncTimer timer;
	private: float timing;
	private: MagicList<InputEvent> activatedEvents;
	private: MagicList<InputEvent> checkingEvents;
    private: static StringANSI playerControlsSuffix;
    private: static StringANSI playerControlsDir;
	public: PlayerControl(void);
    public: virtual ~PlayerControl(void);
	public: virtual bool IsEquals(const Mouse::Event& _event1, const Mouse::Event& _event2)const;
	private: void MouseEventFilter(Mouse::Event _event);
	public: virtual bool IsEquals(const Keyboard::Event& _event1, const Keyboard::Event& _event2)const;
	private: void KeyboardEventFilter(Keyboard::Event _event);
	private: void ComboUpdate(InputEvent* _event);
	private: void SequenceUpdate(InputEvent* _event);
	private: bool IsActivated(InputEvent* _event);
    public: MagicList<InputEvent>& GetActivatedEvents(void);
    public: MagicList<InputEvent>& GetEvents(void);
	private: void Activate(void);
	private: bool IsActivated(void);
	private: void Deactivate(void);
	public: void AddEvent(InputEvent* _event);
	public: ActivationMode GetActivationMode(void)const;
	public: void SetActivationMode(ActivationMode _mode);
	public: float GetTimingInSec(void)const;
	public: void SetTimingInSec(float _timing);
	public: float GetRemainedTimeInSec(void);
	public: virtual void Destroy(void);
	public: bool IsExist(void)const;
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static PlayerControl* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static PlayerControl* _LoadFromFile(File& _file);
};


#endif 
