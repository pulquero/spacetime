/* Bruno Barberi Gnecco <brunobg@geocities.com>, 2001. */

/* Converted to C/C++ by Crossroads V1.0*/

typedef struct Point2Struct {
	double x, y;
} Point2;

typedef struct Point3Struct {
	double x, y, z;
} Point3;

/*
 *  The following variables define the 3D mesh.
 *  The number of elements for vertices can be calculated
 *  as sizeof(obj_vertex)/sizeof(Point3). The number of elements
 *  in the face arrays can be calculated as sizeof(obj_face)/sizeof(long).
 *
 *  ------------ MainStreet -------------
 *  extern Point3 MainStreet_vertex[];
 *  extern long MainStreet_face[];
 *  extern Point2 MainStreet_uv[];
 *  extern long MainStreet_uvface[];
 *  ------------ Street1 -------------
 *  extern Point3 Street1_vertex[];
 *  extern long Street1_face[];
 *  extern Point2 Street1_uv[];
 *  extern long Street1_uvface[];
 *  ------------ Street2 -------------
 *  extern Point3 Street2_vertex[];
 *  extern long Street2_face[];
 *  extern Point2 Street2_uv[];
 *  extern long Street2_uvface[];
 *  ------------ Box01 -------------
 *  extern Point3 Box01_vertex[];
 *  extern long Box01_face[];
 *  ------------ Box02 -------------
 *  extern Point3 Box02_vertex[];
 *  extern long Box02_face[];
 *  ------------ Box03 -------------
 *  extern Point3 Box03_vertex[];
 *  extern long Box03_face[];
 *  ------------ Box04 -------------
 *  extern Point3 Box04_vertex[];
 *  extern long Box04_face[];
 *  ------------ Box05 -------------
 *  extern Point3 Box05_vertex[];
 *  extern long Box05_face[];
 *  ------------ Box06 -------------
 *  extern Point3 Box06_vertex[];
 *  extern long Box06_face[];
 *  ------------ Box07 -------------
 *  extern Point3 Box07_vertex[];
 *  extern long Box07_face[];
 *  ------------ Box08 -------------
 *  extern Point3 Box08_vertex[];
 *  extern long Box08_face[];
 *  ------------ Box09 -------------
 *  extern Point3 Box09_vertex[];
 *  extern long Box09_face[];
 *  ------------ Car01 -------------
 *  extern Point3 Car01_vertex[];
 *  extern long Car01_face[];
 *  ------------ Box10 -------------
 *  extern Point3 Box10_vertex[];
 *  extern long Box10_face[];
 *  ------------ Box11 -------------
 *  extern Point3 Box11_vertex[];
 *  extern long Box11_face[];
 *  ------------ Box12 -------------
 *  extern Point3 Box12_vertex[];
 *  extern long Box12_face[];
 *  ------------ Box13 -------------
 *  extern Point3 Box13_vertex[];
 *  extern long Box13_face[];
 *  ------------ Box14 -------------
 *  extern Point3 Box14_vertex[];
 *  extern long Box14_face[];
 *  ------------ Box15 -------------
 *  extern Point3 Box15_vertex[];
 *  extern long Box15_face[];
 *  ------------ Box16 -------------
 *  extern Point3 Box16_vertex[];
 *  extern long Box16_face[];
 *  ------------ Box17 -------------
 *  extern Point3 Box17_vertex[];
 *  extern long Box17_face[];
 *  ------------ Box18 -------------
 *  extern Point3 Box18_vertex[];
 *  extern long Box18_face[];
 *  ------------ Box19 -------------
 *  extern Point3 Box19_vertex[];
 *  extern long Box19_face[];
 *  ------------ Box20 -------------
 *  extern Point3 Box20_vertex[];
 *  extern long Box20_face[];
 *  ------------ Box21 -------------
 *  extern Point3 Box21_vertex[];
 *  extern long Box21_face[];
 *  ------------ Box22 -------------
 *  extern Point3 Box22_vertex[];
 *  extern long Box22_face[];
 *  ------------ Box23 -------------
 *  extern Point3 Box23_vertex[];
 *  extern long Box23_face[];
 *  ------------ Box24 -------------
 *  extern Point3 Box24_vertex[];
 *  extern long Box24_face[];
 *  ------------ Box25 -------------
 *  extern Point3 Box25_vertex[];
 *  extern long Box25_face[];
 *  ------------ Box26 -------------
 *  extern Point3 Box26_vertex[];
 *  extern long Box26_face[];
 *  ------------ Box27 -------------
 *  extern Point3 Box27_vertex[];
 *  extern long Box27_face[];
 *  ------------ Box28 -------------
 *  extern Point3 Box28_vertex[];
 *  extern long Box28_face[];
 *  ------------ Box29 -------------
 *  extern Point3 Box29_vertex[];
 *  extern long Box29_face[];
 *  ------------ Box30 -------------
 *  extern Point3 Box30_vertex[];
 *  extern long Box30_face[];
 *  ------------ Box31 -------------
 *  extern Point3 Box31_vertex[];
 *  extern long Box31_face[];
 *  ------------ Box32 -------------
 *  extern Point3 Box32_vertex[];
 *  extern long Box32_face[];
 *  ------------ Box33 -------------
 *  extern Point3 Box33_vertex[];
 *  extern long Box33_face[];
 *  ------------ Box34 -------------
 *  extern Point3 Box34_vertex[];
 *  extern long Box34_face[];
 *  ------------ Box35 -------------
 *  extern Point3 Box35_vertex[];
 *  extern long Box35_face[];
 *  ------------ Sidewalk1 -------------
 *  extern Point3 Sidewalk1_vertex[];
 *  extern long Sidewalk1_face[];
 *  extern Point2 Sidewalk1_uv[];
 *  extern long Sidewalk1_uvface[];
 *  ------------ Sidewalk2 -------------
 *  extern Point3 Sidewalk2_vertex[];
 *  extern long Sidewalk2_face[];
 *  extern Point2 Sidewalk2_uv[];
 *  extern long Sidewalk2_uvface[];
 *  ------------ Sidewalk3 -------------
 *  extern Point3 Sidewalk3_vertex[];
 *  extern long Sidewalk3_face[];
 *  extern Point2 Sidewalk3_uv[];
 *  extern long Sidewalk3_uvface[];
 *  ------------ Sidewalk4 -------------
 *  extern Point3 Sidewalk4_vertex[];
 *  extern long Sidewalk4_face[];
 *  extern Point2 Sidewalk4_uv[];
 *  extern long Sidewalk4_uvface[];
 *  ------------ Sidewalk5 -------------
 *  extern Point3 Sidewalk5_vertex[];
 *  extern long Sidewalk5_face[];
 *  extern Point2 Sidewalk5_uv[];
 *  extern long Sidewalk5_uvface[];
 *  ------------ Sidewalk6 -------------
 *  extern Point3 Sidewalk6_vertex[];
 *  extern long Sidewalk6_face[];
 *  extern Point2 Sidewalk6_uv[];
 *  extern long Sidewalk6_uvface[];
 *  ------------ Sidewalk7 -------------
 *  extern Point3 Sidewalk7_vertex[];
 *  extern long Sidewalk7_face[];
 *  extern Point2 Sidewalk7_uv[];
 *  extern long Sidewalk7_uvface[];
 *  ------------ Sidewalk8 -------------
 *  extern Point3 Sidewalk8_vertex[];
 *  extern long Sidewalk8_face[];
 *  extern Point2 Sidewalk8_uv[];
 *  extern long Sidewalk8_uvface[];
 *  ------------ Gutter1 -------------
 *  extern Point3 Gutter1_vertex[];
 *  extern long Gutter1_face[];
 *  extern Point2 Gutter1_uv[];
 *  extern long Gutter1_uvface[];
 *  ------------ Gutter2 -------------
 *  extern Point3 Gutter2_vertex[];
 *  extern long Gutter2_face[];
 *  extern Point2 Gutter2_uv[];
 *  extern long Gutter2_uvface[];
*/



