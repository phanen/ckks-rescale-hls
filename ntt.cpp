#include "top.h"
using namespace std;

void ntt_core(UDTYPE W, UDTYPE Wprime, UDTYPE modulus, UDTYPE two_times_modulus,
              UDTYPE operanda_in, UDTYPE operandb_in, UDTYPE &operanda,
              UDTYPE &operandb) {
#pragma HLS INLINE off
  UDTYPE currX, Q;
  UDTYPE operanda_temp, operandb_temp;

  operanda_temp = operanda_in;
  operandb_temp = operandb_in;

  currX =
      operanda_temp - (two_times_modulus &
                       (UDTYPE)(-(DTYPE)(operanda_temp >= two_times_modulus)));
  UDTYPE2 result = Wprime * operandb_temp;
  Q = result(2 * BITWIDTH - 1, BITWIDTH);
  Q = W * (operandb_temp)-Q * modulus;
  Q = Q - (modulus & UDTYPE(-DTYPE(Q >= modulus)));
  operanda = currX + Q;
  operandb = currX + (two_times_modulus - Q);
}
