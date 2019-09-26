#ifndef GRAPHICSPOLYGON_H
#define GRAPHICSPOLYGON_H
#include <Engine/Core/Shape/Point/Point.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Shape/Quad/Quad.h>
class GraphicsPolygon: public virtual GraphicsObject
{
	private: bool wireframeEnable;
    public: SimpleList<Vector3> vertices;
	public: GraphicsPolygon(void);
	public: virtual ~GraphicsPolygon(void);
	public: void SetWireframeEnable(bool _enable);
	public: bool IsWireframeEnabled(void)const;
	public: virtual void Draw(void);
	public: static void _Draw(SimpleList<Vector3>& _vertices, Vector3 _color, float _alpha = 1.0f, bool _wireframe = true);
	public: static void _Draw(SimpleList<Vector3>& _vertices, Vector4 _color, bool _wireframe = true);
	public: static void _Draw(SimpleList<Vector3>& _vertices, RGBA _color, bool _wireframe = true);
};




#endif 