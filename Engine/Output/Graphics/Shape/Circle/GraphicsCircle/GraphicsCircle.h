#ifndef GRAPHICSCIRCLE_H
#define GRAPHICSCIRCLE_H
#include <Engine/Core/Shape/Point/Point.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Shape/Circle/Circle.h>
class GraphicsCircle: public virtual GraphicsObject,
	                  public Circle
{
	public: int32 segments;
	private: bool wireframeEnable;
	public: GraphicsCircle(void);
	public: virtual ~GraphicsCircle(void);
	public: void SetWireframeEnable(bool _enable);
	public: bool IsWireframeEnabled(void)const;
	public: virtual void Draw(void);
	public: static void _Draw(Vector3 _position, float _radius, int32 _segments, Vector3 _color, float _alpha = 1.0f, bool _wireframe = true);
	public: static void _Draw(Vector3 _position, float _radius, int32 _segments, Vector4 _color, bool _wireframe = true);
	public: static void _Draw(Vector3 _position, float _radius, int32 _segments, RGBA _color, bool _wireframe = true);
};




#endif 