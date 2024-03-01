#include "valve.h"

valve::valve()
{

}

valve::~valve()
{

}

#if (TARGET_SYSTEM == _WIN32_)
#else
bool valve::getLineValue(int line)
{
    int gpioPort = getPortGPIO(line);
	auto request = ::gpiod::chip(CHIP_PATH)
		.prepare_request()
		.set_consumer("get-line-value")
		.add_line_settings(
			line_offset,
			::gpiod::line_settings().set_direction(
				::gpiod::line::direction::INPUT))
		.do_request();

	::std::cout << line_offset << "="
		<< (request.get_value(line_offset) ==
			::gpiod::line::value::ACTIVE ?
			"Active" :
			"Inactive")
		<< ::std::endl;

	return true;
}

bool valve::initValve()
{
    if (CHIP_PATH=="" || CHIP_PATH==" ") {
        perror("Error: GPIO chip path is not set.");
        return false;
    }

    for (int i = 0; i <= MAX_VALVES; ++i) {
        if(getActivation())
        // Ouvrir le chip GPIO
        struct gpiod_chip gpioChip(CHIP_PATH, getPortGPIO(i));
        if (!gpioChip) {
            perror("Error: Failed to open GPIO chip.");
            return false;
        }

        // Obtenez la ligne GPIO correspondant au port GPIO a actionner
        struct GpioLine gpioLine(gpioChip.getLine(getPortGPIO(i)));
        if (!gpioLine) {
            perror("Error: Failed to get GPIO line.");
            return false;
        }

        // Verifiez les valeurs 
        // Assurez-vous que la valeur est valide (0 ou 1)
        if (getEtatInitialVannes(i) < 0 || 
            getEtatInitialVannes(i) > 1) {
            perror("Error: Invalid input value. Must be 0 or 1.");
            return false;
        }

        // Actionnez la vanne 
        if (gpioLine.setValue(getEtatInitialVannes(i)) < 0) {
            perror("Error: Failed to set GPIO value.");
            return false;
        }
    }
    return true;
}
#endif

extern "C"
{
    int16_t getValveValue(int index)
    {
        int16_t valveValue = getValeur(index);
        return valveValue;
    }
}
