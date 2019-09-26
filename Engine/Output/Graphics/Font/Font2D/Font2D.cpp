#include "Font2D.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>


Font2D* Font2D::pickedFont = NIL;
 

int CALLBACK EnumFontProc(ENUMLOGFONTEXA* _logicFont, NEWTEXTMETRICEXA* _physicFont, DWORD _fontType, LPARAM _lparam)
{
	if(_fontType & TRUETYPE_FONTTYPE)
	{
		Font2D::supportedFonts.AddToTail(new Font2D::FontType((char*)_logicFont->elfFullName, "TRUE_TYPE"));
	}
	else if(_fontType & RASTER_FONTTYPE)
	{
		Font2D::supportedFonts.AddToTail(new Font2D::FontType((char*)_logicFont->elfFullName, "RASTER"));
	}
	return 1;
}


MagicList<Font2D::FontType> Font2D::supportedFonts;
bool Font2D::checkSupportedFonts = false;



Font2D::Font2D(void): fontInitialized(false), textBuffer(NIL)
{}



Font2D::~Font2D(void)
{
	Destroy();
	
	Destroying();
}



bool Font2D::_IsFontSupported(StringANSI _fontName)
{
	return Font2D::supportedFonts.Find(_fontName, &Font2D::FontType::Name) != NIL;
}

void Font2D::_CheckSupportedFonts(Renderer* _renderer)
{
	if(_renderer)
	{ 
		if(!Font2D::checkSupportedFonts)
		{
			LOGFONTA logicFont;
			logicFont.lfCharSet = DEFAULT_CHARSET;
			logicFont.lfFaceName[0] = '\0';

			EnumFontFamiliesExA(_renderer->GetDeviceContext(), &logicFont, (FONTENUMPROCA)EnumFontProc, 0, 0);
			Font2D::checkSupportedFonts = true;

			Font2D::supportedFonts.Unique(&Font2D::FontType::Name, NIL);
		}
	}
}

int32 Font2D::FontWeight(void)const
{
	return fontWeight;
}


void Font2D::SetFontWeight(int32 _weight)
{
	if(_weight < 0) { _weight = 0; }
	else if(_weight > 1000) { _weight = 1000; }
	
	fontWeight = _weight;

}








StringANSI Font2D::FontName(void)const
{
	return fontName;
}



void Font2D::Destroy(void)
{
	if(IsPicked()) { Font2D::pickedFont = NIL; } 

	if(fontInitialized)
	{
		glDeleteLists(base, 96);
		DestroyTextBuffer();
		fontInitialized = false;
	}
}


void Font2D::DestroyTextBuffer(void)
{
	if(textBuffer)
	{ 
		delete []textBuffer;
		textBuffer = NIL;
	}
}

void Font2D::CreateTextBuffer(void)
{
	DestroyTextBuffer();
	textBuffer = new char[textBufferSize + 1];
}





MagicList<Font2D::FontType>& Font2D::_SupportedFonts(void)
{
	return supportedFonts;
}


bool Font2D::Create(Font2D::Define _define)
{
	if(fontInitialized)
	{  
		Destroy();
	}
  	fontSize = _define.fontSize;
	fontName = _define.fontName;
	SetFontWeight(_define.fontWeight);
	italic = _define.italic;
	textBufferSize = _define.textBufferSize;
	SetColorRGBA(_define.color);

	if(Font2D::_IsFontSupported(fontName))
	{
		HFONT font = CreateFontA(-fontSize,
								0,
								0,
								0,
								fontWeight,
								italic,
								0,
								0,
								ANSI_CHARSET,
								OUT_TT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								ANTIALIASED_QUALITY,
								FF_DONTCARE | DEFAULT_PITCH,
								fontName.c_str());
			
		if(font)
		{
			HFONT hOld = (HFONT)SelectObject(Renderer::_GetPicked()->GetDeviceContext(), font);
			base = glGenLists(96);
			wglUseFontBitmaps(Renderer::_GetPicked()->GetDeviceContext(),
							  32,
							  96,
							  base);
		 
			SelectObject(Renderer::_GetPicked()->GetDeviceContext(), hOld);
			DeleteObject(font);
			CreateTextBuffer();
			fontInitialized = true;
			return true;
		}
	}
	return false;
}


bool Font2D::Italic(void)const
{
	return italic;
}


int32 Font2D::TextBufferSize(void)const
{
	return textBufferSize;
}


int32 Font2D::FontSize(void)const
{
	return fontSize;
}




void Font2D::DrawScreenText(Vector3 _position, StringANSI _string, ...)
{
	if(textBuffer)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		va_list argList;
		va_start(argList, _string);
		_vsnprintf(textBuffer, textBufferSize, _string.c_str(), argList);
		va_end(argList);

		glColor4fv(GetColorRGBA().GetData());

		glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32); 
		glRasterPos3f(_position.x, _position.y, _position.z);
		glCallLists(strlen(textBuffer), GL_UNSIGNED_BYTE, textBuffer);
		glPopAttrib();

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
	}
}



void Font2D::Draw(void)
{}


void Font2D::DrawWorldText(Vector3 _position, StringANSI _string, ...)
{
	if(textBuffer)
	{
		va_list argList;
		va_start(argList, _string);
		int32 result = _vsnprintf(textBuffer, textBufferSize, _string.c_str(), argList);
		va_end(argList);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(GetColorRGBA().GetData());

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32); 
	glRasterPos3f(_position.x, _position.y, _position.z);
	glCallLists(strlen(textBuffer), GL_UNSIGNED_BYTE, textBuffer);
	glPopAttrib();

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}



bool Font2D::IsPicked(void)const
{
	return (Font2D::pickedFont == this);
}


Font2D* Font2D::_GetPicked(void)
{
	return Font2D::pickedFont;
}



void Font2D::Pick(void)
{
	Font2D::pickedFont = this;
}


bool Font2D::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool Font2D::LoadFromFile(File& _file)
{
	return false;
}