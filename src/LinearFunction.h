#ifndef LinearFunction_H
#define LinearFunction_H

#include "Common.h"

namespace BulletsMng
{

	struct LinearFunction
	{
		float Acoef;
		float Bcoef;
		float Ccoef;
		float angleCoef;

		LinearFunction();
		LinearFunction( const glm::vec2& p1, const glm::vec2& p2 );

		void initWithTwoPoints( const glm::vec2& p1, const glm::vec2& p2 );

		bool isParallel( const LinearFunction& other ) const;

		glm::vec2 crossPoint( const LinearFunction& other ) const;
	};


}

#endif