// using random numbers in GPU code:
// 1. seed using the thread id and a Wang Hash: seed = WangHash( (threadidx+1)*17 )
// 2. from there on: use RandomInt / RandomFloat
uint WangHash( uint s ) 
{ 
	s = (s ^ 61) ^ (s >> 16);
	s *= 9, s = s ^ (s >> 4);
	s *= 0x27d4eb2d;
	s = s ^ (s >> 15); 
	return s; 
}
uint RandomInt( uint* s ) // Marsaglia's XOR32 RNG
{ 
	*s ^= *s << 13;
	*s ^= *s >> 17;
	* s ^= *s << 5; 
	return *s; 
}
float RandomFloat( uint* s ) 
{ 
	return RandomInt( s ) * 2.3283064365387e-10f; // = 1 / (2^32-1)
}

// ray tracing structs

struct Intersection
{
	float t;			// intersection distance along ray
	float u, v;			// barycentric coordinates of the intersection
	uint instPrim;		// instance index (12 bit) and primitive index (20 bit)
};

struct Ray
{
	float3 O, D, rD;	// in OpenCL, each of these will be padded to 16 bytes
	struct Intersection hit;
};

struct Tri
{
	float v0x, v0y, v0z, dummy1;
	float v1x, v1y, v1z, dummy2;
	float v2x, v2y, v2z, dummy3;
	float cx, cy, cz, dummy4;
};

struct TriEx
{
	float2 uv0, uv1, uv2;
	float N0x, N0y, N0z;
	float N1x, N1y, N1z;
	float N2x, N2y, N2z;
	float dummy;
};

struct BVHNode
{
	float minx, miny, minz;
	int leftFirst;
	float maxx, maxy, maxz;
	int triCount;
};

struct TLASNode
{
	float minx, miny, minz;
	uint leftRight; // 2x16 bits
	float maxx, maxy, maxz;
	uint BLAS;
};

struct BVHInstance
{
	float16 transform;
	float16 invTransform; // inverse transform
	uint dummy[16];
};

// ray tracing helper functions

uint RGB32FtoRGB8( float3 c )
{
	int r = (int)(min( c.x, 1.f ) * 255);
	int g = (int)(min( c.y, 1.f ) * 255);
	int b = (int)(min( c.z, 1.f ) * 255);
	return (r << 16) + (g << 8) + b;
}

float3 RGB8toRGB32F( uint c )
{
	float s = 1 / 256.0f;
	int r = (c >> 16) & 255;
	int g = (c >> 8) & 255;
	int b = c & 255;
	return (float3)(r * s, g * s, b * s);
}

float3 TransformVector( float3* V, float16* T )
{
	return (float3)(
		dot( T->s012, *V ),
		dot( T->s456, *V ),
		dot( T->s89A, *V )
	);
}

float3 TransformPosition( float3* V, float16* T )
{
	return (float3)(
		dot( T->s012, *V ) + T->s3,
		dot( T->s456, *V ) + T->s7,
		dot( T->s89A, *V ) + T->sb
	);
}

void IntersectTri( struct Ray* ray, struct Tri* tri, const uint instPrim )
{
	float3 v0 = (float3)(tri->v0x, tri->v0y, tri->v0z);
	float3 v1 = (float3)(tri->v1x, tri->v1y, tri->v1z);
	float3 v2 = (float3)(tri->v2x, tri->v2y, tri->v2z);
	float3 edge1 = v1 - v0, edge2 = v2 - v0;
	float3 h = cross( ray->D, edge2 );
	float a = dot( edge1, h );
	if (fabs( a ) < 0.00001f) return; // ray parallel to triangle
	float f = 1 / a;
	float3 s = ray->O - v0;
	float u = f * dot( s, h );
	if (u < 0 | u > 1) return;
	const float3 q = cross( s, edge1 );
	const float v = f * dot( ray->D, q );
	if (v < 0 | u + v > 1) return;
	const float t = f * dot( edge2, q );
	if (t > 0.0001f && t < ray->hit.t)
		ray->hit.t = t, ray->hit.u = u,
		ray->hit.v = v, ray->hit.instPrim = instPrim;
}

float IntersectAABB( struct Ray* ray, struct BVHNode* node )
{
	float tx1 = (node->minx - ray->O.x) * ray->rD.x, tx2 = (node->maxx - ray->O.x) * ray->rD.x;
	float tmin = min( tx1, tx2 ), tmax = max( tx1, tx2 );
	float ty1 = (node->miny - ray->O.y) * ray->rD.y, ty2 = (node->maxy - ray->O.y) * ray->rD.y;
	tmin = max( tmin, min( ty1, ty2 ) ), tmax = min( tmax, max( ty1, ty2 ) );
	float tz1 = (node->minz - ray->O.z) * ray->rD.z, tz2 = (node->maxz - ray->O.z) * ray->rD.z;
	tmin = max( tmin, min( tz1, tz2 ) ), tmax = min( tmax, max( tz1, tz2 ) );
	if (tmax >= tmin && tmin < ray->hit.t && tmax > 0) return tmin; else return 1e30f;
}

