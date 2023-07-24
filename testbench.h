#include "top.h"

using namespace std;

template <unsigned data_amount>
void read_data_1d(UDTYPEin *data, string path)
{

    ifstream file;
    file.open(path, ios::in);

    UDTYPEin data_temp;
    int index = 0;
    while (!file.eof())
    {
        file >> data_temp;
        data[index] = data_temp;
        index++;

        if (index == data_amount)
            break;
    }
}

template <unsigned data_amount>
void read_data_1d_28(UDTYPE *data, string path)
{

    ifstream file;
    file.open(path, ios::in);

    UDTYPEin data_temp;
    int index = 0;
    while (!file.eof())
    {
        file >> data_temp;
        data[index] = data_temp;
        index++;

        if (index == data_amount)
            break;
    }
}