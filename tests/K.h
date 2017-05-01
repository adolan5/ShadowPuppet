#include <iostream>
#include <fstream>
#include <unistd.h>
#include <libfreenect.h>
#include <libfreenect_sync.h>

#ifndef KINECT_H
#define KINECT_H

using namespace std;

const int DIM_X = 640;
const int DIM_Y = 480;

class Depth{
	private:
		short* depth;
		int range;
	public:
		Depth( int r ):
			range(r){
			depth_state = 0;
			
			if( freenect_sync_set_tilt_degs(0, 0) < 0 ){
				cerr << "NO KINECT\n";
				throw string("Can't access kinect");
			}
		}
		void roberts_cross();
		void limit_depth();
		void save_depth( string filename );
		void snapshot( int timer );
};

#endif
