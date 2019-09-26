// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include "MyTree.h"




// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene* scene;// графическая сцена

	private: MyTree tree;// бинарное дерево

	private: bool testEnable;// если true, то будут автоматически создаваться и уничтожаться узлы дерева со случайными ключами


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), testEnable(false)
	{}

			
	// виртуальный деструктор
	public: virtual ~Application(void)
	{
		delete scene;
	} 

	// создает приложение
	public: bool Create(Renderer::Define _define)
	{
		// создать графическое ядро
		if(Renderer::Create(_define))
		{
			// сцена...
			{
				
				scene = new GraphicsScene();
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA::WHITE_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}

			 


			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;
				def.pitchMax = 0.0f;// запретить камере опускаться ниже пола (плоскости XZ)
				def.zoomMax = 15;
				def.zoomMin = 6;



				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				camera->Pick("SimpleBinaryTree.example_01"); 
				
				// задать изометрический ракурс камеры
				//camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, Angle::HALF_PI/3.0f, 0.0f);
			}


			SetBackgroundColor(RGBA::GRAY_COLOR);// цвет фона
			
			GraphicsPoint<Vector3D>::_SetPointSize(10);// размер точек 
			GraphicsLine<Vector3D>::_SetLineWidth(3.0f);// толщина линий


			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна	
			Mouse::_ShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}




		


	// функция кадра
	public: virtual void Update(float _delta)
	{
		if(testEnable) { tree.Test(0, 1000); }

		// двигаем зайца за текстуру и обратно
		//quad->SetPosition(quad->GetPosition() + Vector3D(0,0,0.01));
	}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		//scene->Draw();
		tree.Draw();// отобразить дерево
	
		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		int32 i = 0;
		
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Size: %d", tree.Size()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Root: %d", tree.Root()); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.IsEmpty: %d", tree.IsEmpty()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "marked node: %d", tree.Marked());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected node: %d", tree.Selected());

		if(tree.Selected())// если есть выделенный узел
		{
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.data: %d", tree.Selected()->data);

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.left: %d", tree.Selected()->Child(0));
	
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.right: %d", tree.Selected()->Child(1));

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.parent: %d", tree.Selected()->Parent());
		}

		
	
		
		
		
	

		
	
		//int32 val;
		

		//uint64 uu = 0;
		//Bitset::_SetBit(uu, 0, 1);
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.sizeOfTexture: %.2f/%.2f", quad->Texture()->SizeOfTexture().width, quad->Texture()->SizeOfTexture().height); 
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %d", glGetTextureLevelParameteriv(GL_TEXTURE_2D, , 0, GL_TEXTURE_WIDTH)); 
		//Bitset::_SetBit(uu, 1, 1);
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %s", Bitset::_String(uu).c_str()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %d", uu); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vertices: %d", mesh->AmountOfVertices()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "indices: %d", mesh->AmountOfIndices()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "normals: %d", mesh->AmountOfNormals()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeinByte: %d", mesh->SizeInByte()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeInKbyte: %.5f", Memory::_FromByteToKB(mesh->SizeInByte())); 
 


		/*
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape detail: %d", mesh->DetailLevel()); 
	    font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape name: %s", mesh->ShapeNameString().c_str()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "colored material.type: %s", mesh->ColoredMaterialTypeString().c_str());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape attributes: %s", mesh->StringAttributes().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape vertex colors: %d", mesh->AmountOfVertexColors()); 

		*/ 


		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
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
			if(_event.key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			} 
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Child(0)); }
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Child(1)); }
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Parent()); } 
			}
			else if(_event.key == Keyboard::S)
			{
				tree.Select(tree.Root());  
			}
			else if(_event.key == Keyboard::M)
			{
				tree.SetMark(tree.Selected());// отметить выделенный узел
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				tree.Select(tree.Extremum(false));
			}
			else if(_event.key == Keyboard::NUM_5)
			{ 
				tree.Select(tree.Extremum(true));
			}
			else if(_event.key == Keyboard::C)
			{
				tree.Select(NIL);// сбросить выделенный узел
				tree.SetMark(NIL);// обнулить отмеченный узел
		
				tree.Clear();
			}
			else if(_event.key == Keyboard::F)
			{
				MyTree::Node* finded = NIL;
				if(tree.Marked()) { finded = tree.Find(tree.Marked()->data); } // найти отмеченный узел
				if(finded) { tree.Select(finded); } // выделить найденный узел
			}
			else if(_event.key == Keyboard::R)
			{ 
				tree.Del(tree.Selected());//исключить выделенный узел
			}
			else if(_event.key == Keyboard::A)
			{
				int32 key = Random::_RandomInt32(0, 10);

				if(tree.Add(key))
				{
					tree.Select(tree.Find(key));// выделить добавленный узел
				}
			}
			else if(_event.key == Keyboard::T)
			{
				testEnable = !testEnable;
				
				if(testEnable)// если нужно запустить тест
				{
					tree.Select(NIL);
					tree.SetMark(NIL);
				}
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
	def.realTimeWorkerDef.windowDef.tittle = "SimpleBinaryTree.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	def.faceCullMode[Renderer::BACK] = false;
	def.zBufferEnabled = false;
	def.blendEnable = true;// цветовое смешивание
	
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;
	//def.vSync = false;
	//def.drawCallController.SetMode(CallController::UNLIM);

	
	Application* app = new Application;// создать окно
	
	if(app->Application::Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
