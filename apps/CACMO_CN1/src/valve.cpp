#include "valve.h"

#if (TARGET_SYSTEM == _WIN32_)
#else
struct gpiod_chip* chip;
struct gpiod_line* line;
unsigned int offsets[MAX_VALVES];
int values[MAX_VALVES];
int err;
#endif

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

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return false;
    }

    for (int i = 0; i <= MAX_VALVES; ++i) {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            offsets[i] = getPortGPIO(i+1);
            values[i] = getEtatInitialVannes(i+1);
            printf("offsets[i]:%d\n", offsets[i]);
            printf("values[i]:%d\n", values[i]);

            line = gpiod_chip_get_line(chip, offsets[i]);
            if (!line)
            {
                perror("gpiod_chip_get_lines");
                gpiod_chip_close(chip);
                return false;
            }

            // Request the line for output
            if (gpiod_line_request_output(line, "test", 0) < 0) {
                perror("Request line as output failed");
                gpiod_line_release(line);
                gpiod_chip_close(chip);
                return false;
            }
            // Verifiez les valeurs 
            // Assurez-vous que la valeur est valide (0 ou 1)
            if (values[i] < 0 ||
                values[i] > 1)
            {
                perror("Error: Invalid input value. Must be 0 or 1.");
                return false;
            }
            else 
            {
                // Actionnez la vanne 
                err = gpiod_line_set_value(line, values[i]);
                if (err)
                {
                    perror("gpiod_line_set_value");
                    return false;
                }
            }
        }
    }
    return true;
}

bool valve::extinctValve()
{
    gpiod_line_release(line);
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
