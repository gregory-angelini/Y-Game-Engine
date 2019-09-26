#ifndef GRAPHICSLINE_H
#define GRAPHICSLINE_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class GraphicsLine: public virtual GraphicsObject,
	                public virtual Line
{
	public: GraphicsLine(void);
	public: virtual ~GraphicsLine(void);
	public: virtual void Draw(void);
	public: static void _Draw(Vector3 _origin, Vector3 _end, Vector3 _color, float _alpha = 1.0f);
	public: static void _Draw(Vector3 _origin, Vector3 _end, Vector4 _color);
	public: static void _Draw(Vector3 _origin, Vector3 _end, RGBA _color);
};



#endif 