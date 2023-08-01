#include "top.h"
#include <string>

using namespace std;

// data_in0 data
// data_in1 rp
// data_in2 srp
// data_out0 ntt
void cryptonets(UDTYPEin *data_in0, UDTYPEin *data_in1, UDTYPEin *data_in2,
                UDTYPEin *data_out0, UDTYPEin *rp_in0) {

// #pragma HLS ALLOCATION instances=square_mc3 limit=1 function
#pragma HLS INTERFACE s_axilite register port = return
#pragma HLS INTERFACE m_axi depth = 16384 port = data_in0 offset =             \
    slave bundle = in1
#pragma HLS INTERFACE m_axi depth = 16384 port = data_in1 offset =             \
    slave bundle = in2
#pragma HLS INTERFACE m_axi depth = 16384 port = data_in2 offset =             \
    slave bundle = in3
#pragma HLS INTERFACE m_axi depth = 81920 port = data_out0 offset =            \
    slave bundle = in3
#pragma HLS INTERFACE m_axi depth = 49152 port = rp_in0 offset =               \
    slave bundle = in2

  //--------Modulus DATA init--------------------------------------------------
  UDTYPE modulus[MODCOUNT1][3] = {
      {66404353, 11391767, 4}, {66420737, 11124097, 4}, {66551809, 8987482, 4},
      {66813953, 4739403, 4},  {66961409, 2364475, 4},  {66994177, 1838133, 4}};

  UDTYPE inv[MODCOUNT1 - 1] = {38735760, 32261385, 7394495, 6073625, 33478661};
  UDTYPE half_layer1[MODCOUNT1] = {33497088, 33497088, 33497088,
                                   33497088, 33497088, 33497088};

  UDTYPE rp[MODCOUNT1][RPBRAMNUM][RPBRAMSIZE];
#pragma HLS ARRAY_PARTITION variable = rp complete dim = 1
#pragma HLS ARRAY_PARTITION variable = rp complete dim = 2
  UDTYPE srp[MODCOUNT1][RPBRAMNUM][RPBRAMSIZE];
#pragma HLS ARRAY_PARTITION variable = srp complete dim = 1
#pragma HLS ARRAY_PARTITION variable = srp complete dim = 2
  UDTYPE idt[MODCOUNT1][RPBRAMNUM][RPBRAMSIZE];
#pragma HLS ARRAY_PARTITION variable = idt complete dim = 1
#pragma HLS ARRAY_PARTITION variable = idt complete dim = 2
  UDTYPE sidt[MODCOUNT1][RPBRAMNUM][RPBRAMSIZE];
#pragma HLS ARRAY_PARTITION variable = sidt complete dim = 1
#pragma HLS ARRAY_PARTITION variable = sidt complete dim = 2

  // rootpowers in
  for (IDXTYPE m = 0; m < MODCOUNT1; m++) {
    for (IDXTYPE idx = 0; idx < N; idx++) {
#pragma HLS PIPELINE
      IDXTYPE i, j;
      i = idx(L_RPBRAMNUM - 1, 0);
      j = (idx >> L_RPBRAMNUM)(L_RPBRAMSIZE - 1, 0);

      UDTYPEin rp_temp;
      rp_temp = rp_in0[idx + m * N];

      // note the HP ports
      rp[m][i][j] = rp_temp((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH);
      srp[m][i][j] = rp_temp((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH);
      idt[m][i][j] = rp_temp((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH);
      sidt[m][i][j] = rp_temp((3 + 1) * BITWIDTH - 1, 3 * BITWIDTH);
    }
  }

  // data in
  UDTYPE input_data[MODCOUNT1][2][BRAMNUM][BRAMSIZE];
#pragma HLS ARRAY_PARTITION variable = input_data complete dim = 1
#pragma HLS ARRAY_PARTITION variable = input_data complete dim = 2
#pragma HLS ARRAY_PARTITION variable = input_data complete dim = 3

  // data_test
  IDXTYPE idx_data = 0;

  multplain_inplace_cp_mc3(data_in0 + idx_data, data_in1 + idx_data,
                           data_in2 + idx_data, input_data[5], input_data[4],
                           input_data[3], modulus[5], modulus[4], modulus[3]);

  intt_mc3_new(input_data[5], input_data[4], input_data[3], idt[5], idt[4],
               idt[3], sidt[5], sidt[4], sidt[3], modulus[5][0], modulus[4][0],
               modulus[3][0]);

  rescale_para_mc2<1>(input_data[5], input_data[4], input_data[3],
                      half_layer1[5], half_layer1[4], half_layer1[3], inv[4],
                      inv[3], modulus[5], modulus[4], modulus[3]);

  ntt_mc2_new(input_data[4], input_data[3], rp[4], rp[3], srp[4], srp[3],
              modulus[4][0], modulus[3][0]);

  data_in0 += N;
  data_in1 += N;
  data_in2 += N;

  multplain_inplace_cp_mc3(data_in0 + idx_data, data_in1 + idx_data,
                           data_in2 + idx_data, input_data[2], input_data[1],
                           input_data[0], modulus[2], modulus[1], modulus[0]);

  intt_mc3_new(input_data[2], input_data[1], input_data[0], idt[2], idt[1],
               idt[0], sidt[2], sidt[1], sidt[0], modulus[2][0], modulus[1][0],
               modulus[0][0]);

  rescale_para_mc3<1>(input_data[5], input_data[2], input_data[1],
                      input_data[0], half_layer1[5], half_layer1[2],
                      half_layer1[1], half_layer1[0], inv[2], inv[1], inv[0],
                      modulus[5], modulus[2], modulus[1], modulus[0]);

  ntt_mc3_new(input_data[2], input_data[1], input_data[0], rp[2], rp[1], rp[0],
              srp[2], srp[1], srp[0], modulus[2][0], modulus[1][0],
              modulus[0][0]);

// data out
// IDXTYPE out_idx = 0;
out_loop:
  for (IDXTYPE k = 0; k < 2; k++) {
    for (IDXTYPE m = 0; m < MODCOUNT2; m++) {
      for (IDXTYPE idx = 0; idx < N; idx++) {
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE dependent = false type = inter variable = data_out0
        IDXTYPE i, j;
        // i = idx % BRAMNUM;
        // j = (idx / BRAMNUM) % BRAMSIZE;
        i = idx(L_BRAMNUM - 1, 0);
        j = (idx >> L_BRAMNUM)(L_BRAMSIZE - 1, 0);

        // cout << input_data[m][k][i][j] << endl;
        data_out0[idx + k * MODCOUNT2 * N + m * N](
            (0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = input_data[m][k][i][j];
        // out_idx++;
      }
    }
  }
}
