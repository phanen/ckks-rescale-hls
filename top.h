#pragma once

#include <fstream>
#include <iostream>
#include "define.h"
#include "components.h"
#include "multiply.h"
#include "ntt.h"
#include "intt.h"
#include "rescale.h"



void cryptonets(UDTYPEin *data_in0, UDTYPEin *data_in1, UDTYPEin *data_in2,
				UDTYPEin *data_out0, UDTYPEin *rp_in0);

