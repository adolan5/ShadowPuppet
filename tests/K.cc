#include <iostream>
#include <fstream>
#include <libfreenect.h>
#include <libfreenect_sync.h>

#include "K.h"

void K::roberts_cross(){
	for( int i = 0; i < 480; i++ ){
		for( int j = 0; j < 640; j++ ){
			if( abs( depth[i*DIM_X+j] - depth[(i+1)*DIM_X+(j+1)) +
				abs( depth[(i+1)*DIM_X+j] - depth[i*DIM_X+(j+1)) )
				depth[i*DIM_X+j] = 0;
			else
				depth[i*DIM_X+j] = 1;
		}
	}

}

void K::get_depth(){
	uint32_t ts;
	if( freenect_sync_get_depth( (void**)&depth, &ts, 1, FREENECT_DEPTH_11BIT) < 0 ){
		cerr << "NO KINECT\n";
		exit( -1 );
	}
}

void K::save_depth(){
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

void K::snapshot(){
	freenect_sync_set_led( 1, 0 );
	sleep( timer );
	save_depth( range );
	freenect_sync_set_led( 0, 0 );
}