/* --------------- MainStreet ---------------*/
Point3 MainStreet_vertex[] = {
	{ -2.999000,-101.999001,  0.001000}, {  3.001000,-101.999001,  0.001000}, {  3.001000,102.000999,  0.001000},
	{ -2.999000,102.000999,  0.001000}
};

long MainStreet_face[] = {
	0,1,2,-1,
	3,0,2,-1
};

Point2 MainStreet_uv[] = {
	{  1.050000,  1.222223}, {  1.950000,  1.222222}, {  1.950007, 46.555557},
	{  1.050007, 46.555557}
};

long MainStreet_uvface[] = {
	0,1,2,-1,
	3,0,2,-1
};



/* --------------- Street1 ---------------*/
Point3 Street1_vertex[] = {
	{-20.000000,102.000000,  0.000000}, { 20.000000,102.000000,  0.000000}, { 20.000000,108.000000,  0.000000},
	{-20.000000,108.000000,  0.000000}
};

long Street1_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Street1_uv[] = {
	{ -1.499993, 46.111115}, {  4.500007, 46.111111}, {  4.500008, 47.444443},
	{ -1.499993, 47.444447}
};

long Street1_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Street2 ---------------*/
Point3 Street2_vertex[] = {
	{-20.000000,-108.000000,  0.000000}, { 20.000000,-108.000000,  0.000000}, { 20.000000,-102.000000,  0.000000},
	{-20.000000,-102.000000,  0.000000}
};

