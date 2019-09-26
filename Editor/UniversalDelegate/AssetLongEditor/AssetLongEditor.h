#ifndef ASSETLONGEDITOR_H
#define ASSETLONGEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/StringLineEdit/StringLineEdit.h>
class AssetLongEditor: public QWidget
{
	Q_OBJECT
	private: QTextCharFormat unknownArgFormat, standardFormat;
	private: StringLineEdit *fileNameEdit;
    private: QToolButton *openFileButton;
	private: QToolButton *editAssetButton;
	private: QString fileType;
    public: AssetLongEditor(QWidget* _parent = 0);
	public: void SetFileName(QString _fileName);
	public: QString GetFileName(void)const;
	private slots: void NameIsChanged(QString _name);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public slots: void OpenFinder(void);
	public: void SetFileType(QString _fileType);
	public slots: void CreateTexturedAnimation(void);
	public slots: void EditTexturedAnimation(void);
	public slots: void CreateAtlas(void);
	public slots: void EditAtlas(void);
	public slots: void CreateTexture(void);
	public slots: void EditTexture(void);
	public slots: void CreateTrigger(void);
	public slots: void EditTrigger(void);
	public slots: void CreateAction(void);
	public slots: void EditAction(void);
	public slots: void CreateNewEvent(void);
	public slots: void EditEvent(void);
	public slots: void CreateVariable(void);
	public slots: void EditVariable(void);
	public slots: void CreateList(void);
	public slots: void EditList(void);
	public slots: void CreateObject(void);
	public slots: void EditObject(void);
	public slots: void CreatePlayerControl(void);
	public slots: void EditPlayerControl(void);
	public slots: void CreateMesh(void);
	public slots: void EditMesh(void);
	public slots: void CreateHitbox(void);
	public slots: void EditHitbox(void);
	public slots: void CreateScene(void);
	public slots: void EditScene(void);
	public slots: void CreateFloatLerp(void);
	public slots: void EditFloatLerp(void);
	public slots: void CreateVectorLerp(void);
	public slots: void EditVectorLerp(void);
	public slots: void CreateQuaternionLerp(void);
	public slots: void EditQuaternionLerp(void);
	public slots: void CreateCamera(void);
	public slots: void EditCamera(void);
	public slots: void CreateTimer(void);
	public slots: void EditTimer(void);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private: void CheckArg(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
};



#endif 

