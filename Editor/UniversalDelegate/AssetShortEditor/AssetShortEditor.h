#ifndef ASSETSHORTEDITOR_H
#define ASSETSHORTEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/StringLineEdit/StringLineEdit.h>
class AssetShortEditor: public QWidget
{
	Q_OBJECT
	private: QTextCharFormat unknownArgFormat, standardFormat;
	private: StringLineEdit *fileNameEdit;
    private: QToolButton *openFileButton;
	private: QToolButton *editAssetButton;
    public: AssetShortEditor(QWidget* _parent = 0);
	public: void SetFileName(QString _fileName);
	public: QString GetFileName(void)const;
	private slots: void NameIsChanged(QString _name);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public slots: void CreateFinder(void);
	public slots: void EditTexturedAnimation(void);
	public slots: void EditAtlas(void);
	public slots: void EditTexture(void);
	public slots: void EditTrigger(void);
	public slots: void EditAction(void);
	public slots: void EditEvent(void);
	public slots: void EditVariable(void);
	public slots: void EditList(void);
	public slots: void CreateObject(void);
	public slots: void EditObject(void);
	public slots: void EditPlayerControl(void);
	public slots: void EditMesh(void);
	public slots: void EditHitbox(void);
	public slots: void EditScene(void);
	public slots: void EditFloatLerp(void);
	public slots: void EditVectorLerp(void);
	public slots: void EditQuaternionLerp(void);
	public slots: void EditCamera(void);
	public slots: void EditTimer(void);
	private: void CheckArg(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
};



#endif 