long Street2_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Street2_uv[] = {
	{ -1.500000,  0.333334}, {  4.500000,  0.333332}, {  4.500000,  1.666666},
	{ -1.500000,  1.666668}
};

long Street2_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Box01 ---------------*/
Point3 Box01_vertex[] = {
	{-12.000000, 77.000000,  0.000000}, {-12.000000,100.000000,  0.000000}, { -5.000000,100.000000,  0.000000},
	{ -5.000000, 77.000000,  0.000000}, {-12.000000, 77.000000,  8.000000}, { -5.000000, 77.000000,  8.000000},
	{ -5.000000,100.000000,  8.000000}, {-12.000000,100.000000,  8.000000}
};

long Box01_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box02 ---------------*/
Point3 Box02_vertex[] = {
	{-12.000000, 66.000000,  0.000000}, {-12.000000, 77.000000,  0.000000}, { -5.000000, 77.000000,  0.000000},
	{ -5.000000, 66.000000,  0.000000}, {-12.000000, 66.000000, 21.000000}, { -5.000000, 66.000000, 21.000000},
	{ -5.000000, 77.000000, 21.000000}, {-12.000000, 77.000000, 21.000000}
};

long Box02_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box03 ---------------*/
Point3 Box03_vertex[] = {
	{-12.000000, 60.000000,  0.000000}, {-12.000000, 66.000000,  0.000000}, { -5.000000, 66.000000,  0.000000},
	{ -5.000000, 60.000000,  0.000000}, {-12.000000, 60.000000,  3.000000}, { -5.000000, 60.000000,  3.000000},
	{ -5.000000, 66.000000,  3.000000}, {-12.000000, 66.000000,  3.000000}
};

long Box03_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box04 ---------------*/
Point3 Box04_vertex[] = {
	{-12.000000, 49.000000,  0.000000}, {-12.000000, 60.000000,  0.000000}, { -5.000000, 60.000000,  0.000000},
	{ -5.000000, 49.000000,  0.000000}, {-12.000000, 49.000000,  3.400000}, { -5.000000, 49.000000,  3.400000},
	{ -5.000000, 60.000000,  3.400000}, {-12.000000, 60.000000,  3.400000}
};

long Box04_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box05 ---------------*/
Point3 Box05_vertex[] = {
	{-12.000000, 33.000000,  0.000000}, {-12.000000, 49.000000,  0.000000}, { -5.000000, 49.000000,  0.000000},
	{ -5.000000, 33.000000,  0.000000}, {-12.000000, 33.000000,  6.000000}, { -5.000000, 33.000000,  6.000000},
	{ -5.000000, 49.000000,  6.000000}, {-12.000000, 49.000000,  6.000000}
};

long Box05_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box06 ---------------*/
Point3 Box06_vertex[] = {
	{-12.000000, 29.000000,  0.000000}, {-12.000000, 33.000000,  0.000000}, { -5.000000, 33.000000,  0.000000},
	{ -5.000000, 29.000000,  0.000000}, {-12.000000, 29.000000,  3.100000}, { -5.000000, 29.000000,  3.100000},
	{ -5.000000, 33.000000,  3.100000}, {-12.000000, 33.000000,  3.100000}
};

