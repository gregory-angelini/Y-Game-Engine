// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Material/AnimatedMaterial/AnimatedMaterial.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



// класс, реализующий пользовательское окно
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// графическая сцена
	private: Bitset bitset; // массив битов
    private: int32 index1; // индекс выделенного бита для первого битсета
    private: int32 index2; // индекс выделенного бита для второго битсета
	private: int32 builtInType;// встроенный тип данных
	private: int32 selectedBitset;// индекс выделенного битсета
	private: int32 newBitsetSize;// размер для создаваемого битсет
			 
    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), index1(0), index2(0), builtInType(0), selectedBitset(NIL), newBitsetSize(0)
	{}



	// удаляет пользовательские ресурсы
	public: virtual void DestroyUserResources(void)
	{
		//delete scene;
	}


	// создает пользовательские ресурсы и возвращает true в случае успеха
	public: virtual bool CreateUserResources(void)
	{
		// сцена...
		{
			// для создания объекта
			scene = new Scene();
			scene->Rename("scene"); // имя сцены
			AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
		}


		// шрифт...
		{
			Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

			Font2D::Define def;
			def.name = "font";
			def.fontName = "Courier New";// имя шрифта
			def.fontSize = 16;// размер шрифта
			def.fontColor = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

			font2d = new Font2D();
			font2d->Create(def);
			scene->AddObject(font2d);// добавить в сцену
			font2d->Pick();// выбрать шрифт
		}


		// битсет...
		{
			// битсет...
			bitset.Resize(8);// количество бит
			newBitsetSize = 8;
		}
			
					

		// камера...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
			def.rotationEnabled = false;


			camera = new Camera3D();
			camera->Create(def);
			camera->Pick(Title()); 

			scene->AddObject(camera);
		}

		

		Keyboard().SetKeyPressedPeriodInSec(0.1);// частота генерации зажатия клавиши


		// цвет фона...
		SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			

		MoveToCenter();// переместить окно в центр крана
		Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна	
		return true;// вернуть успех
	}


	// функция кадра
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta); 
		Trigger::_ExecuteActions();
	}


	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

	
		// вывод текста...
		//camera->BeginPerspective3D();// начать вывод двухмерной графики

		//scene->Draw();
	
	
		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		// вывести первый битсет
		DrawBitset1();

		// вывести второй битсет
		DrawBitset2();

		// создается новый битсет...
		if(Keyboard().IsKeyPressed(Keyboard::R))// если зажата клавиша R
		{
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// зеленый
			font2d->DrawScreenText(Vector3D(500, 90, -1), "new size: %d", newBitsetSize);
		}

		int32 i = 0;
		
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom: %.2f", camera->DistanceToViewingPoint()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom min/max: %.2f/%.2f", camera->ZoomMin(), camera->ZoomMax()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom speed: %.2f", camera->ZoomSpeed()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sprite.binary state: %s", sprite->GetStringState().c_str()); 
	}


	// выводит битсет встроенного типа int32
	public: void DrawBitset1(void)
	{
		// вывод индексов битсета
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый
		font2d->DrawScreenText(Vector3D(14, 20, -1), "bitset: ");
		
		// если данный битсет выделен
		if(selectedBitset == 0)
		{
			// сменить цвет...
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// зеленый
		}

		font2d->DrawScreenText(Vector3D(14 + (8 * 10), 20, -1), "1");// где 8 - количество символов, 10 - ширина символа в пикселях


		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый

		font2d->DrawScreenText(Vector3D(14 + (1 * 10) + (8 * 10), 20, -1), ", type: int32, size: %d bits, value: %d\r\n", Bitset::_GetBitSize(builtInType), builtInType); 


		// вывод битсета
		font2d->DrawScreenText(Vector3D(14, 40, -1), "%s   - bitset\r\n", Bitset::_ToString(builtInType).c_str()); 


	
		// вывод выделенного бита
		font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// зеленый
		for(int i = 0, line = 0; i < Bitset::_GetBitSize(builtInType); i++, ++line)
		{ 
			if(i == index1 && selectedBitset == 0)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 40, 0), "%d", Bitset::_GetBit(builtInType, index1));
			}
		}

		
		// вывод индексов битсета
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый
		for(int i = 0, line = 0; i < Bitset::_GetBitSize(builtInType); i++, ++line)
		{ 
			if(i == index1)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 60, -1), "%d", i);
			}
		}
		font2d->DrawScreenText(Vector3D(14 + Bitset::_GetBitSize(builtInType) * 10, 60, -1), "   - indices");
	}



	// выводит битсет пользовательского типа 
	public: void DrawBitset2(void)
	{
		// вывод индексов битсета
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый
		
		font2d->DrawScreenText(Vector3D(14, 90, -1), "bitset: ");
		
		// если данный битсет выделен
		if(selectedBitset == 1)
		{
			// сменить цвет...
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// зеленый
		}

		font2d->DrawScreenText(Vector3D(14 + (8 * 10), 90, -1), "2");// где 8 - количество символов, 10 - ширина символа в пикселях


		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый
		font2d->DrawScreenText(Vector3D(14 + (1 * 10) + (8 * 10), 90, -1), ", type: user, size: %d, exist: %d\r\n", bitset.GetBitSize(), bitset.IsExist()); 


		// вывод битсета
		font2d->DrawScreenText(Vector3D(14, 110, -1), "%s   - bitset\r\n", bitset.ToString().c_str()); 


	
		// вывод выделенного бита
		font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// зеленый
		for(int i = 0, line = 0; i < bitset.GetBitSize(); i++, ++line)
		{ 
			if(i == index2 && selectedBitset == 1)
			{  
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 110, 0), "%d", bitset.GetBit(index2));
			}
		}

		
		// вывод индексов битсета
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// белый
		for(int i = 0, line = 0; i < bitset.GetBitSize(); i++, ++line)
		{ 
			if(i == index2)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 130, -1), "%d", i);
			}
		}

		font2d->DrawScreenText(Vector3D(14 + (bitset.GetBitSize() * 10), 130, -1), "   - indices");
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{ 
		if(Keyboard().IsKeyPressed(Keyboard::R))// если зажата клавиша R
		{
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				if(_event->wheel > 0)
				{
					newBitsetSize++;
				}
				else if(_event->wheel < 0)
				{
					newBitsetSize--;
				}
				// контроль диапазона
				if(newBitsetSize < 0) { newBitsetSize = 0; }
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::I))// если зажата клавиша I
		{
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_EnableBits(builtInType, _event->wheel > 0);
				}
				else // иначе это пользовательский битсет
				{
					bitset.EnableBits(_event->wheel > 0);
				}	
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::M))// если зажата клавиша M
		{
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_AND(builtInType, index1, _event->wheel > 0);// инвертировать бит
				}
				else // иначе это пользовательский битсет
				{
					bitset.AND(index2, _event->wheel > 0);// инвертировать бит
				}
			}
		}
	   if(Keyboard().IsKeyPressed(Keyboard::A))// если зажата клавиша A
	   {
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_OR(builtInType, index1, _event->wheel > 0);// инвертировать бит
				}
				else // иначе это пользовательский битсет
				{
					bitset.OR(index2, _event->wheel > 0);// инвертировать бит
				}
			}
		}
       if(Keyboard().IsKeyPressed(Keyboard::D))// если зажата клавиша D
	   {
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_XOR(builtInType, index1, _event->wheel > 0);// инвертировать бит
				}
				else // иначе это пользовательский битсет
				{
					bitset.XOR(index2, _event->wheel > 0);// инвертировать бит
				}
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::E))// если зажата клавиша E
		{
			// если смещается колесико мыши
			if(_event->type == Mouse::WHEEL)
			{
				// если это builtInType
				if(selectedBitset == 0)
				{
					Bitset::_EQUAL(builtInType, index1, _event->wheel > 0);// инвертировать бит
				}
				else // иначе это пользовательский битсет
				{
					bitset.EQUAL(index2, _event->wheel > 0);// инвертировать бит
				}
			}
		}
		Mouse().ClearEventList();// очистить буфер событий
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		if(_event->type == Keyboard::KEY_DOWN)
		{
			if(_event->key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event->key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_RESIZING);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			} 
			else if(_event->key == Keyboard::UP_ARROW)
			{
				selectedBitset = 0;// выбрать другой битсет
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				selectedBitset = 1;// выбрать другой битсет
			}
			else if(_event->key == Keyboard::SPACE)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_NOT(builtInType, index1);// инвертировать бит
				}
				else // иначе это пользовательский битсет
				{
					bitset.NOT(index2);// инвертировать бит
				}
			}	
			else if(_event->key == Keyboard::N)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					Bitset::_NOT(builtInType);// инвертировать битсет
				}
				else // иначе это пользовательский битсет
				{
					bitset.NOT();// инвертировать битсет
				}
			}
		} 
		else if(_event->type == Keyboard::KEY_UP)
		{
			if(_event->key == Keyboard::R)
			{ 
				bitset.Resize(newBitsetSize);
			} 
		}
		else if(_event->type == Keyboard::KEY_PRESSED)
		{
			if(_event->key == Keyboard::LEFT_ARROW)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					if(index1 > 0) // контроль нижнего диапазона
					{
						--index1;
					}
				}
				else // иначе это пользовательский битсет
				{
					if(index2 > 0) // контроль нижнего диапазона
					{
						--index2;
					}
				}
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				if(selectedBitset == 0)// если это builtInType
				{
					if(index1 < Bitset::_GetBitSize(builtInType) - 1)// контроль верхнего диапазона
					{
						++index1;
					}
				}
				else // иначе это пользовательский битсет
				{
					if(index2 < bitset.GetBitSize() - 1)// контроль верхнего диапазона
					{
						++index2;
					}
				}
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий
		return _event;
	}
};



// главная точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание графического ядра
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Bitset.example_01";
	
	
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
