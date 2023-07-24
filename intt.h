#include "define.h"
using namespace std;

void inverse_ntt_core(UDTYPE W, UDTYPE Wprime, UDTYPE modulus, UDTYPE two_times_modulus,
					  UDTYPE operanda_in, UDTYPE operandb_in, UDTYPE &operanda, UDTYPE &operandb);

template <unsigned corenum, unsigned bramnum, unsigned bramsize, unsigned stagemax>
void intt_4core_new(UDTYPE operand[bramnum][bramsize],
					UDTYPE idt[RPBRAMNUM][RPBRAMSIZE],
					UDTYPE sidt[RPBRAMNUM][RPBRAMSIZE],
					UDTYPE modulus)
{
#pragma HLS INLINE off

	UDTYPE operanda[corenum];
#pragma HLS ARRAY_PARTITION variable = operanda complete dim = 0
	UDTYPE operandb[corenum];
#pragma HLS ARRAY_PARTITION variable = operandb complete dim = 0
	UDTYPE operanda_[corenum];
#pragma HLS ARRAY_PARTITION variable = operanda_ complete dim = 0
	UDTYPE operandb_[corenum];
#pragma HLS ARRAY_PARTITION variable = operandb_ complete dim = 0

	UDTYPE operanda_out[corenum];
#pragma HLS ARRAY_PARTITION variable = operanda_out complete dim = 0
	UDTYPE operandb_out[corenum];
#pragma HLS ARRAY_PARTITION variable = operandb_out complete dim = 0
	UDTYPE operanda_2out[corenum];
#pragma HLS ARRAY_PARTITION variable = operanda_2out complete dim = 0
	UDTYPE operandb_2out[corenum];
#pragma HLS ARRAY_PARTITION variable = operandb_2out complete dim = 0

	UDTYPE two_times_modulus = modulus << 1;
	UDTYPE one_u64 = 1;
	uint32_t step_num = bramsize;
	uint32_t stage_num = STAGENUM;
	uint32_t stage1_max = stagemax;

	uint32_t MEa;
	uint32_t MEb;

	uint32_t arrayWindex[corenum];
#pragma HLS ARRAY_PARTITION variable = arrayWindex complete dim = 0

intt_stage2:
	for (uint32_t i = 0; i < stage_num - stage1_max; i++)
	{

		uint32_t temp4 = stage_num - 1 - i;
		uint32_t temp_par = corenum >> i;
		// uint32_t temp5 = i - stage1_max;

		MEa = 0;
		MEb = 0;

		for (int m = 0; m < corenum; m++)
		{
			arrayWindex[m] = (1 << temp4) + (m >> i);
		}

	intt_stage2_inner:
		for (uint32_t j = 0; j < step_num; j++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable = operand inter false
#pragma HLS DEPENDENCE variable = operanda inter false
#pragma HLS DEPENDENCE variable = operandb inter false

			UDTYPE w[corenum];
#pragma HLS ARRAY_PARTITION variable = w complete dim = 1
			UDTYPE sw[corenum];
#pragma HLS ARRAY_PARTITION variable = sw complete dim = 1

			uint32_t temp_adder = temp_par * j;

			// 			for (uint32_t m = 0; m < corenum; m++)
			// 			{
			// #pragma HLS UNROLL
			// 				uint32_t x = (arrayWindex[m] + temp_adder);
			// 				//				w[m]=rp[x];
			// 				//				sw[m]=srp[x];
			// 				w[m] = idt[0];
			// 				sw[m] = sidt[0];
			// 			}

			if (i == 0)
			{
				IDXTYPE x = (arrayWindex[0] + temp_adder);
				IDXTYPE rp_idx0_j;
				rp_idx0_j = x >> L_RPBRAMNUM;

				w[0] = idt[0][rp_idx0_j];
				sw[0] = sidt[0][rp_idx0_j];
				w[1] = idt[1][rp_idx0_j];
				sw[1] = sidt[1][rp_idx0_j];
				w[2] = idt[2][rp_idx0_j];
				sw[2] = sidt[2][rp_idx0_j];
				w[3] = idt[3][rp_idx0_j];
				sw[3] = sidt[3][rp_idx0_j];

				operanda[0] = operand[0][MEa];
				operandb[0] = operand[1][MEb];
				operanda[1] = operand[2][MEa];
				operandb[1] = operand[3][MEb];
				operanda[2] = operand[4][MEa];
				operandb[2] = operand[5][MEb];
				operanda[3] = operand[6][MEa];
				operandb[3] = operand[7][MEb];

				for (uint32_t m = 0; m < corenum; m++)
				{
#pragma HLS UNROLL

					inverse_ntt_core(w[m], sw[m], modulus, two_times_modulus,
									 operanda[m], operandb[m], operanda_[m], operandb_[m]);
				}

				operand[0][MEa] = operanda_[0];
				operand[1][MEb] = operandb_[0];
				operand[2][MEa] = operanda_[1];
				operand[3][MEb] = operandb_[1];
				operand[4][MEa] = operanda_[2];
				operand[5][MEb] = operandb_[2];
				operand[6][MEa] = operanda_[3];
				operand[7][MEb] = operandb_[3];
			}
			else if (i == 1)
			{
				IDXTYPE x = (arrayWindex[0] + temp_adder);
				IDXTYPE rp_idx0_i, rp_idx0_j;
				rp_idx0_i = x(L_RPBRAMNUM - 1, 0);
				rp_idx0_j = x >> L_RPBRAMNUM;

				UDTYPE rp_t1, srp_t1;
				UDTYPE rp_t2, srp_t2;

				rp_t1 = idt[rp_idx0_i][rp_idx0_j];
				srp_t1 = sidt[rp_idx0_i][rp_idx0_j];

				rp_t2 = idt[rp_idx0_i + 1][rp_idx0_j];
				srp_t2 = sidt[rp_idx0_i + 1][rp_idx0_j];

				w[0] = rp_t1;
				sw[0] = srp_t1;
				w[1] = rp_t1;
				sw[1] = srp_t1;
				w[2] = rp_t2;
				sw[2] = srp_t2;
				w[3] = rp_t2;
				sw[3] = srp_t2;

				operanda[0] = operand[0][MEa];
				operandb[0] = operand[2][MEb];
				operanda[1] = operand[1][MEa];
				operandb[1] = operand[3][MEb];
				operanda[2] = operand[4][MEa];
				operandb[2] = operand[6][MEb];
				operanda[3] = operand[5][MEa];
				operandb[3] = operand[7][MEb];

				for (uint32_t m = 0; m < corenum; m++)
				{
#pragma HLS UNROLL
					//					cout << "w[m]" << w[m] << endl;
					//					cout << "sw[m]" << sw[m] << endl;
					//
					//					cout << "in: operanda[m]" << operanda[m] << endl;
					//					cout << "in: operandb[m]" << operandb[m] << endl;
					inverse_ntt_core(w[m], sw[m], modulus, two_times_modulus,
									 operanda[m], operandb[m], operanda_[m], operandb_[m]);

					//					cout << "out: operanda[m]" << operanda_[m] << endl;
					//					cout << "out: operandb[m]" << operandb_[m] << endl;
				}

				operand[0][MEa] = operanda_[0];
				operand[2][MEb] = operandb_[0];
				operand[1][MEa] = operanda_[1];
				operand[3][MEb] = operandb_[1];
				operand[4][MEa] = operanda_[2];
				operand[6][MEb] = operandb_[2];
				operand[5][MEa] = operanda_[3];
				operand[7][MEb] = operandb_[3];
			}
			else if (i == 2)
			{
				for (uint32_t m = 0; m < corenum; m++)
				{
#pragma HLS UNROLL

					IDXTYPE x = (arrayWindex[0] + temp_adder);
					IDXTYPE rp_idx0_i, rp_idx0_j;
					rp_idx0_i = x(L_RPBRAMNUM - 1, 0);
					rp_idx0_j = x >> L_RPBRAMNUM;
					// cout << "x = " << x << endl;

					w[m] = idt[rp_idx0_i][rp_idx0_j];
					sw[m] = sidt[rp_idx0_i][rp_idx0_j];
				}

				operanda[0] = operand[0][MEa];
				operandb[0] = operand[4][MEb];
				operanda[1] = operand[1][MEa];
				operandb[1] = operand[5][MEb];
				operanda[2] = operand[2][MEa];
				operandb[2] = operand[6][MEb];
				operanda[3] = operand[3][MEa];
				operandb[3] = operand[7][MEb];

				for (uint32_t m = 0; m < corenum; m++)
				{
#pragma HLS UNROLL
					//					cout << "w[m]" << w[m] << endl;
					//					cout << "sw[m]" << sw[m] << endl;
					//
					//					cout << "in: operanda[m]" << operanda[m] << endl;
					//					cout << "in: operandb[m]" << operandb[m] << endl;
					inverse_ntt_core(w[m], sw[m], modulus, two_times_modulus,
									 operanda[m], operandb[m], operanda_[m], operandb_[m]);
					//					cout << "out: operanda[m]" << operanda_[m] << endl;
					//					cout << "out: operandb[m]" << operandb_[m] << endl;
				}

				operand[0][MEa] = operanda_[0];
				operand[4][MEb] = operandb_[0];
				operand[1][MEa] = operanda_[1];
				operand[5][MEb] = operandb_[1];
				operand[2][MEa] = operanda_[2];
				operand[6][MEb] = operandb_[2];
				operand[3][MEa] = operanda_[3];
				operand[7][MEb] = operandb_[3];
			}

			MEa += 1;
			MEb += 1;
		}
	}

intt_stage1_without_last:
	for (uint32_t i = stage_num - stage1_max; i < stage_num - 1; i++)
	{
		// uint32_t stepsize = step_num >> (i + 1);

		// stepsize formula is wrong
		uint32_t stepsize = 1 << (i - (stage_num - stage1_max));

		/*std::cout << " stepsize = " <<stepsize << std::endl;
		std::cout << " i = " <<i << std::endl;
		std::cout << " stage_num = " <<stage_num << std::endl;
		std::cout << " stage1_max = " <<stage1_max << std::endl;*/

		uint32_t temp2 = i - (stage_num - stage1_max - 1);
		uint32_t temp1 = step_num >> temp2;
		uint32_t temp3 = (one_u64 << temp2) - one_u64;
		uint32_t stage_tempa = 12;

		MEa = 0;
		MEb = stepsize;

		// cout << "stepsize" << stepsize << endl;

	intt_stage1_inner:
		for (uint32_t j = 0; j < step_num; j++)
		{
#pragma HLS PIPELINE II = 2
#pragma HLS DEPENDENCE variable = operand inter false

			// UDTYPE w0 = idt[temp1 + (j >> temp2)];
			// UDTYPE sw0 = sidt[temp1 + (j >> temp2)];
			// UDTYPE w00 = idt[temp1 + ((j + 1) >> temp2)];
			// UDTYPE sw00 = sidt[temp1 + ((j + 1) >> temp2)];

			IDXTYPE rp_idx0 = temp1 + (j >> temp2);
			IDXTYPE rp_idx00 = temp1 + ((j + 1) >> temp2);
			IDXTYPE rp_idx0_i, rp_idx0_j, rp_idx00_i, rp_idx00_j;

			rp_idx0_i = rp_idx0(L_RPBRAMNUM - 1, 0);
			rp_idx0_j = rp_idx0 >> L_RPBRAMNUM;
			rp_idx00_i = rp_idx00(L_RPBRAMNUM - 1, 0);
			rp_idx00_j = rp_idx00 >> L_RPBRAMNUM;

			UDTYPE w0 = idt[rp_idx0_i][rp_idx0_j];
			UDTYPE sw0 = sidt[rp_idx0_i][rp_idx0_j];
			UDTYPE w00 = idt[rp_idx00_i][rp_idx00_j];
			UDTYPE sw00 = sidt[rp_idx00_i][rp_idx00_j];

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL
				//				cout << "m = " << m << endl;
				//				cout << "MEa" << MEa << endl;
				//				cout << "MEb = " << MEb << endl;
				//				cout << "[m][MEa] = " << operand[m][MEa] << endl;
				//				cout << "[m][MEb] = " << operand[m][MEb] << endl;

				operanda[m] = operand[m][MEa];
				operandb[m] = operand[m][MEb];
				operanda_[m] = operand[m + corenum][MEa];
				operandb_[m] = operand[m + corenum][MEb];
			}

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL
				/*
								if (i == stage_tempa){
									cout << "w[m]" << w0 << endl;
									cout << "sw[m]" << sw0 << endl;

									cout << "in: operanda[m]" << operanda[m] << endl;
									cout << "in: operandb[m]" << operandb[m] << endl;
								}*/

				inverse_ntt_core(w0, sw0, modulus, two_times_modulus, operanda[m], operandb[m],
								 operanda_out[m], operandb_out[m]);

				/*if (i == stage_tempa){
				cout << "out: operanda[m]" << operanda_out[m] << endl;
				cout << "out: operandb[m]" << operandb_out[m] << endl;
				}*/

				/*if (i == stage_tempa){
					cout << "w[m]" << w0 << endl;
					cout << "sw[m]" << sw0 << endl;

					cout << "in: operanda[m]" << operanda_[m] << endl;
					cout << "in: operandb[m]" << operandb_[m] << endl;
				}*/

				inverse_ntt_core(w00, sw00, modulus, two_times_modulus, operanda_[m], operandb_[m],
								 operanda_2out[m], operandb_2out[m]);

				/*if (i == stage_tempa){
				cout << "out: operanda[m]" << operanda_2out[m] << endl;
				cout << "out: operandb[m]" << operandb_2out[m] << endl;
				}*/
				// inverse_ntt_negacyclic_harvey_lazy_core(w0, sw0, modulus, two_times_modulus, operanda[m], operandb[m]);
			}
			j++;

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL

				/*std::cout << " m = " <<m << std::endl;
				std::cout << " m + corenum = " <<m + corenum << std::endl;
				std::cout << " MEa = " <<MEa << std::endl;
				std::cout << " MEb = " <<MEb << std::endl;*/

				operand[m][MEa] = operanda_out[m];
				operand[m][MEb] = operandb_out[m];
				operand[m + corenum][MEa] = operanda_2out[m];
				operand[m + corenum][MEb] = operandb_2out[m];
			}

			if (((j + 1) & temp3) == 0)
			{
				MEa += stepsize + 1;
				MEb += stepsize + 1;
			}
			else
			{
				MEa += 1;
				MEb += 1;
			}
		}
	}

