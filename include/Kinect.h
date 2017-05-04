#include <iostream>
#include <fstream>
#include <unistd.h>
#include <libfreenect.h>
#include <libfreenect_sync.h>
#include <vector>
#include <utility>

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
		Depth( int r ): range(r){
			depth = 0;
			
			if( freenect_sync_set_tilt_degs(0, 0) < 0 ){
				cerr << "NO KINECT\n";
				throw string("Can't access kinect");
			}
		}
		~Depth(){
			free( depth );
		}		

		vector<pair<int,int>> get_coords();

		void roberts_cross();
		void dilate();
	
		void standard_collection(bool test);	
		void get_depth();
		void limit_depth();
		void save_depth( const string& filename );
		void snapshot( );
};

#endif
