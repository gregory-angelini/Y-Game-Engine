#include "VariableTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>



VariableTableWidget::VariableTableWidget(QWidget* _parent): QTableWidget(MAX_ROWS, MAX_COLUMNS, _parent)
{

	blockSignals(true);
	createVariableComboList.push_back("INTEGER");
	createVariableComboList.push_back("FLOAT");
	createVariableComboList.push_back("BOOLEAN");
	createVariableComboList.push_back("STRING");
	createVariableComboList.push_back("VECTOR_2");
	createVariableComboList.push_back("VECTOR_3");
	createVariableComboList.push_back("QUATERNION");
	QStringList columnLabels;
	columnLabels.push_back("NAME");
	columnLabels.push_back("TYPE");
	columnLabels.push_back("VALUE");
	columnLabels.push_back("LOW BOUND");
	columnLabels.push_back("UPPER BOUND");

	setVerticalHeaderLabels(columnLabels);
	QStringList rowLabels;
	rowLabels.push_back("VALUE");

	setHorizontalHeaderLabels(rowLabels);
	for(int i = 0; i < MAX_ROWS; ++i)
	{
		for(int j = 0; j < MAX_COLUMNS; ++j)
		{
			QTableWidgetItem* it = new QTableWidgetItem;
			it->setTextAlignment(Qt::AlignHCenter);
			QFont font("Verdana", 10, QFont::Normal, false);
			it->setFont(font);

			it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

			setItem(i, j, it);
		}
	}
	for(int i = 0; i < MAX_ROWS; i++)
	{
		setRowHeight(i, 25);
	}

	setSelectionMode(QAbstractItemView::NoSelection);

	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	verticalHeader()->setStyleSheet("QHeaderView::section { background-color: gray; color: white; }");
	horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: gray; color: white; }");
	setColumnWidth(VALUE, 290); 

	 
	SetDefaultState();
	delegate = new UniversalDelegate(this);
	setItemDelegate(delegate);
	

	delegate->SetCellDelegate(indexFromItem(item(NAME_PROPERTY, VALUE)), UniversalDelegate::TEXT_EDITOR);
	
	delegate->SetCellDelegate(indexFromItem(item(TYPE_PROPERTY, VALUE)), UniversalDelegate::COMBO_BOX_EDITOR);
	delegate->SetComboValuesForCell(indexFromItem(item(TYPE_PROPERTY, VALUE)), createVariableComboList);
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(UpdateRow(QTableWidgetItem*)));

	setFixedSize(MAX_COLUMNS * 210 + 100-20+60+40, MAX_ROWS * 30 - 2);

	blockSignals(false);
}



VariableTableWidget::~VariableTableWidget(void)
{
	delete delegate;
}


void VariableTableWidget::SetCellDelegate(int _row, int _editor)
{				
	QTableWidgetItem* it = item(_row, VALUE);
	QModelIndex index = indexFromItem(it);

	delegate->SetCellDelegate(index, _editor);
}
	


QString VariableTableWidget::GetCellText(int _row, int _column)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		if(_column >= 0 && _column < MAX_COLUMNS)
		{
			return item(_row, _column)->text();
		}
	}
	return "";
}

 
void VariableTableWidget::SetCellText(int _row, int _column, QString _value)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		if(_column >= 0 && _column < MAX_COLUMNS)
		{
			item(_row, _column)->setText(_value);
		}
	}
}


 
QString VariableTableWidget::IsOk(void)
{
	if(!IsValid())
	{
		return item(NAME_PROPERTY, VALUE)->text();
	}
	return QString();
}



bool VariableTableWidget::IsValid(void)const
{
	for(int i = 0; i < MAX_ROWS; i++)
	{
		if(propertyEnabled[i] && !propertyValid[i]) { return false; }
	}
	return true;
}


void VariableTableWidget::SetDefaultState(void)
{
	propertyValid[NAME_PROPERTY] = true;
	propertyValid[TYPE_PROPERTY] = false;
	propertyValid[VALUE_PROPERTY] = false;
	propertyValid[LOWER_BOUND_PROPERTY] = true;
	propertyValid[UPPER_BOUND_PROPERTY] = true;

	propertyEnabled[NAME_PROPERTY] = true;
	propertyEnabled[TYPE_PROPERTY] = true;
	propertyEnabled[VALUE_PROPERTY] = true;
	propertyEnabled[LOWER_BOUND_PROPERTY] = true;
	propertyEnabled[UPPER_BOUND_PROPERTY] = true;

	item(NAME_PROPERTY, VALUE)->setText("");
	item(TYPE_PROPERTY, VALUE)->setText("");
	item(VALUE_PROPERTY, VALUE)->setText("");
	item(LOWER_BOUND_PROPERTY, VALUE)->setText("");
	item(UPPER_BOUND_PROPERTY, VALUE)->setText("");
} 


 
void VariableTableWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		QTableWidgetItem* it = currentItem();

		if(it)
		{
			QModelIndex index = indexFromItem(it);
			
			int row = index.row();

			UpdateItem(it, row);
			
			editItem(it);
		}
	}
}


