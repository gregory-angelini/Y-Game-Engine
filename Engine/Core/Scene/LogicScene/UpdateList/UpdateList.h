#ifndef UPDATELIST_H
#define UPDATELIST_H
#include <Engine/Core/Object/LogicObject/LogicObject.h>
class UpdateList: public MagicList<LogicObject>,
				  public MessageHandler<int32>::Receiver
{
	friend class LogicScene;
	private: LogicScene* scene;
	public: UpdateList(void);
	public: virtual ~UpdateList(void);
	public: AddressDerived* AddObject(LogicObject* _object);
	private: bool IsParentScene(LogicScene* _scene);
	public: virtual void RemoveNodeEvent(MagicNode* _node);
	public: virtual void AddNodeEvent(MagicNode* _node);
	public: AbstractObject* FindObject(StringANSI _name);

};


#endif 