#ifndef CONFIG__HPP
#define CONFIG__HPP

#include "DateTime.hpp"
#include "State.hpp"

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

        virtual void execute ();
        /**
         * \brief display all config on USART0
         * \return void
         */
        void print_config ();
        /**
         * \brief update confif register
         * \param request (???)
         * \return void
         */
        void update (char request[]);
        /**
         * \brief add char to request
         * \param c (???)
         * \return void
         */
        void addToRequest (char c);
};

#endif
