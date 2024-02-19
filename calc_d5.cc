#define NOFILTER
#include <unistd.h>
#include <detector.h>
#include "apply_poni2023.h"
#include <Read_D5WOS.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
const int image_size_D5 = 578 * 960;
detector *D5;
int *dataD5 = 0;
int *mtD5 = 0;
char D5_name[256];
char WOS_name[256];

double scale = 1;
plot_data *plotdata_D5;
average_data *avgdata_D5;
read_h5 *reader ;

double D5_q0 = 0.01;
double D5_q1 = 1.46;
double D5_qstep = 0.005;
double d5_range[3] = {0.01, 0.625, 0.005};
double wos_range[3] = {0.45, 4, 0.01};

int size_D5, size_WOS;

FILE *gp;
apply_poni *poniWOS = NULL;
apply_poni *poniD5 = NULL;
vector<string> file_name_list;


void write(char fmt[], const char filename[], average_data *ptr, int size)
{
    char outfile[255];
    snprintf(outfile, 255, fmt, filename);
    ofstream fo(outfile);
    fo.write(reinterpret_cast<char *>(ptr), sizeof(average_data)*size);
}
void write(char fmt[], const char filename[], double *ptr, int size)
{
    char outfile[255];
    snprintf(outfile, 255, fmt, filename);
    ofstream fo(outfile);
    fo.write(reinterpret_cast<char *>(ptr), sizeof(double)*size);
}

int main(int argc, char *argv[])
{
    ifstream fi(argv[1]);
    fi.getline(WOS_name, 256);
    fi.getline(D5_name, 256);
    D5 = new detector(D5_name);
    poniD5 = new apply_poni(D5);
    dataD5 = new int[image_size_D5];
    mtD5 = new int[image_size_D5];
    plotdata_D5 = new plot_data[10000];
 
    fi.getline(D5_name, 256);
    reader = new read_h5(D5_name, 0);
    reader->read_D5(dataD5);
    D5->load_response("/Users/yoshi/analysis/D2AM_base_files/D5_flat_carbon1.dat");
    poniD5->read_poni("/Users/yoshi/data/X-ray/D2AM/2024/D5_scan0006.poni");
    D5-> load_mask("/Users/yoshi/analysis/D2AM_base_files/maskD5.dat");

    int d5size = (D5_q1-D5_q0)/ D5_qstep;
    poniD5->init_integrate(D5_q0, D5_q1, D5_qstep);
    avgdata_D5 = new average_data[d5size];
    poniD5->integrate(dataD5, avgdata_D5);
    ofstream fo("outD5");
    fo.write(reinterpret_cast<char *>(avgdata_D5), d5size*sizeof(average_data));

}
