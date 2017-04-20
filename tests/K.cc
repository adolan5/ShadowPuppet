#include <iostream>
#include <fstream>
#include <libfreenect.h>
#include <libfreenect_sync.h>

using namespace std;

void get_depth( short* depth ){
	uint32_t ts;
	if( freenect_sync_get_depth( (void**)&depth, &ts, 1, FREENECT_DEPTH_11BIT) < 0 ){
		cerr << "NO KINECT\n";
		exit( -1 );
	}
}

void save_depth( int range ){
	short* depth = 0;
	get_depth( depth );

	ofstream ofile;
	ofile.open("sample.pbm");
	
	ofile << 'P';
	ofile << '1';
	ofile << '\n';
	ofile << "640 480\n";
	
	for( int i = 0; i < 480; i++ ){
		for( int j = 0; j < 640; j++ ){
			if( depth[i*640+j] < range )
				ofile << 1;
			else
				ofile << 0;
		}
		ofile << '\n';
	}

	ofile.close();
}

void snapshot( int range, int timer ){
	freenect_sync_set_led( 1, 0 );
	sleep( timer );
	save_depth( range );
	freenect_sync_set_led( 0, 0 );
}

int main(int argc, char* argv[]){
	if( argc == 1 )
		return 1;
	freenect_sync_set_tilt_degs(0, 0);
	snapshot( atoi(argv[1]), atoi(argv[2]) );

	return 0;
}
