#ifndef MOUSE_H
#define MOUSE_H
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <windows.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h> 
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
class Window;
class Mouse
{
	public: enum Key
	{ 
		NO_KEY = 0,
		LEFT_BUTTON = 0x01,
		RIGHT_BUTTON = 0x02,
		MIDDLE_BUTTON = 0x04
	};
	public: enum EventType
	{
		NO_EVENT_TYPE = 0,
		KEY_DOWN = 1,
		KEY_UP = 2,
		DBL_CLICK = 3,
		MOVE = 4,
		WHEEL = 5
	};
	public: enum 
	{
		MOUSE_DEVICE = 2
	};
	public: enum
	{
		ENABLE_SHOW_CURSOR_MESSAGE = 81
	};
	public: class Event: public InputEvent
	{
		friend class Mouse; 
	    private: int64 time;
	    private: EventType type;
		private: Key key;
		private: Vector2 position;
	    private: Vector2 offset;
		private: int32 wheel;
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
		public: Vector2 GetPosition(void)const;
		public: Vector2 GetOffset(void)const;
		public: int32 GetWheel(void)const;
		public: Key GetKey(void)const;
		public: virtual int32 GetEventType(void)const;
		public: virtual int32 GetDeviceType(void)const;
		public: bool IsExactEquals(const Event& _event)const;
		public: bool IsInexactEquals(const Event& _event)const;
		public: virtual bool SaveToFile(File& _file);
		public: virtual bool LoadFromFile(File& _file);
	};
	friend class Window;
	private: MagicList<Event> events;
	private: Vector2 previousPos;
	private: bool state[5];
	private: SimpleList<Key> pressedKeysList;
    private: static bool cursorVisible;
	private: Mouse(void);
    private: Event* EventFilter(Window* _window, UINT _msg, WPARAM _wParam, LPARAM _lParam);
    public: const MagicList<Event>& GetEventList(void);
	public: void ClearEventList(void);
	public: Vector2 GetPreviousCursorPosition(void);
	public: bool IsKeyPressed(Key _key)const;
	public: bool IsKeyReleased(Key _key)const;
	public: const SimpleList<Key>& GetPressedKeysList(void);
	public: static void _SetCursorPosition(const Vector2& _position, const Window* _window = NIL);
	public: static Vector2 _GetCursorPosition(const Window* _window = NIL);
	public: static void _MoveCursorToCenter(const Window* _window = NIL);
	public: static Vector2 _GetDistanceFrom(const Vector2& _point);
	public: static bool _IsCursorOver(const Window* _window, bool _onlyClientRect = true);
	public: static void _SetShowCursor(bool _enable);
	public: static bool _IsShowCursor(void);
};



#endif 
