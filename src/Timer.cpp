#include "Timer.h"

namespace BulletsMng
{

	Timer::Timer( const std::string& name, bool show )
		: _name( name )
		, _showInOutput( show )
		, _startTime( std::chrono::steady_clock::now() )
		, _stoped( false )
	{
	}
	void Timer::setIsShow( bool isShow )
	{
		_showInOutput = isShow;
	}
	void Timer::stop()
	{
		_duration = std::chrono::steady_clock::now() - _startTime;
	}
	std::chrono::steady_clock::duration& Timer::getDuration()
	{
		if( !_stoped )
				stop();

		return _duration;
	}
	Timer::~Timer()
	{
		if ( _showInOutput )
		{
			if( !_stoped )
				stop();

			Console::log( _name, " = ", _duration.count() );

		}
	}

}