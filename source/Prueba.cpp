#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <cmath>

#ifndef WIDTH	
#define WIDTH 10
#endif // !WIDTH
#ifndef PRECISION
#define PRECISION 6
#endif // !PRECISION
#ifndef PRECISION2
#define PRECISION2 3
#endif // !PRECISION2


using namespace std;

double closest(std::vector<double> const& vec, double value) {
	auto const it = std::lower_bound(vec.begin(), vec.end(), value);
	if (it == vec.end()) { return -1; }
	return *it;
}

int main() {

	typedef std::vector<vector<double>> Matrix;
	typedef std::vector<double> Vector;
	double MINF; //Lower limit og magnitude given in the tabl
	double MSUP; //Upper limit of magnitude given in the table
	int NMAG; //Number of magnitudes for which intensity is given
	double DMAG;
	double RINF; //Lower limit of distance given in the table
	double RSUP; //Upper limit of distance given in the table
	int NRAD; //Number of distances for which intensity is given
	double TYPEMD; //An integer indicating the type of distance used by the attenuation table
	double DLRAD;
	double NT; //Number of periods
	double S;
	double sigma = 0.7;
	double AMAX = 0; //Type of stadistic distribution
	double size;// Number of periods



	//--- ATTENUATION VARIABLES ----

	double f0, f1, f2, R0, R1, R2, Rcd, Ztor;
	R0 = 10;
	R1 = 70;
	R2 = 140;
	S = 0.0;

	Vector periodsreq;
	Vector frequenciesreq;
	double aux1, aux2;

	//-------------------------------

	//------- DEBUG VARIABLES -------
	double A, B, C, D, E, F, G;
	//-------------------------------

	ifstream periods;
	periods.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/data/periods.txt");

	//If we can read/write great
	while (periods.good())
	{
		periods >> aux1;
		periodsreq.push_back(aux1);
	}

#if 0
	for (size_t i = 0; i < periodsreq.size(); i++)
	{
		cout << periodsreq[i] << endl;
	}

#endif // 0

	for (size_t i = 0; i < periodsreq.size(); i++) // Changing periods to frequencies
	{
		aux2 = 1 / periodsreq.at(i);
		frequenciesreq.push_back(aux2);
	}
#if 0
	cout << periodsreq.size() << " " << frequenciesreq.size() << endl;
	for (size_t i = 0; i < frequenciesreq.size(); i++)
	{
		cout << frequenciesreq[i] << endl;
	}
#endif // 1


	// ---------------------------------- ASKING FOR DATA ------------------------------------------------

	//::std::cout << "Enter the minimum magnitude, maximum and the number of intermediate quantities: " << endl;
	//cin >> MINF >> MSUP >> NMAG;
	MINF = 3.5;
	MSUP = 8.0;
	NMAG = 12;
	//cout << "Enter the minimum distance, maximum and the number of intermediate distances: " << endl;
	//cin >> RINF >> RSUP >> NRAD;
	RINF = 10.0;
	RSUP = 500.0;
	NRAD = 20;

	// -------------------------------------- FINISH ------------------------------------------------------
	Vector aceleraciones(NRAD);

	//-------------------------------- GENRATING COEFICENTS -----------------------------------------------

	Matrix results(periodsreq.size());//vertical size of Matrix
	Matrix table6(26);//AB06, Table 6
	Matrix table9(26);//AB06, Table 9
	vector <double> coficentesreq(10);
	int index1, index2;//Positions to interpolate

	if (S == 0.0)
	{

		// Open our file tabla6.txt
		ifstream inFile1;
		inFile1.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/data/table6.txt");


		vector <double> f6(26);// Vector of table 6's frequencies
		vector <double> coef(12);// Vector of table 6's coeficents 
								 // If we can read/write great
		if (inFile1.good())
		{
			for (size_t i = 0; i < 26; i++) {
				for (size_t j = 0; j < 12; j++)	inFile1 >> coef.at(j);
				table6.at(i) = coef;
			}
		}

		for (size_t i = 0; i < 26; i++)
			f6.at(i) = table6.at(i).at(0);



		// Plot table, only if is necessary 1 = show, 2 = hide
#if 0
		cout << "Tabla 6:" << endl;
		for (size_t i = 0; i < 26; i++) {
			for (size_t j = 0; j < 12; j++)	cout << setw(WIDTH) << table6.at(i).at(j) << " ";
			cout << endl;
		}

		cout << endl;
		//------------------------------------------------------------------------------------------------------------------OK

		for (size_t i = 0; i < frequenciesreq.size(); i++)
			cout << frequenciesreq.at(i) << endl;
#endif // 1

		for (size_t k = 0; k < frequenciesreq.size(); k++)
		{
			double nearfrequency = 0.0;

			double required = frequenciesreq[k];
			if ((required == 100) || (required == 90.909090909090921)) {

				if (required == 100)
				{
					for (size_t j = 0; j < 10; j++)	coficentesreq.at(j) = table6.at(24).at(j + 2);
					results.at(k) = coficentesreq;
				}
				else
				{
					for (size_t j = 0; j < 10; j++)	coficentesreq.at(j) = table6.at(25).at(j + 2);
					results.at(k) = coficentesreq;
				}
			}
			else
			{
				nearfrequency = closest(f6, required);
				std::vector<double>::iterator it = std::find(f6.begin(), f6.end(), nearfrequency);
				index1 = std::distance(f6.begin(), it);

				if ((nearfrequency <= required))
				{
					if (index1 == 25) { index2 = index1 - 1; }
					else { index2 = index1 + 1; }
				}

				if ((nearfrequency >= required))
				{
					if (index1 == 0) { index2 = index1 + 1; }
					else { index2 = index1 - 1; }
				}


#if 0
				cout << index1 << " " << index2 << endl;
#endif // 0

				for (size_t j = 0; j < 10; j++)
				{
					coficentesreq.at(j) = (((frequenciesreq[k] - table6.at(index1).at(0))*table6.at(index2).at(j + 2)) + ((table6.at(index2).at(0) - frequenciesreq[k])*table6.at(index1).at(j + 2))) / (table6.at(index2).at(0) - table6.at(index1).at(0));
				}
				results.at(k) = coficentesreq;
			}
		}

		//------------------------------------------------------------------------------------------------------------------OK
#if 1
		ofstream coefT6; // Archive
		coefT6.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/results/resultsT6.dat");
		coefT6 << "#Coefficents for differents frequencies (Table6 - Atkinson and Boore, 2006)" << endl;
		coefT6 << setw(WIDTH) << "f(Hz)" << setw(WIDTH) << "T(sec)" << setw(WIDTH) << "c1" << setw(WIDTH) << "c2" << setw(WIDTH) << "c3" << setw(WIDTH) << "c4" << setw(WIDTH) << "c5" << setw(WIDTH) << "c6" << setw(WIDTH) << "c7" << setw(WIDTH) << "c8" << setw(WIDTH) << "c9" << setw(WIDTH) << "c10" << endl;

		for (size_t i = 0; i < periodsreq.size(); i++)
		{

			for (size_t j = 0; j < 10; j++)
				coefT6 << setw(WIDTH) << setprecision(4) << results.at(i).at(j);
			coefT6 << endl;
		}


#endif // 0

	}
	else {
		// Open our file tabla9.txt
		ifstream inFile2;
		inFile2.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/data/table9.txt");


		vector <double> f9(26);// Vector of table 9's frequencies
		vector <double> coef(12);// Vector of table 9's coeficents 
								 // If we can read/write great
		if (inFile2.good())
		{
			for (size_t i = 0; i < 26; i++) {
				for (size_t j = 0; j < 12; j++)	inFile2 >> coef.at(j);
				table9.at(i) = coef;
			}
		}

		for (size_t i = 0; i < 26; i++)
			f9.at(i) = table9.at(i).at(0);



		// Plot table, only if is necessary 1 = show, 2 = hide
#if 0
		cout << "Tabla 9:" << endl;
		for (size_t i = 0; i < 26; i++) {
			for (size_t j = 0; j < 12; j++)	cout << setw(WIDTH) << table9.at(i).at(j) << " ";
			cout << endl;
		}

		cout << endl;
		//------------------------------------------------------------------------------------------------------------------OK

		for (size_t i = 0; i < frequenciesreq.size(); i++)
			cout << frequenciesreq.at(i) << endl;
#endif // 1

		for (size_t k = 0; k < frequenciesreq.size(); k++)
		{
			double nearfrequency = 0.0;

			double required = frequenciesreq[k];
			if ((required == 100) || (required == 90.90)) {

				if (required == 100)
				{
					for (size_t j = 0; j < 10; j++)	coficentesreq.at(j) = table6.at(24).at(j + 2);
					results.at(k) = coficentesreq;
				}
				else
				{
					for (size_t j = 0; j < 10; j++)	coficentesreq.at(j) = table6.at(25).at(j + 2);
					results.at(k) = coficentesreq;
				}
			}
			else
			{
				nearfrequency = closest(f9, required);
				std::vector<double>::iterator it = std::find(f9.begin(), f9.end(), nearfrequency);
				index1 = std::distance(f9.begin(), it);

				if ((nearfrequency <= required))
				{
					if (index1 == 25) { index2 = index1 - 1; }
					else { index2 = index1 + 1; }
				}

				if ((nearfrequency >= required))
				{
					if (index1 == 0) { index2 = index1 + 1; }
					else { index2 = index1 - 1; }
				}


#if 0
				cout << index1 << " " << index2 << endl;
#endif // 0

				for (size_t j = 0; j < 10; j++)
				{
					coficentesreq.at(j) = (((frequenciesreq[k] - table9.at(index1).at(0))*table9.at(index2).at(j + 2)) + ((table9.at(index2).at(0) - frequenciesreq[k])*table9.at(index1).at(j + 2))) / (table9.at(index2).at(0) - table9.at(index1).at(0));
				}
				results.at(k) = coficentesreq;
			}
		}

		//------------------------------------------------------------------------------------------------------------------OK
#if 1
		ofstream coefT9; // Archive
		coefT9.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/results/resultsT9.dat");
		coefT9 << "#Coefficents for differents frequencies (Table9 - Atkinson and Boore, 2006)" << endl;
		coefT9 << setw(WIDTH) << "f(Hz)" << setw(WIDTH) << "T(sec)" << setw(WIDTH) << "c1" << setw(WIDTH) << "c2" << setw(WIDTH) << "c3" << setw(WIDTH) << "c4" << setw(WIDTH) << "c5" << setw(WIDTH) << "c6" << setw(WIDTH) << "c7" << setw(WIDTH) << "c8" << setw(WIDTH) << "c9" << setw(WIDTH) << "c10" << endl;

		for (size_t i = 0; i < periodsreq.size(); i++)
		{

			for (size_t j = 0; j < 10; j++)
				coefT9 << setw(WIDTH) << setprecision(4) << results.at(i).at(j);
			coefT9 << endl;
		}


#endif // 0

	}

	//--------------------------------------- FINISH -------------------------------------------------------

	//-------------------------- CHECKING ----------------------------------------

#if 0
	for (size_t i = 0; i < frequenciesreq.size(); i++)
	{
		for (size_t j = 0; j < 10; j++)
			cout << setw(WIDTH) << setprecision(4) << results.at(i).at(j);
		cout << endl;
	}
#endif // 0

	//----------------------- FINISH CHECKING -------------------------------------

#if 1

	//---------------------- GENERATING VALUES -------------------------------
	Vector distances(NRAD);
	Vector magnitudes(NMAG);
	DMAG = (MSUP - MINF) / (NMAG - 1);
	DLRAD = (log10(RSUP) - log10(RINF)) / (NRAD - 1);

	
	for (size_t i = 0; i < NRAD; i++)
	{
		distances[i] = pow(10, log10(RINF) + i*DLRAD);
	}
	distances[NRAD - 1] = RSUP; //Changing last value
	for (size_t i = 0; i < NMAG; i++)
	{
		magnitudes[i] = MINF + i*DMAG;
	}
	magnitudes[NMAG - 1] = MSUP;//Changing last value

								//---------------------------- FINISH -----------------------------------

								//---------------------------- OUTPUT -----------------------------------
	//Value Type of distance
	//	1 (or blank) Focal
	//	2 Epicentral
	//	3 Joyner and Boore
	//	4 Closest to rupture area(Rrup)

	TYPEMD = 4;

#if 1
	cout << "MAGNITUDES:" << endl;
	for (size_t i = 0; i < NMAG; i++)
	{
		cout << magnitudes[i] << endl;
	}
	cout << endl;
#endif // 0
#if 1
	cout << "DISTANCES:" << endl;
	for (size_t i = 0; i < NRAD; i++)
	{
		cout << distances[i] << endl;
	}
	cout << endl;
#endif // 0

	ofstream ab06;
	ab06.open("C:/Users/Hugo Ninnanya/Documents/GibHub/aninanya/BOORE/BOORE/results/prueba.ATN");

	ab06 << setprecision(PRECISION2);
	//ab06 << setw(WIDTH) << "#" << setw(WIDTH) << ": Description" << setw(WIDTH) << ": Sample attenuation file constructed for illustration purposes (2008)" << endl;	
	//ab06 << setw(WIDTH) << "#" << setw(WIDTH) << ": Units" << setw(WIDTH) << ": cm/sec/sec" << endl;
	//ab06 << setw(WIDTH) << "#" << setw(WIDTH) << ": Distribution" << setw(WIDTH) << ": 2" << endl;
	//ab06 << setw(WIDTH) << "#" << setw(WIDTH) << ": Dimension" << setw(WIDTH) << ": Aceleration" << endl;
	ab06 << setw(WIDTH) << MINF << setw(WIDTH) << MSUP << setw(WIDTH) << NMAG << endl;
	ab06 << setw(WIDTH) << RINF << setw(WIDTH) << RSUP << setw(WIDTH) << NRAD << setw(WIDTH)<< TYPEMD<<endl;


	for (size_t i = 0; i < periodsreq.size(); i++)//Loop over periods
	{
		ab06 << setw(WIDTH) << periodsreq.at(i) << setw(WIDTH) << sigma << setw(WIDTH) << AMAX << endl;
		for (size_t j = 0; j < NMAG; j++)//Loop over magnitudes
		{
			Ztor = 21 - 2.5*magnitudes[j];
			for (size_t k = 0; k < NRAD; k++)//Loop over coeficents
			{
				Rcd = pow(pow(distances[k], 2.0) + pow(Ztor, 2.0), 0.5);
				f0 = max(log10(R0 / Rcd), 0.0);
				f1 = min(log10(Rcd), log10(R1));
				f2 = max(log10(Rcd / R2), 0.0);
				A = results.at(i).at(0);
				B = results.at(i).at(1)*magnitudes.at(j);
				C = results.at(i).at(2)*(pow(magnitudes.at(j), 2.0));
				D = (results.at(i).at(3) + results.at(i).at(4)*magnitudes.at(j))*f1;
				E = (results.at(i).at(5) + results.at(i).at(6)*magnitudes.at(j))*f2;
				F = (results.at(i).at(7) + results.at(i).at(8)*magnitudes.at(j))*f0;
				G = results.at(i).at(9)*Rcd;
				aceleraciones[k] = pow(10.0, A + B + C + D + E + F + G + S);

				ab06 << setw(WIDTH) << aceleraciones[k];//Saving values
			}
			ab06 << endl;
		}
	}

#endif // 0

	system("pause");
	return 0;
}