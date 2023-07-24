#include "top.h"
using namespace std;

void rescale_core(UDTYPE in, UDTYPE encrypted_last_mod, UDTYPE half_mod, UDTYPE &out,
				  UDTYPE modulus, UDTYPE modulus_ratio_0, UDTYPE modulus_ratio_1, UDTYPE inv_last)
{
#pragma HLS INLINE off

	UDTYPE temp_destination;

	temp_destination = barrett_reduce_63(encrypted_last_mod, modulus, modulus_ratio_1);
	temp_destination = sub_uint_uint_mod(temp_destination, half_mod, modulus);

	UDTYPE temp_result_sub;
	DTYPE borrow = sub_uint64(in, temp_destination, &temp_result_sub);
	temp_destination = temp_result_sub + (modulus & UDTYPE(-borrow));

	out = HMult_single_return(temp_destination, inv_last,
							  modulus, modulus_ratio_0, modulus_ratio_1);
}

void intt_mc3_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				  UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE], UDTYPE encrypted2[2][BRAMNUM][BRAMSIZE],
				  UDTYPE idt0[RPBRAMNUM][RPBRAMSIZE], UDTYPE idt1[RPBRAMNUM][RPBRAMSIZE], UDTYPE idt2[RPBRAMNUM][RPBRAMSIZE],
				  UDTYPE sidt0[RPBRAMNUM][RPBRAMSIZE], UDTYPE sidt1[RPBRAMNUM][RPBRAMSIZE], UDTYPE sidt2[RPBRAMNUM][RPBRAMSIZE],
				  UDTYPE modulus0, UDTYPE modulus1, UDTYPE modulus2)
{
#pragma HLS INLINE off

	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[0], idt0, sidt0, modulus0);
	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[1], idt0, sidt0, modulus0);
	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[0], idt1, sidt1, modulus1);
	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[1], idt1, sidt1, modulus1);
	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted2[0], idt2, sidt2, modulus2);
	intt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted2[1], idt2, sidt2, modulus2);
}



void ntt_mc3_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				 UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE], UDTYPE encrypted2[2][BRAMNUM][BRAMSIZE],
				 UDTYPE rp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp1[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp2[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE srp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp1[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp2[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE modulus0, UDTYPE modulus1, UDTYPE modulus2)
{
#pragma HLS INLINE off

	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[0], rp0, srp0, modulus0);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[1], rp0, srp0, modulus0);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[0], rp1, srp1, modulus1);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[1], rp1, srp1, modulus1);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted2[0], rp2, srp2, modulus2);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted2[1], rp2, srp2, modulus2);
}



void ntt_mc2_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				 UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE],
				 UDTYPE rp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp1[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE srp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp1[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE modulus0, UDTYPE modulus1)
{
#pragma HLS INLINE off

	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[0], rp0, srp0, modulus0);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted0[1], rp0, srp0, modulus0);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[0], rp1, srp1, modulus1);
	ntt_4core_new<CORENUM, BRAMNUM, BRAMSIZE, STAGEMAX>(
		encrypted1[1], rp1, srp1, modulus1);
}

