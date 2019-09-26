#include "UniversalDelegate.h"
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
#include <Engine/Core/Parser/List/FloatList/FloatList.h>
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h>
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>
#include <Editor/UniversalDelegate/LoadingModeEditor/LoadingModeEditor.h>
#include <Editor/UniversalDelegate/LimitEditor/LimitEditor.h>
#include <Editor/UniversalDelegate/ActionTypeEditor/ActionTypeEditor.h>
#include <Editor/UniversalDelegate/AssetShortEditor/AssetShortEditor.h>
#include <Editor/UniversalDelegate/AssetLongEditor/AssetLongEditor.h>
#include <Engine/Core/Parser/Variable/AbstractVariable.h>
#include <Editor/UniversalDelegate/EventTypeEditor/EventTypeEditor.h>
#include <Editor/BoolSpinBox/BoolSpinBox.h>
#include <Editor/UniversalDelegate/IntEditorEx/IntEditorEx.h>
#include <Editor/UniversalDelegate/FloatEditorEx/FloatEditorEx.h>
#include <Editor/TriggerCreator/EnumerationExprLineEdit/EnumerationExprLineEdit.h>
#include <Editor/TriggerCreator/BooleanExprLineEdit/BooleanExprLineEdit.h>
#include <Editor/TriggerCreator/EqualityExprLineEdit/EqualityExprLineEdit.h>
#include <Editor/TriggerCreator/StringExprLineEdit/StringExprLineEdit.h>
#include <Editor/TriggerCreator/ArithmeticExprLineEdit/ArithmeticExprLineEdit.h>
#include <Editor/UniversalDelegate/StringExprEditor/StringExprEditor.h>
#include <Editor/UniversalDelegate/BooleanExprEditor/BooleanExprEditor.h>
#include <Editor/UniversalDelegate/EqualityExprEditor/EqualityExprEditor.h>
#include <Editor/UniversalDelegate/ArithmeticExprEditor/ArithmeticExprEditor.h>
#include <Editor/UniversalDelegate/EnumerationExprEditor/EnumerationExprEditor.h>
#include <Editor/UniversalDelegate/VariableEditor/VariableEditor.h>
#include <Editor/UniversalDelegate/ListEditor/ListEditor.h>
#include <Editor/UniversalDelegate/Vector3Editor/Vector3Editor.h>
#include <Editor/UniversalDelegate/Vector2Editor/Vector2Editor.h>
#include <Editor/UniversalDelegate/QuaternionEditor/QuaternionEditor.h>
#include <Editor/UniversalDelegate/NameEditor/NameEditor.h>
#include <Editor/UniversalDelegate/VirtualKeyEditor/VirtualKeyEditor.h>
#include <Engine/Core/Parser/Variable/Variable.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Editor/UniversalDelegate/ScalableObjectEditor/ScalableObjectEditor.h>
#include <Editor/UniversalDelegate/ColorableObjectEditor/ColorableObjectEditor.h>
#include <Editor/UniversalDelegate/TranslatableObjectEditor/TranslatableObjectEditor.h>
#include <Editor/UniversalDelegate/RotatableObjectEditor/RotatableObjectEditor.h>
#include <Editor/UniversalDelegate/LogicObjectEditor/LogicObjectEditor.h>
#include <Editor/UniversalDelegate/GraphicsObjectEditor/GraphicsObjectEditor.h>
#include <Editor/UniversalDelegate/LerpingObjectEditor/LerpingObjectEditor.h>
#include <Editor/UniversalDelegate/PlayingObjectEditor/PlayingObjectEditor.h>
#include <Editor/UniversalDelegate/ActivatableObjectEditor/ActivatableObjectEditor.h>
#include <Engine/Core/Time/Timer/Timer.h>



UniversalDelegate::UniversalDelegate(QObject* _parent): QStyledItemDelegate(_parent)
{}


UniversalDelegate::~UniversalDelegate(void)
{
	Clear();
}


void UniversalDelegate::SetColumnDelegate(int _column, int _editorType) 
{
	columnDelegate[_column] = _editorType;
}


void UniversalDelegate::SetCellDelegate(const QModelIndex _index, int _editorType) 
{
	cellDelegate[_index] = _editorType;
}