intt_stage1_last:
	for (uint32_t i = stage_num - 1; i < stage_num; i++)
	{
		// uint32_t stepsize = step_num >> (i + 1);

		// stepsize formula is wrong
		uint32_t stepsize = 1 << (i - (stage_num - stage1_max));

		/*std::cout << " stepsize = " <<stepsize << std::endl;
		std::cout << " i = " <<i << std::endl;
		std::cout << " stage_num = " <<stage_num << std::endl;
		std::cout << " stage1_max = " <<stage1_max << std::endl;*/

		uint32_t temp2 = i - (stage_num - stage1_max - 1);
		uint32_t temp1 = step_num >> temp2;
		uint32_t temp3 = (one_u64 << temp2) - one_u64;
		uint32_t stage_tempa = 12;

		MEa = 0;
		MEb = stepsize;

		// cout << "stepsize" << stepsize << endl;

	intt_stage1_inner_last:
		for (uint32_t j = 0; j < step_num; j++)
		{
#pragma HLS PIPELINE II = 2
#pragma HLS DEPENDENCE variable = operand inter false

			// UDTYPE w0 = idt[temp1 + (j >> temp2)];
			// UDTYPE sw0 = sidt[temp1 + (j >> temp2)];
			// UDTYPE w00 = idt[temp1 + ((j + 1) >> temp2)];
			// UDTYPE sw00 = sidt[temp1 + ((j + 1) >> temp2)];

			IDXTYPE rp_idx0 = temp1 + (j >> temp2);
			IDXTYPE rp_idx00 = temp1 + ((j + 1) >> temp2);
			IDXTYPE rp_idx0_i, rp_idx0_j, rp_idx00_i, rp_idx00_j;

			rp_idx0_i = rp_idx0(L_RPBRAMNUM - 1, 0);
			rp_idx0_j = rp_idx0 >> L_RPBRAMNUM;
			rp_idx00_i = rp_idx00(L_RPBRAMNUM - 1, 0);
			rp_idx00_j = rp_idx00 >> L_RPBRAMNUM;

			UDTYPE w0 = idt[rp_idx0_i][rp_idx0_j];
			UDTYPE sw0 = sidt[rp_idx0_i][rp_idx0_j];
			UDTYPE w00 = idt[rp_idx00_i][rp_idx00_j];
			UDTYPE sw00 = sidt[rp_idx00_i][rp_idx00_j];

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL
				operanda[m] = operand[m][MEa];
				operandb[m] = operand[m][MEb];
				operanda_[m] = operand[m + corenum][MEa];
				operandb_[m] = operand[m + corenum][MEb];
			}

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL
				inverse_ntt_core(w0, sw0, modulus, two_times_modulus, operanda[m], operandb[m],
								 operanda_out[m], operandb_out[m]);

				inverse_ntt_core(w00, sw00, modulus, two_times_modulus, operanda_[m], operandb_[m],
								 operanda_2out[m], operandb_2out[m]);
			}
			j++;

			for (int m = 0; m < corenum; m++)
			{
#pragma HLS UNROLL
				operand[m][MEa] = static_cast<UDTYPE>(operanda_out[m]) - (modulus & static_cast<UDTYPE>(-static_cast<DTYPE>(operanda_out[m] >= modulus)));
				operand[m][MEb] = static_cast<UDTYPE>(operandb_out[m]) - (modulus & static_cast<UDTYPE>(-static_cast<DTYPE>(operandb_out[m] >= modulus)));
				operand[m + corenum][MEa] = static_cast<UDTYPE>(operanda_2out[m]) - (modulus & static_cast<UDTYPE>(-static_cast<DTYPE>(operanda_2out[m] >= modulus)));
				operand[m + corenum][MEb] = static_cast<UDTYPE>(operandb_2out[m]) - (modulus & static_cast<UDTYPE>(-static_cast<DTYPE>(operandb_2out[m] >= modulus)));

				// operand[m][MEa] = operanda_out[m];
				// operand[m][MEb] = operandb_out[m];
				// operand[m + corenum][MEa] = operanda_2out[m];
				// operand[m + corenum][MEb] = operandb_2out[m];
			}

			if (((j + 1) & temp3) == 0)
			{
				MEa += stepsize + 1;
				MEb += stepsize + 1;
			}
			else
			{
				MEa += 1;
				MEb += 1;
			}
		}
	}
}
