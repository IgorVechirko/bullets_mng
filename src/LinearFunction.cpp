#include "LinearFunction.h"


namespace BulletsMng
{

	LinearFunction::LinearFunction()
		: Acoef( 0.0f )
		, Bcoef( 0.0f )
		, Ccoef( 0.0f )
		, angleCoef( 0.0f )
	{
	}

	LinearFunction::LinearFunction( const glm::vec2& p1, const glm::vec2& p2 )
		: Acoef( p1.y - p2.y )
		, Bcoef( p2.x - p1.x )
		, Ccoef( p1.x*p2.y - p2.x*p1.y )
		, angleCoef( Acoef/Bcoef )
	{
	}

	void LinearFunction::initWithTwoPoints( const glm::vec2& p1, const glm::vec2& p2 )
	{ 
		Acoef = p1.y - p2.y;
		Bcoef = p2.x - p1.x;
		Ccoef = p1.x*p2.y - p2.x*p1.y;
		angleCoef = Acoef/Bcoef;
	}

	bool LinearFunction::isParallel( const LinearFunction& other ) const
	{
		return abs(angleCoef - other.angleCoef) <= std::numeric_limits<float>::epsilon();
	}

	glm::vec2 LinearFunction::crossPoint( const LinearFunction& other ) const
	{
		float det = Acoef*other.Bcoef - other.Acoef*Bcoef;

		assert( abs(det) > std::numeric_limits<float>::epsilon() && "denominator must be not zero, use isParallel before" );

		glm::vec2 result( 0.0f );

		result.x = (Bcoef*other.Ccoef - Ccoef*other.Bcoef) / det;
		result.y = (Ccoef*other.Acoef - Acoef*other.Ccoef) / det;

		return result;
	}
}