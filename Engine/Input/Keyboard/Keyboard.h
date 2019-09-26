#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <windows.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
class Window;
class Keyboard
{
	public: enum Key
	{
		NO_KEY        = 0,
        ESC           = 0x1B,
		BACKSPACE     = 0x08,
		TAB           = 0x09,
        ENTER         = 0x0D,
		SPACE         = 0x20,
		SHIFT         = 0x10,
        CTRL          = 0x11,
		ALT           = 0x12,
		LEFT_WINDOW   = 0x5B,
        RIGHT_WINDOW  = 0x5C,
		APPS          = 0x5D,
		PAUSE         = 0x13,
        CAPS_LOCK     = 0x14,
		NUM_LOCK      = 0x90,
		SCROLL_LOCK   = 0x91,
		PAGE_UP       = 0x21,
		PAGE_DOWN     = 0x22,
		HOME          = 0x24,
        END           = 0x23,
		PRINT_SCREEN  = 0x2C,
		INSERT        = 0x2D,
		DEL           = 0x2E,
        LEFT_ARROW    = 0x25,
		UP_ARROW      = 0x26,
		RIGHT_ARROW   = 0x27,
        DOWN_ARROW    = 0x28,
		NUM_0         = 0x30,
		NUM_1         = 0x31,
        NUM_2         = 0x32,
		NUM_3         = 0x33,
		NUM_4         = 0x34,
        NUM_5         = 0x35,
		NUM_6         = 0x36,
		NUM_7         = 0x37,
        NUM_8         = 0x38,
		NUM_9         = 0x39,
		A             = 0x41,
        B             = 0x42,
		C             = 0x43,
		D             = 0x44,
        E             = 0x45,
		F             = 0x46,
		G             = 0x47,
        H             = 0x48,
		I             = 0x49,
		J             = 0x4A,
        K             = 0x4B,
		L             = 0x4C,
		M             = 0x4D,
        N             = 0x4E,
		O             = 0x4F,
		P             = 0x50,
        Q             = 0x51,
		R             = 0x52,
		S             = 0x53,
        T             = 0x54,
		U             = 0x55,
		V             = 0x56,
        W             = 0x57,
		X             = 0x58,
		Y             = 0x59,
        Z             = 0x5A,
		GRAVE         = 0xC0,
		MINUS         = 0xBD,
        EQUALS        = 0xBB,
		BACKSLASH     = 0xDC,
		LEFT_BRACKET  = 0xDB,
        RIGHT_BRACKET = 0xDD,
		SEMICOLON     = 0xBA,
		APOSTROPHE    = 0xDE,
        COMMA         = 0xBC,
		PERIOD        = 0xBE,
		SLASH         = 0xBF,
        NUMPAD_0      = 0x60,
		NUMPAD_1      = 0x61,
		NUMPAD_2      = 0x62,
        NUMPAD_3      = 0x63,
		NUMPAD_4      = 0x64,
		NUMPAD_5      = 0x65,
        NUMPAD_6      = 0x66,
		NUMPAD_7      = 0x67,
		NUMPAD_8      = 0x68,
        NUMPAD_9      = 0x69,
		MULTIPLY      = 0x6A,
		DIVIDE        = 0x6F,
        ADD           = 0x6B,
		SUBTRACT      = 0x6D,
		DECIMAL       = 0x6E,
        F1			  = 0x70,
		F2            = 0x71,
		F3            = 0x72,
        F4            = 0x73,
		F5            = 0x74,
		F6            = 0x75,
        F7            = 0x76,
		F8            = 0x77,
		F9            = 0x78,
        F10           = 0x79,
		F11           = 0x7A,
		F12           = 0x7B
	};
	public: enum EventType
	{
		NO_EVENT_TYPE = 0,
		KEY_DOWN = 6,
		KEY_UP = 7
	};
	public: enum
	{
		AMOUNT_OF_KEYS = 256
	};
	public: enum
	{
		KEYBOARD_DEVICE = 1
	};
	public: enum
	{
		CREATE_KEYBOARD_EVENT_MESSAGE = 41
	};
	public: class Event: public InputEvent
	{
		friend class Keyboard; 
		private: int64 time;
	    private: EventType type;
	    private: Key key;
		private: bool autoRepeat;
		private: StringANSI keyName;
		private: int32 messageId;
		public: Event(void);
		public: Event(const Event& _event);
		public: virtual ~Event(void);
		public: Event& operator = (const Event& _event);
		public: void SetKey(Key _key);
		public: void SetEventType(EventType _type);
		public: virtual int32 GetMessageId(void)const;
		public: static int32 _GetMessageOfEventType(int32 _type);
		public: int64 GetTime(void)const;
		public: Key GetKey(void)const;
		public: StringANSI GetKeyName(void)const;
		public: virtual int32 GetEventType(void)const;
		public: virtual int32 GetDeviceType(void)const;
		public: bool IsAutoRepeat(void)const;
		public: bool IsExactEquals(const Event& _event)const;
		public: bool IsInexactEquals(const Event& _event)const;
		public: virtual bool SaveToFile(File& _file);
		public: virtual bool LoadFromFile(File& _file);
	};
	friend class Window;
	private: MagicList<Event> events;
	private: static char *keyNames[];
	private: uint8 state[AMOUNT_OF_KEYS];
	private: SimpleList<Key> pressedKeysList;
	private: Keyboard(void);
	public: MagicList<Event>& GetEventList(void);
	public: void ClearEventList(void);
	private: Event* EventFilter(Window* _window, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	public: static void _CreateEvent(int32 _key, int32 _type);
	public: const uint8* GetKeyboardState(void);
    public: static StringANSI _GetFormatKeyName(Keyboard::Event* _event);
	public: static StringANSI _GetKeyName(Key _key);
	public: static Key _GetKeyId(StringANSI _key);
	public: bool IsKeyPressed(Key _key)const;
	public: static bool _IsKeyPressed(Key _key);
	public: bool IsKeyReleased(Key _key)const;
	public: static bool _IsKeyReleased(Key _key);
	public: bool IsKeyEnabled(Key _key)const;
	public: static bool _IsKeyEnabled(Key _key);
	public: const SimpleList<Key>& GetPressedKeysList(void);
};



#endif 
