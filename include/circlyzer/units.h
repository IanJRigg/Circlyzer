#ifndef UNITS_H
#define UNITS_H

// Ohms
double operator"" _ohm(long double x)
{
    return x;
}

double operator"" _kohm(long double x)
{
	return x * 1000;
}

double operator"" _Mohm(long double x)
{
	return x * 1000000;
}

// Farads
double operator"" _F(long double x)
{
	return x;
}

double operator"" _mF(long double x)
{
    return x / 1000;
}

double operator"" _muF(long double x)
{
	return x / 1000000;
}

double operator"" _nF(long double x)
{
	return x / 1000000000;
}

// Henrys
double operator"" _H(long double x)
{
	return x;
}

double operator"" _mH(long double x)
{
	return x / 1000;
}

#endif
