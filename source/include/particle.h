#ifndef _ENGINE_PARTICLE
#define _ENGINE_PARTICLE

namespace engine
{
    class particle
	{
	public:
		float x,y,z;		
		float red,green,blue;

        bool visible;

		int type;

	public:
		particle() 
        { 
            x = y = z = 0.0f; 
            visible = false; 
			type = 0;
			
        }

		void update(float multipler = 1.0f)
		{
			//y += (0.001f * multipler);
			y += 0.01f;
			if(y > 6.0f) 
			{
				visible = false;
			}
		}
	};
};

#endif