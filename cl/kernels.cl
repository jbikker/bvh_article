#include "template/common.h"
#include "cl/tools.cl"

__kernel void render( write_only image2d_t target, const int offset )
{
	// plot a pixel to outimg
	const int x = get_global_id( 0 ), red = x / 3 + offset;
	const int y = get_global_id( 1 ), green = y / 3;
	write_imagef( target, (int2)(x, y), (float4)( red / 256.0f, green / 256.0f, 0, 1 ) );
}

// EOF