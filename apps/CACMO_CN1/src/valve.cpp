#include "valve.h"

valve::valve()
{

}

valve::~valve()
{

}

#if (TARGET_SYSTEM == _WIN32_)
#else
bool valve::initValve()
{
    if (CHIP_PATH=="" || CHIP_PATH==" ") {
        perror("Error: GPIO chip path is not set.");
        return false;
    }

    unsigned int offsets[1];

    int values[1];
    int err;

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return false;
    }



    for (int i = 0; i <= MAX_VALVES; ++i) {
        if (getActivation(i + nbValuesCN_Out_ByCN))
        {
            offsets[i] = getPortGPIO(i);
            values[i] = getEtatInitialVannes(i);

            err = gpiod_chip_get_lines(chip, offsets, 1, &lines);
            if (err)
            {
                perror("gpiod_chip_get_lines");
                return false;
            }

            memset(&config, 0, sizeof(config));
            config.consumer = "valve";
            config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
            config.flags = 0;

            // Verifiez les valeurs 
            // Assurez-vous que la valeur est valide (0 ou 1)
            if (getEtatInitialVannes(i) < 0 ||
                getEtatInitialVannes(i) > 1) 
            {
                perror("Error: Invalid input value. Must be 0 or 1.");
                return false;
            }

            // Actionnez la vanne 
            err = gpiod_line_set_value_bulk(&lines, values);
            if (err)
            {
                perror("gpiod_line_set_value_bulk");
                goto cleanup;
            }
        }
    }
    return true;
}

bool valve::extinctValve()
{
    gpiod_line_release_bulk(&lines);
    gpiod_chip_close(chip);

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
