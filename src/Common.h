#ifndef Common_H
#define Common_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <stack>
#include <deque>
#include <array>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define MAKE_UNCOPYABLE(__TYPE__)\
__TYPE__( const __TYPE__& ) = delete;\
__TYPE__( __TYPE__&& ) = delete;\
const __TYPE__& operator= ( const __TYPE__& ) = delete;\
const __TYPE__& operator= ( __TYPE__&& ) = delete;\


namespace BulletsMng
{
	class Console
	{

	public:
	
		static void log()
		{
			std::cout << std::endl;
		}
		template<typename FirstType, typename... Types>
		static void log( const FirstType& firstArg, const Types&... otherArgs )
		{
			std::cout << firstArg;
			log( otherArgs... );
		}
	};
}



#endif