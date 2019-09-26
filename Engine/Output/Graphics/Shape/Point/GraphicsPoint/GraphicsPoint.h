#ifndef GRAPHICSPOINT_H
#define GRAPHICSPOINT_H
#include <Engine/Core/Shape/Point/Point.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class GraphicsPoint: public virtual GraphicsObject
{
	public: GraphicsPoint(void);
	public: virtual ~GraphicsPoint(void);
	public: virtual void Draw(void);
	public: static void _Draw(Vector3 _position, Vector3 _color, float _alpha = 1.0f);
	public: static void _Draw(Vector3 _position, Vector4 _color);
	public: static void _Draw(Vector3 _position, RGBA _color);
};




#endif 