long Box06_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box07 ---------------*/
Point3 Box07_vertex[] = {
	{-12.000000, 12.000000,  0.000000}, {-12.000000, 29.000000,  0.000000}, { -5.000000, 29.000000,  0.000000},
	{ -5.000000, 12.000000,  0.000000}, {-12.000000, 12.000000,  9.000000}, { -5.000000, 12.000000,  9.000000},
	{ -5.000000, 29.000000,  9.000000}, {-12.000000, 29.000000,  9.000000}
};

long Box07_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box08 ---------------*/
Point3 Box08_vertex[] = {
	{-12.000000,-22.000000,  0.000000}, {-12.000000, 12.000000,  0.000000}, { -5.000000, 12.000000,  0.000000},
	{ -5.000000,-22.000000,  0.000000}, {-12.000000,-22.000000,  5.000000}, { -5.000000,-22.000000,  5.000000},
	{ -5.000000, 12.000000,  5.000000}, {-12.000000, 12.000000,  5.000000}
};

long Box08_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box09 ---------------*/
Point3 Box09_vertex[] = {
	{-12.000000,-42.000000,  0.000000}, {-12.000000,-22.000000,  0.000000}, { -5.000000,-22.000000,  0.000000},
	{ -5.000000,-42.000000,  0.000000}, {-12.000000,-42.000000, 34.000000}, { -5.000000,-42.000000, 34.000000},
	{ -5.000000,-22.000000, 34.000000}, {-12.000000,-22.000000, 34.000000}
};

long Box09_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Car01 ---------------*/
Point3 Car01_vertex[] = {
	{ -2.000000,-97.000000,  0.000000}, { -2.000000,-93.000000,  0.000000}, {  0.000000,-93.000000,  0.000000},
	{  0.000000,-97.000000,  0.000000}, { -2.000000,-97.000000,  1.707140}, {  0.000000,-97.000000,  1.707140},
	{  0.000000,-93.000000,  1.707140}, { -2.000000,-93.000000,  1.707140}
};

long Car01_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box10 ---------------*/
Point3 Box10_vertex[] = {
	{-12.000000,-56.000000,  0.000000}, {-12.000000,-42.000000,  0.000000}, { -5.000000,-42.000000,  0.000000},
	{ -5.000000,-56.000000,  0.000000}, {-12.000000,-56.000000,  8.000000}, { -5.000000,-56.000000,  8.000000},
	{ -5.000000,-42.000000,  8.000000}, {-12.000000,-42.000000,  8.000000}
};

long Box10_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box11 ---------------*/
Point3 Box11_vertex[] = {
	{-30.000000,-80.000000,  0.000000}, {-30.000000,-56.000000,  0.000000}, { -5.000000,-56.000000,  0.000000},
	{ -5.000000,-80.000000,  0.000000}, {-30.000000,-80.000000, 45.000000}, { -5.000000,-80.000000, 45.000000},
	{ -5.000000,-56.000000, 45.000000}, {-30.000000,-56.000000, 45.000000}
};

long Box11_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box12 ---------------*/
Point3 Box12_vertex[] = {
	{-30.000000,-100.000000,  0.000000}, {-30.000000,-80.000000,  0.000000}, { -5.000000,-80.000000,  0.000000},
	{ -5.000000,-100.000000,  0.000000}, {-30.000000,-100.000000, 20.000000}, { -5.000000,-100.000000, 20.000000},
	{ -5.000000,-80.000000, 20.000000}, {-30.000000,-80.000000, 20.000000}
};

long Box12_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box13 ---------------*/
Point3 Box13_vertex[] = {
	{-20.000000,-124.286026,  0.000000}, {-20.000000,-110.000000,  0.000000}, {  4.855305,-110.000000,  0.000000},
	{  4.855305,-124.286026,  0.000000}, {-20.000000,-124.286026,  6.000000}, {  4.855305,-124.286026,  6.000000},
	{  4.855305,-110.000000,  6.000000}, {-20.000000,-110.000000,  6.000000}
};

