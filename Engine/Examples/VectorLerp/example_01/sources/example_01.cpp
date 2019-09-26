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
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>


// класс, реализующий графическую точку
class GraphicsPointEx: public TexturedSprite// текстурированный спрайт
{
	// DATA
	private: bool selected;// если true, то вершина ывделена

	// METHODS
	// конструктор без параметров
	public: GraphicsPointEx(void): selected(false)
	{}


	// выделяет точку
	public: void Select(void)
	{
		selected = true;
		SetColor(RGBA(255, 0, 0, 255));
	}


	// снимает выделение с точки
	public: void Unselect(void)
	{
		selected = false;
		SetColor(RGBA(255, 255, 255, 255));
	}


	// возвращает true если точка выделена
	public: bool IsSelected(void)const
	{
		return selected;
	}
};

// класс, реализующий цветной квад в трехмерном пространстве
class GraphicsQuad: public TexturedSprite// текстурированный спрайт
{
	// DATA
	private: Quad size;// размер квада


	// METHODS
	// конструктор без параметров
    public: GraphicsQuad(void)
	{}


	// инициализирующий конструктор
	public: GraphicsQuad(Vector3D _leftBottom, Vector3D _rightTop)
	{
		size = Quad(_leftBottom, _rightTop);// запомнить размер
		CreateQuad2D(size);// создать квад
	}


	// деструктор
    public: virtual ~GraphicsQuad(void)
	{
		Destroying();
	}


	// возвращает размер (ширину и высоту) квада 
	public: Vector2D GetSize(void)const
	{
		return size.GetSize();
	}


	// возвращает true если точка _point находится внутри квада
	public: bool IsPointInside(Vector3D _point)
	{
		// каждая вершина квада должна находится на своем месте; если это не так, мы меняем местами вершины прямоугольника	
		return size.IsPointInside(_point);
	}

	// визуализация квада
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			// нарисовать контур квада...

