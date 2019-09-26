// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/File/File.h>
#include <Engine/Math/Vector/Vector2D/Vector2D.h>


#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// для текстур

#include <Engine/Math/Ratio/Ratio.h>

#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>

#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>


// класс, реализующий приложение
class Application: public Renderer
{
	// DATA
	private: float zoom; // расстояние от наблюдателя до куба
	private: float spinX, spinY, spinZ;// угол вращения куба по осям X,Y,Z
	private: float step;// размер шага угла вращения
    
	private: GLuint filter; // фильтр текстур
	private: GLuint texture[3]; // идентификаторы текстур

   
	private: float fov;// значение FOV для перспективы
	private: float aspect; // аспект перспективы
	private: float zFar, zNear;// дальняя и ближняя плоскости отсечения

	private: GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>* box;// трехмерный прямоугольник, имитирующий коробку
	private: Camera3D* camera;// камера
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// графическая сцена
	private: Font2D* font2d;// шрифт


    // METHODS
    // инициализирующий конструктор
	public: Application(void): zoom(-10.0f), filter(0), spinX(0.0f), spinY(0.0f), spinZ(0.0f), step(15.0f), fov(0.0f), aspect(0.0f), zFar(0.0f), zNear(0.0f), box(NIL), scene(NIL), camera(NIL), font2d(NIL)
	{}


	// виртуальный деструктор
	public: virtual ~Application(void)
	{
		delete scene;
	}

