#ifndef SetupScene_H
#define SetupScene_H


#include "SceneBase.h"

namespace BulletsMng
{

	class SetupScene : public SceneBase
	{

		

		void runBulletsMngScene();


	protected:


		virtual void onOpened( const std::map<std::string,std::string>& scneneParams ) override;


	public:

		SetupScene( IApplication* application );
		virtual ~SetupScene();



	};


}



#endif