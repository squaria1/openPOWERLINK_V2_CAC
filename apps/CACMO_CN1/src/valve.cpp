#include "valve.h"

valves::valves()
{

}

valves::~valves()
{

}

bool valves::getLineValue(char chip_path, int line_offset)
{
	auto request = ::gpiod::chip(chip_path)
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

bool valves::initValves(char* chip_path, int offset, int16_t* valveslist)
{
    if (chip_path=="" || chip_path==" ") {
        perror("Error: GPIO chip path is not set.");
        return false;
    }

    // Ouvrir le chip GPIO
    struct gpiod_chip gpioChip(chip_path, offset);
    if (!gpioChip) {
        perror("Error: Failed to open GPIO chip.");
        return false;
    }

    // Obtenez la ligne GPIO correspondant au port GPIO � actionner
    struct GpioLine gpioLine(gpioChip.getLine(offset));
    if (!gpioLine) {
        perror("Error: Failed to get GPIO line.");
        return false;
    }

    // V�rifiez les valeurs 
    for (int i = 1; i <= MAX_VALVES; ++i) {
        // Assurez-vous que la valeur est valide (0 ou 1)
        if (valveslist(i) != 0 && value != 1) {
            perror("Error: Invalid input value. Must be 0 or 1.");
            return false;
        }

        // Actionnez la vanne 
        if (gpioLine.setValue(valveslist(i)) < 0) {
            perror("Error: Failed to set GPIO value.");
            return false;
        }
    }
    return true;
}

int16_t* getValvesValue(int16_t* benoitlist)
{
    int16_t* valveslist;
    for (int i = 1; i <= MAX_VALVES; ++i) {
        valveslist[i] = benoitlist[i];
    }

    // Retourner le tableau d'entiers
    return valveslist;
}
