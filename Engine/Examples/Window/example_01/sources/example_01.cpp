// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Core/Window/Window.h>
#include <Engine/Output/File/File.h>


// класс, реализующий пользовательское окно
class MyWindow: public Window,
			    public MessageHandler<int32>::Receiver
{
	// DATA
	private: StringANSI str;// строка
	private: PAINTSTRUCT paint; // структура для рисования в окне
	private: HDC windowDC;// контекст устройства в памяти
	private: HBITMAP frame; // кадр с изображением, который нужно отобразить на экран
	private: HANDLE oldObject;// предыдущий выделенный объект
    private: RECT rect;// прямоугольник клиентской области окна, который должен быть обновлен (перерисован)
	private: HBRUSH whiteBrush;// кисть для фона
	private: static int32 windowCount;// счетчик окон, который используется для генерации имен


    // METHODS
    // инициализирующий конструктор
	public: MyWindow(const Window::Define& _define)
	{
		//Connect(ResizeEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyResize));
		//Connect(MoveEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMove));
		//Connect(MinimizedEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMinimized));
		//Connect(MaximizedEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMaximized));

		// создать окно
		if(Create(_define))
		{
			// увеличить счетчик созданных окон
			windowCount++;
			SetTittle("Window" + NumberToString(windowCount));//заголовок окна

			CreateBuffer(); // создать задний буфер
			whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);// цвет фоновой кисти 
			
			Flush(); // выполнить отложенные события
		}
	}


	// функция кадра
    public: virtual int32 Frame(void)
	{
		int32 result = Window::Frame();// вызвать обработчик сообщений

		// если окно существует
		if(result)
		{
			//Flush(); // выполнить отложенные события
			UpdateBuffer();// обновить содержимое кадра и вывести на экран
			//Sleep(60);// заснуть на 60 миллисекунд
		}

		return result;
	}
	

	// обновляет содержимое кадра
	public: void UpdateBuffer(void)
	{
		// прямоугольник клиентской области окна, который нужно обновить (перерисовать); мы используем всю клиентскую область
		rect.left = 0;
		rect.top = 0;
		rect.right = ClientAreaSize().width;
		rect.bottom = ClientAreaSize().height;
 
		FillRect(windowDC, &rect, whiteBrush);// закрасить фон белым цветом


		// назначить серый цвет текста
		SetTextColor(windowDC, RGB(100, 100, 100));

		int32 j = 0;

		// файл логов
		str = "log file: " + LogFile();
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());
				
		// фокус окна
		str = "focus: " + NumberToString(IsKeyboardFocus());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());
				
		// координаты клиентской области окна
		str = "client area position: " + NumberToString(ClientAreaPosition().x) + ", " + NumberToString(ClientAreaPosition().y);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		
		// координаты окна
		str = "window position: " + NumberToString(WindowPosition().x) + ", " + NumberToString(WindowPosition().y);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100


		// размер клиентской области
		str = "client area size: " + NumberToString(ClientAreaSize().width) + ", " + NumberToString(ClientAreaSize().height);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
	
		// размер всего окна
		str = "window size: " + NumberToString(WindowSize().width) + ", " + NumberToString(WindowSize().height);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		

		// оконный режим
		str = "windowed: " + NumberToString(IsWindowed());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		

		// количество окон
		str = "amount of windows: " + NumberToString(Window::_WindowList().Size());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100


		// если родительское окно
		str = "is parent: " + NumberToString(IsParentWindow());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		
		if(IsParentWindow())// если это родительское окно
		{
			SimpleList<Window*> childs = ChildWindows();// взять список дочерних окон
			
			// перечислить все дочерние окна
			for(int i = 0; i < ChildWindows().Size(); ++i)
			{
				str = "child_" + NumberToString(i+1) + ": " + NumberToString(childs[i]->data->Tittle());
				::TextOutA(windowDC, 10 + (i+1) * 130, j, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
			}
		}

		// если дочернее окно
		str = "is child: " + NumberToString(IsChildWindow());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		
		if(IsChildWindow())// если это дочернее окно
		{
			str = ", parent: " + NumberToString(ParentWindow()->Tittle());
			::TextOutA(windowDC, 10 + 70, j, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		}

		// буферизация событий
		str = "event buffering: " + NumberToString(IsEventBuffering());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		
		// системное время
		str = "elapsed time: " + NumberToString(WorkingTimeSec());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// отобразить текст message в координатах 100, 100
		
	
		// функция InvalidateRect посылает сообщение WM_PAINT с указанием прямоугольника клиентской области окна, которая должна быть обновлена (перерисована)
		::InvalidateRect(Handle(), // дескриптор окна, клиентская область которого должна быть обновлена
					     &rect, // прямоугольник, который будет добавлен к обновляемой области (если 0, то будет перерисована вся клиентская область)
					     false); // если true, то фон клиентской области будет стерт, при вызове ::BeginPaint; если false, то фон остается неизменным
	}


	// создает дочернее окно
	public: void CreateChildWindow(void)
	{
		// описание окна
		Window::Define def;
		def.SetTemplateStyle(Window::WND_RESIZING);
		def.parentWindow = Handle();// использовать текущее окно в качестве родительского
        def.hSmallIcon = LoadIcon(Instance(), MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	
		MyWindow* window = new MyWindow(def);// создать окно
	}


	// перерисовывает клиентскую область окна
	public: void Draw(void)
	{
		// мы будем использовать двойную буферизацию (рисование в память, а затем копирования нарисованного изображения в окно)

		// HDC - декстриптор контекста графического устройства, которое описывает видеосистему и изображаемую поверхность
		HDC screen = ::BeginPaint(Handle(), &paint); // где функция BeginPaint подготавливает окно для рисования и возвращает дескриптор контекста графического устройства (так же он находится по адресу paint.hdc)
		
		// копируем из windowDC в screen
		// paint используется для указания области обновления (в данном случае это вся клиентская область)
		StretchBlt(screen, // цель
			       paint.rcPaint.left, 
				   paint.rcPaint.top,
				   paint.rcPaint.right,
				   paint.rcPaint.bottom,
				   windowDC, // источник
				   paint.rcPaint.left,
				   paint.rcPaint.top,
				   paint.rcPaint.right,
				   paint.rcPaint.bottom,
				   SRCCOPY);
				   
		// закончить рисование; освободить контекст графического устройства
		EndPaint(Handle(), &paint);
	} 


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// очистить буфер событий
	}

	
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::NUM_1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::NUM_2)
			{	
				SetWindowStyle(Window::WND_VIEWPORT);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_3)
			{
				SetWindowStyle(Window::WND_TITTLE);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_4)
			{
				SetWindowStyle(Window::WND_LIGHT);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_5)
			{
				SetWindowStyle(Window::WND_CLASSIC);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				SetWindowStyle(Window::WND_RESIZING);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			}
			else if(_event.key == Keyboard::C)
			{
				CreateChildWindow();// создать дочернее окно
			}
			else if(_event.key == Keyboard::M)
			{
				MoveToCenter();// переместить окно в центр экрана
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий 
	}




	// уничтожает контекст устройства в памяти, и изображение, имитирующее задний буфер
	private: void DestroyBuffer(void)
	{
		// уничтожаем графические ресурсы...
		oldObject = (HBITMAP)SelectObject(windowDC, oldObject);// снять выделение с объекта frame, выделив предыдущий объект oldObject
				
		DeleteObject(frame);// удалить изображение 
		DeleteDC(windowDC);// удалить контекст устройства
	}

	// создает контекст устройства в памяти, и изображение, которое будет имитировать задний буфер
	private: void CreateBuffer(void)
	{
		if(Handle())// если окно существует
		{
			// создаем графические ресурсы...
			HDC dc = GetDC(Handle());// захватываем контекст окна
			
			windowDC = CreateCompatibleDC(dc);// создать контекст устройства в памяти, совместимое с контекстом графического устройтва 
			frame = CreateCompatibleBitmap(dc, ClientAreaSize().width, ClientAreaSize().height);// создать изображение, совместимое с контекстом графического устройтва; размер изображения соответствует клиентской области окна
			
			::ReleaseDC(Handle(), dc);// освободить контекст окна
					
			oldObject = (HBITMAP)SelectObject(windowDC, frame);// выбрать объект frame для контекста устройства windowDC; запомнить предыдущий выделенный объект, и поместить его в oldObject
		}
	}


	// вызывается при изменении размеров окна
	// примечание: в качестве аргумента принимается новый размер клиентской области окна
	public: virtual void ResizeEventFilter(Size2D _size)
	{
		// пересоздать задний буфер
		DestroyBuffer();
		CreateBuffer();

		// обновить кадр
		UpdateBuffer();
	}
	


	
	public: void MyResize(Size2D _size)
	{
		WinAPI::_MessageBox("resize","");

		// пересоздать задний буфер
		DestroyBuffer();
		CreateBuffer();

		// обновить кадр
		UpdateBuffer();
	}


    public: void MyMove(Vector2D _position)
	{
		WinAPI::_MessageBox("move","");
	}



    public: void MyMinimized(void)
	{
		WinAPI::_MessageBox("Minimized","");
	}

    public: void MyMaximized(void)
	{
		WinAPI::_MessageBox("Maximized","");
	}




	// обработчик сообщений
	public: virtual LRESULT Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam)
	{
		// обработка сообщений
		switch(_message)
		{ 	
			// окно уничтожается
			case WM_DESTROY:
			{
				DestroyBuffer();
				break;// прервать оператор switch 
			}

			// окно перерисовывается; актуально для оконного режима
			case WM_PAINT:
			{ 
				Draw();// обновить клиентскую область окна
				break;// прервать оператор switch 
			}

			default: break;// игнорировать остальные сообщения
		}
		
		
		// вызвать базовый обработчик
		return Window::Proc(_hWindow, _message, _wParam, _lParam); 
	}
	
	
				
	// РАЗМЕР...
	// возвращает размер (в байтах) динамических данных, которыми обладает объект
	public: virtual int32 DynamicSizeInByte(void)const
	{
		return Window::DynamicSizeInByte() + MessageHandler<int32>::Receiver::DynamicSizeInByte();
	}

	// возвращает размер (в байтах) статических данных, которыми обладает объект
	public: virtual int32 StaticSizeInByte(void)const
	{
		return sizeof(*this);
	}
};

// статические данные
int32 MyWindow::windowCount = 0;



// главная точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание окна
	Window::Define def;
	def.SetTemplateStyle(Window::WND_RESIZING);// стиль окна
	def.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	//def.eventBuffering = true;

	MyWindow* window = new MyWindow(def);// создать окно
	

	// пока хотя бы одно окно существует
	while(Window::_WindowList().IsNotEmpty())
	{
		// обойти все существующие окна
		for(int i = 0; i < Window::_WindowList().Size(); ++i)
		{
			Window::_WindowList()[i]->Frame();// обработка сообщений
		}
	}

	delete window;
	return 0;
}


 
