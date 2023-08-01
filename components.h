#pragma once
#include "define.h"

//--------------22-03-09
UDTYPE add_uint64(UDTYPE operand1, UDTYPE operand2, UDTYPE *result);

UDTYPE add_uint64_generic(UDTYPE operand1, UDTYPE operand2, uint8_t carry,
                          UDTYPE *result);

UDTYPE Hadd_single(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus_value);

uint8_t sub_uint64(UDTYPE operand1, UDTYPE operand2, UDTYPE *result);

uint8_t sub_uint64_generic(UDTYPE operand1, UDTYPE operand2, uint8_t borrow,
                           UDTYPE *result);

UDTYPE sub_uint_uint_mod(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus);

UDTYPE Hsub_single(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus);

UDTYPE barrett_reduce_63(UDTYPE input, UDTYPE modulus, UDTYPE modulus_ratio1);

UDTYPE barrett_reduce_128(UDTYPE input0, UDTYPE input1, UDTYPE modulus,
                          UDTYPE modulus_ratio0, UDTYPE modulus_ratio1);

UDTYPE HMult_single_return(UDTYPE operand1, UDTYPE operand2, UDTYPE modulus,
                           UDTYPE modulus_ratio_0, UDTYPE modulus_ratio_1);
