#ifndef PLAYINGOBJECTEDITOR_H
#define PLAYINGOBJECTEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/StringExprLineEdit/StringExprLineEdit.h>
#include <Editor/StringLineEdit/StringLineEdit.h>
class PlayingObjectEditor: public QWidget
{
	Q_OBJECT
	private: QTextCharFormat unknownArgFormat, standardFormat;
	private: StringLineEdit *fileNameEdit;
    private: QToolButton *openFileButton;
    private: QToolButton *editAssetButton;
    public: PlayingObjectEditor(QWidget* _parent = 0);
	private slots: void NameIsChanged(QString _name);
	public: void SetFileName(QString _fileName);
	public: QString GetFileName(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public slots: void OpenFinder(void);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public slots: void CreateObject(void);
	public slots: void EditTexturedAnimation(void);
	public slots: void EditFloatLerp(void);
	private: void CheckArg(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
};





#endif 

