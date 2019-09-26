#ifndef GRAPHICSQUAD_H
#define GRAPHICSQUAD_H
#include <Engine/Core/Shape/Point/Point.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Shape/Quad/Quad.h>
class GraphicsQuad: public virtual GraphicsObject,
	                public Quad
{
	private: bool wireframeEnable;
	public: GraphicsQuad(void);
	public: virtual ~GraphicsQuad(void);
	public: void SetWireframeEnable(bool _enable);
	public: bool IsWireframeEnabled(void)const;
	public: virtual void Draw(void);
	public: static void _Draw(Quad _quad, Vector3 _color, float _alpha = 1.0f, bool _wireframe = true);
	public: static void _Draw(Quad _quad, Vector4 _color, bool _wireframe = true);
	public: static void _Draw(Quad _quad, RGBA _color, bool _wireframe = true);
};




#endif 