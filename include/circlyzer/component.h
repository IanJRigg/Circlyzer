#ifndef COMPONENT_H
#define COMPONENT_H

#include <complex>

enum class Component_Type : uint32_t
{
    Resistor,
    Capacitor,
    Inductor,
    Voltage_Source
};

struct Component
{
    Component(const Component_Type type) :
        type(type) 
    { 

    }

    virtual ~Component() = default;

    Component_Type type;
};

struct Resistor : Component
{
    Resistor(const double resistance) : 
        Component(Component_Type::Resistor), 
        resistance(resistance) 
    {

    }

    virtual ~Resistor() = default;

    double resistance;

    std::complex<double> get_impedence() const
    {
        return resistance;
    }
};

struct Capacitor : Component
{
    double capacitance;

    std::complex<double> get_impedence(const double frequency) const
    {
        using namespace std::complex_literals;
        return (1.0 + 0.0i) / (1.0i * frequency * capacitance);
    }
};

struct Inductor : Component
{
    double inductance;

    std::complex<double> get_impedence(const double frequency) const
    {
        using namespace std::complex_literals;
        return (1.0i * frequency * inductance);
    }
};

struct Voltage_Source : Component
{
    std::complex<double> voltage;
};

#endif