long Box13_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box14 ---------------*/
Point3 Box14_vertex[] = {
	{-10.000000,110.000000,  0.000000}, {-10.000000,118.000000,  0.000000}, { 10.000000,118.000000,  0.000000},
	{ 10.000000,110.000000,  0.000000}, {-10.000000,110.000000, 39.000000}, { 10.000000,110.000000, 39.000000},
	{ 10.000000,118.000000, 39.000000}, {-10.000000,118.000000, 39.000000}
};

long Box14_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box15 ---------------*/
Point3 Box15_vertex[] = {
	{-10.000000,110.000000, 39.000004}, {-10.000000,118.000000, 39.000004}, { 10.000000,118.000000, 39.000004},
	{ 10.000000,110.000000, 39.000004}, { -5.000000,112.000000, 47.000004}, {  5.000000,112.000000, 47.000004},
	{  5.000000,116.000000, 47.000004}, { -5.000000,116.000000, 47.000004}
};

long Box15_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box16 ---------------*/
Point3 Box16_vertex[] = {
	{  5.000000, 95.000000,  0.000000}, {  5.000000,100.000000,  0.000000}, { 12.000000,100.000000,  0.000000},
	{ 12.000000, 95.000000,  0.000000}, {  5.000000, 95.000000,  9.000000}, { 12.000000, 95.000000,  9.000000},
	{ 12.000000,100.000000,  9.000000}, {  5.000000,100.000000,  9.000000}
};

long Box16_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box17 ---------------*/
Point3 Box17_vertex[] = {
	{  5.000000, 91.000000,  0.000000}, {  5.000000, 95.000000,  0.000000}, { 12.000000, 95.000000,  0.000000},
	{ 12.000000, 91.000000,  0.000000}, {  5.000000, 91.000000,  4.000000}, { 12.000000, 91.000000,  4.000000},
	{ 12.000000, 95.000000,  4.000000}, {  5.000000, 95.000000,  4.000000}
};

long Box17_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box18 ---------------*/
Point3 Box18_vertex[] = {
	{  5.000000, 81.000000,  0.000000}, {  5.000000, 91.000000,  0.000000}, { 12.000000, 91.000000,  0.000000},
	{ 12.000000, 81.000000,  0.000000}, {  5.000000, 81.000000,  5.000000}, { 12.000000, 81.000000,  5.000000},
	{ 12.000000, 91.000000,  5.000000}, {  5.000000, 91.000000,  5.000000}
};

long Box18_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box19 ---------------*/
Point3 Box19_vertex[] = {
	{  5.000000, 78.000000,  0.000000}, {  5.000000, 81.000000,  0.000000}, { 12.000000, 81.000000,  0.000000},
	{ 12.000000, 78.000000,  0.000000}, {  5.000000, 78.000000,  3.000000}, { 12.000000, 78.000000,  3.000000},
	{ 12.000000, 81.000000,  3.000000}, {  5.000000, 81.000000,  3.000000}
};

long Box19_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box20 ---------------*/
Point3 Box20_vertex[] = {
	{  5.000000, 68.000000,  0.000000}, {  5.000000, 78.000000,  0.000000}, { 12.000000, 78.000000,  0.000000},
	{ 12.000000, 68.000000,  0.000000}, {  5.000000, 68.000000,  4.000000}, { 12.000000, 68.000000,  4.000000},
	{ 12.000000, 78.000000,  4.000000}, {  5.000000, 78.000000,  4.000000}
};

long Box20_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box21 ---------------*/
Point3 Box21_vertex[] = {
	{  5.000000, 57.000000,  0.000000}, {  5.000000, 68.000000,  0.000000}, { 12.000000, 68.000000,  0.000000},
	{ 12.000000, 57.000000,  0.000000}, {  5.000000, 57.000000,  7.000000}, { 12.000000, 57.000000,  7.000000},
	{ 12.000000, 68.000000,  7.000000}, {  5.000000, 68.000000,  7.000000}
};

long Box21_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box22 ---------------*/
Point3 Box22_vertex[] = {
	{  5.000000, 50.000000,  0.000000}, {  5.000000, 57.000000,  0.000000}, { 12.000000, 57.000000,  0.000000},
	{ 12.000000, 50.000000,  0.000000}, {  5.000000, 50.000000, 20.000000}, { 12.000000, 50.000000, 20.000000},
	{ 12.000000, 57.000000, 20.000000}, {  5.000000, 57.000000, 20.000000}
};

