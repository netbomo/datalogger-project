#ifndef CONFIG__HPP
#define CONFIG__HPP

#include "../lib/main.h"
#include "../lib/DateTime.h"
#include "../lib/State.h"

class Config : public State {
    // Associations
    // Attributes
    private :
        DateTime currentTime;
        /// is the char array use to save character from USART0
        char request[20];
        /// this is the request string's index
        char request_i;
    // Operations
    public :
        Config ();
        virtual ~Config();

        inline void backspace(){ request_i -= 2;		/**< move on left */}

        inline void endLine(){ request[request_i-1] = '\0';}	/**< put end line to request */


        virtual void execute ();

        virtual void print(Usart &usart);

        /**
         * \brief display all config on USART0
         * \return void
         */
        void display();

        /**
         * \brief Load saved config data from the eeprom
         * \return void
         */
        void load_eeprom ();

        /**
         * \brief Update saved config data in the eeprom
         * \return void
         */
        void update_epprom ();

        /**
         * \brief Clear the eeprom memory
         * \return void
         */
        void initialize_eeprom ();

        /**
         * \brief add char to request
         * \param c (???)
         * \return void
         */
        void addToRequest (char c);

        unsigned char atoc(char string[]);

};

#endif
