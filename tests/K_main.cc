#include "K.h"

int main( int argc, char* argv[] ){
	Depth k( ::atoi(argv[1]) );
	k.snapshot();
	k.save_depth( "Pre-Dilate" );
	k.dilate();
	k.dilate();
	k.dilate();
	k.dilate();
	k.dilate();
	k.save_depth( "Post-Dilate" );
	k.roberts_cross();
	k.save_depth( "Post-RC" );

}