long Box22_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box23 ---------------*/
Point3 Box23_vertex[] = {
	{  5.000000, 40.000000,  0.000000}, {  5.000000, 50.000000,  0.000000}, { 12.000000, 50.000000,  0.000000},
	{ 12.000000, 40.000000,  0.000000}, {  5.000000, 40.000000,  9.000000}, { 12.000000, 40.000000,  9.000000},
	{ 12.000000, 50.000000,  9.000000}, {  5.000000, 50.000000,  9.000000}
};

long Box23_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box24 ---------------*/
Point3 Box24_vertex[] = {
	{  5.042060, 18.907608,  0.000000}, {  5.042060, 39.907608,  0.000000}, { 12.042060, 39.907608,  0.000000},
	{ 12.042060, 18.907608,  0.000000}, {  5.042060, 18.907608,  4.000000}, { 12.042060, 18.907608,  4.000000},
	{ 12.042060, 39.907608,  4.000000}, {  5.042060, 39.907608,  4.000000}
};

long Box24_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box25 ---------------*/
Point3 Box25_vertex[] = {
	{  5.000000,  7.000000,  0.000000}, {  5.000000, 19.000000,  0.000000}, { 12.000000, 19.000000,  0.000000},
	{ 12.000000,  7.000000,  0.000000}, {  5.000000,  7.000000, 15.000000}, { 12.000000,  7.000000, 15.000000},
	{ 12.000000, 19.000000, 15.000000}, {  5.000000, 19.000000, 15.000000}
};

long Box25_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box26 ---------------*/
Point3 Box26_vertex[] = {
	{  5.000000,  4.000000,  0.000000}, {  5.000000,  7.000000,  0.000000}, { 12.000000,  7.000000,  0.000000},
	{ 12.000000,  4.000000,  0.000000}, {  5.000000,  4.000000,  3.000000}, { 12.000000,  4.000000,  3.000000},
	{ 12.000000,  7.000000,  3.000000}, {  5.000000,  7.000000,  3.000000}
};

long Box26_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box27 ---------------*/
Point3 Box27_vertex[] = {
	{  5.000000,-15.000000,  0.000000}, {  5.000000,  4.000000,  0.000000}, { 12.000000,  4.000000,  0.000000},
	{ 12.000000,-15.000000,  0.000000}, {  5.000000,-15.000000, 10.000000}, { 12.000000,-15.000000, 10.000000},
	{ 12.000000,  4.000000, 10.000000}, {  5.000000,  4.000000, 10.000000}
};

long Box27_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box28 ---------------*/
Point3 Box28_vertex[] = {
	{  5.000000,-28.000000,  0.000000}, {  5.000000,-15.000000,  0.000000}, { 12.000000,-15.000000,  0.000000},
	{ 12.000000,-28.000000,  0.000000}, {  5.000000,-28.000000, 31.000000}, { 12.000000,-28.000000, 31.000000},
	{ 12.000000,-15.000000, 31.000000}, {  5.000000,-15.000000, 31.000000}
};

long Box28_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box29 ---------------*/
Point3 Box29_vertex[] = {
	{  5.000000,-38.000000,  0.000000}, {  5.000000,-28.000000,  0.000000}, { 12.000000,-28.000000,  0.000000},
	{ 12.000000,-38.000000,  0.000000}, {  5.000000,-38.000000,  6.000000}, { 12.000000,-38.000000,  6.000000},
	{ 12.000000,-28.000000,  6.000000}, {  5.000000,-28.000000,  6.000000}
};

long Box29_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box30 ---------------*/
Point3 Box30_vertex[] = {
	{  5.000000,-47.000000,  0.000000}, {  5.000000,-38.000000,  0.000000}, { 12.000000,-38.000000,  0.000000},
	{ 12.000000,-47.000000,  0.000000}, {  5.000000,-47.000000,  4.000000}, { 12.000000,-47.000000,  4.000000},
	{ 12.000000,-38.000000,  4.000000}, {  5.000000,-38.000000,  4.000000}
};

