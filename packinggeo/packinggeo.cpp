#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int Nx = 96;
const int Ny = 108;
const int Nz = 96;

const int Q = 19;
const int cx[19] = { 0,1,-1,0,0,0,0,1,-1,1,-1,1,-1,1,-1,0,0,0,0 };
const int cy[19] = { 0,0,0,1,-1,0,0,1,1,-1,-1,0,0,0,0,1,-1,1,-1 };
const int cz[19] = { 0,0,0,0,0,1,-1,0,0,0,0,1,1,-1,-1,1,1,-1,-1 };

int* h_geo;

void pre_geo()
{
	int indexf[19];
	for (int z = 0; z < Nz; z++)
	{
		for (int y = 0; y < Ny; y++)
		{
			for (int x = 0; x < Nx; x++)
			{
				int index = z * Nx * Ny + y * Nx + x;
				h_geo[index] = 0;

				if (y < Ny / 2)
				{
					if (z == 0 || z == Nz)										
					{
						if (x < 48)
						{
							if (y <= (-1) * 49 * x / 47 + 51)
								h_geo[index] = 1;
						}
						else if (x > 47)
						{
							if (y <= 49 * x / 47 - 48)
								h_geo[index] = 1;
						}
					}
					else if (z > 0 && z < Nz)										
					{
						if (x >= z)						
						{
							h_geo[index] = h_geo[y * Nx + (x - z)];
						}
						else if (x < z)
						{
							h_geo[index] = h_geo[y * Nx + (x - z) + (Nx - 1)];
						}
					}
				}

				else if (y > Ny / 2)
				{
					h_geo[index] = h_geo[z * Ny * Nx + (Ny - 1 - y) * Nx + (Nx - 1 - x)];
				}
			}
		}
	}

	for (int z = 0; z < Nz; z++)
	{
		for (int y = 0; y < Ny; y++)
		{
			for (int x = 0; x < Nx; x++)
			{
				int index = z * Nx * Ny + y * Nx + x;
				if (h_geo[index] == 0)
					for (int q = 0; q < 19; q++)
					{
						int i_1 = (x + cx[q] + Nx) % Nx;
						int j_1 = (y + cy[q] + Ny) % Ny;
						int k_1 = (z + cz[q] + Nz) % Nz;
						indexf[q] = k_1 * Nx * Ny + j_1 * Nx + i_1;
						if (h_geo[indexf[q]] == 1) // boundary
							h_geo[index] = 2;
					}
				if (z == 0 && h_geo[index] == 0) // inlet
					h_geo[index] = 3;
				if (z == Nz - 1 && h_geo[index] == 0) // outlet
					h_geo[index] = 4;				
			}
		}
	}
}

void outputdoc()
{
	ofstream outputgeo;
	outputgeo.open("D:\\packing96.dat");

	for (int z = 0; z < Nz; z++)
	{
		for (int y = 0; y < Ny; y++)
		{
			for (int x = 0; x < Nx; x++)
			{
				int k = z * Nx * Ny + y * Nx + x;
				outputgeo << h_geo[k] << endl;
			}
		}
	}
	outputgeo.close();
}

void outputtest()
{
	for (int z = 0; z < Nz; z++)
	{
		for (int y = 0; y < Ny; y++)
		{
			for (int x = 0; x < Nx; x++)
			{
				int k = z * Nx * Ny + y * Nx + x;
				cout << h_geo[k];
			}cout << endl;
		}cout << endl;
	}
}

int main()
{
	h_geo = (int*)malloc(sizeof(int) * Nx * Ny * Nz);

	pre_geo();

	outputdoc();
	//outputtest();

	free(h_geo);
	return 0;
}
