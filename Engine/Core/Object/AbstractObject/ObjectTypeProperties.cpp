#include "AbstractObject.h"
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
#include <Engine/Core/Parser/List/FloatList/FloatList.h>
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h>
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>
#include <Engine/Core/Parser/List/TranslatableObjectList/TranslatableObjectList.h>
#include <Engine/Core/Parser/List/RotatableObjectList/RotatableObjectList.h>
#include <Engine/Core/Parser/List/ScalableObjectList/ScalableObjectList.h>
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>
#include <Engine/Core/Parser/List/AbstractObjectList/AbstractObjectList.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
#include <Engine/Core/Parser/Set/Set.h>
#include <Engine/Core/Parser/List/SetList/SetList.h>




AbstractObject::ObjectTypeProperties::ObjectTypeProperties(void)
{
	ObjectTypeProperty* property;
	property = new ObjectTypeProperty(AbstractObject::ABSTRACT_OBJECT_CLASS);
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(LogicObject::LOGIC_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(ColorableObject::COLORABLE_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(TranslatableObject::TRANSLATABLE_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(RotatableObject::ROTATABLE_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(ScalableObject::SCALABLE_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(GraphicsObject::GRAPHICS_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)ColorableObject::COLORABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)TranslatableObject::TRANSLATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ScalableObject::SCALABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)RotatableObject::ROTATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(GraphicsStrip::GRAPHICS_STRIP_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsObject::GRAPHICS_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(ObjectScene::OBJECT_SCENE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(LogicScene::LOGIC_SCENE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)ObjectScene::OBJECT_SCENE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)LogicObject::LOGIC_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(GraphicsScene::GRAPHICS_CONTAINER_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsObject::GRAPHICS_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ObjectScene::OBJECT_SCENE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(TiledSpace3D::TILED_SPACE_3D_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsContainer::GRAPHICS_CONTAINER_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(GraphicsScene::GRAPHICS_SCENE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsContainer::GRAPHICS_CONTAINER_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Scene::SCENE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)LogicScene::LOGIC_SCENE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsScene::GRAPHICS_SCENE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractSurface::ABSTRACT_SURFACE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(PolygonalSurface::POLYGONAL_SURFACE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractSurface::ABSTRACT_SURFACE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)TranslatableObject::TRANSLATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ScalableObject::SCALABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)RotatableObject::ROTATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(GraphicsSurface::GRAPHICS_SURFACE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)PolygonalSurface::POLYGONAL_SURFACE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsObject::GRAPHICS_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(TexturingSurface::TEXTURING_SURFACE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)GraphicsSurface::GRAPHICS_SURFACE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Mesh::MESH_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Hitbox::HITBOX_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(PlayingObject::PLAYING_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Timer::TIMER_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)PlayingObject::PLAYING_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)LogicObject::LOGIC_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Set::SET_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Plane::PLANE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractSurface::ABSTRACT_SURFACE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)TranslatableObject::TRANSLATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractList::ABSTRACT_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<int32>::INTEGER_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<float>::FLOAT_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<bool>::BOOLEAN_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<StringANSI>::STRING_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<Vector2>::VECTOR_2_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<Vector3>::VECTOR_3_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<Quaternion>::QUATERNION_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<AbstractObject>::OBJECT_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<TranslatableObject>::TRANSLATABLE_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<ScalableObject>::SCALABLE_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<RotatableObject>::ROTATABLE_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<ColorableObject>::COLORABLE_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(List<Set>::SET_LIST_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractList::ABSTRACT_LIST_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Texture2D::TEXTURE_2D_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Atlas2D::ATLAS_2D_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractAnimation::ABSTRACT_ANIMATION_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)PlayingObject::PLAYING_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)LogicObject::LOGIC_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(TexturedAnimation::TEXTURED_ANIMATION_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractAnimation::ABSTRACT_ANIMATION_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractVariable::ABSTRACT_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<int32>::INTEGER_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<float>::FLOAT_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<bool>::BOOLEAN_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<StringANSI>::STRING_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ColorableObject::COLORABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)TranslatableObject::TRANSLATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ScalableObject::SCALABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractVariable::ABSTRACT_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)RotatableObject::ROTATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(ActivatableObject::ACTIVATABLE_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractEvent::EVENT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)Variable<bool>::BOOLEAN_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ActivatableObject::ACTIVATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Trigger::TRIGGER_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)Variable<bool>::BOOLEAN_VARIABLE_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)ActivatableObject::ACTIVATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(PlayerControl::PLAYER_CONTROL_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(AbstractAction::ACTION_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(PlayingObject::PLAYING_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(FloatLerp::FLOAT_LERP_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)LogicObject::LOGIC_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)PlayingObject::PLAYING_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(LerpingObject::LERPING_OBJECT_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)AbstractObject::ABSTRACT_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(QuaternionLerp::QUATERNION_LERP_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)LerpingObject::LERPING_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(VectorLerp::VECTOR_LERP_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)LerpingObject::LERPING_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
	property = new ObjectTypeProperty(Camera3D::CAMERA_3D_CLASS);
	property->AddChildProperty(supportedProperties.Find((int64)TranslatableObject::TRANSLATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	property->AddChildProperty(supportedProperties.Find((int64)RotatableObject::ROTATABLE_OBJECT_CLASS, &ObjectTypeProperty::GetType));
	supportedProperties.Add(property, &ObjectTypeProperty::GetType);
}
