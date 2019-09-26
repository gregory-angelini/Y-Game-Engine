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
#include <Engine/Output/Graphics/Shape/Mesh/ColoredMesh/ColoredMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Shape/Mesh/TexturedMesh/TexturedMesh.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>



// класс, реализующий гизмо (компас)
class Gizmo: public GraphicsObject// графический объект
{
	// режим вращения
	public: enum RotationMode
	{
		EULER_SCROLLBAR, // задание ориентации с помощью углов эйлера
		SPHERICAL_SWING_TWIST,// сферические координаты; сфера вращается по самой короткой дуге 
		SPHERICAL_ST_TWO_POLE,// сферические координаты; сфера вращается по самой короткой дуге, от ближайшего полюса по оси Z
		SPHERICAL_FROM_TO//сферические координаты; сфера ориентируется из центра сферы в точку на поверхности сферы
	};


	// DATA
	private: Vector3 xAxis;// направление оси X
	private: Vector3 yAxis;// направление оси Y
	private: Vector3 zAxis;// направление оси Z
	private: RGBA xColor;// цвет оси X
	private: RGBA yColor;// цвет оси Y
    private: RGBA zColor;// цвет оси Z
	private: ColoredMesh *xAxisArrow, *yAxisArrow, *zAxisArrow;// указатели (наконечники) осей X/Y/Z

    private: int32 segments;// количество сегментов, из которых будут состоять кривые
	private: Vector3 point1, point2;// вершины линии
	private: float scale;// масштаб для орбит, чтобы их радиус был больше раидуса сферы, и они не перекрывались ею
	private: float step;// угол между началом и концом линии в радианах
	private: Quaternion qX;// поворот вектора на один шаг (step) вокруг X
	private: Quaternion qY;// поворот вектора на один шаг (step) вокруг оси Y
	private: Quaternion qZ;// поворот вектора на один шаг (step) вокруг оси Z

	private: Vector3 xAxisBegin, xAxisEnd, yAxisBegin, yAxisEnd, zAxisBegin, zAxisEnd;// начало и конец осей X/Y/Z
	private: Font2D* font;// шрифт для отображения названий осей
	private: Quaternion quaternion;// кватернион (ориентацию), что был задан с помощью гизмо
	private: RotationMode rotationMode;// текущий режим вращения