void UniversalDelegate::CloseEditor(QWidget* _widget)
{
	emit commitData(_widget);
	
	if(!_widget->underMouse())
	{
		emit closeEditor(_widget);
	}
}


QWidget* UniversalDelegate::createEditor(QWidget* _parent, const QStyleOptionViewItem& _option, const QModelIndex& _index)const
{
	Q_UNUSED(_option)

	int currentRole = -1;
	
	if(columnDelegate.contains(_index.column()))
	{
		currentRole = columnDelegate.value(_index.column(), NO_EDIT);
	}
	else
	{
		if(cellDelegate.contains(_index))
		{
			currentRole = cellDelegate.value(_index, NO_EDIT);
		}
	}
	switch(currentRole)
	{
		case TEXT_EDITOR:
		{
			QLineEdit *editor = new QLineEdit(_parent);
			return editor;
		}

		case LIMIT_EDITOR: 
		{
			LimitEditor *editor = new LimitEditor(_parent);
			return editor;
		}

		case LOADING_MODE_EDITOR:
		{
			LoadingModeEditor *editor = new LoadingModeEditor(_parent);
			return editor;
		}

		case ACTION_TYPE_EDITOR:
		{
			ActionTypeEditor *editor = new ActionTypeEditor(_parent);
			return editor;
		}

		case EVENT_TYPE_EDITOR:
		{
			EventTypeEditor *editor = new EventTypeEditor(_parent);
			return editor;
		}

		case COMBO_BOX_EDITOR:
		{
			QComboBox *editor = new QComboBox(_parent);

			QList<QString> values;

			if(columnDelegate.contains(_index.column()))
			{
				values = valuesOfComboForColumn.value(_index.column());
			}
			else if(cellDelegate.contains(_index))
			{ 
				values = valuesOfComboForCell.value(_index);
			}

			QList<QString>::const_iterator it = values.begin();
			while(it != values.end())
			{
				QString value = *it;
				editor->addItem(value);
				++it;
			}
			return editor;
		}

		case NAME_EDITOR:
		{
			NameEditor *editor = new NameEditor(_parent);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			return editor;   
		}

		case ANY_ASSET_EDITOR:
		{
			AssetShortEditor *editor = new AssetShortEditor(_parent);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case OBJECT_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			
			editor->SetFileType(AbstractObject::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TIMER_EDITOR: 
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			
			editor->SetFileType(Timer::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case SCENE_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			 
			editor->SetFileType(ObjectScene::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case PLAYER_CONTROL_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);

			editor->SetFileType(PlayerControl::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ABSTRACT_LIST_EDITOR:
		{  
			AssetLongEditor* editor = new AssetLongEditor(_parent); 

			editor->SetFileType(AbstractList::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case INTEGER_LIST_EDITOR:
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<int32>::INTEGER_TYPE);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*))); 
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case SET_LIST_EDITOR:
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<Set>::SET_TYPE);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*))); 
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_LIST_EDITOR:
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<float>::FLOAT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case BOOLEAN_LIST_EDITOR: 
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<bool>::BOOLEAN_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case STRING_LIST_EDITOR: 
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<StringANSI>::STRING_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		} 

		case VECTOR_2_LIST_EDITOR: 
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<Vector2>::VECTOR_2_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_LIST_EDITOR: 
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<Vector3>::VECTOR_3_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case QUATERNION_LIST_EDITOR:  
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<Quaternion>::QUATERNION_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TRANSLATABLE_LIST_EDITOR:  
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*))); 
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case SCALABLE_LIST_EDITOR:  
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<ScalableObject>::SCALABLE_OBJECT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ROTATABLE_LIST_EDITOR:  
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<RotatableObject>::ROTATABLE_OBJECT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case COLORABLE_LIST_EDITOR:  
		{
			ListEditor *editor = new ListEditor(_parent);
			editor->SetListType(List<ColorableObject>::COLORABLE_OBJECT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case MESH_EDITOR:
		{  
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Mesh::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}
			
		case HITBOX_EDITOR:
		{  
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Hitbox::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TRIGGER_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Trigger::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ACTION_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(AbstractAction::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case EVENT_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(AbstractEvent::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TEXTURED_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(TexturedAnimation::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ABSTRACT_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(TexturedAnimation::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ATLAS_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Atlas2D::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TEXTURE_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Texture2D::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case CAMERA_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(Camera3D::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_LERP_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(FloatLerp::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_LERP_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(VectorLerp::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case QUATERNION_LERP_EDITOR:
		{
			AssetLongEditor* editor = new AssetLongEditor(_parent);
			editor->SetFileType(QuaternionLerp::_GetFileSuffix().c_str());

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case BOOL_EDITOR:
		{
			BoolSpinBox *editor = new BoolSpinBox(_parent); 

			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}
	
		case INT_EDITOR: 
		{
			IntSpinBox *editor = new IntSpinBox(_parent);

			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case INT_EX_EDITOR: 
		{
			IntEditorEx *editor = new IntEditorEx(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_EDITOR: 
		{
			FloatSpinBox *editor = new FloatSpinBox(_parent);

			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_EX_EDITOR: 
		{
			FloatEditorEx *editor = new FloatEditorEx(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}
		
		case FLOAT_0_1_EDITOR: 
		{
			FloatEditorEx *editor = new FloatEditorEx(_parent);
			editor->Set_0_1_Mode();

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_0_360_EDITOR: 
		{
			FloatEditorEx *editor = new FloatEditorEx(_parent);
			editor->Set_0_360_Mode();

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ENUMERATION_EXPR_EDITOR:
		{
			EnumerationExprEditor *editor = new EnumerationExprEditor(_parent);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case BOOLEAN_EXPR_EDITOR:
		{
			BooleanExprEditor *editor = new BooleanExprEditor(_parent);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case EQUALITY_EXPR_EDITOR:
		{
			EqualityExprEditor *editor = new EqualityExprEditor(_parent);
						
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case STRING_EXPR_EDITOR:
		{
			StringExprEditor *editor = new StringExprEditor(_parent);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case INTEGER_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor *editor = new ArithmeticExprEditor(_parent, Variable<int32>::INTEGER_TYPE);
						
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor *editor = new ArithmeticExprEditor(_parent, Variable<float>::FLOAT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_2_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor *editor = new ArithmeticExprEditor(_parent, Variable<Vector2>::VECTOR_2_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor *editor = new ArithmeticExprEditor(_parent, Variable<Vector3>::VECTOR_3_TYPE);
						
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}
		
		case QUATERNION_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor *editor = new ArithmeticExprEditor(_parent, Variable<Quaternion>::QUATERNION_TYPE);
						
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ABSTRACT_VARIABLE_EDITOR:
		{  
			AssetLongEditor* editor = new AssetLongEditor(_parent); 

			editor->SetFileType(AbstractVariable::_GetFileSuffix().c_str());
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case BOOLEAN_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<bool>::BOOLEAN_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		} 

		case INTEGER_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<int32>::INTEGER_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case STRING_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<StringANSI>::STRING_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case FLOAT_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<float>::FLOAT_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_2_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<Vector2>::VECTOR_2_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<Vector3>::VECTOR_3_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case QUATERNION_VARIABLE_EDITOR:
		{
			VariableEditor *editor = new VariableEditor(_parent);
			editor->SetVariableType(Variable<Quaternion>::QUATERNION_TYPE);
			
			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_EDITOR:
		{ 
			Vector3Editor* editor = new Vector3Editor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_RGB_EDITOR:
		{
			Vector3Editor* editor = new Vector3Editor(_parent);
			editor->SetRGBMode();

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_3_HSL_EDITOR:
		{
			Vector3Editor* editor = new Vector3Editor(_parent);
			editor->SetHSLMode();

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case VECTOR_2_EDITOR:
		{
			Vector2Editor* editor = new Vector2Editor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case QUATERNION_EDITOR:
		{
			QuaternionEditor* editor = new QuaternionEditor(_parent);
			
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor; 
		}
	
		case VIRUAL_KEY_EDITOR:
		{
			VirtualKeyEditor *editor = new VirtualKeyEditor(_parent); 
			return editor;
		}

		case ROTATABLE_OBJECT_EDITOR:
		{
			RotatableObjectEditor *editor = new RotatableObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case TRANSLATABLE_OBJECT_EDITOR:
		{
			TranslatableObjectEditor *editor = new TranslatableObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case ACTIVATABLE_OBJECT_EDITOR:
		{
			ActivatableObjectEditor *editor = new ActivatableObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case SCALABLE_OBJECT_EDITOR:
		{
			ScalableObjectEditor *editor = new ScalableObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		} 

		case COLORABLE_OBJECT_EDITOR:
		{
			ColorableObjectEditor *editor = new ColorableObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case LOGIC_OBJECT_EDITOR:
		{
			LogicObjectEditor *editor = new LogicObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case GRAPHICS_OBJECT_EDITOR:
		{  
			GraphicsObjectEditor *editor = new GraphicsObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case LERPING_OBJECT_EDITOR:
		{  
			LerpingObjectEditor *editor = new LerpingObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case PLAYING_OBJECT_EDITOR:
		{  
			PlayingObjectEditor *editor = new PlayingObjectEditor(_parent);

			connect(editor, SIGNAL(FocusOut(QWidget*)), this, SLOT(CloseEditor(QWidget*)));
			editor->setFocusPolicy(Qt::StrongFocus);
			return editor;
		}

		case NO_EDIT:
		{
			return 0;
		}
	}
	return 0;
}


void UniversalDelegate::setEditorData(QWidget* _editor, const QModelIndex& _index)const
{ 
	int currentRole = -1;
	
	if(columnDelegate.contains(_index.column()))
	{
		currentRole = columnDelegate.value(_index.column(), NO_EDIT);
	}
	else
	{
		if(cellDelegate.contains(_index))
		{
			currentRole = cellDelegate.value(_index, NO_EDIT);
		}
	}
	switch(currentRole)
	{ 
		case TEXT_EDITOR:
		{
			QLineEdit* lineEdit = static_cast<QLineEdit*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->setText(value);
			break;
		}

		case LIMIT_EDITOR: 
		{
			LimitEditor *spinBox = static_cast<LimitEditor*>(_editor);
			int value = _index.model()->data(_index, Qt::EditRole).toInt();
			spinBox->SetValue(value); 
			break;
		}

		case LOADING_MODE_EDITOR:
		{
			LoadingModeEditor *spinBox = static_cast<LoadingModeEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			spinBox->SetValue(value); 
			break;
		}

		case ACTION_TYPE_EDITOR:
		{
			ActionTypeEditor* editor = static_cast<ActionTypeEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();

			if(editor->SelectProperty(value))
			{
				editor->setModelColumn(ActionTypeEditor::ACTION_TYPE);

				int index;

				if((index = editor->findData(value, Qt::EditRole)) != -1)
				{
					editor->setCurrentIndex(index);
				}
			}
			break; 
		}

		case EVENT_TYPE_EDITOR:
		{
			EventTypeEditor* editor = static_cast<EventTypeEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();

			if(editor->SelectProperty(value))
			{
				editor->setModelColumn(EventTypeEditor::EVENT_TYPE);

				int index;

				if((index = editor->findData(value, Qt::EditRole)) != -1)
				{
					editor->setCurrentIndex(index);
				}
			}
			break; 
		}

		case ANY_ASSET_EDITOR:
		{
			AssetShortEditor* editor = static_cast<AssetShortEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case NAME_EDITOR:
		{
			NameEditor* editor = static_cast<NameEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;
		}

		case COMBO_BOX_EDITOR:
		{
			QComboBox* comboBox = static_cast<QComboBox*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			comboBox->setCurrentIndex(comboBox->findText(value));
			break; 
		}

		case OBJECT_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case TIMER_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case SCENE_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case PLAYER_CONTROL_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ABSTRACT_LIST_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case INTEGER_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case SET_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case FLOAT_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case BOOLEAN_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case STRING_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_2_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_3_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case QUATERNION_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case TRANSLATABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case SCALABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ROTATABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case COLORABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case MESH_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}
		
		case HITBOX_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case TRIGGER_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ACTION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case EVENT_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case TEXTURED_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ABSTRACT_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ATLAS_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}
		
		case TEXTURE_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case CAMERA_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case FLOAT_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case QUATERNION_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case BOOL_EDITOR:
		{ 
			BoolSpinBox *checkBox = static_cast<BoolSpinBox*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			checkBox->setValue(value == "true" ? 1 : 0);
			break;
		}

		case INT_EDITOR:
		{
			IntSpinBox *intSpinBox = static_cast<IntSpinBox*>(_editor);
			float value = _index.model()->data(_index, Qt::EditRole).toInt();
			intSpinBox->setValue(value);
			break; 
		}

		case INT_EX_EDITOR:
		{
			IntEditorEx *spinBox = static_cast<IntEditorEx*>(_editor);
			int value = _index.model()->data(_index, Qt::EditRole).toInt();
			spinBox->SetValue(value);
			break;
		}

		case FLOAT_EDITOR:
		{
			FloatSpinBox *doubleSpinBox = static_cast<FloatSpinBox*>(_editor);
			double value = _index.model()->data(_index, Qt::EditRole).toDouble();
			doubleSpinBox->setValue(value);
			break; 
		}

		case FLOAT_EX_EDITOR: 
		{
			FloatEditorEx *spinBox = static_cast<FloatEditorEx*>(_editor);
			float value = _index.model()->data(_index, Qt::EditRole).toFloat();
			spinBox->SetValue(value);
			break;
		}

		case FLOAT_0_1_EDITOR: 
		{
			FloatEditorEx *spinBox = static_cast<FloatEditorEx*>(_editor);
			float value = _index.model()->data(_index, Qt::EditRole).toFloat();
			spinBox->SetValue(value);
			break;
		}

		case FLOAT_0_360_EDITOR: 
		{
			FloatEditorEx *spinBox = static_cast<FloatEditorEx*>(_editor);
			float value = _index.model()->data(_index, Qt::EditRole).toFloat();
			spinBox->SetValue(value);
			break;
		}

		case ENUMERATION_EXPR_EDITOR:
		{
			EnumerationExprEditor* lineEdit = static_cast<EnumerationExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}
		
		case BOOLEAN_EXPR_EDITOR:
		{
			BooleanExprEditor* lineEdit = static_cast<BooleanExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case EQUALITY_EXPR_EDITOR:
		{
			EqualityExprEditor* lineEdit = static_cast<EqualityExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case STRING_EXPR_EDITOR:
		{
			StringExprEditor* lineEdit = static_cast<StringExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case INTEGER_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case FLOAT_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case VECTOR_2_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case VECTOR_3_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case QUATERNION_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			lineEdit->SetText(value);
			break;
		}

		case ABSTRACT_VARIABLE_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case BOOLEAN_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case INTEGER_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case STRING_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case FLOAT_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_2_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_3_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case QUATERNION_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case VECTOR_3_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
						
			SimpleList<StringANSI> list = SplitString(value.toAscii().data(), ";");

			if(list.GetSize() == 3)
			{
				float x = QString(list[0]->key.c_str()).toFloat();
				float y = QString(list[1]->key.c_str()).toFloat();
				float z = QString(list[2]->key.c_str()).toFloat();
				editor->SetX(x);
				editor->SetY(y);
				editor->SetZ(z);
			}
			break;
		}

		case VECTOR_3_RGB_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
						
			SimpleList<StringANSI> list = SplitString(value.toAscii().data(), ";");

			if(list.GetSize() == 3)
			{
				float x = QString(list[0]->key.c_str()).toFloat();
				float y = QString(list[1]->key.c_str()).toFloat();
				float z = QString(list[2]->key.c_str()).toFloat();
				editor->SetX(x);
				editor->SetY(y);
				editor->SetZ(z);
			}
			break;
		}

		case VECTOR_3_HSL_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
						
			SimpleList<StringANSI> list = SplitString(value.toAscii().data(), ";");

			if(list.GetSize() == 3)
			{
				float x = QString(list[0]->key.c_str()).toFloat();
				float y = QString(list[1]->key.c_str()).toFloat();
				float z = QString(list[2]->key.c_str()).toFloat();
				editor->SetX(x);
				editor->SetY(y);
				editor->SetZ(z);
			}
			break;
		}

		case VECTOR_2_EDITOR:
		{
			Vector2Editor* editor = static_cast<Vector2Editor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
						
			SimpleList<StringANSI> list = SplitString(value.toAscii().data(), ";");

			if(list.GetSize() == 2)
			{
				float x = QString(list[0]->key.c_str()).toFloat();
				float y = QString(list[1]->key.c_str()).toFloat();
				editor->SetX(x);
				editor->SetY(y);
			}
			break;
		}

		case QUATERNION_EDITOR:
		{
			QuaternionEditor* editor = static_cast<QuaternionEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			
			SimpleList<StringANSI> list = SplitString(value.toAscii().data(), ";");

			if(list.GetSize() == 4)
			{
				float x = QString(list[0]->key.c_str()).toFloat();
				float y = QString(list[1]->key.c_str()).toFloat();
				float z = QString(list[2]->key.c_str()).toFloat();
				float w = QString(list[3]->key.c_str()).toFloat();
				editor->SetX(x);
				editor->SetY(y);
				editor->SetZ(z);
				editor->SetW(w);  
			}
			break;
		}

		case VIRUAL_KEY_EDITOR:
		{
			VirtualKeyEditor *editor = static_cast<VirtualKeyEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->setText(value);
			break;
		}

		case ROTATABLE_OBJECT_EDITOR:
		{
			RotatableObjectEditor* editor = static_cast<RotatableObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case TRANSLATABLE_OBJECT_EDITOR:
		{
			TranslatableObjectEditor* editor = static_cast<TranslatableObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case ACTIVATABLE_OBJECT_EDITOR:
		{
			ActivatableObjectEditor* editor = static_cast<ActivatableObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case COLORABLE_OBJECT_EDITOR:
		{
			ColorableObjectEditor* editor = static_cast<ColorableObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case SCALABLE_OBJECT_EDITOR:
		{ 
			ScalableObjectEditor* editor = static_cast<ScalableObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}


		case LOGIC_OBJECT_EDITOR:
		{
			LogicObjectEditor* editor = static_cast<LogicObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case GRAPHICS_OBJECT_EDITOR:
		{
			GraphicsObjectEditor* editor = static_cast<GraphicsObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}	

		case LERPING_OBJECT_EDITOR:
		{
			LerpingObjectEditor* editor = static_cast<LerpingObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case PLAYING_OBJECT_EDITOR:
		{
			PlayingObjectEditor* editor = static_cast<PlayingObjectEditor*>(_editor);
			QString value = _index.model()->data(_index, Qt::EditRole).toString();
			editor->SetFileName(value);
			break;  
		}

		case NO_EDIT:
		{ 
			break;
		} 
	}
}


void UniversalDelegate::setModelData(QWidget* _editor, QAbstractItemModel* _model, const QModelIndex& _index)const
{
	int currentRole = -1;
	
	if(columnDelegate.contains(_index.column()))
	{
		currentRole = columnDelegate.value(_index.column(), NO_EDIT);
	}
	else
	{
		if(cellDelegate.contains(_index))
		{
			currentRole = cellDelegate.value(_index, NO_EDIT);
		}
	}
	switch(currentRole)
	{
		case TEXT_EDITOR:
		{
			QLineEdit* lineEdit = static_cast<QLineEdit*>(_editor);
			_model->setData(_index, lineEdit->text());
			break;
		}

		case LIMIT_EDITOR: 
		{
			LimitEditor* spinBox = static_cast<LimitEditor*>(_editor);
			_model->setData(_index, spinBox->GetValueString()); 
			break;
		}

		case LOADING_MODE_EDITOR:
		{
			LoadingModeEditor* spinBox = static_cast<LoadingModeEditor*>(_editor);
			_model->setData(_index, spinBox->GetValueString()); 
			break;
		}

		case ACTION_TYPE_EDITOR:
		{
			ActionTypeEditor* editor = static_cast<ActionTypeEditor*>(_editor);
			
			if(editor->GetSelectedActionType() != -1)
			{
				QString type = editor->itemText(editor->currentIndex());

				if(!type.isEmpty())
				{
					if(editor->IsAction(type))
					{
						_model->setData(_index, type, Qt::EditRole);
					}
				}
			}
			break;
		}

		case EVENT_TYPE_EDITOR:
		{
			EventTypeEditor* editor = static_cast<EventTypeEditor*>(_editor);
			
			if(editor->GetSelectedEventType() != -1)
			{
				QString type = editor->itemText(editor->currentIndex());

				if(!type.isEmpty())
				{
					if(editor->IsEvent(type))
					{
						_model->setData(_index, type, Qt::EditRole);
					}
				}
			}
			break;
		}

		case ANY_ASSET_EDITOR:
		{
			AssetShortEditor* editor = static_cast<AssetShortEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case NAME_EDITOR:
		{
			NameEditor* editor = static_cast<NameEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case COMBO_BOX_EDITOR:
		{
			QComboBox* comboBox = static_cast<QComboBox*>(_editor);
			_model->setData(_index, comboBox->itemText(comboBox->currentIndex()), Qt::EditRole);
			break;
		}

		case OBJECT_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TIMER_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case SCENE_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case PLAYER_CONTROL_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ABSTRACT_LIST_EDITOR:
		{  
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case INTEGER_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case SET_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case FLOAT_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case BOOLEAN_LIST_EDITOR:
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case STRING_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_2_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_3_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case QUATERNION_LIST_EDITOR: 
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TRANSLATABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}
	
		case SCALABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ROTATABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case COLORABLE_LIST_EDITOR:  
		{
			ListEditor* editor = static_cast<ListEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case MESH_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}
			
		case HITBOX_EDITOR:
		{ 
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TRIGGER_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ACTION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case EVENT_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TEXTURED_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ABSTRACT_ANIMATION_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ATLAS_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TEXTURE_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case CAMERA_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case FLOAT_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case QUATERNION_LERP_EDITOR:
		{
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case BOOL_EDITOR:
		{
			BoolSpinBox* checkBox = static_cast<BoolSpinBox*>(_editor);
			_model->setData(_index, checkBox->value() ? "true" : "false"); 
			break;
		}
				
		case INT_EDITOR:
		{
			IntSpinBox* intSpinBox = static_cast<IntSpinBox*>(_editor);
			intSpinBox->interpretText();
			_model->setData(_index, intSpinBox->text());
			break; 
		}

		case INT_EX_EDITOR:
		{
			IntEditorEx* spinBox = static_cast<IntEditorEx*>(_editor);
			_model->setData(_index, spinBox->GetValueString());
			break;
		}

		case FLOAT_EDITOR:
		{
			FloatSpinBox* doubleSpinBox = static_cast<FloatSpinBox*>(_editor);
			doubleSpinBox->interpretText();
			_model->setData(_index, doubleSpinBox->text());
			break; 
		}

		case FLOAT_EX_EDITOR: 
		{
			FloatEditorEx* spinBox = static_cast<FloatEditorEx*>(_editor);
			_model->setData(_index, spinBox->GetValueString());
			break;
		}

		case FLOAT_0_1_EDITOR: 
		{
			FloatEditorEx* spinBox = static_cast<FloatEditorEx*>(_editor);
			_model->setData(_index, spinBox->GetValueString());
			break;
		}

		case FLOAT_0_360_EDITOR: 
		{
			FloatEditorEx* spinBox = static_cast<FloatEditorEx*>(_editor);
			_model->setData(_index, spinBox->GetValueString());
			break;
		}

		case ENUMERATION_EXPR_EDITOR:
		{
			EnumerationExprEditor* lineEdit = static_cast<EnumerationExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case BOOLEAN_EXPR_EDITOR:
		{
			BooleanExprEditor* lineEdit = static_cast<BooleanExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case EQUALITY_EXPR_EDITOR:
		{
			EqualityExprEditor* lineEdit = static_cast<EqualityExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case STRING_EXPR_EDITOR:
		{
			StringExprEditor* lineEdit = static_cast<StringExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case INTEGER_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case FLOAT_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case VECTOR_2_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case VECTOR_3_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case QUATERNION_ARITHMETIC_EXPR_EDITOR:
		{
			ArithmeticExprEditor* lineEdit = static_cast<ArithmeticExprEditor*>(_editor);
			_model->setData(_index, lineEdit->GetText());
			break;
		}

		case ABSTRACT_VARIABLE_EDITOR:
		{  
			AssetLongEditor* editor = static_cast<AssetLongEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case BOOLEAN_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case INTEGER_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case STRING_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case FLOAT_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_2_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_3_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case QUATERNION_VARIABLE_EDITOR:
		{
			VariableEditor* editor = static_cast<VariableEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case VECTOR_3_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			_model->setData(_index, editor->GetXString() + ";" + editor->GetYString() + ";" + editor->GetZString());
			break;  
		}

		case VECTOR_3_RGB_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			_model->setData(_index, editor->GetXString() + ";" + editor->GetYString() + ";" + editor->GetZString());
			break;  
		}

		case VECTOR_3_HSL_EDITOR:
		{
			Vector3Editor* editor = static_cast<Vector3Editor*>(_editor);
			_model->setData(_index, editor->GetXString() + ";" + editor->GetYString() + ";" + editor->GetZString());
			break;  
		}

		case VECTOR_2_EDITOR:
		{
			Vector2Editor* editor = static_cast<Vector2Editor*>(_editor);
			_model->setData(_index, editor->GetXString() + ";" + editor->GetYString());
			break;  
		}

		case QUATERNION_EDITOR:
		{
			QuaternionEditor* editor = static_cast<QuaternionEditor*>(_editor);
			_model->setData(_index, editor->GetXString() + ";" + editor->GetYString() + ";" + editor->GetZString() + ";" + editor->GetWString());
			break;  
		}

		case VIRUAL_KEY_EDITOR:
		{
			VirtualKeyEditor* editor = static_cast<VirtualKeyEditor*>(_editor);
			_model->setData(_index, editor->text(), Qt::EditRole);
			break;
		}
 
		case ROTATABLE_OBJECT_EDITOR:
		{
			RotatableObjectEditor* editor = static_cast<RotatableObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case TRANSLATABLE_OBJECT_EDITOR:
		{
			TranslatableObjectEditor* editor = static_cast<TranslatableObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case ACTIVATABLE_OBJECT_EDITOR:
		{
			ActivatableObjectEditor* editor = static_cast<ActivatableObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case COLORABLE_OBJECT_EDITOR:
		{
			ColorableObjectEditor* editor = static_cast<ColorableObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case SCALABLE_OBJECT_EDITOR:
		{
			ScalableObjectEditor* editor = static_cast<ScalableObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case LOGIC_OBJECT_EDITOR:
		{
			LogicObjectEditor* editor = static_cast<LogicObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case GRAPHICS_OBJECT_EDITOR:
		{
			GraphicsObjectEditor* editor = static_cast<GraphicsObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case LERPING_OBJECT_EDITOR:
		{
			LerpingObjectEditor* editor = static_cast<LerpingObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case PLAYING_OBJECT_EDITOR:
		{
			PlayingObjectEditor* editor = static_cast<PlayingObjectEditor*>(_editor);
			_model->setData(_index, editor->GetFileName());
			break;  
		}

		case NO_EDIT:
		{ 
			break;
		}
	}
}

	

void UniversalDelegate::updateEditorGeometry(QWidget* _editor, const QStyleOptionViewItem& _option, const QModelIndex& _index)const
{
	Q_UNUSED(_index);
	_editor->setGeometry(_option.rect);
}


void UniversalDelegate::SetComboValuesForCell(QModelIndex _index, QList<QString> _values)
{
	valuesOfComboForCell[_index] = _values;
}


void UniversalDelegate::SetComboValuesForColumn(int _column, QList<QString> _values)
{
	valuesOfComboForColumn[_column] = _values;
}

	
QList<QString> UniversalDelegate::GetComboValuesForCell(QModelIndex _index)const
{
	QList<QString> values;
	return valuesOfComboForCell.value(_index, values);
}


int UniversalDelegate::GetCurrentDelegate(QModelIndex _index)const
{
	return cellDelegate.value(_index, 0);
}


void UniversalDelegate::Clear(void)
{
	valuesOfComboForCell.clear();
	valuesOfComboForColumn.clear();
	cellDelegate.clear();
	columnDelegate.clear();
}