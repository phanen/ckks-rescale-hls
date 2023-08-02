#include "top.h"
using namespace std;

void ntt_core(UDTYPE W, UDTYPE Wprime, UDTYPE modulus, UDTYPE two_times_modulus,
              UDTYPE operanda_in, UDTYPE operandb_in, UDTYPE &operanda,
              UDTYPE &operandb) {
#pragma HLS INLINE off
  UDTYPE currX, Q;

  currX = operanda_in;
  if (operanda_in >= two_times_modulus)
    currX = currX - two_times_modulus;
  // currX = operanda_in - (two_times_modulus &
  //                        (UDTYPE)(-(DTYPE)(operanda_in >=
  //                        two_times_modulus)));
  UDTYPE2 result = Wprime * operandb_in;
  Q = result(2 * BITWIDTH - 1, BITWIDTH);
  Q = W * operandb_in - Q * modulus;
  if (Q >= modulus)
    Q = Q - modulus;
  // Q = Q - (modulus & UDTYPE(-DTYPE(Q >= modulus)));
  operanda = currX + Q;
  operandb = currX + (two_times_modulus - Q);
}