	// METHODS
	// конструктор без параметров
	public: Gizmo(void): segments(360), scale(1.51f), xAxisArrow(NIL), yAxisArrow(NIL), zAxisArrow(NIL), font(NIL)
	{
		rotationMode = EULER_SCROLLBAR;

		step = Angle::DOUBLE_PI / (float)segments;
		qX = Quaternion(Matrix4x4::_RotationX(step));
		qY = Quaternion(Matrix4x4::_RotationY(step));
		qZ = Quaternion(Matrix4x4::_RotationZ(step));

		xAxis = Vector3::UNIT_X;
		yAxis = Vector3::UNIT_Y;
		zAxis = Vector3::UNIT_Z;

		xColor = RGBA(163, 21, 21, 255);
		yColor = RGBA(118, 185, 0, 255);
		zColor = RGBA(55, 111, 194, 255);


		// шрифт...
		Font2D::Define def;
		def.fontName = "Arial";// имя шрифта
		def.fontWeight = Font2D::WEIGHT_BOLD;// жирный шрифт
		def.fontSize = 18;// размер шрифта
		font = new Font2D();
		font->Create(def);


		// ось X...
	    xAxisBegin = -xAxis * (scale * 1.5f);
		xAxisEnd = xAxis * (scale * 1.5f);
	
		xAxisArrow = new ColoredMesh();
		xAxisArrow->CreatePyramid3D(0.04f);
		xAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// вытянуть по оси Y
		xAxisArrow->SetColor(xColor + RGBA(1, 0, 0, 0));
		xAxisArrow->SetPosition(xAxisEnd);
		xAxisArrow->SetRotation(Matrix4x4::_RotationZ(-Angle::HALF_PI));// повернуть на -90 градусов вокруг оси Z


		// ось Y...
	    yAxisBegin = -yAxis * (scale * 1.5f);
		yAxisEnd = yAxis * (scale * 1.5f);
	
		yAxisArrow = new ColoredMesh();
		yAxisArrow->CreatePyramid3D(0.04f);
		yAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// вытянуть по оси Y
		yAxisArrow->SetColor(yColor + RGBA(0, 1, 0, 0));
		yAxisArrow->SetPosition(yAxisEnd);
		

		// ось Z...
	    zAxisBegin = -zAxis * (scale * 1.5f);
		zAxisEnd = zAxis * (scale * 1.5f);
	
		zAxisArrow = new ColoredMesh();
		zAxisArrow->CreatePyramid3D(0.04f);
		zAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// вытянуть по оси Y
		zAxisArrow->SetColor(zColor + RGBA(0, 0, 1, 0));
		zAxisArrow->SetPosition(zAxisEnd);
		zAxisArrow->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));// повернуть на 90 градусов вокруг оси X
	}


	// деструктор
	public: virtual ~Gizmo(void)
	{
		delete font; 

		delete xAxisArrow;
		delete yAxisArrow;
		delete zAxisArrow;
	}


	// возвращает цвет оси вращения X
	public: RGBA GetXColor(void)const
	{
		return xColor;
	}


	// возвращает цвет оси вращения Y
	public: RGBA GetYColor(void)const
	{
		return yColor;
	}


	// возвращает цвет оси вращения Z
	public: RGBA GetZColor(void)const
	{
		return zColor;
	}


	// задает новый режим вращения
	public: void SetRotationMode(RotationMode _mode)
	{
		rotationMode = _mode;
	}


	// возвращает текущий режим вращения
	public: RotationMode GetRotationMode(void)const
	{
		return rotationMode;
	}


	// возвращает текущий режим вращения
	public: StringANSI GetRotationModeString(void)const
	{
		switch(rotationMode)
		{
			case EULER_SCROLLBAR:
			{
				return "EULER_SCROLLBAR";
			}
		
			case SPHERICAL_SWING_TWIST:
			{
				return "SPHERICAL_SWING_TWIST";
			}

			case SPHERICAL_FROM_TO:
			{
				return "SPHERICAL_FROM_TO";
			}

			case SPHERICAL_ST_TWO_POLE:
			{
				return "SPHERICAL_ST_TWO_POLE";
			}
		}
		return "";
	}


	// возвращает кватернион (ориентацию), что был задан с помощью гизмо
	public: Quaternion GetQuaternion(void)const
	{ 
		return quaternion;
	}



	// ВИЗУАЛИЗАЦИЯ...
	// отображает ось X
	private: void DrawXAxis(void)
	{
		GraphicsLine::_Draw(xAxisBegin, xAxisEnd, xColor + RGBA(1, 0, 0, 0));
					
		xAxisArrow->Draw();

		font->SetColor(xColor + RGBA(1, 0, 0, 0));//задать цвет текста
		font->DrawWorldText(xAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "X");// отобразить имя оси
	}


	// отображает ось Y
	private: void DrawYAxis(void)
	{
		GraphicsLine::_Draw(yAxisBegin, yAxisEnd, yColor + RGBA(0, 1, 0, 0));
					
		yAxisArrow->Draw();

		font->SetColor(yColor + RGBA(0, 1, 0, 0));//задать цвет текста
		font->DrawWorldText(yAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Y");// отобразить имя оси
	}


	// отображает ось Z
	private: void DrawZAxis(void)
	{
		GraphicsLine::_Draw(zAxisBegin, zAxisEnd, zColor + RGBA(0, 0, 1, 0));
		
		zAxisArrow->Draw();

		font->SetColor(zColor + RGBA(0, 0, 1, 0));//задать цвет текста
		font->DrawWorldText(zAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Z");// отобразить имя оси
	}


	// отображает гизмо
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			BeginTransform();

			//if(rotationMode == EULER_SCROLLBAR)
			{
				GraphicsLine::_SetLineWidth(3.0f);// толщина линий
					
				// отобразить орбиту вращения вокруг оси оси X...
				point1 = yAxis * scale;// начинаем рисование орбиты с начала оси Y
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qX * point1;// поворачиваем предыдущий вектор на qX
					GraphicsLine::_Draw(point1, point2, xColor + RGBA(1, 0, 0, 0));
					point1 = point2;
				}

				DrawXAxis();// отобразить ось X


				// отобразить орбиту вращения вокруг оси оси Y...
				point1 = xAxis * scale;// начинаем рисование орбиты с начала оси X
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qY * point1;// поворачиваем предыдущий вектор на qY
					GraphicsLine::_Draw(point1, point2, yColor + RGBA(0, 1, 0, 0));
					point1 = point2;
				}

				DrawYAxis();// отобразить ось Y
				

				// отобразить орбиту вращения вокруг оси оси Z...
				point1 = xAxis * scale;// начинаем рисование орбиты с начала оси X
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qZ * point1;// поворачиваем предыдущий вектор на qZ
					GraphicsLine::_Draw(point1, point2, zColor + RGBA(0, 0, 1, 0));
					point1 = point2;
				}

				DrawZAxis();// отобразить ось Z
			
				GraphicsLine::_SetLineWidth(6.0f);// толщина линий
			}
			//else if(rotationMode == SPHERICAL*) 
			{}

			EndTransform();
		}
	}
};