void VariableTableWidget::UpdateItem(QTableWidgetItem* _it, int _row)
{ 
	QModelIndex index = indexFromItem(_it);

	if(_row == VALUE_PROPERTY)
	{
		delegate->SetCellDelegate(index, GetEditorTypeForCell(_it));
	}
	else if(_row == LOWER_BOUND_PROPERTY)
	{
		delegate->SetCellDelegate(index, GetEditorTypeForCell(_it));
	}
	else if(_row == UPPER_BOUND_PROPERTY)
	{
		delegate->SetCellDelegate(index, GetEditorTypeForCell(_it));
	} 
} 



int VariableTableWidget::GetEditorTypeForCell(QTableWidgetItem* _it)
{
	QModelIndex index = indexFromItem(_it);

	if(index.row() == VALUE_PROPERTY)
	{
		if(item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
		{ 
			return UniversalDelegate::INT_EX_EDITOR;
		}
	    else if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT")
		{
			return UniversalDelegate::FLOAT_EX_EDITOR;
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			return UniversalDelegate::BOOL_EDITOR;
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "STRING")
		{
			return UniversalDelegate::TEXT_EDITOR;
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_2")
		{
			return UniversalDelegate::VECTOR_2_EDITOR;
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "VECTOR_3")
		{
			return UniversalDelegate::VECTOR_3_EDITOR;
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "QUATERNION")
		{
			return UniversalDelegate::QUATERNION_EDITOR;
		}
	}
	else if(index.row() == LOWER_BOUND_PROPERTY)
	{
		if(item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
		{ 
			return UniversalDelegate::INT_EX_EDITOR;
		}
	    else if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT")
		{
			return UniversalDelegate::FLOAT_EX_EDITOR;
		}
	}
	else if(index.row() == UPPER_BOUND_PROPERTY)
	{
		if(item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
		{ 
			return UniversalDelegate::INT_EX_EDITOR;
		}
	    else if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT")
		{
			return UniversalDelegate::FLOAT_EX_EDITOR;
		}
	}

	return UniversalDelegate::NO_EDIT;
}


void VariableTableWidget::SetPropertyValid(int _row, bool _valid)
{
	if(_row >= 0 && _row < MAX_ROWS)
	{
		propertyValid[_row] = _valid;
		
		if(_valid)
		{
			item(_row, VALUE)->setToolTip("");
			item(_row, VALUE)->setBackground(QBrush(QColor(255,255,255)));
		}
		else 
		{
			item(_row, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
	}
}


void VariableTableWidget::SetPropertyEnable(int _row, bool _enable)
{ 
	if(_row >= 0 && _row < MAX_ROWS)
	{
		propertyEnabled[_row] = _enable;
		
		if(!_enable)
		{ 
			item(_row, VALUE)->setToolTip("");
			SetCellText(_row, VALUE, "---"); 
		}
	}
}


void VariableTableWidget::UpdatePropertyStatus(void)
{
	if(item(NAME_PROPERTY, VALUE)->text().length() > 0)
	{
		int32 errorCode;
		if((errorCode = BooleanExprParser::_IsNameValid(item(NAME_PROPERTY, VALUE)->text().toStdString())) >= 0)
		{
			QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(item(NAME_PROPERTY, VALUE)->text().at(errorCode)) + "'";
			item(NAME_PROPERTY, VALUE)->setToolTip(text); 
			
			SetPropertyValid(NAME_PROPERTY, false); 
			item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
		}
		else
		{
			item(NAME_PROPERTY, VALUE)->setToolTip("");

			SetPropertyValid(NAME_PROPERTY, true);
			item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(255,255,255)));
		}
	} 
	else
	{ 
		SetPropertyValid(NAME_PROPERTY, false);
		item(NAME_PROPERTY, VALUE)->setBackground(QBrush(QColor(231,186,188)));
	}
	if(item(TYPE_PROPERTY, VALUE)->text().length() > 0)
	{
		SetPropertyValid(TYPE_PROPERTY, true);
	}
	else 
	{ 
		SetPropertyValid(TYPE_PROPERTY, false);
	}
	if(item(TYPE_PROPERTY, VALUE)->text() == "STRING" ||
	   item(VALUE_PROPERTY, VALUE)->text().length() > 0)
	{
		SetPropertyValid(VALUE_PROPERTY, true);
	}
	else 
	{ 
		SetPropertyValid(VALUE_PROPERTY, false);
	}

	
	if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT" ||
	   item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
	{ 
		SetPropertyEnable(LOWER_BOUND_PROPERTY, true);
		SetPropertyEnable(UPPER_BOUND_PROPERTY, true);
	}
	if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT" ||
	   item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
	{ 
		if(item(LOWER_BOUND_PROPERTY, VALUE)->text().length() > 0)
		{
			SetPropertyValid(LOWER_BOUND_PROPERTY, true);
		}
		else
		{
			SetPropertyValid(LOWER_BOUND_PROPERTY, false);
		}
	}
	else
	{ 
		SetPropertyEnable(LOWER_BOUND_PROPERTY, false);
		SetPropertyValid(LOWER_BOUND_PROPERTY, true);
	}
	if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT" ||
	   item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
	{ 
		if(item(UPPER_BOUND_PROPERTY, VALUE)->text().length() > 0)
		{
			SetPropertyValid(UPPER_BOUND_PROPERTY, true);
		}
		else
		{
			SetPropertyValid(UPPER_BOUND_PROPERTY, false);
		}
	}
	else
	{
		SetPropertyEnable(UPPER_BOUND_PROPERTY, false);
		SetPropertyValid(UPPER_BOUND_PROPERTY, true);
	}
}


void VariableTableWidget::UpdateRow(QTableWidgetItem* _it)
{ 
	QModelIndex index = indexFromItem(_it);

	if(index.row() == TYPE_PROPERTY)
	{ 
		SetPropertyValid(VALUE_PROPERTY, false);
		item(VALUE_PROPERTY, VALUE)->setText("");

		SetPropertyValid(LOWER_BOUND_PROPERTY, false);
		item(LOWER_BOUND_PROPERTY, VALUE)->setText("");

		SetPropertyValid(UPPER_BOUND_PROPERTY, false);
		item(UPPER_BOUND_PROPERTY, VALUE)->setText("");

		if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT")
		{
			item(LOWER_BOUND_PROPERTY, VALUE)->setText(QString::number(Numerical<float>::_LowerValue()));
			
			item(UPPER_BOUND_PROPERTY, VALUE)->setText(QString::number(Numerical<float>::_UpperValue()));
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
		{
			item(LOWER_BOUND_PROPERTY, VALUE)->setText(QString::number(Numerical<int32>::_LowerValue()));
			
			item(UPPER_BOUND_PROPERTY, VALUE)->setText(QString::number(Numerical<int32>::_UpperValue()));
		}
	}
	else if(index.row() == LOWER_BOUND_PROPERTY ||
		    index.row() == UPPER_BOUND_PROPERTY ||
			index.row() == VALUE_PROPERTY)
	{
		if(item(TYPE_PROPERTY, VALUE)->text() == "INTEGER")
		{
			int32 value = Numerical<int32>::_GetMin(Numerical<int32>::_GetMax(item(VALUE_PROPERTY, VALUE)->text().toInt(), item(LOWER_BOUND_PROPERTY, VALUE)->text().toInt()), item(UPPER_BOUND_PROPERTY, VALUE)->text().toInt());
			item(VALUE_PROPERTY, VALUE)->setText(QString::number(value));
		}
		else if(item(TYPE_PROPERTY, VALUE)->text() == "FLOAT")
		{
			float value = Numerical<float>::_GetMin(Numerical<float>::_GetMax(item(VALUE_PROPERTY, VALUE)->text().toFloat(), item(LOWER_BOUND_PROPERTY, VALUE)->text().toFloat()), item(UPPER_BOUND_PROPERTY, VALUE)->text().toFloat());
			item(VALUE_PROPERTY, VALUE)->setText(QString::number(value));
		}
	}

	UpdatePropertyStatus();
}


void VariableTableWidget::keyPressEvent(QKeyEvent* _event)
{
	if(_event->key() == Qt::Key_Return)
	{
		QTableWidgetItem* it = currentItem();
		
		if(it)
		{
			QModelIndex index = indexFromItem(it);

			int row = index.row();

			UpdateItem(it, row);
			
			editItem(it);
		}
	}
}


