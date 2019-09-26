// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Core/Window/Window.h>
#include <Engine/Output/File/File.h>
#include <Engine/Core/RealTimeWorker/RealTimeWorker.h>




// класс, реализующий логическое ядро
class Application: public RealTimeWorker
{ 
	// DATA
	private: float time;// время таймера
	private: StringANSI str;// строка

	private: PAINTSTRUCT paint; // структура для рисования в окне
	private: HDC windowDC;// контекст устройства в памяти
	private: HBITMAP frame; // кадр с изображением, который нужно отобразить на экран
	private: HANDLE oldObject;// предыдущий выделенный объект
    private: RECT rect;// прямоугольник клиентской области окна, который должен быть обновлен (перерисован)
	private: HBRUSH whiteBrush;// кисть для фона



    // METHODS
    // инициализирующий конструктор
	public: Application(const RealTimeWorker::Define& _define): time(0.0f)
	{
		if(Create(_define))
		{
			CreateBuffer(); // создать задний буфер
			whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);// цвет фоновой кисти 
			
			Flush(); // выполнить отложенные события
		}
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


		// таймер
		str = "timer: " + NumberToString(time);
		::TextOutA(windowDC, 10, 10, str.c_str(), str.size());
				
		// FPS
		str = "FPS: " + NumberToString(UpdateCallController().FPS());
		::TextOutA(windowDC, 10, 30, str.c_str(), str.size());
		
		// 
		str = "FPS mode: " + StringANSI((UpdateCallController().Mode() == CallController::FIXED) ? "fixed" : (UpdateCallController().Mode() == CallController::UNLIM) ? "unlimited" : "none");
		::TextOutA(windowDC, 10, 50, str.c_str(), str.size());
		

		// sleep
		str = "sleep: " + NumberToString(Sleep());
		::TextOutA(windowDC, 10, 70, str.c_str(), str.size());
				
		// unsuspended
		str = "unsuspended: " + NumberToString(Unsuspended());
		::TextOutA(windowDC, 10, 90, str.c_str(), str.size());
		
		// функция InvalidateRect посылает сообщение WM_PAINT с указанием прямоугольника клиентской области окна, которая должна быть обновлена (перерисована)
		::InvalidateRect(Handle(), // дескриптор окна, клиентская область которого должна быть обновлена
					     &rect, // прямоугольник, который будет добавлен к обновляемой области (если 0, то будет перерисована вся клиентская область)
					     false); // если true, то фон клиентской области будет стерт, при вызове ::BeginPaint; если false, то фон остается неизменным
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


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// очистить буфер событий
	}


	// обработка событий клавиатуры
    public: virtual void KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::S)
			{
				SetSleep(!Sleep());
			}
			else if(_event.key == Keyboard::U)
			{	
				SetUnsuspended(!Unsuspended());
			}
			else if(_event.key == Keyboard::M)
			{
				if(UpdateCallController().Mode() == CallController::FIXED)
				{
					UpdateCallController().SetMode(CallController::UNLIM);
				}
				else if(UpdateCallController().Mode() == CallController::UNLIM)
				{
					UpdateCallController().SetMode(CallController::FIXED);
				}
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий 
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
		return RealTimeWorker::Proc(_hWindow, _message, _wParam, _lParam);
	}



	// функция кадра
	public: virtual void Update(float _delta)
	{
		time += _delta;// добавить дельту к общему времени
		UpdateBuffer();// обновить содержимое кадра
	}
		
			
	// РАЗМЕР...
	// возвращает размер (в байтах) динамических данных, которыми обладает объект
	public: virtual int32 DynamicSizeInByte(void)const
	{
		return RealTimeWorker::DynamicSizeInByte();
	}

	// возвращает размер (в байтах) статических данных, которыми обладает объект
	public: virtual int32 StaticSizeInByte(void)const
	{
		return sizeof(*this);
	}
};



// главная точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание логического ядра
	RealTimeWorker::Define def;
	def.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна
	def.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.windowDef.tittle ="RealTimeWorker";// заголовок окна

	Application* app = new Application(def);// создать окно
	app->Loop();// запустить цикл событий
	
	delete app;// удалить ядро
	return 0;
}


 