// BVH traversal

void BVHIntersect( struct Ray* ray, uint instanceIdx,
	struct Tri* tri, struct BVHNode* bvhNode, uint* triIdx )
{
	struct BVHNode* node = &bvhNode[0], * stack[32];
	uint stackPtr = 0;
	while (1)
	{
		if (node->triCount > 0) // isLeaf()
		{
			for (uint i = 0; i < node->triCount; i++)
			{
				uint instPrim = (instanceIdx << 20) + triIdx[node->leftFirst + i];
				IntersectTri( ray, &tri[instPrim & 0xfffff /* 20 bits */], instPrim );
			}
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		struct BVHNode* child1 = &bvhNode[node->leftFirst];
		struct BVHNode* child2 = &bvhNode[node->leftFirst + 1];
		float dist1 = IntersectAABB( ray, child1 );
		float dist2 = IntersectAABB( ray, child2 );
		if (dist1 > dist2)
		{
			float d = dist1; dist1 = dist2; dist2 = d;
			struct BVHNode* c = child1; child1 = child2; child2 = c;
		}
		if (dist1 == 1e30f)
		{
			if (stackPtr == 0) break; else node = stack[--stackPtr];
		}
		else
		{
			node = child1;
			if (dist2 != 1e30f) stack[stackPtr++] = child2;
		}
	}
}

void TransformRay( struct Ray* ray, float16* invTransform )
{
	// do the transform
	ray->D = TransformVector( &ray->D, invTransform );
	ray->O = TransformPosition( &ray->O, invTransform );
	// update ray direction reciprocals
	ray->rD = (float3)(1.0f / ray->D.x, 1.0f / ray->D.y, 1.0f / ray->D.z);
}

void InstanceIntersect( struct Ray* ray, struct BVHInstance* bvhInstance,
	int blasIdx, struct Tri* tri, struct BVHNode* bvhNode, uint* triIdx )
{
	// backup and transform ray using instance transform
	struct Ray backup = *ray;
	TransformRay( ray, &bvhInstance->invTransform );
	// traverse the BLAS
	BVHIntersect( ray, blasIdx, tri, bvhNode, triIdx );
	// restore ray without overwriting intersection record
	backup.hit = ray->hit;
	*ray = backup;
}

void TLASIntersect( struct Ray* ray, struct Tri* tri, 
	struct BVHInstance* bvhInstance, struct TLASNode* tlasNode, 
	struct BVHNode* bvhNode, uint* triIdx )
{
	// initialize reciprocals for TLAS traversal
	ray->rD = (float3)(1.0f / ray->D.x, 1.0f / ray->D.y, 1.0f / ray->D.z);
	// use a local stack instead of a recursive function
	struct TLASNode* node = &tlasNode[0], *stack[32];
	uint stackPtr = 0;
	// traversl loop; terminates when the stack is empty
	while (1)
	{
		if (node->leftRight == 0) // isLeaf()
		{
			// current node is a leaf: intersect instance
			InstanceIntersect( ray, &bvhInstance[node->BLAS], node->BLAS, tri, bvhNode, triIdx );
			// pop a node from the stack; terminate if none left
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		// current node is an interior node: visit child nodes, ordered
		struct TLASNode* child1 = &tlasNode[node->leftRight & 0xffff];
		struct TLASNode* child2 = &tlasNode[node->leftRight >> 16];
		float dist1 = IntersectAABB( ray, child1 );
		float dist2 = IntersectAABB( ray, child2 );
		if (dist1 > dist2) 
		{ 
			float d = dist1; dist1 = dist2; dist2 = d;
			struct TLASNode* c = child1; child1 = child2; child2 = c;
		}
		if (dist1 == 1e30f)
		{
			// missed both child nodes; pop a node from the stack
			if (stackPtr == 0) break; else node = stack[--stackPtr];
		}
		else
		{
			// visit near node; push the far node if the ray intersects it
			node = child1;
			if (dist2 != 1e30f) stack[stackPtr++] = child2;
		}
	}
}

// skydome

float3 SampleSky( float3* D, float* skyPixels )
{
	float phi = atan2( D->z, D->x );
	uint u = (uint)(3200 * (phi > 0 ? phi : (phi + 2 * PI)) * INV2PI - 0.5f);
	uint v = (uint)(1600 * acos( D->y ) * INVPI - 0.5f);
	uint skyIdx = (u + v * 3200) % (3200 * 1600);
	return 0.65f * (float3)(skyPixels[skyIdx * 3], skyPixels[skyIdx * 3 + 1], skyPixels[skyIdx * 3 + 2]);
}

// EOF