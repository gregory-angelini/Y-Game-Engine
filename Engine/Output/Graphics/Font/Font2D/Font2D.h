#ifndef FONT2D_H
#define FONT2D_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
int CALLBACK EnumFontProc(ENUMLOGFONTEXA* _logicFont,
						  NEWTEXTMETRICEXA* _physicFont,
						  DWORD _fontType,
						  LPARAM _lparam);
class Font2D: public virtual GraphicsObject
{
	public: enum
	{
		WEIGHT_DEFAULT = FW_DONTCARE,
		WEIGHT_NORMAL = FW_NORMAL,
		WEIGHT_BOLD = FW_BOLD,
		WEIGHT_HEAVY = FW_HEAVY
	}; 
	public: class FontType: public MagicNode
	{
		friend int CALLBACK EnumFontProc(ENUMLOGFONTEXA* _logicFont, NEWTEXTMETRICEXA* _physicFont, DWORD _fontType, LPARAM _lparam);
		private: StringANSI name;
		private: StringANSI type;
		public: FontType(void);
		private: FontType(StringANSI _name, StringANSI _type);
		public: StringANSI Name(void)const;
		public: StringANSI Type(void)const;
	};
	public: class Define
	{
		public: int32 fontSize;
		public: StringANSI fontName;
		public: int32 fontWeight;
		public: bool italic;
		public: int32 textBufferSize;
		public: Vector4 color;
		public: Define(void);
	};
	private: int32 fontSize;
	private: StringANSI fontName;
	private: int32 fontWeight;
	private: bool italic;
    private: int32 textBufferSize;
	private: GLuint base;
	private: bool fontInitialized;
	private: char* textBuffer;
	private: static MagicList<FontType> supportedFonts;
	private: static bool checkSupportedFonts;
	private: static Font2D* pickedFont;
	friend int CALLBACK EnumFontProc(ENUMLOGFONTEXA* _logicFont, NEWTEXTMETRICEXA* _physicFont, DWORD _fontType, LPARAM _lparam);
	public: Font2D(void);
	public: virtual ~Font2D(void);
	public: bool Create(Define _define);
    public: void Pick(void);
	public: static Font2D* _GetPicked(void);
    public: bool IsPicked(void)const;
	private: void CreateTextBuffer(void);
	private: void DestroyTextBuffer(void);
    public: int32 TextBufferSize(void)const;
	public: int32 FontSize(void)const;
	private: void SetFontWeight(int32 _weight);
	public: StringANSI FontName(void)const;
	public: static void _CheckSupportedFonts(Renderer* _renderer);
	public: static bool _IsFontSupported(StringANSI _fontName);
	public: static MagicList<FontType>& _SupportedFonts(void);
	public: bool Italic(void)const;
	public: int32 FontWeight(void)const;
	public: void Destroy(void);
	public: void DrawScreenText(Vector3 _position, StringANSI _string, ...);
	public: void DrawWorldText(Vector3 _position, StringANSI _string, ...);
	public: virtual void Draw(void);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};




#endif 