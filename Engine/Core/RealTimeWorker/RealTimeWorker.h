#ifndef REALTIMEWORKER_H
#define REALTIMEWORKER_H
#include <Engine/Core/Window/Window.h>
#include <Engine/Core/Call/CallController/CallController.h>
class RealTimeWorker: public Window
{
	public: class Define
	{
		public: Window::Define windowDef;
		public: bool sleep;
		public: bool unsuspended;
	    public: CallController updateCallController;
		public: Define(void);
	};
	friend class Renderer;
	private: CallController updateCallController;

	private: bool sleep;
	private: bool unsuspended;
	public: RealTimeWorker(void);
	public: virtual ~RealTimeWorker(void);
	public: bool Create(RealTimeWorker::Define _define);
	private: void SetState(RealTimeWorker::Define _define);
    public: void SetSleep(bool _sleep);
	public: bool GetSleep(void)const;
	public: void SetUnsuspended(bool _unsuspended);
	public: bool IsUnsuspended(void)const;
	public: RealTimeWorker::Define GetState(void)const;
	public: void Loop(void);
	public: virtual int32 Frame(void);
	public: virtual void Update(float _delta);
	public: CallController& GetUpdateCallController(void);
};




#endif 
