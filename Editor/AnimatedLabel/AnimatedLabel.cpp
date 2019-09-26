#include "AnimatedLabel.h"



AnimatedLabel::AnimatedLabel(QWidget* _parent): QLabel(_parent), fps(0.0f), currentFrame(0), frameWidth(64), frameHeight(64), pause(true)
{
		
	UpdateAnim();

	show();
}


void AnimatedLabel::Pause(void)
{
	disconnect(&timer, SIGNAL(timeout()), this, SLOT(UpdateAnim()));
}



void AnimatedLabel::Play(void)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(UpdateAnim()));
}



void AnimatedLabel::SetFrames(QList<QPixmap> _frames)
{
	frames = _frames;

	if(!frames.isEmpty())
	{
		SetCurrentFrame(0);
	}
}



void AnimatedLabel::SetFPS(float _fps)
{
	fps = _fps;

	if(fps > 0.0f)
	{
		timer.start(1000.0f / fps);
	}
	else
	{
		timer.stop();
	}
}


float AnimatedLabel::GetFPS(void)const
{
	return fps;
}



void AnimatedLabel::SetCurrentFrame(int _index)
{
	if(_index >= 0 && _index < GetAmountOfFrames())
	{
		currentFrame = _index;

		setPixmap(frames.at(currentFrame));
	}
}


int AnimatedLabel::GetCurrentFrame(void)const
{
	return currentFrame;
}



int AnimatedLabel::GetAmountOfFrames(void)
{
	return frames.size();
}


void AnimatedLabel::UpdateAnim(void)
{ 
	if(!frames.empty())
	{
		if(currentFrame > (frames.size() - 1))
		{
			currentFrame = 0;
		}
		setPixmap(frames.at(currentFrame).scaled(frameWidth, frameHeight, Qt::KeepAspectRatio));
	
		currentFrame++;
	}
}





void AnimatedLabel::SetFrameWidth(float _width)
{
	frameWidth = _width;
}
	


void AnimatedLabel::SetFrameHeight(float _height)
{
	frameHeight = _height;
}



int AnimatedLabel::GetFrameWidth(void)
{
	return frameWidth;
}



int AnimatedLabel::GetFrameHeight(void)
{
	return frameHeight;
}