long Box30_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box31 ---------------*/
Point3 Box31_vertex[] = {
	{  5.000000,-67.000000,  0.000000}, {  5.000000,-47.000000,  0.000000}, { 12.000000,-47.000000,  0.000000},
	{ 12.000000,-67.000000,  0.000000}, {  5.000000,-67.000000,  5.000000}, { 12.000000,-67.000000,  5.000000},
	{ 12.000000,-47.000000,  5.000000}, {  5.000000,-47.000000,  5.000000}
};

long Box31_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box32 ---------------*/
Point3 Box32_vertex[] = {
	{  5.000000,-76.000000,  0.000000}, {  5.000000,-67.000000,  0.000000}, { 12.000000,-67.000000,  0.000000},
	{ 12.000000,-76.000000,  0.000000}, {  5.000000,-76.000000, 13.000000}, { 12.000000,-76.000000, 13.000000},
	{ 12.000000,-67.000000, 13.000000}, {  5.000000,-67.000000, 13.000000}
};

long Box32_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box33 ---------------*/
Point3 Box33_vertex[] = {
	{  5.000000,-88.000000,  0.000000}, {  5.000000,-76.000000,  0.000000}, { 12.000000,-76.000000,  0.000000},
	{ 12.000000,-88.000000,  0.000000}, {  5.000000,-88.000000,  7.000000}, { 12.000000,-88.000000,  7.000000},
	{ 12.000000,-76.000000,  7.000000}, {  5.000000,-76.000000,  7.000000}
};

long Box33_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box34 ---------------*/
Point3 Box34_vertex[] = {
	{  5.000000,-100.000000,  0.000000}, {  5.000000,-88.000000,  0.000000}, { 18.000000,-88.000000,  0.000000},
	{ 18.000000,-100.000000,  0.000000}, {  5.000000,-100.000000, 16.000000}, { 18.000000,-100.000000, 16.000000},
	{ 18.000000,-88.000000, 16.000000}, {  5.000000,-88.000000, 16.000000}
};

long Box34_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Box35 ---------------*/
Point3 Box35_vertex[] = {
	{  5.000000,-124.000000,  0.000000}, {  5.000000,-110.000000,  0.000000}, { 18.000000,-110.000000,  0.000000},
	{ 18.000000,-124.000000,  0.000000}, {  5.000000,-124.000000,  4.000000}, { 18.000000,-124.000000,  4.000000},
	{ 18.000000,-110.000000,  4.000000}, {  5.000000,-110.000000,  4.000000}
};

long Box35_face[] = {
	0,1,2,-1,
	2,3,0,-1,
	4,5,6,-1,
	6,7,4,-1,
	0,3,5,-1,
	5,4,0,-1,
	3,2,6,-1,
	6,5,3,-1,
	2,1,7,-1,
	7,6,2,-1,
	1,0,4,-1,
	4,7,1,-1
};



/* --------------- Sidewalk1 ---------------*/
Point3 Sidewalk1_vertex[] = {
	{ -5.000000,-102.000000,  0.150000}, { -3.000000,-102.000000,  0.150000}, { -3.000000,102.000000,  0.150000},
	{ -5.000000,102.000000,  0.150000}
};

long Sidewalk1_face[] = {
	0,1,2,-1,
	3,0,2,-1
};

Point2 Sidewalk1_uv[] = {
	{ -0.500003,-10.833333}, { -0.277780,-10.833334}, { -0.277775, 11.833333},
	{ -0.499997, 11.833333}
};

long Sidewalk1_uvface[] = {
	0,1,2,-1,
	3,0,2,-1
};



/* --------------- Sidewalk2 ---------------*/
Point3 Sidewalk2_vertex[] = {
	{  3.000000,-102.000000,  0.150000}, {  5.000000,-102.000000,  0.150000}, {  5.000000,102.000000,  0.150000},
	{  3.000000,102.000000,  0.150000}
};

long Sidewalk2_face[] = {
	0,1,2,-1,
	3,0,2,-1
};

Point2 Sidewalk2_uv[] = {
	{  0.388886,-10.833334}, {  0.611108,-10.833334}, {  0.611114, 11.833333},
	{  0.388892, 11.833333}
};

long Sidewalk2_uvface[] = {
	0,1,2,-1,
	3,0,2,-1
};



