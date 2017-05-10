#ifndef MEASURE__HPP
#define MEASURE__HPP

#include "../lib/State.h"

class Measure : public State {

    public :
    // Operations
        Measure ();
        virtual ~Measure();

        virtual void execute ();

        virtual void print(Usart &usart);

        static bool flag_data_frequencies_ready;

    private :
        /**
         * \brief this method clear data value to save news
         * \return void
         */
        void clear_data_array ();
        /**
         * \brief this method initialize frequence measurement for anemometer and RPM
         * \return void
         */
        void freq_init_measurement ();
        /**
         * \brief read the windvane value en map it between 0 to 359 degres
         * \return void
         */
        void windvane_value ();
        /**
         * \brief read power value - not define realy (see emonLib)
         * \return void
         */
        void power_read_value ();
        /**
         * \brief read the temperature value from one DS18b20 sensor
         * \return void
         */
        void temperature_read_value ();
        /**
         * \brief read value from anemometer and RPM
         * \return void
         */
        void freq_read_value ();
        /**
         * \brief calcul average from the data array
         * \return void
         */
        void calc_average ();

};

#endif