			BeginTransform();
			TexturedSprite::Draw();
			GraphicsLine::_Draw(GetVertex(0), GetVertex(1), GetColor());
			GraphicsLine::_Draw(GetVertex(1), GetVertex(2), GetColor());
			GraphicsLine::_Draw(GetVertex(2), GetVertex(3), GetColor());
			GraphicsLine::_Draw(GetVertex(3), GetVertex(0), GetColor());
			EndTransform();
		}
	}

	// задает левую нижнюю вершину прямоугольника
	// примечание: остальные вершины будут автоматически изменены таким образом, чтобы углы прямоугольника оставались прямыми
	public: void SetVertex1(Vector3D _position)
	{
		size.SetVertex1(_position);

		// обновить вершины сетки
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

				
	// задает правую нижнюю вершину прямоугольника
	// примечание: остальные вершины будут автоматически изменены таким образом, чтобы углы прямоугольника оставались прямыми
	public: void SetVertex2(Vector3D _position)
	{
		size.SetVertex2(_position);

		// обновить вершины сетки
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

			
	// задает правую верхнюю вершину прямоугольника
	// примечание: остальные вершины будут автоматически изменены таким образом, чтобы углы прямоугольника оставались прямыми
	public: void SetVertex3(Vector3D _position)
	{
		size.SetVertex3(_position);

		// обновить вершины сетки
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

			
	// задает левую верхнюю вершину прямоугольника
	// примечание: остальные вершины будут автоматически изменены таким образом, чтобы углы прямоугольника оставались прямыми
	public: void SetVertex4(Vector3D _position)
	{
		size.SetVertex4(_position);

		// обновить вершины сетки
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}
};



// пользовательское приложение
class Application: public Engine,// движок
         	       public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена

	private: GraphicsPointEx* fantomVertex;// фантомная вершина
	private: GraphicsLine* fantomLine;// фантомная линия

	private: VectorLerp* vectorLerp;// интерполятор вектора
	private: MagicList<GraphicsPointEx> vertices;// созданные вершины
	private: Texture2D* texture;// текстура точки
	private: GraphicsPointEx* lockedVertex;// перемещаемая вершина
    private: TexturedSprite* clenchedHandCursor;// курсор "сжатая рука"
	private: TexturedSprite* unclenchedHandCursor;// курсор "разжатая рука"
	private: bool showControlVertices;// если true, то отображать линии между контрольными вершинами
	private: GraphicsQuad* fantomQuad;// фантомный квад
	private: float step;// шаг смещения выделенных вершин при их перемещении
	
 


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), vectorLerp(NIL), fantomVertex(NIL), fantomLine(NIL), texture(NIL), lockedVertex(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), showControlVertices(true), fantomQuad(NIL), step(0.009f)
	{
		vertices.SetClearMode(MagicContainer::DELETE_NODE);
	}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera;
		delete font;

		delete fantomVertex;
		delete fantomLine;

		delete vectorLerp;
		delete texture;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		
		delete scene;
	} 



	// создает приложение
	public: bool Create(Engine::Define _define)
	{
		// создаем движок...
		if(Engine::Create(_define))
		{
			// сцена...
			{
				scene = new Scene();
				AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
				scene->Pick();
			}

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontColor = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}
	


			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::TRACKBALL_FIRST_PERSON;
				def.rotationEnabled = false;// запретить вращение камеры
			
				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(); // выбрать камеру
			}
		

			
			{// vectorLerp
				vectorLerp = new VectorLerp();
				vectorLerp->SetAmountOfSegment(30);
				//scene->AddLogicObject(vectorLerp);
			}


			// курсор "разжатая рука"...
			{
				unclenchedHandCursor = new TexturedSprite();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
				unclenchedHandCursor->SetTexturedMaterialEnable(true);
				unclenchedHandCursor->BindTexture(AssetLibrary::_LoadCommonAsset<Texture2D>("unclenched"));

				unclenchedHandCursor->SetShowEnable(true);// включить отображение
				scene->AddGraphicsObject(unclenchedHandCursor);// добавить в графическую сцену
			}


			// курсор "сжатая рука"...
			{
				clenchedHandCursor = new TexturedSprite();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
				clenchedHandCursor->SetTexturedMaterialEnable(true);
				clenchedHandCursor->BindTexture(AssetLibrary::_LoadCommonAsset<Texture2D>("clenched"));

				clenchedHandCursor->SetShowEnable(true);// включить отображение
				scene->AddGraphicsObject(clenchedHandCursor);// добавить в графическую сцену
			}

			// фантомный квад...
			{
				fantomQuad = new GraphicsQuad(Vector3D(0.10f, 0.10f, -0.998f), Vector3D(-0.10f, -0.10f, -0.998f));
				fantomQuad->SetColor(RGBA(0, 255, 0, 50));// цвет вершины
				Texture2D* texture = new Texture2D();
				texture->Create(Vector2D(8, 8));
				fantomQuad->SetTexturedMaterialEnable(true);
				fantomQuad->BindTexture(texture);
				fantomQuad->SetShowEnable(false);// выключить отображение
				//scene->AddGraphicsObject(fantomQuad);// добавить в графическую сцену
			}

			// фантомная вершина...
			{
				texture = AssetLibrary::_LoadCommonAsset<Texture2D>("point");
				fantomVertex = new GraphicsPointEx();
				fantomVertex->CreateQuad2D(Quad(Vector3D(), Vector2D(0.010f, 0.010f)));
				fantomVertex->SetTexturedMaterialEnable(true);
				fantomVertex->BindTexture(texture);

				//fantomVertex->SetColor(RGBA(255,0,0,255));// цвет вершины
				//GraphicsPointEx::_SetPointSize(6.0f);// задать размер точек
				fantomVertex->SetShowEnable(false);// выключить отображение
				scene->AddGraphicsObject(fantomVertex);// добавить в графическую сцену
			}
						
			// фантомная линия...
			{
				fantomLine = new GraphicsLine();
				
				// цвет линии
				fantomLine->SetColor(RGBA(234, 170, 68, 90));// цвет линии
				GraphicsLine::_SetLineWidth(2.0f);// задать толщину линий
				fantomLine->SetShowEnable(false);// выключить отображение
				scene->AddGraphicsObject(fantomLine);// добавить в графическую сцену
			}

			Keyboard().SetKeyPressedPeriodInSec(0.08f);


			// цвет фона...
			SetBackgroundColor(RGBA(67, 67, 67, 255));
			
			Mouse::_ShowCursor(true);// показать курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}



	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		if(Camera3D::_GetPicked())// если есть выделенная камера
		{
			Camera3D::_GetPicked()->BeginPerspective3D();// вывод трехмерной графики
		}

		// визуализировать исходную линию
		if(showControlVertices)
		{
			for(SimpleList<Vector3D>::Node* it = vectorLerp->GetControlVertices().GetHead(); it != NIL; it = it->GetRight())
			{
				if(it->GetRight())
				{
					GraphicsLine::_Draw(it->key + Vector3D(0, 0, -0.002), it->GetRight()->key + Vector3D(0, 0, -0.002), RGBA(234, 170, 68, 255));
				}
			}
		}
		

		// визуализировать интерполированную линию
		for(SimpleList<Vector3D>::Node* it = vectorLerp->GetCurveVertices().GetHead(); it != NIL; it = it->GetRight())
		{
			if(it->GetRight())
			{
				GraphicsLine::_Draw(it->key + Vector3D(0, 0, -0.001), it->GetRight()->key + Vector3D(0, 0, -0.001), RGBA(0, 255, 0, 255));
			}
		}

		

		if(Scene::_GetPicked())// если есть текущая сцена
		{
			Scene::_GetPicked()->Draw();// отобразить объекты сцены
		}

		//Engine::Draw(_delta);
					

		camera->BeginScreen2D();

		
		// вывод текста...
		int32 i = 0;

		
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of segment: %d", vectorLerp->GetAmountOfSegment());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of control vertices: %d", vectorLerp->GetControlVertices().GetSize());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of curve vertices: %d", vectorLerp->GetCurveVertices().GetSize());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "lerp mode: %s", vectorLerp->GetLerpMode() == VectorLerp::BAZIER ? "BAZIER" : "LAGRANGE");
	}


			
	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		// вычислить позицию курсора в мировых координатах...
		Vector3D worldPos;
		camera->SetProjectionType(Camera3D::PERSPECTIVE_PROJECTION);

		Vector3D winPos = Vector3D(Mouse::_CursorPosition(this).x, Mouse::_CursorPosition(this).y, 0.9);// рисуем на дальней плоскости отсечения
		winPos.y = ClientAreaSize().y - Mouse::_CursorPosition(this).y - 1; // инвертировать ось y; позиция курсора представлена в экранной системе координат, где ось Y растет сверху вниз

		camera->UnProject(winPos, worldPos);//получить мировые координаты курсора
					

		if(_event->type == Mouse::KEY_PRESSED)// если произошло зажатие кнопки
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если зажат левый клик
			{
				if(lockedVertex == NIL)// если нет перемещаемой вершины
				{
					// проверить попытку захвата новой вершины...
					if(!fantomVertex->IsShowEnabled())// если не создается новая вершина
					{
						Vector3D intersectionPoint;// точка пересечения курсора и одной из опорных вершин кривой

						// если курсор указывает на одну из существующих вершин...
						for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
						{
							if(it->GetNode()->IsUnderCursor(intersectionPoint) != -1)// если вершина сталкивается с курсором
							{
								// начать перемещение вершины...
								lockedVertex = it->GetNode();// захватить перемещаемую вершину
								
								// показать специализированный курсор...	
								unclenchedHandCursor->SetShowEnable(false);// выключить отображение
								scene->GetDrawList().Remove(unclenchedHandCursor, MagicContainer::REMOVE_NODE);// исключить из графической сцены
								
								if(Mouse::_IsShowCursor()) { Mouse::_ShowCursor(false); }// скрыть курсор 
								clenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// обновить позицию
								
								clenchedHandCursor->SetShowEnable(true);// включить отображение
								scene->AddGraphicsObject(clenchedHandCursor);// добавить в графическую сцену
								// спрятать фантомную линию...
								//fantomLine->SetShowEnable(false);// выключить отображение
										
								// спрятать фантомную вершину...
								//fantomVertex->SetShowEnable(false);// выключить отображение
								break;// прервать цикл
							}
						}
					}

					// начало создания новой вершины...
					if(lockedVertex == NIL && !fantomVertex->IsShowEnabled())// иначе произошел клик в пустую область и если не создается новая вершина
					{
						if(!unclenchedHandCursor->IsShowEnabled())// если нет фокуса на вершине (приготовление к захвату)
						{
							// обновить позицию фантомной вершины...
							fantomVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));
							fantomVertex->SetShowEnable(true);// включить отображение

							// отобразить фантомную линию...
							if(vectorLerp->GetControlVertices().IsNotEmpty())// если есть вершины на сцене
							{
								// отобразить фантомную линию между последней вершиной списка и фантомной вершиной
								fantomLine->Set(vectorLerp->GetControlVertices().GetTail()->key, fantomVertex->GetPosition());// создать фантомную линию
								fantomLine->SetShowEnable(true);// включить отображение
							}
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// начало выделения
			{
				if(!fantomQuad->IsShowEnabled())// выполнить только один раз
				{
					// обновить фантомный квад...
					fantomQuad->SetShowEnable(true);// отобразить
					scene->AddGraphicsObject(fantomQuad);// добавить в графическую сцену

					fantomQuad->SetVertex4(Vector3D(worldPos.x, worldPos.y, -0.998f));// задать левую верхнюю вершину	
					fantomQuad->SetVertex2(Vector3D(worldPos.x, worldPos.y, -0.998f));// задать правую нижнюю вершину
				} 
			}
		}
		else if(_event->type == Mouse::KEY_UP)// иначе кнопка отпущена
		{
			if(_event->key == Mouse::LEFT_BUTTON)
			{
				if(lockedVertex)// если мы перемещаем вершину
				{
					// скрыть специализированный курсор...
					clenchedHandCursor->SetShowEnable(false); // выключить отображение
					scene->GetDrawList().Remove(clenchedHandCursor, MagicContainer::REMOVE_NODE);// исключить из графической сцены
								
					unclenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// обновить позицию 
					unclenchedHandCursor->SetShowEnable(true);// включить отображение
					scene->AddGraphicsObject(unclenchedHandCursor);// добавить в графическую сцену
					lockedVertex = NIL;// закончить перемещение вершины
				}
				else// иначе мы создаем новую вершину
				{
					// конец создания новой вершины...

					// спрятать фантомную линию...
					fantomLine->SetShowEnable(false);// выключить отображение
							
					// спрятать фантомную вершину...
					fantomVertex->SetShowEnable(false);// выключить отображение

					// обновить кривую...
					if((vectorLerp->GetLerpMode() == VectorLerp::BAZIER && vectorLerp->GetControlVertices().GetSize() < 10) || vectorLerp->GetLerpMode() == VectorLerp::LAGRANGE)// ставим ограничитель на 10 вершин для режима BAZIER
					{
						// создать реальную вершину на базе фантомной
						if(vectorLerp->GetControlVertices().AddUniqueToTail(fantomVertex->GetPosition()))
						{					
							// создать дублирующую графическую вершину...
							GraphicsPointEx* vertex = new GraphicsPointEx();
							vertex->CreateQuad2D(Quad(Vector3D(), Vector2D(0.010f, 0.010f)));
							vertex->SetPosition(fantomVertex->GetPosition());
							vertex->SetTexturedMaterialEnable(true);
							vertex->BindTexture(texture);
							scene->AddGraphicsObject(vertex);// добавить в графическую сцену
							vertices.AddToTail(vertex);

							vectorLerp->Build();// перестроить кривую
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// конец выделения
			{
				if(fantomQuad->IsShowEnabled()) // если рисуется квад
				{
					// спрятать фантомный квад...
					fantomQuad->SetShowEnable(false);// скрыть
					scene->GetDrawList().Remove(fantomQuad, MagicContainer::REMOVE_NODE);// исключить из графической сцены

					// проверить все контрольные вершины на предмет пересечения с фантомным квадом...
					for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
					{
						if(fantomQuad->IsPointInside(it->GetNode()->GetPosition()))
						{
							it->GetNode()->Select(); // выделить точку
						}
						else
						{
							it->GetNode()->Unselect();// снять выделение
						}
					}
				}
			}
		}
		else if(_event->type == Mouse::MOVE)// если курсор движется
		{
			if(fantomQuad->IsShowEnabled())// если рисуется квад
			{
				fantomQuad->SetVertex2(Vector3D(worldPos.x, worldPos.y, -0.998f));// задать правую нижнюю вершину

				if(fantomQuad->IsClockwise())// если вершины расположены по часовой стрелке
				{ 
					fantomQuad->InverseVertices();// обратить направление вершин
				}

				// проверить все контрольные вершины на предмет пересечения с фантомным квадом...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(fantomQuad->IsPointInside(it->GetNode()->GetPosition()))
					{
						it->GetNode()->Select(); // выделить точку
					}
					else
					{
						it->GetNode()->Unselect();// снять выделение
					}
				}
				
				if(lockedVertex == NIL)// если курсор не указывает на опорную вершину, и если вершина не перемещается
				{
					unclenchedHandCursor->SetShowEnable(false);// выключить отображение
					if(!Mouse::_IsShowCursor()) { Mouse::_ShowCursor(true); }// показать курсор 
				}
			}
			else if(lockedVertex)// если перемещаем вершину
			{
				// обновить кривую... 
				lockedVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));// обновить позицию перемещаемой вершины
				int32 index = dynamic_cast<MagicList<GraphicsPointEx>::AddressDerived*>(lockedVertex->GetAddress(&vertices))->GetIndex();// взять индекс графической вершины в списке; этот индекс совпадает с индексом опорной вершины в интерполяторе vectorLerp
				vectorLerp->GetControlVertices()[index]->key = lockedVertex->GetPosition();// обновить позицию опорной вершины
				vectorLerp->Build();// перестроить кривую

					
				// обновить позицию специализированного курсора...
				clenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// обновить позицию
			}
			else if(lockedVertex == NIL && !fantomVertex->IsShowEnabled())// если нет перемещаемой вершины и если не создается новая вершина
			{
				bool collision = false;

				// проверить все опорные вершины на предмет пересечения с курсором...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					Vector3D intersectionPoint;// точка пересечения курсора и одной из опорных вершин кривой

					// если курсор указывает на одну из существующих вершин...
					if(it->GetNode()->IsUnderCursor(intersectionPoint) != -1)// если вершина сталкивается с курсором
					{
						collision = true; 

						// показать специализированный курсор...
						if(Mouse::_IsShowCursor()) { Mouse::_ShowCursor(false); }// скрыть курсор 

						unclenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// обновить позицию 
						unclenchedHandCursor->SetShowEnable(true);// включить отображение
						break;// прервать цикл
					}
				}

				if(!collision && lockedVertex == NIL)// если курсор не указывает на опорную вершину, и если вершина не перемещается
				{
					unclenchedHandCursor->SetShowEnable(false);// выключить отображение
					if(!Mouse::_IsShowCursor()) { Mouse::_ShowCursor(true); }// показать курсор 
				}
			}
			else if(lockedVertex == NIL)// если создается новая вершина
			{
				// обновить позицию фантомной вершины...
				fantomVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));

				// отобразить фантомную линию...
				if(vectorLerp->GetControlVertices().IsNotEmpty())// если есть вершины на сцене
				{
					// отобразить фантомную линию между последней вершиной списка и фантомной вершиной
					fantomLine->Set(vectorLerp->GetControlVertices().GetTail()->key, fantomVertex->GetPosition());// создать фантомную линию
				}
			}
		}

		return Engine::MouseEventFilter(_event);
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
			else if(_event->key == Keyboard::C)
			{
				vectorLerp->GetControlVertices().Clear();
				vectorLerp->GetCurveVertices().Clear();
				vertices.Clear();
				lockedVertex = NIL;// сбросить перемещаемую вершину
			}
			else if(_event->key == Keyboard::S)
			{
				showControlVertices = !showControlVertices;
			}
			else if(_event->key == Keyboard::D)
			{
				MagicList<GraphicsPointEx> deletionList;// список удаления

				// удалить все выбранные контрольные вершины...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetTailAddress(); it != NIL; it = it->GetLeft())// идем с конца в начало, т.к. в обратном случае индексация для контрольных вершин будет отличаться от индексов дублирующего списка графических вершин
				{
					if(it->GetNode()->IsSelected())// если вершина выделена
					{
						int32 index = it->GetIndex();// взять индекс вершины
						vectorLerp->GetControlVertices().Remove(vectorLerp->GetControlVertices()[index]);// удалить аналогичную вершину из интерполятора
						deletionList.AddToTail(it->GetNode());// добавить в список удаления
					}
				}
				deletionList.Clear(MagicContainer::DELETE_NODE);

				vectorLerp->Build();// перестроить кривую
			}
			if(_event->key == Keyboard::NUM_1)
			{
				vectorLerp->SetLerpMode(VectorLerp::BAZIER);
				vectorLerp->Build();// перестроить кривую
			}
			if(_event->key == Keyboard::NUM_2)
			{
				vectorLerp->SetLerpMode(VectorLerp::LAGRANGE);
				vectorLerp->Build();// перестроить кривую
			}
		}
		else if(_event->type == Keyboard::KEY_PRESSED)
		{
			if(_event->key == Keyboard::MINUS)
			{
				vectorLerp->SetAmountOfSegment(vectorLerp->GetAmountOfSegment() - 1);
				
				vectorLerp->Build();// перестроить кривую
			}
			else if(_event->key == Keyboard::EQUALS)
			{
				vectorLerp->SetAmountOfSegment(vectorLerp->GetAmountOfSegment() + 1);
				
				vectorLerp->Build();// перестроить кривую
			}
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				// переместить все выделенные вершины на один шаг влево по оси X
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// если вершина выделена
					{
						int32 index = it->GetIndex();// взять индекс вершины
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key - Vector3D(step, 0.0f, 0.0f);// изменить аналогичную вершину
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() - Vector3D(step, 0.0f, 0.0f));
					}
				}
				vectorLerp->Build();// перестроить кривую
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				// переместить все выделенные вершины на один шаг вправо по оси X
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// если вершина выделена
					{
						int32 index = it->GetIndex();// взять индекс вершины
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key + Vector3D(step, 0.0f, 0.0f);// изменить аналогичную вершину
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() + Vector3D(step, 0.0f, 0.0f));
					}
				}
				vectorLerp->Build();// перестроить кривую
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				// переместить все выделенные вершины на один шаг вверх по оси Y
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// если вершина выделена
					{
						int32 index = it->GetIndex();// взять индекс вершины
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key + Vector3D(0.0f, step, 0.0f);// изменить аналогичную вершину
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() + Vector3D(0.0f, step, 0.0f));
					}
				}
				vectorLerp->Build();// перестроить кривую
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				// переместить все выделенные вершины на один шаг вниз по оси Y
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// если вершина выделена
					{
						int32 index = it->GetIndex();// взять индекс вершины
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key - Vector3D(0.0f, step, 0.0f);// изменить аналогичную вершину
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() - Vector3D(0.0f, step, 0.0f));
					}
				}
				vectorLerp->Build();// перестроить кривую
			}
		}


		return Engine::KeyboardEventFilter(_event);
	}
};



// точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание движка...
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "VectorLerp.example_01";
	//def.rendererDef.zBufferEnabled = false;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
