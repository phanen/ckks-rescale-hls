INC=/opt/Xilinx/Vitis_HLS/2023.1/include/

CXX=c++
all:
	${CXX} *.cpp -I${INC}
