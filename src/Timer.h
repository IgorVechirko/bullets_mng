#ifndef Timer_H
#define Timer_H

#include "Common.h"

namespace BulletsMng
{

	class Timer
	{
		bool _showInOutput;

		bool _stoped;

		std::string _name;

		std::chrono::steady_clock::time_point _startTime;
		std::chrono::steady_clock::duration _duration;

	public:

		Timer( const std::string& name = "timer", bool show = true );

		void setIsShow( bool isShow );
		void stop();
		std::chrono::steady_clock::duration& getDuration();

		virtual ~Timer();


	};

}


#endif