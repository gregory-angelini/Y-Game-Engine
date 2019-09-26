#ifndef GRAPHICSSTRIP_H
#define GRAPHICSSTRIP_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class GraphicsStrip: public virtual GraphicsObject
{
	public: enum: int64
	{
		GRAPHICS_STRIP_CLASS = 1073741824
	};
	public: enum
	{
			CHANGE_STRIP_VECTORS_MESSAGE = 237,
			CHANGE_STRIP_TRANSLATABLES_MESSAGE = 238,
			CHANGE_STRIP_SCALABLES_MESSAGE = 239,
			CHANGE_STRIP_COLORABLES_MESSAGE = 240
	};
	private: SimpleList<Vector3> vertices;
	public: GraphicsStrip(void);
	public: virtual ~GraphicsStrip(void);
	public: SimpleList<Vector3>& GetVertices(void);
	public: virtual void Destroy(void);
	public: virtual void Draw(void);
	public: static void _Draw(SimpleList<Vector3>& _vertices, Vector3 _color, float _alpha = 1.0f);
	public: static void _Draw(SimpleList<Vector3>& _vertices, Vector4 _color);
	public: static void _Draw(SimpleList<Vector3>& _vertices, RGBA _color);
	public: using AbstractObject::SaveToFile;
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	public: using AbstractObject::LoadFromFile;
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};



#endif 