/* --------------- Sidewalk3 ---------------*/
Point3 Sidewalk3_vertex[] = {
	{-20.000000,-102.000000,  0.150000}, { -5.000000,-102.000000,  0.150000}, { -5.000000,-100.000000,  0.150000},
	{-20.000000,-100.000000,  0.150000}
};

long Sidewalk3_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk3_uv[] = {
	{ -1.722228,-22.944445}, { -0.055561,-22.944445}, { -0.055561,-22.722223},
	{ -1.722228,-22.722223}
};

long Sidewalk3_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Sidewalk4 ---------------*/
Point3 Sidewalk4_vertex[] = {
	{  5.000000,-102.000000,  0.150000}, { 20.000000,-102.000000,  0.150000}, { 20.000000,-100.000000,  0.150000},
	{  5.000000,-100.000000,  0.150000}
};

long Sidewalk4_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk4_uv[] = {
	{  1.055550,-22.944445}, {  2.722217,-22.944445}, {  2.722217,-22.722223},
	{  1.055550,-22.722223}
};

long Sidewalk4_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Sidewalk5 ---------------*/
Point3 Sidewalk5_vertex[] = {
	{-20.000000,100.000000,  0.150000}, { -5.000000,100.000000,  0.150000}, { -5.000000,102.000000,  0.150000},
	{-20.000000,102.000000,  0.150000}
};

long Sidewalk5_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk5_uv[] = {
	{ -1.722223, -0.499999}, { -0.055556, -0.500000}, { -0.055556, -0.277778},
	{ -1.722223, -0.277777}
};

long Sidewalk5_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Sidewalk6 ---------------*/
Point3 Sidewalk6_vertex[] = {
	{  5.000000,100.000000,  0.150000}, { 20.000000,100.000000,  0.150000}, { 20.000000,102.000000,  0.150000},
	{  5.000000,102.000000,  0.150000}
};

long Sidewalk6_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk6_uv[] = {
	{  1.055555, -0.500000}, {  2.722222, -0.500001}, {  2.722222, -0.277778},
	{  1.055555, -0.277778}
};

long Sidewalk6_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Sidewalk7 ---------------*/
Point3 Sidewalk7_vertex[] = {
	{-20.000000,-110.000000,  0.150000}, { 20.000000,-110.000000,  0.150000}, { 20.000000,-108.000000,  0.150000},
	{-20.000000,-108.000000,  0.150000}
};

long Sidewalk7_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk7_uv[] = {
	{ -1.722228,-23.833334}, {  2.722216,-23.833336}, {  2.722216,-23.611113},
	{ -1.722228,-23.611111}
};

long Sidewalk7_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Sidewalk8 ---------------*/
Point3 Sidewalk8_vertex[] = {
	{-20.000000,108.000000,  0.150000}, { 20.000000,108.000000,  0.150000}, { 20.000000,110.000000,  0.150000},
	{-20.000000,110.000000,  0.150000}
};

long Sidewalk8_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Sidewalk8_uv[] = {
	{ -1.722222,  0.388889}, {  2.722222,  0.388888}, {  2.722222,  0.611111},
	{ -1.722222,  0.611112}
};

long Sidewalk8_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Gutter1 ---------------*/
Point3 Gutter1_vertex[] = {
	{ -3.000006,-102.000000,  0.000000}, { -2.999994,102.000000,  0.000000}, { -2.999994,102.000000,  0.150000},
	{ -3.000006,-102.000000,  0.150000}
};

long Gutter1_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Gutter1_uv[] = {
	{  0.000500,  0.000823}, {  0.999501,  0.000500}, {  0.999501,  0.999177},
	{  0.000500,  0.999501}
};

long Gutter1_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};



/* --------------- Gutter2 ---------------*/
Point3 Gutter2_vertex[] = {
	{  3.000001,102.000000,  0.000003}, {  2.999999,-102.000000, -0.000003}, {  2.999999,-102.000000,  0.149997},
	{  3.000001,102.000000,  0.150003}
};

long Gutter2_face[] = {
	0,1,2,-1,
	0,2,3,-1
};

Point2 Gutter2_uv[] = {
	{  0.022987,  1.391628}, {  9.013996,  0.412379}, {  9.013996,  0.432126},
	{  0.022987,  1.411374}
};

long Gutter2_uvface[] = {
	0,1,2,-1,
	0,2,3,-1
};

