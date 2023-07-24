#include <iostream>
#include "top.h"
#include "testbench.h"

using namespace std;

#define DATA_IN0 16384
#define DATA_IN1 16384
#define DATA_IN2 16384
#define DATA_OUT0 81920
#define RP_IN0 49152

int main()
{
	// data read buffer
	static UDTYPE cipher_data[98304];
	static UDTYPE plain_data[49152];
	static UDTYPE rp_data[49152];
	static UDTYPE srp_data[49152];
	static UDTYPE idt_data[49152];
	static UDTYPE sidt_data[49152];
	static UDTYPE golden_out[DATA_OUT0];

	string path = "D:/Project/220723_code/220930-DATA/221017-RescaleData-save/";
	string cipher_path = path + "input_c.dat";
	string plain_path = path + "input_p.dat";
	string rp_path = path + "rootpowers/rp.dat";
	string srp_path = path + "rootpowers/srp.dat";
	string idt_path = path + "rootpowers/idt.dat";
	string sidt_path = path + "rootpowers/sidt.dat";
	string golden_path = path + "output_c.dat";

	read_data_1d_28<98304>(cipher_data, cipher_path);
	read_data_1d_28<49152>(plain_data, plain_path);
	read_data_1d_28<49152>(rp_data, rp_path);
	read_data_1d_28<49152>(srp_data, srp_path);
	read_data_1d_28<49152>(idt_data, idt_path);
	read_data_1d_28<49152>(sidt_data, sidt_path);
	read_data_1d_28<DATA_OUT0>(golden_out, golden_path);

	// data input stream
	static UDTYPEin data_in0[DATA_IN0];
	static UDTYPEin data_in1[DATA_IN1];
	static UDTYPEin data_in2[DATA_IN2];
	static UDTYPEin rp_in0[RP_IN0];
	static UDTYPEin data_out0[DATA_OUT0] = {0};

	for (int i = 0; i < N; i++)
	{
		data_in0[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[5 * N + i];
		data_in0[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[11 * N + i];
		data_in0[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[5 * N + i];

		data_in1[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[4 * N + i];
		data_in1[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[10 * N + i];
		data_in1[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[4 * N + i];

		data_in2[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[3 * N + i];
		data_in2[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[9 * N + i];
		data_in2[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[3 * N + i];
	}

	for (int i = N; i < DATA_IN0; i++)
	{
		data_in0[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[2 * N + i - N];
		data_in0[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[8 * N + i - N];
		data_in0[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[2 * N + i - N];

		data_in1[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[1 * N + i - N];
		data_in1[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[7 * N + i - N];
		data_in1[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[1 * N + i - N];

		data_in2[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = cipher_data[0 * N + i - N];
		data_in2[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = cipher_data[6 * N + i - N];
		data_in2[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = plain_data[0 * N + i - N];
	}

	for (int i = 0; i < RP_IN0; i++)
	{
		rp_in0[i]((0 + 1) * BITWIDTH - 1, 0 * BITWIDTH) = rp_data[i];
		rp_in0[i]((1 + 1) * BITWIDTH - 1, 1 * BITWIDTH) = srp_data[i];
		rp_in0[i]((2 + 1) * BITWIDTH - 1, 2 * BITWIDTH) = idt_data[i];
		rp_in0[i]((3 + 1) * BITWIDTH - 1, 3 * BITWIDTH) = sidt_data[i];
	}

	cryptonets(data_in0, data_in1, data_in2, data_out0, rp_in0);

	// result test
	// for (int i = 0; i < 10; i++)
	// {
	// 	cout << "Golden_out data is " << golden_out[i] << endl;
	// 	cout << "data_out data is " << data_out[i] << endl;
	// }

	// compare data_out with golden_out
	for (int i = 0; i < 5 * N; i++)
	{
		if (data_out0[i] != golden_out[i])
		{
			cout << "----- error!!!-----" << endl;
			cout << "The error index is " << i << endl;
			cout << "Golden_out data is " << golden_out[i] << endl;
			cout << "data_out data is " << data_out0[i] << endl;
			return 1;
		}
	}

	return 0;
}
