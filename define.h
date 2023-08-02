#include <ap_int.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <hls_math.h>
#include <hls_stream.h>
#include <iostream>
#include <stdint.h>
#include <string>

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
// #define STAGE1_MAX 10
// #define CORENUMTEMP 4
// #define BRAMNUMTEMP 8
// #define BRAMSIZETEMP 1024
//------------------------------

#define BITWIDTH 28
typedef ap_uint<BITWIDTH> UDTYPE;
typedef ap_int<BITWIDTH> DTYPE;
typedef ap_uint<2 * BITWIDTH> UDTYPE2;
typedef ap_uint<128> UDTYPEin;
typedef ap_uint<32> IDXTYPE;

#define MODCOUNT1 6
#define MODCOUNT2 5
#define MODCOUNT3 4
#define MODCOUNT4 3
#define MODCOUNT5 2
#define MODCOUNT6 1

#define ROOTMODCOUNT 6
#define INITMODCOUNT 7