	// создает графическое ядро
	public: bool Create(Renderer::Define _define)
	{
		// создать графическое ядро
		if(Renderer::Create(_define))
		{
			// загружаем текстуру
			LoadTexture();

			
			// включаем плавную ракраску (интерполирование по вершинам)
            //glShadeModel(GL_SMOOTH);
			

			// сцена...
			{
				GraphicsScene<Vector3D, Quaternion, Size3D>::Define def;
				def.logicSceneDef.scale = Size3D(1, 1, 1);
				
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D>();
				scene->Create(def);
			}

			// куб...
			{
				GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>::Define def;
				def.box = Box3D(Vector3D(0, 0, 0), Size3D(2, 2, 2));
				def.graphicsMeshDef.name = "box3d";
				
				box = new GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>();
				box->Create(def);
				

				// буфер цветов
				box->CreateColors(); 

				box->SetColor(0, ColorRGBA::_Random());
				box->SetColor(1, ColorRGBA::_Random());
				box->SetColor(2, ColorRGBA::_Random());
				box->SetColor(3, ColorRGBA::_Random());

				box->SetColor(4, ColorRGBA::_Random());
				box->SetColor(5, ColorRGBA::_Random());
				box->SetColor(6, ColorRGBA::_Random());
				box->SetColor(7, ColorRGBA::_Random());
				scene->AddObject(box, true, true);// добавить в сцену
			} 

			
			// камера...
			{
				Camera3D::Define def;
				zoom = 13.0f;
				def.position = Vector3D(0.0f, 0.0f, zoom);
				def.name = "camera";

				camera = new Camera3D();
				camera->Create(def);
							
				fov = camera->FOV();
				aspect = camera->Aspect();
				zFar = camera->ZFar();
				zNear = camera->ZNear();
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);// подписаться на событие изменения размеров окна
			}
			

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
			}
			Flush();// выполнить отложенные события
			return true;// вернуть успех
		} 
		// иначе вернуть неудачу
		return false;
	}
	
	// загружает текстуру для тестовой модели
	private: void LoadTexture(void)
	{
		AUX_RGBImageRec* tex = auxDIBImageLoad("phoenix.bmp"); // загружаем изображение как массив данных из трех компонент: красного, зеленого и синего
 
		glGenTextures(3, &texture[0]);// создаем три текстуры
				
		// создание первой текстуры
		glBindTexture(GL_TEXTURE_2D, texture[0]); // двухмерная текстура
				
				
		// фильтрация текстур; не использовать фильтрацию
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	 
		glTexImage2D(GL_TEXTURE_2D, 
					 0, 
					 3, 
					 tex->sizeX, tex->sizeY,
					 0, 
					 GL_RGB, 
					 GL_UNSIGNED_BYTE, 
					 tex->data);


		// создание второй текстуры
		glBindTexture(GL_TEXTURE_2D, texture[1]);
				
		// линейная фильтрация
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 
			         0, 
					 3, 
					 tex->sizeX, tex->sizeY,
					 0, 
					 GL_RGB, 
					 GL_UNSIGNED_BYTE, 
					 tex->data);


		// создание третьей текстуры
		glBindTexture(GL_TEXTURE_2D, texture[2]);
				
		// mip-фильтрация; создается множество текстур разного размера, одна из которых (наиболее подходящая) будет использоваться при фильтрации;
	    // при использовании такого режима фильтрации, размеры текстуры могут быть любыми (а не только кратными 2, как в случае с предыдущими режимами фильтрации, которые накладывают подобные ограничения)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  

		gluBuild2DMipmaps(GL_TEXTURE_2D, 
			              3, 
						  tex->sizeX, 
						  tex->sizeY,
						  GL_RGB, 
						  GL_UNSIGNED_BYTE, 
						  tex->data); 


		// освобождаем изображение
		if(tex) // если текстура существует
		{
			if(tex->data) // если изображение текстуры существует
			{
				free(tex->data); // освобождение памяти изображения текстуры
			}

			free(tex); // освобождение памяти под структуру
		}

		// смешивание
		//glColor4f(1.0f,1.0f,1.0f,0.5f);   // Полная яркость, прозрачность 50%; альфа = 0.0 (полная прозрачность), 1.0 - полная непрозрачность
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE); // Функция смешивания базирующаяся на значении альфы
	} 





	// функция кадра
	public: void Update(float _delta)
	{
		spinX += _delta * step;

		// контролируем выход за пределых 360 градусов
		if(spinX > 360.0f) { spinX = spinX - 360.0f; }

				
		spinY += _delta * step;

		// контролируем выход за пределых 360 градусов
		if(spinY > 360.0f) { spinY = spinY - 360.0f; }


		spinZ += _delta * step;

		// контролируем выход за пределых 360 градусов
		if(spinZ > 360.0f) { spinZ = spinZ - 360.0f; }
	}


	// отображает текстурированную коробку
	public: void DrawBox(void)
	{
		glEnable(GL_TEXTURE_2D); // включить наложение текстур

		glColor3f(1.0f, 1.0f, 1.0f);// белый цвет текста

		glBindTexture(GL_TEXTURE_2D, texture[filter]);  // выбор текстуры основываясь на filter

		// отображаем текстурированный куб
		glBegin(GL_QUADS);  // Начало рисования четырехугольников
		 
		// Передняя грань
		//glNormal3f(0.0f, 0.0f, 1.0f);   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 


		// Задняя грань
		//glNormal3f(0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 

  
		// Верхняя грань
		//glNormal3f(0.0f, 1.0f, 0.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 

  
		// Нижняя грань
		//glNormal3f(0.0f,-1.0f, 0.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 

  
		// Правая грань
		//glNormal3f(1.0f, 0.0f, 0.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 

  
		// Левая грань
		//glNormal3f(-1.0f, 0.0f, 0.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glEnd(); 
		glDisable(GL_TEXTURE_2D); // отменить наложение текстур
	}


	// вызывается при изменении размеров окна
	public: virtual void ResizeEventFilter(Size2D _size)
	{ 
		Renderer::Resize(_size);// выбор базового обработчика
	}



	// функция рендера
	public: void Draw(float _delta)
	{
		//MakeCurrent();
		//Flush();
		ClearScene();// очистить сцену
		
 
		// трансформации...
		Renderer::_SetCurrentStack(Renderer::MODELVIEW); // работаем с матрицей вида
		Renderer::_PushMatrix();// сохранить текущую матрицу (трансформацию) в буфер

		// объединить трансформацию (матрицу) сцены с текущей (внешней) установленной трансформацией 
		Renderer::_MultMatrix(Matrix4x4(Vector3D::UNIT_X, Angle::_DegreesToRadians(spinX)) * 
			                  Matrix4x4(Vector3D::UNIT_Y, Angle::_DegreesToRadians(spinY)) * 
				              Matrix4x4(Vector3D::UNIT_Z, Angle::_DegreesToRadians(spinZ)));

		if(IsKeyboardFocus() && Mouse::_IsAnyKeyPressed())// если зажата окна из кнопок мыши
		{
			scene->Draw();// отобразить все объекты сцены
		}
		else
		{
			DrawBox();// отобразить коробку
		}
		
		// трансформации... 
		Renderer::_PopMatrix();// восстановить предыдущую матрицу (трансформацию) из буфера

		camera->Begin2D();// начать вывод двухмерной графики


		// позиция камеры
		font2d->DrawScreenText(Vector3D(10, 20, -1), "box.Center(X: %.1f, Y:%.1f, Z:%.1f); box.Size(width:%.1f, height:%.1f, length:%.1f)", box->Box().Center().x, box->Box().Center().y, box->Box().Center().z, box->Box().Size().width, box->Box().Size().height, box->Box().Size().length); 

		// позиция камеры
		font2d->DrawScreenText(Vector3D(10, 40, -1), "camera.Position(X: %.1f, Y:%.1f, Z:%.1f)", camera->Position().x, camera->Position().y, camera->Position().z); 



		// фильтрация текстур
		StringANSI filterName;
		if(filter == 0) { filterName = "nearest"; }
		else if(filter == 1) { filterName = "linear"; }
		else if(filter == 2) { filterName = "mipmap"; } 
		font2d->DrawScreenText(Vector3D(10, 60, -1), "texture filtering: %s", filterName.c_str());  
		

		StringANSI fpsMode; 
		if(UpdateCallController().Mode() == CallController::FIXED)
		{
			fpsMode = "fixed";
		}
		else if(UpdateCallController().Mode() == CallController::UNLIM)
		{
			fpsMode = "unlim";
		}

		// логическое ядро
		font2d->DrawScreenText(Vector3D(10, 80, -1), "logic core mode: %s   FPS: %d, max steps: %d", fpsMode.c_str(), UpdateCallController().FPS(), UpdateCallController().MaxSteps()); 
		
		

		if(DrawCallController().Mode() == CallController::FIXED)
		{
			fpsMode = "fixed";
		}
		else if(DrawCallController().Mode() == CallController::UNLIM)
		{
			fpsMode = "unlim";
		}
		

		// графическое ядро
		font2d->DrawScreenText(Vector3D(10, 100, -1), "draw core mode: %s   FPS: %d  VSYNC: %d", fpsMode.c_str(), DrawCallController().FPS(), VSync()); 


		// буфер глубины
		font2d->DrawScreenText(Vector3D(10, 120, -1), "z-buffer: %d, z-clear value: %f, z-depth: %d", ZBuffer(), ZBufferClearValue(), ZBufferDepth()); 

	
		// fov
		font2d->DrawScreenText(Vector3D(10, 140, -1), "fov: %f", camera->FOV()); 
	
		// аспект
		font2d->DrawScreenText(Vector3D(10, 160, -1), "aspect: %f", camera->Aspect()); 

		// ближняя и дальняя плоскости отсечения
		font2d->DrawScreenText(Vector3D(10, 180, -1), "zNear: %f, zFar: %f", camera->ZNear(), camera->ZFar()); 


		// формат цвета
		StringANSI colorFormat;
		if(ColorFormat() == Renderer::RGBA)
		{ colorFormat = "RGBA"; } 
		font2d->DrawScreenText(Vector3D(10, 200, -1), "color depth: %d, colorFormat: %s", GetColorDepth(), colorFormat.c_str()); 


		// вьюпорт
		font2d->DrawScreenText(Vector3D(10, 220, -1), "viewport(auto: %d)", ViewportAutoSize());
		font2d->DrawScreenText(Vector3D(10, 240, -1), "viewport(x: %.0f, y: %.0f, width: %.0f, height: %.0f)", ViewportPosition().x, ViewportPosition().y, ViewportSize().width, ViewportSize().height); 
		
		// режим визуализации граней
		StringANSI faceDrawMode;
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::POINT) { faceDrawMode = "point"; }
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::LINE) { faceDrawMode = "line"; }
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::FILL) { faceDrawMode = "fill"; }
		font2d->DrawScreenText(Vector3D(10, 260, -1), "front face draw mode: %s", faceDrawMode.c_str()); 
		
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::POINT) { faceDrawMode = "point"; }
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::LINE) { faceDrawMode = "line"; }
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::FILL) { faceDrawMode = "fill"; }
		font2d->DrawScreenText(Vector3D(10, 280, -1), "back face draw mode: %s", faceDrawMode.c_str()); 
		

		// режим отсечения граней
		font2d->DrawScreenText(Vector3D(10, 300, -1), "front face cull mode: %d", GetFaceCullMode(Renderer::FRONT));
		font2d->DrawScreenText(Vector3D(10, 320, -1), "back face cull mode: %d", GetFaceCullMode(Renderer::BACK)); 

		font2d->DrawScreenText(Vector3D(10, 340, -1), "front face clockwise: %d", FrontFaceClockwise()); 
 
		font2d->DrawScreenText(Vector3D(10, 360, -1), "Application.SizeInByte: %d", SizeInByte() + scene->SizeInByte() + camera->SizeInByte() + font2d->SizeInByte()); 
	
 
		camera->Begin3D();// начать вывод трехмерной графики
	} 
 
	

 
	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(Keyboard::_IsKeyPressed(Keyboard::F))// смена фильтра для текстур
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					filter++;
				}
				else if(_event.wheel < 0)
				{
					filter--;
				}
				// контроль диапазона
				if(filter > 2 || filter < 0) { filter = 0; }
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::O))// управление FOV
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					fov+=0.1f;
				}
				else if(_event.wheel < 0)
				{
					fov-=0.1f;
				}
				// контроль диапазона
				if(fov > 359.0f || fov < 0.0f) { fov = 0.0f; }

				camera->SetFOV(fov);
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::S))// управление аспектом
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					aspect+=0.1f;
				}
				else if(_event.wheel < 0)
				{
					aspect-=0.1f;
				}

				camera->SetAspect(aspect);
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::N))// ближняя плоскость отсечения
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					zNear+=1.0f;
				}
				else if(_event.wheel < 0)
				{
					zNear-=1.0f;
				}

				camera->SetZNear(zNear); 
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::M))// дальняя плоскость отсечения 
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					zFar+=1.0f;
				}
				else if(_event.wheel < 0)
				{
					zFar-=1.0f;
				}

				camera->SetZFar(zFar); 
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::L))// режим логического ядра
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0 || _event.wheel < 0)
				if(UpdateCallController().Mode() == CallController::FIXED)
				{ UpdateCallController().SetMode(CallController::UNLIM); }
				else { UpdateCallController().SetMode(CallController::FIXED); }
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::R))// режим графического ядра
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0 || _event.wheel < 0)
				if(DrawCallController().Mode() == CallController::FIXED)
				{ DrawCallController().SetMode(CallController::UNLIM); }
				else { DrawCallController().SetMode(CallController::FIXED); }
			}
		}
		else if(_event.type == Mouse::WHEEL)
		{
			if(_event.wheel > 0)
			{
				zoom += 1.0f;
			}
			else if(_event.wheel < 0)
			{
				zoom -= 1.0f;
			}	
			camera->SetPosition(Vector3D(camera->Position().x, camera->Position().y, zoom));
		}

		Mouse().ClearEventList();// очистить буфер событий
	}


	// обработка событий клавиатуры
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
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::Z)
			{
				SetZBuffer(!ZBuffer());
			}
			else if(_event.key == Keyboard::V)
			{
				SetVSync(!VSync());
			}	
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий 
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
	Renderer::Define def;
	def.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.realTimeWorkerDef.windowDef.tittle = "Renderer";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::POINT;
	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;


	
	Application* app = new Application;// создать окно
	app->Application::Create(def);
	app->Loop();// запустить цикл событий

	delete app;
	return 0;
}
