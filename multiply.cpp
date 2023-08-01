#include "top.h"

using namespace std;

void multplain_inplace_cp_mc3(UDTYPEin *in0, UDTYPEin *in1, UDTYPEin *in2,
                              UDTYPE result0[2][BRAMNUM][BRAMSIZE],
                              UDTYPE result1[2][BRAMNUM][BRAMSIZE],
                              UDTYPE result2[2][BRAMNUM][BRAMSIZE],
                              UDTYPE modulus0[3], UDTYPE modulus1[3],
                              UDTYPE modulus2[3]) {
#pragma HLS INLINE off

  UDTYPEin in_temp[3];
  UDTYPEin encrypted[3][2];
  UDTYPEin plain[3];
#pragma HLS ARRAY_PARTITION variable = in_temp complete dim = 0
#pragma HLS ARRAY_PARTITION variable = encrypted complete dim = 0
#pragma HLS ARRAY_PARTITION variable = plain complete dim = 0

  IDXTYPE i, j;

Hmult_loop:
  for (IDXTYPE idx = 0; idx < N; idx++) {
#pragma HLS PIPELINE
    // i = idx % BRAMNUM;
    // j = (idx / BRAMNUM) % BRAMSIZE;

    i = idx(L_BRAMNUM - 1, 0);
    j = (idx >> L_BRAMNUM)(L_BRAMSIZE - 1, 0);

    in_temp[0] = in0[idx];
    in_temp[1] = in1[idx];
    in_temp[2] = in2[idx];

    encrypted[0][0] = (in_temp[0])((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH);
    encrypted[0][1] = (in_temp[0])((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH);
    plain[0] = (in_temp[0])((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH);

    // test
    // cout << encrypted[0][0] << endl;
    // cout << encrypted[0][1] << endl;
    // cout << plain[0] << endl;

    result0[0][i][j] = HMult_single_return(
        encrypted[0][0], plain[0], modulus0[0], modulus0[1], modulus0[2]);
    result0[1][i][j] = HMult_single_return(
        encrypted[0][1], plain[0], modulus0[0], modulus0[1], modulus0[2]);

    // cout << "result0[0][i][j] = " << result0[0][i][j] << endl;
    // cout << "result0[1][i][j] = " << result0[1][i][j] << endl;

    encrypted[1][0] = (in_temp[1])((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH);
    encrypted[1][1] = (in_temp[1])((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH);
    plain[1] = (in_temp[1])((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH);
    result1[0][i][j] = HMult_single_return(
        encrypted[1][0], plain[1], modulus1[0], modulus1[1], modulus1[2]);
    result1[1][i][j] = HMult_single_return(
        encrypted[1][1], plain[1], modulus1[0], modulus1[1], modulus1[2]);

    encrypted[2][0] = (in_temp[2])((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH);
    encrypted[2][1] = (in_temp[2])((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH);
    plain[2] = (in_temp[2])((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH);
    result2[0][i][j] = HMult_single_return(
        encrypted[2][0], plain[2], modulus2[0], modulus2[1], modulus2[2]);
    result2[1][i][j] = HMult_single_return(
        encrypted[2][1], plain[2], modulus2[0], modulus2[1], modulus2[2]);
  }
}
