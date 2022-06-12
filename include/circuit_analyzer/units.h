#ifndef UNITS_H
#define UNITS_H

// Ohms
double operator"" _ohm(const double x)
{
    return x;
}

double operator"" _kohm(const double x)
{
	return x * 1000;
}

double operator"" _Mohm(const double x)
{
	return x * 1000000;
}

// Farads
double operator"" _F(const double x)
{
	return x;
}

double operator"" _mF(const double x)
{
    return x / 1000;
}

double operator"" _muF(const double x)
{
	return x / 1000000;
}

double operator"" _nF(const double x)
{
	return x / 1000000000;
}

// Henrys
double operator"" _H(const double x)
{
	return x;
}

double operator"" _mH(const double x)
{
	return x / 1000;
}

#endif
