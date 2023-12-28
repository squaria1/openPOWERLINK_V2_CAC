#include <string>

#ifndef SENSOR_H
#define SENSOR_H

class sensor
{
    public:
        sensor();
        virtual ~sensor();

        std::string Getname() { return name; }
        void Setname(std::string val) { name = val; }
        int Gettemperature() { return temperature; }
        void Settemperature(int val) { temperature = val; }
        unsigned int Getpressure() { return pressure; }
        void Setpressure(unsigned int val) { pressure = val; }
        int initSensor();


    protected:

    private:
        std::string name;
        int temperature;
        unsigned int pressure;
};

#endif // SENSOR_H
