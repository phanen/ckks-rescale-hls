#pragma once
#include "define.h"
using namespace std;

void rescale_core(UDTYPE in, UDTYPE encrypted_last_mod, UDTYPE half_mod, UDTYPE &out,
				  UDTYPE modulus, UDTYPE modulus_ratio_0, UDTYPE modulus_ratio_1, UDTYPE inv_last);

void intt_mc3_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				  UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE], UDTYPE encrypted2[2][BRAMNUM][BRAMSIZE],
				  UDTYPE idt0[RPBRAMNUM][RPBRAMSIZE], UDTYPE idt1[RPBRAMNUM][RPBRAMSIZE], UDTYPE idt2[RPBRAMNUM][RPBRAMSIZE],
				  UDTYPE sidt0[RPBRAMNUM][RPBRAMSIZE], UDTYPE sidt1[RPBRAMNUM][RPBRAMSIZE], UDTYPE sidt2[RPBRAMNUM][RPBRAMSIZE],
				  UDTYPE modulus0, UDTYPE modulus1, UDTYPE modulus2);
void ntt_mc3_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				 UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE], UDTYPE encrypted2[2][BRAMNUM][BRAMSIZE],
				 UDTYPE rp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp1[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp2[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE srp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp1[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp2[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE modulus0, UDTYPE modulus1, UDTYPE modulus2);



void ntt_mc2_new(UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
				 UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE],
				 UDTYPE rp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE rp1[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE srp0[RPBRAMNUM][RPBRAMSIZE], UDTYPE srp1[RPBRAMNUM][RPBRAMSIZE],
				 UDTYPE modulus0, UDTYPE modulus1);



// BNC = BRAMNUM para count
template <unsigned BNC>
void rescale_para_mc3(UDTYPE encrypted_last[2][BRAMNUM][BRAMSIZE],
					  UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
					  UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE],
					  UDTYPE encrypted2[2][BRAMNUM][BRAMSIZE],
					  UDTYPE half_last, UDTYPE half_mod0, UDTYPE half_mod1, UDTYPE half_mod2,
					  UDTYPE inv_last0, UDTYPE inv_last1, UDTYPE inv_last2,
					  UDTYPE modulus_fac_last[3],
					  UDTYPE modulus_fac0[3],
					  UDTYPE modulus_fac1[3],
					  UDTYPE modulus_fac2[3])
{
#pragma HLS PIPELINE off
	UDTYPE last_temp;

	for (size_t j = 0; j < BRAMSIZE; j++)
	{
		for (size_t k = 0; k < 2; k++)
		{
			for (size_t i = 0; i < BRAMNUM; i += BNC)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable = encrypted0 inter false
#pragma HLS DEPENDENCE variable = encrypted1 inter false
#pragma HLS DEPENDENCE variable = encrypted2 inter false

				for (size_t c = 0; c < BNC; c++)
				{

					size_t i_c;
					i_c = i + c;

					// cout << i_c << endl;
					// cout << "encrypted_last[" << k << "][" << i_c << "][" << j << "] = " << encrypted_last[k][i_c][j] << endl;
					// cout << "half_last = " << half_last << endl;
					last_temp = barrett_reduce_63(encrypted_last[k][i_c][j] + half_last, modulus_fac_last[0], modulus_fac_last[2]);
					// cout << "last_temp = " << last_temp << endl;

					rescale_core(encrypted0[k][i_c][j], last_temp, half_mod0, encrypted0[k][i_c][j],
								 modulus_fac0[0], modulus_fac0[1], modulus_fac0[2], inv_last0);
					rescale_core(encrypted1[k][i_c][j], last_temp, half_mod1, encrypted1[k][i_c][j],
								 modulus_fac1[0], modulus_fac1[1], modulus_fac1[2], inv_last1);
					rescale_core(encrypted2[k][i_c][j], last_temp, half_mod2, encrypted2[k][i_c][j],
								 modulus_fac2[0], modulus_fac2[1], modulus_fac2[2], inv_last2);
				}
			}
		}
	}
}

// BNC = BRAMNUM para count
template <unsigned BNC>
void rescale_para_mc2(UDTYPE encrypted_last[2][BRAMNUM][BRAMSIZE],
					  UDTYPE encrypted0[2][BRAMNUM][BRAMSIZE],
					  UDTYPE encrypted1[2][BRAMNUM][BRAMSIZE],
					  UDTYPE half_last, UDTYPE half_mod0, UDTYPE half_mod1,
					  UDTYPE inv_last0, UDTYPE inv_last1,
					  UDTYPE modulus_fac_last[3],
					  UDTYPE modulus_fac0[3],
					  UDTYPE modulus_fac1[3])
{
#pragma HLS PIPELINE off
	UDTYPE last_temp;

	for (size_t j = 0; j < BRAMSIZE; j++)
	{
		for (size_t k = 0; k < 2; k++)
		{
			for (size_t i = 0; i < BRAMNUM; i += BNC)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable = encrypted0 inter false
#pragma HLS DEPENDENCE variable = encrypted1 inter false

				for (size_t c = 0; c < BNC; c++)
				{

					size_t i_c;
					i_c = i + c;

					last_temp = barrett_reduce_63(encrypted_last[k][i_c][j] + half_last, modulus_fac_last[0], modulus_fac_last[2]);

					rescale_core(encrypted0[k][i_c][j], last_temp, half_mod0, encrypted0[k][i_c][j],
								 modulus_fac0[0], modulus_fac0[1], modulus_fac0[2], inv_last0);
					rescale_core(encrypted1[k][i_c][j], last_temp, half_mod1, encrypted1[k][i_c][j],
								 modulus_fac1[0], modulus_fac1[1], modulus_fac1[2], inv_last1);
				}
			}
		}
	}
}