// класс, реализующий графическую сферу
class GraphicsSphere: public ColoredMesh,// графическая полигональная сетка
	                  public LogicObject// логический объект
{
	// идентификаторы выделенной оси вращения
	public: enum
	{
		NO_SELECTED,
		CIRCLE_X,
		CIRCLE_Y,
		CIRCLE_Z
	};

	// DATA
	private: Vector3 xAxis;// направление оси X
	private: Vector3 yAxis;// направление оси Y
	private: Vector3 zAxis;// направление оси Z
	private: RGBA xColor;// цвет оси X
	private: RGBA yColor;// цвет оси Y
    private: RGBA zColor;// цвет оси Z
	private: int32 segments;// количество сегментов, из которых будут состоять кривые
	private: Vector3 point1, point2;// вершины линии
	private: float scale;// масштаб для орбит, чтобы их радиус был больше раидуса сферы, и они не перекрывались ею
	private: float step;// угол между началом и концом линии в радианах
	private: Quaternion qX;// поворот вектора на один шаг (step) вокруг X
	private: Quaternion qY;// поворот вектора на один шаг (step) вокруг оси Y
	private: Quaternion qZ;// поворот вектора на один шаг (step) вокруг оси Z
	private: ColoredMesh* face;// ориентация объекта по умолчанию
	private: RGBA faceColor;// цвет маркера текущей ориентации
	private: int32 selected;// идентификатор выделенной (текущей) оси вращения
	private: SimpleList<Vector3> pathList;// список вершин, описывающих траекторию интерполяции
	private: ColoredMesh* mesh;// цветной круг, который используется для пометки стартовой и целевой позиций интерполяции
	private: Vector3 startLerp, targetLerp;// стартовая и целевая ориентации для интерполяции
	private: bool startLerpValid, targetLerpValid;// если true, то стартовая/целевая позиция для интерполяции была задана
	private: SimpleList<Vector3> vList;// список вершин, описывающий круг
	private: bool showPath;// если true, то отображать траекторию интерполяции


	// METHODS
	// конструктор без параметров
	public: GraphicsSphere(void): segments(360 / 5), scale(1.01f), selected(NO_SELECTED), mesh(NIL), startLerpValid(false), targetLerpValid(false), showPath(false)
	{
		mesh = new ColoredMesh();
		mesh->CreateCircle2D(Circle(Vector3(), 0.07f), 16);
		mesh->SetColor(RGBA(200, 200, 200, 255));

		// взять список вершин, описывающих круг
		for(int32 i = 1; i < mesh->GetAmountOfVertices(); i++)// игнорируем первую вершину, в качестве которой выступает центр круга
		{
			vList.AddToTail(mesh->GetVertex(i));
		}
		vList.AddToTail(mesh->GetVertex(1));// добавить замыкающую линию


		step = Angle::DOUBLE_PI / (float)segments;
		qX = Quaternion(Matrix4x4::_RotationX(step));
		qY = Quaternion(Matrix4x4::_RotationY(step));
		qZ = Quaternion(Matrix4x4::_RotationZ(step));

		xAxis = Vector3::UNIT_X;
		yAxis = Vector3::UNIT_Y;
		zAxis = Vector3::UNIT_Z;

		xColor = RGBA(163, 21, 21, 255);
		yColor = RGBA(118, 185, 0, 255);
		zColor = RGBA(55, 111, 194, 255);

		faceColor = RGBA(RGBA::BLACK_COLOR);

		// создать маркер текущей ориентации
		face = new ColoredMesh();
		face->CreatePyramid3D(0.08f);
		face->SetPosition(zAxis);
		face->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));// повернуть на 90 градусов вокруг оси X
		face->SetColor(faceColor);
	}

	// деструктор
	public: virtual ~GraphicsSphere(void)
	{
		delete face;
		delete mesh;
	}


	// сохраняет текущую ориентацию в качестве стартовой позиции интерполяции
	public: void SetStartQ(void)
	{
		startLerp = GetRotation() * Vector3::UNIT_Z;
		startLerpValid = true;
	}


	// сохраняет текущую ориентацию в качестве целевой позиции интерполяции
	public: void SetTargetQ(void)
	{
		targetLerp = GetRotation() * Vector3::UNIT_Z;
		targetLerpValid = true;
	}


	// обновляет состояние сферы
	public: virtual void Update(float _delta)
	{
		face->SetColor(RGBA::_GetRandom());
	}


	
	// возвращает список вершин, описывающих траекторию интерполяции
	public: SimpleList<Vector3>& GetPathList(void)
	{
		return pathList;
	}
	


	// отображает сферу
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			ColoredMesh::Draw();

			BeginTransform();

			/* сфера имеет радиус 1; вектора осей тоже нормализированы и имеют радиус 1, таким образом нам достаточно 
			   поворачивать вектора чтобы получить точку на поверхности сферы */ 

			// отобразить орбиту вращения вокруг оси оси X...
			point1 = yAxis * scale;// начинаем рисование орбиты с начала оси 
			SimpleList<Vector3> xList;
			xList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{  
				point2 = qX * point1;// поворачиваем предыдущий вектор на qX
				
				xList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_X ? RGBA(RGBA::YELLOW_COLOR) : xColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(xList, selected == CIRCLE_X ? RGBA(RGBA::YELLOW_COLOR) : xColor);
		
			// отобразить орбиту вращения вокруг оси оси Y...
			point1 = xAxis * scale;// начинаем рисование орбиты с начала оси 
			SimpleList<Vector3> yList;
			yList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{
				point2 = qY * point1;// поворачиваем предыдущий вектор на qY
				yList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_Y ? RGBA(RGBA::YELLOW_COLOR) : yColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(yList, selected == CIRCLE_Y ? RGBA(RGBA::YELLOW_COLOR) : yColor);


			// отобразить орбиту вращения вокруг оси оси Z...
			point1 = xAxis * scale;// начинаем рисование орбиты с начала оси 
			SimpleList<Vector3> zList;
			zList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{
				point2 = qZ * point1;// поворачиваем предыдущий вектор на qZ
				zList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_Z ? RGBA(RGBA::YELLOW_COLOR) : zColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(zList, selected == CIRCLE_Z ? RGBA(RGBA::YELLOW_COLOR) : zColor);

			// отобразить ось Up и ось Right для маркера чтобы было ясно как он ориентирован по оси Z
			GraphicsLine::_SetLineWidth(3.0f);// толщина линий
			face->BeginTransform();
			
			GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (xAxis / 4.0f) + Vector3(0, 0.04f, 0), xColor + RGBA(1, 0, 0, 0));
			GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (-zAxis / 4.0f) + Vector3(0, 0.04f, 0), yColor + RGBA(0, 1, 0, 0));// т.к. мы повернули маркер face, то мы не используем напрямую вектор yAxis для представления оси Y
			
			face->EndTransform();
			GraphicsLine::_SetLineWidth(6.0f);// толщина линий

			face->Draw();// отобразить указатель ориентации сферы

			EndTransform();

					
			// рисуем круги для стартовой и целевой позиций интерполяции...	
			if(startLerpValid)// если стартовая позиция была задана
			{
				// помещаем круг поверх сферы
				Vector3 v = Vector3::_FromTo(Vector3(), startLerp);
				v.Normalize();
				v.z *= 1.005f;

				mesh->SetPosition(v);
				mesh->SetRotation(Quaternion(Vector3::UNIT_Z, startLerp));// ориентируем круг из центра сферы на поверхность сферы
				//mesh->Draw(); 


				GraphicsLine::_SetLineWidth(2.0f);// толщина линий
				mesh->BeginTransform();
				GraphicsLine::_Draw(vList, mesh->GetColor());

				// рисуем крестовину для наглядности поворота по оси Z
				//GraphicsLine::_SetLineWidth(4.0f);// толщина линий
				Vector3 v1(0, 0.07f, 0);
				Vector3 v2(0, -0.07f, 0);
				GraphicsLine::_Draw(v1, v2, mesh->GetColor());

				Vector3 v3(-0.07f, 0, 0);
				Vector3 v4(0.07f, 0, 0);
				GraphicsLine::_Draw(v3, v4, mesh->GetColor());
				//GraphicsList::_Draw(v1, v2, mesh->GetColor());
				
				mesh->EndTransform();
			}

			if(targetLerpValid)// если целевая позиция была задана
			{
				// помещаем круг поверх сферы
				Vector3 v = Vector3::_FromTo(Vector3(), targetLerp);
				v.Normalize();
				v.z *= 1.005f;

				mesh->SetPosition(v);
				mesh->SetRotation(Quaternion(Vector3::UNIT_Z, targetLerp));// ориентируем круг из центра сферы на поверхность сферы
				//mesh->Draw(); 

				GraphicsLine::_SetLineWidth(2.0f);// толщина линий
				mesh->BeginTransform();
				GraphicsLine::_Draw(vList, mesh->GetColor());
				
				// рисуем крестовину для наглядности поворота по оси Z
				//GraphicsLine::_SetLineWidth(4.0f);// толщина линий
				Vector3 v1(0, 0.07f, 0);
				Vector3 v2(0, -0.07f, 0);
				GraphicsLine::_Draw(v1, v2, mesh->GetColor());

				Vector3 v3(-0.07f, 0, 0);
				Vector3 v4(0.07f, 0, 0);
				GraphicsLine::_Draw(v3, v4, mesh->GetColor());
				//GraphicsList::_Draw(v1, v2, mesh->GetColor());
				
				mesh->EndTransform();
			}

			//if(showPath)// если нужно отобразить путь интерполяции
			{
				GraphicsLine::_Draw(pathList, RGBA(RGBA::GREEN_COLOR));
			}
		}
	}
 
		
	// возвращает цвет локальной оси вращения X
	public: RGBA GetXColor(void)const
	{
		return xColor;
	}

	// возвращает цвет локальной оси вращения Y
	public: RGBA GetYColor(void)const
	{
		return yColor;
	}


	// возвращает цвет локальной оси вращения Z
	public: RGBA GetZColor(void)const
	{
		return zColor;
	}


	// выделяет вращение вокруг оси X в качестве текущего
	public: void SelectX(void)
	{
		selected = CIRCLE_X;
	}


	// выделяет вращение вокруг оси Y в качестве текущего
	public: void SelectY(void)
	{
		selected = CIRCLE_Y;
	}


	// выделяет вращение вокруг оси Z в качестве текущего
	public: void SelectZ(void)
	{
		selected = CIRCLE_Z;
	}


	// возвращает идентификатор текущей оси вращения
	public: int32 GetSelected(void)const
	{
		return selected;
	}

	// снимает выделение с текущей оси вращения
	public: void Unselect(void)
	{
		selected = NO_SELECTED;
	}


	// СОХРАНЕНИЕ И ЗАГРУЗКА...
	// сохраняет объект в конец файла _file, и возвращает true в случае успеха
	// примечание: если файл не открыт на запись, то возвращается false
	public: virtual bool SaveToFile(File& _file)
	{
		return true;
	}

	
	// загружает объект из файла _file, и возвращает true в случае успеха
	// примечание: если файл не существует или не открыт на чтение, то вернется false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return true;
	}
};



// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera, *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена

	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: GraphicsSphere* sphere;// графическая сфера
	private: Gizmo* gizmo;// гизмо (внешняя неподвижная система координат)

	private: Vector3 previousCursorPosition;// позиция курсора на сфере в предыдущий момент времени
	private: Vector3 startCursorPosition;// позиция курсора на сфере в момент старта вращения сферы
	private: bool startSphereRotation;// если true, то осуществляется вращение сферы
    
    // курсоры
    private: TexturedMesh* clenchedHandCursor;// курсор "сжатая рука"
	private: TexturedMesh* unclenchedHandCursor;// курсор "разжатая рука"
	private: TexturedMesh* arrowCursor;// курсор "стрелка"
	private: StringANSI relativeRotateString;// относительное вращение сферы в виде строки
	private: FloatLerp* fLerp;// числовой интерполятор
	private: QuaternionLerp* qLerp;// кватернионный интерполятор
	private: IAction* action1;// действие
	private: Trigger* updateRotation;// коннектор, осуществляющий связь между сферой и кватернионным интерплятором


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), rightPressed(false), sphere(NIL), gizmo(NIL), startSphereRotation(false), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), fLerp(NIL), qLerp(NIL), action1(NIL), updateRotation(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera;
		delete camera2d;

		delete font;
		delete gizmo;
		delete sphere;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		delete arrowCursor;
		delete fLerp;
		delete qLerp;
		
		//delete action1;
		delete updateRotation;

		delete scene;
	} 


	// возвращает относительные углы вращения pitch/yaw/roll в виде строки
	public: StringANSI GetRelativeRotateString(void)
	{
		return "relative rotate: " + relativeRotateString;
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
				//scene->Rename("scene");// имя сцены
				//AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
				scene->Pick();
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.color = RGBA(RGBA::GRAY_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = false;// запретить вращение камеры
				def.nearZoom = def.nearZoom * 18.0f;
				def.farZoom = def.farZoom * 5.0f;
				def.distanceToViewingPoint = 4.3f;
				def.zoomEnable = true;
				def.projectionMode = Camera3D::PERSPECTIVE_PROJECTION;

				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				


				camera = new Camera3D();
				camera->Create(def);
				
				// задать минимальный и максимальный зум камеры
				//camera->SetNearZoom(camera->GetNearZoom() * 18.0f);
				//camera->SetFarZoom(camera->GetFarZoom() * 5.0f);
				//camera->SetDistanceToViewingPoint(170.0f); 

				camera->Pick(); // выбрать камеру
				//camera->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::PI)));// наблюдать объект в лицо, с противоположной стороны
				scene->BindCamera(camera);
			}
		
			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				
				camera2d = new Camera3D();
				camera2d->Create(def);
			}

			{// графическая сфера...
				sphere = new GraphicsSphere();
				sphere->SetColor(RGBA(230,230,230,255));// серый цвет
				sphere->CreateIcosphere3D(4);// создать сферу на базе икосаэдра
				sphere->Rename("sphere");

				scene->AddGraphicsObject(sphere);// добавить в графическую сцену
				scene->AddLogicObject(sphere);// добавить в логическую сцену
				
				AssetLibrary::_GetAssets().AddObject(sphere);
				//sphere->SetRotation(Quaternion(Matrix4x4::_RotationX(Angle::PI)));// задаем ориентацию "задом наперед"
			}

			// гизмо..
			{
				gizmo = new Gizmo();
				gizmo->SetRotation(Quaternion());
				gizmo->SetShowEnable(false);// скрыть
				scene->AddGraphicsObject(gizmo);// добавить в графическую сцену
			}

			
			// числовой интерполятор...
			{
				fLerp = new FloatLerp();
				fLerp->Rename("FIXED_TIME");// задать имя 
				fLerp->SetPlayingSpeed(3.0f);// скорость движения в секундах
				fLerp->SetTarget(10.0f);// расстояние до цели
				//fLerp->Play();
				fLerp->SetLerpMode(FloatLerp::FIXED_TIME);//единицы в секунду
				scene->AddLogicObject(fLerp);
			}


			// кватернионный интерполятор...
			{
				qLerp = new QuaternionLerp();
				qLerp->Rename("qLerp");
				qLerp->BindFloatLerp(fLerp);
				qLerp->SetFloatLerpBuildInEnable(true);
				AssetLibrary::_GetAssets().AddObject(qLerp);
			}


			
			// триггер, обновляющий ориентацию сферы...
			{
				updateRotation = new Trigger();
				//updateRotation->Rename("update_rotation");
				updateRotation->SetActionExpr("compute_quaternion_current");// реакция
			
				// действие...
				action1 = Trigger::_CreateAction(QuaternionLerp::COMPUTE_QUATERNION_CURRENT_MESSAGE, "qLerp", "sphere");
				action1->Rename("compute_quaternion_current");
				AssetLibrary::_GetAssets().AddObject(action1);

				updateRotation->SetEventExpr("");// условия
			}
			

			// курсор "разжатая рука"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("unclenched");

				unclenchedHandCursor = new TexturedMesh();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(24, 29)));
				unclenchedHandCursor->BindAnimation(anim);
				
				unclenchedHandCursor->ConvertTo2DObject();
				unclenchedHandCursor->SetShowEnable(true);// включить отображение
				//scene->AddGraphicsObject(unclenchedHandCursor);// добавить в графическую сцену
			}


			// курсор "сжатая рука"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("clenched");

				clenchedHandCursor = new TexturedMesh();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(22, 24)));
				clenchedHandCursor->BindAnimation(anim);

				clenchedHandCursor->SetShowEnable(true);// включить отображение
				clenchedHandCursor->ConvertTo2DObject();
			}

			
			// курсор "стрелка"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("cursor");

				arrowCursor = new TexturedMesh();
				arrowCursor->CreateQuad2D(Quad(Vector3(14/2, 24/2, 0), Vector2(14, 24)));
				arrowCursor->BindAnimation(anim);

				arrowCursor->SetShowEnable(true);// включить отображение
				arrowCursor->ConvertTo2DObject();
				Mouse::_PickCursor(arrowCursor);// показать курсор
			}


			// мышь...
			Mouse::_SetShowSystemCursor(false);// скрыть системный курсор
			Mouse().SetKeyPressedPeriodInSec(0.04f);// увеличить чувствительность мыши

	
			GraphicsLine::_SetLineWidth(6.0f);// толщина линий
			GraphicsPoint::_SetPointSize(3.0f);// размер точек


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));// белый цвет

			MoveToCenter();// переместить окно в центр экрана
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}



	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		if(fLerp->IsPlaying())
		{
			Vector3 v = sphere->GetTransformation() * Vector3::UNIT_Z;
			v *= 1.005f;
			sphere->GetPathList().AddToTail(v);
		}
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
		

		// задать новую трансформацию сцены...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;

		font->DrawScreenText(Vector3(10, i+=20, -1), "gizmo.edit mode: %s", gizmo->GetRotationModeString().c_str());

		if(sphere->GetSelected() != GraphicsSphere::NO_SELECTED)
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "%s", GetRelativeRotateString().c_str());
		}

		
			
		font->DrawScreenText(Vector3(10, i+=20, -1), "quaternion: %.3f/%.3f/%.3f/%.3f", sphere->GetRotation().x, sphere->GetRotation().y, sphere->GetRotation().z, sphere->GetRotation().w);

		/*
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.buildIn: %d", qLerp->IsFloatLerpBuildInEnabled());

		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.mode: %d", qLerp->GetFloatLerp()->GetLerpMode());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing duration: %f", qLerp->GetFloatLerp()->GetPlayingDurationInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing speed: %f", qLerp->GetFloatLerp()->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing: %d", qLerp->GetFloatLerp()->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.target: %f", qLerp->GetFloatLerp()->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.current: %f", qLerp->GetFloatLerp()->GetCurrent());


		*/ 
		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_DOWN)// если кнопка отжата
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик
			{
				if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					if(!rightPressed)// если камера не вращается
					{
						// начало вращения сферы...
						if(!startSphereRotation && sphere->GetSelected() != GraphicsSphere::NO_SELECTED)
						{
							Mouse::_PickCursor(clenchedHandCursor);// показать курсор

							Vector3 intersectedPoint;// точка пересечения 

							if(sphere->IsIntersected(camera->GetSelectRay(), intersectedPoint) != -1)// если есть пересечение курсора со сферой
							{
								startSphereRotation = true;
								startCursorPosition = intersectedPoint;// запомнить стартовую позицию курсора
								previousCursorPosition = intersectedPoint;// взять позицию курсора
							}
						}
					}
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)// если кнопка зажата
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик
			{
				if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_ST_TWO_POLE)
				{
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// если прозошел клик по сфере
					{
						Vector3 v1(Vector3::UNIT_Z);
						
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);
					
						if(Vector3::_GetDot(v1, v2) < 0.0f)// если вектора указывают в разных направления
						{
							v1 = -Vector3::UNIT_Z;// лицавая сторона сферы будет ориентирована вдоль отрицательной оси Z
							sphere->SetRotation(Quaternion(v1, v2));
							sphere->SetRotation(sphere->GetRotation() * Quaternion(Euler(0.0f, Angle::PI, 0.0f)));

							//sphere->GetControlPoints().AddToTail(sphere->GetRotation() * Vector3::UNIT_Z);
						}
						else
						{
							sphere->SetRotation(Quaternion(v1, v2));
							//sphere->GetControlPoints().AddToTail(sphere->GetRotation() * Vector3::UNIT_Z);
						}
					}
				}
				else if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_SWING_TWIST)
				{
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// если прозошел клик по сфере
					{
						Vector3 v1(Vector3::UNIT_Z);
						
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);	
						
						sphere->SetRotation(Quaternion(v1, v2));
					}
				}
				else if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_FROM_TO)
				{					
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// если прозошел клик по сфере
					{
						sphere->SetRotation(Quaternion(Matrix3x3<Vector3>::_LookAt(sphere->GetPosition(), intersectedPoint, Vector3::UNIT_Y)));
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// правый клик
			{
				// начало вращения камеры...
				if(!rightPressed)// выполняем только один раз
				{
					Mouse::_MoveCursorToCenter(Renderer::_Picked());// перемещаем мышь в центр клиентской области окна 
					camera->SetRotationEnable(true);// разрешить вращение камеры
					rightPressed = true;
					Mouse::_PickCursor(NIL);
					sphere->Unselect();// снять выделение
					//camera->SetZoomEnable(false);// запретить зуминг
				}
			}
		} 
		else if(_event->type == Mouse::KEY_UP)// если кнопка отпущена
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик
			{
				if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					// конец вращения сферы...
					if(startSphereRotation) 
					{ 
						startSphereRotation = false; // завершить вращение
						Mouse::_PickCursor(unclenchedHandCursor);// показать курсор
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// если правый клик
			{
				// конец вращения камеры...
				if(rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(false); // запретить вращение камеры
					rightPressed = false;
					Mouse::_PickCursor(arrowCursor);
				}
			}
		}	
		else if(_event->type == Mouse::WHEEL)// вращение колесика мыши
		{
			if(!rightPressed)// если камера не вращается
			{
				Mouse::_PickCursor(arrowCursor);// показать курсор
				sphere->Unselect();// снять выделение
			}
		}
		else if(_event->type == Mouse::MOVE)
		{
			if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
			{
				if(!startSphereRotation && !rightPressed)// если камера не вращается и сфера не вращается
				{
					if(sphere->GetXColor() == GetPixelColorUnderCursor())// если курсор указывает на локальную ось вращения X
					{
						sphere->SelectX();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// показать курсор
					}
					else if(sphere->GetYColor() == GetPixelColorUnderCursor())// если курсор указывает на локальную ось вращения Y
					{
						sphere->SelectY();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// показать курсор
					}
					else if(sphere->GetZColor() == GetPixelColorUnderCursor())// если курсор указывает на локальную ось вращения Z
					{
						sphere->SelectZ();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// показать курсор
					}
					else
					{
						// реагируем на серый (цвет сферы)и белый цвета (цвет фона)
						if(GetPixelColorUnderCursor() == RGBA(230,230,230,255) || GetPixelColorUnderCursor() == RGBA(RGBA::WHITE_COLOR))
						{
							Mouse::_PickCursor(arrowCursor);// показать курсор

							sphere->Unselect();// снять выделение
							gizmo->SetShowEnable(false);
						}
					}
				}
				else if(startSphereRotation && !rightPressed)// если камера не вращается и сфера вращается
				{
					Vector3 intersectedPoint;// точка пересечения 

					if(sphere->IsIntersected(camera->GetSelectRay(), intersectedPoint) != -1)// если есть пересечение курсора со сферой
					{
						Vector3 currentCursorPosition = intersectedPoint;// взять позицию курсора

						Vector3 v1 = Vector3::_FromTo(sphere->GetPosition(), previousCursorPosition);
						v1.Normalize();// нормализировать вектор перед тем как его вращать
						v1 = Quaternion::_GetInverse(sphere->GetRotation()) * v1;// сбрасываем трансформацию сферы, получая аналогичные точки на сфере, но без учета трансформаций вращения; теперь углы между векторами относительно глобальных осей X/Y/Z будут справедливы независимо от трансформации вращения, действующей на сферу

						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), currentCursorPosition);
						v2.Normalize();
						v2 = Quaternion::_GetInverse(sphere->GetRotation()) * v2;

						Vector3 v3 = Vector3::_FromTo(sphere->GetPosition(), startCursorPosition);
						v3.Normalize();// нормализировать вектор перед тем как его вращать
						v3 = Quaternion::_GetInverse(sphere->GetRotation()) * v3;// сбрасываем трансформацию сферы, получая аналогичные точки на сфере, но без учета трансформаций вращения; теперь углы между векторами относительно глобальных осей X/Y/Z будут справедливы независимо от трансформации вращения, действующей на сферу

						switch(sphere->GetSelected())
						{
							case GraphicsSphere::CIRCLE_X:
							{
								float x1 = v1.AngleX();
								float x2 = v2.AngleX();
								
								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationX(x2 - x1)));
								previousCursorPosition = currentCursorPosition;
								
			
								float x3 = v3.AngleX();
								relativeRotateString = "[" + NumberToString((int32)Angle::_RadiansToDegrees(x2 - x3)) + ", 0, 0]";
								break;
							}

							case GraphicsSphere::CIRCLE_Y:
							{
								float y1 = v1.AngleY();
								float y2 = v2.AngleY();

								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationY(y2 - y1)));
								previousCursorPosition = currentCursorPosition;
								

								float y3 = v3.AngleY();
								relativeRotateString = "[0, " + NumberToString((int32)Angle::_RadiansToDegrees(y2 - y3)) + ", 0]";
								break;
							}

							case GraphicsSphere::CIRCLE_Z:
							{
								float z1 = v1.AngleZ();
								float z2 = v2.AngleZ();

								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationZ(z2 - z1)));
								previousCursorPosition = currentCursorPosition;
								
								
								float z3 = v3.AngleZ();
								relativeRotateString = "[0, 0, " + NumberToString((int32)Angle::_RadiansToDegrees(z2 - z3)) + "]";
								break;
							}
						}
					}
				}
			}
		}

		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

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
			else if(_event->key == Keyboard::X)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)));// наблюдать фронтально
					gizmo->SetShowEnable(true);
					
					// инвертируем вращение для правильного скроллинга
					camera->SetReverseScrollY(false);
					camera->SetViewingPointYAxis(Vector3::UNIT_Y);
				}
			} 
			else if(_event->key == Keyboard::Y)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Euler(-Angle::HALF_PI, Angle::HALF_PI, 0)));// наблюдать объект сверху
					gizmo->SetShowEnable(true);
					
					// инвертируем вращение для правильного скроллинга
					camera->SetReverseScrollY(true);
					camera->SetViewingPointYAxis(Vector3::UNIT_X);
				}
			} 
			else if(_event->key == Keyboard::Z)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Euler(0.0f, 0.0f, 0.0f)));// наблюдать объект в лицо, с противоположной стороны
					gizmo->SetShowEnable(true);
					
					// инвертируем вращение для правильного скроллинга
					camera->SetViewingPointYAxis(Vector3::UNIT_Y);
				}
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				gizmo->SetRotationMode(Gizmo::EULER_SCROLLBAR);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_SWING_TWIST);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::NUM_3)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_ST_TWO_POLE);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::NUM_4)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_FROM_TO);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::H)
			{
				sphere->SetRotation(Quaternion());
				//qLerp->Set(Quaternion(), Quaternion());
			}
			else if(_event->key == Keyboard::A)
			{
				qLerp->SetFrom(sphere->GetRotation());
				sphere->SetStartQ();
			}
			else if(_event->key == Keyboard::B)
			{
				qLerp->SetTo(sphere->GetRotation());
				sphere->SetTargetQ();
			}
			else if(_event->key == Keyboard::SPACE)
			{
				qLerp->GetFloatLerp()->Stop();
				qLerp->GetFloatLerp()->Play();
				sphere->GetPathList().Clear();
			}
			else if(_event->key == Keyboard::C)
			{
				delete action1;
			}
			/*
			else if(_event->key == Keyboard::S)
			{
				qLerp->SaveToFile();
				delete qLerp;
				delete fLerp;
			}
			else if(_event->key == Keyboard::L)
			{
				qLerp = new QuaternionLerp();
				//qLerp->Destroy();
				qLerp->LoadFromFile("qLerp");
				AssetLibrary::_GetAssets().AddObject(qLerp);

				sphere->SetRotation(qLerp->GetFrom());
				sphere->SetStartQ();

				sphere->SetRotation(qLerp->GetTo());
				sphere->SetTargetQ();

				scene->AddLogicObject(qLerp->GetFloatLerp());

				updateRotation->SetValue(true);
			}
			*/ 
		} 

		return _event;
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "QuaternionLerp.example_01";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
