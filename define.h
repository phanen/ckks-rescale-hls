#include <ap_int.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>

#define N 8192
#define STAGENUM 13

#define NN 16384

#define CORENUM 4
#define BRAMNUM 8
#define BRAMSIZE 1024
#define STAGEMAX 10

#define L_BRAMNUM 3
#define L_BRAMSIZE 10
#define L_N 13

#define RPBRAMNUM 4
#define RPBRAMSIZE 2048
#define L_RPBRAMNUM 2
#define L_RPBRAMSIZE 11

// temp-----------------------
//#define STAGE1_MAX 10
//#define CORENUMTEMP 4
//#define BRAMNUMTEMP 8
//#define BRAMSIZETEMP 1024
//------------------------------

#define BITWIDTH 28
typedef ap_uint<BITWIDTH> UDTYPE;
typedef ap_int<BITWIDTH> DTYPE;
typedef ap_uint<2 * BITWIDTH> UDTYPE2;
typedef ap_uint<128> UDTYPEin;
typedef ap_uint<32> IDXTYPE;

//#define CORENUM_RESCALE_INTT 4
//#define CORENUM_RESCALE_NTT 4
//#define CORENUM_KSWITCH_INTT0 4

//#define PORTNUM 2
//#define PACKNUM 1
//#define PACKNUM_PMUL 2
//#define PACKNUM_EPMUL 2
//#define BRAMNUM_INTT0 8
//#define BRAMNUM_INTT1 8
//#define BRAMNUM_NTT0 8
//#define BRAMNUM_NTT1 8
//#define BRAMNUM_DYADICLAST 8
//#define BRAMNUM_DYADIC 8

//#define BRAMSIZE_INTT0 1024
//#define BRAMSIZE_INTT1 1024
//#define BRAMSIZE_NTT0 1024
//#define BRAMSIZE_NTT1 1024
//#define BRAMSIZE_DYADICLAST 1024
//#define BRAMSIZE_DYADIC 1024

//#define CORENUM_INTT0 4
//#define CORENUM_NTT0 4
//#define CORENUM_INTT1 4
//#define CORENUM_NTT1 4
//#define CORENUM_DYADIC 4
//#define CORENUM_MS 1

/*
#define POLYS_SIZE 49152/2
#define POLYS_SIZE1 40960/2
#define POLYS_SIZE2 32768/2
#define POLYS_SIZE3 24576/2

#define RELINKEYS_SIZE 491520/2
#define POLYS_SIZE_28TO56 24576/2
#define POLYS_SIZE1_28TO56 20480/2
#define POLYS_SIZE2_28TO56 16384/2
#define RELINKEYS_SIZE_28TO56 245760/2
*/

#define MODCOUNT1 6
#define MODCOUNT2 5
#define MODCOUNT3 4
#define MODCOUNT4 3
#define MODCOUNT5 2
#define MODCOUNT6 1

//#define MOD_LAYER1 6
//#define MOD_LAYER2 5
//#define MOD_LAYER3 4
//#define MOD_LAYER4 3
//#define MOD_LAYER5 2
//#define MOD_OUT 1

#define ROOTMODCOUNT 6
#define INITMODCOUNT 7

//#define STAGE1_MAX 11  //2core
//#define STAGE1_MAX 10  //2core
//#define STAGE1_MAX 9  //4core

//#define STAGE1_MAX 8  //8core
//#define STAGE1_MAX 7  //16core
//#define STAGE1_MAX 6  //32core

//#define STAGE1_MAX 12  //2core STAGENUM 13
//#define STAGE1_MAX 11  //2core STAGENUM 13
//#define STAGE1_MAX 10  //4core STAGENUM 13

//#define STAGE1_MAX 9  //8core STAGENUM 13
//#define STAGE1_MAX 8  //16core STAGENUM 13
//#define STAGE1_MAX 7  //32core STAGENUM 13
