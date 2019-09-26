#ifndef MATERIAL_H
#define MATERIAL_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Numerical/Numerical.h>
// CLASS --------------------------------------------------------------------------------------------


// класс, реализующий источник света
class Light: public virtual AbstractObject // объект
{
	// METHODS
	// конструктор без параметров
	public: Light(void);



	// деструктор
	public: virtual ~Light(void);
};

#endif 