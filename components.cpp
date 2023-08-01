#include "top.h"

using namespace std;

UDTYPE add_uint64(UDTYPE operand1, UDTYPE operand2, UDTYPE *result) {
  *result = operand1 + operand2;
  return UDTYPE(*result < operand1); // ���*result С�� operand1�����1
}

UDTYPE add_uint64_generic(UDTYPE operand1, UDTYPE operand2, uint8_t carry,
                          UDTYPE *result) {
  operand1 += operand2;
  *result = operand1 + carry;
  return (operand1 < operand2) || (~operand1 < carry);
}

UDTYPE Hadd_single(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus_value) {
#pragma HLS INLINE off
  UDTYPE sum = operand1 + operand2;
  UDTYPE result;
  result = sum - (modulus_value & UDTYPE(-DTYPE(sum >= modulus_value)));
  return result;
}

uint8_t sub_uint64(UDTYPE operand1, UDTYPE operand2, UDTYPE *result) {
#pragma HLS INLINE off
  *result = operand1 - operand2;
  return uint8_t(operand2 > operand1);
}

UDTYPE Hsub_single(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus) {
#pragma HLS INLINE off
  UDTYPE temp, result;
  DTYPE borrow = sub_uint64(operand1, operand2, &temp);
  result = temp + (modulus & UDTYPE(-borrow));
  return result;
}

uint8_t sub_uint64_generic(UDTYPE operand1, UDTYPE operand2, uint8_t borrow,
                           UDTYPE *result) {
  UDTYPE diff = operand1 - operand2;
  *result = diff - (borrow != 0);
  return (diff > operand1) || (diff < borrow);
}

UDTYPE sub_uint_uint_mod(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus) {
  UDTYPE temp;
  DTYPE borrow = sub_uint64_generic(operand1, operand2, 0, &temp);
  return UDTYPE(temp) + (modulus & UDTYPE(-borrow));
}

UDTYPE barrett_reduce_63(UDTYPE input, UDTYPE modulus,
                         UDTYPE modulus_ratio1) // ���input��ʵ����63-bit��
{
// #pragma HLS ALLOCATION instances=mul limit=1 operation
#pragma HLS INLINE off
  UDTYPE tmp1[2];
  UDTYPE2 tmp2;
  tmp2 = input * modulus_ratio1;
  tmp1[0] = tmp2(BITWIDTH - 1, 0);
  tmp1[1] = tmp2(2 * BITWIDTH - 1, BITWIDTH);
  // Barrett subtraction
  tmp1[0] = input - tmp1[1] * modulus;

  return (UDTYPE)(tmp1[0]) -
         (modulus & (UDTYPE)(-(UDTYPE)(tmp1[0] >= modulus)));
}

UDTYPE barrett_reduce_128(UDTYPE input0, UDTYPE input1, UDTYPE modulus,
                          UDTYPE modulus_ratio0, UDTYPE modulus_ratio1) {

  // Reduces input using base 2^64 Barrett reduction
  // input allocation size must be 1BITWIDTH bits

  UDTYPE tmp1, tmp2[2], tmp3, carry;
  UDTYPE2 tmp4;

  // Multiply input and const_ratio
  // Round 1
  // multiply_uint64_hw64(input[0], modulus_ratio0, &carry);
  carry = (input0 * modulus_ratio0)(2 * BITWIDTH - 1, BITWIDTH);

  tmp4 = input0 * modulus_ratio1;
  tmp2[0] = tmp4(BITWIDTH - 1, 0);
  tmp2[1] = tmp4(2 * BITWIDTH - 1, BITWIDTH);
  // multiply_uint64(input[0], modulus_ratio1, tmp2);
  tmp3 = tmp2[1] + add_uint64_generic(tmp2[0], carry, 0, &tmp1);

  // Round 2
  tmp4 = input1 * modulus_ratio0;
  tmp2[0] = tmp4(BITWIDTH - 1, 0);
  tmp2[1] = tmp4(2 * BITWIDTH - 1, BITWIDTH);
  // multiply_uint64(input[1], modulus_ratio0, tmp2);
  carry = tmp2[1] + add_uint64_generic(tmp1, tmp2[0], 0, &tmp1);

  // This is all we care about
  tmp1 = input1 * modulus_ratio1 + tmp3 + carry;

  // Barrett subtraction
  tmp3 = input0 - tmp1 * modulus;

  // One more subtraction is enough
  return UDTYPE(tmp3) - (modulus & UDTYPE(-DTYPE(tmp3 >= modulus)));
}

UDTYPE HMult_single_return(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus,
                           UDTYPE modulus_ratio_0, UDTYPE modulus_ratio_1) {
// #pragma HLS ALLOCATION instances=mul limit=6 operation
#pragma HLS INLINE off

  UDTYPE z[2], tmp1, tmp2[2], tmp3, carry;
  UDTYPE2 tmpz, tmp4;
  tmpz = (operand1) * (operand2);
  z[0] = tmpz(BITWIDTH - 1, 0);
  z[1] = tmpz(2 * BITWIDTH - 1, BITWIDTH);

  // Multiply input and const_ratio
  // Round 1
  carry = (z[0] * modulus_ratio_0)(2 * BITWIDTH - 1, BITWIDTH);
  tmp4 = z[0] * modulus_ratio_1;
  tmp2[0] = tmp4(BITWIDTH - 1, 0);
  tmp2[1] = tmp4(2 * BITWIDTH - 1, BITWIDTH);
  tmp3 = tmp2[1] + add_uint64(tmp2[0], carry, &tmp1);

  // Round 2
  tmp4 = z[1] * modulus_ratio_0;
  tmp2[0] = tmp4(BITWIDTH - 1, 0);
  tmp2[1] = tmp4(2 * BITWIDTH - 1, BITWIDTH);
  carry = tmp2[1] + add_uint64(tmp1, tmp2[0], &tmp1);

  // This is all we care about
  tmp1 = z[1] * modulus_ratio_1 + tmp3 + carry;

  // Barrett subtraction
  tmp3 = z[0] - tmp1 * modulus;

  // Claim: One more subtraction is enough
  UDTYPE result = tmp3 - (modulus & UDTYPE(-DTYPE(tmp3 >= modulus)));

  return result;
}
