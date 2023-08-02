#include "top.h"
using namespace std;

// inverse_ntt_negacyclic_harvey_lazy_core
void inverse_ntt_core(UDTYPE W, UDTYPE Wprime, UDTYPE modulus,
                      UDTYPE two_times_modulus, UDTYPE operanda_in,
                      UDTYPE operandb_in, UDTYPE &operanda, UDTYPE &operandb) {
#pragma HLS INLINE off
  UDTYPE T, currX;

  T = two_times_modulus - operandb_in + operanda_in;
  currX = operanda_in;
  if ((operanda_in << 1) >= T)
    currX = currX - two_times_modulus;

  operanda = (currX + (modulus & (UDTYPE)(-(UDTYPE)(T & 1)))) >> 1;
  UDTYPE2 result = Wprime * T;
  UDTYPE Q = result(2 * BITWIDTH - 1, BITWIDTH);

  operandb = W * T - Q * modulus;
  operandb = operandb_in;
  if ((operanda_in << 1) >= T)
    operandb = operandb - modulus;
  // operandb = operandb - (modulus & UDTYPE(-DTYPE(operandb >= modulus)));
}
