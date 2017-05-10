#ifndef OUTPUT__HPP
#define OUTPUT__HPP

#include "../lib/State.h"

class Output : public State {
    // Attributes
    public :
    // Operations
        Output ();

        virtual ~Output();

        virtual void execute ();

        virtual void print(Usart &usart);

        void usart0_print ();

        void usart1_print ();

        void SDCard_print ();

        // Constants
        static const unsigned char USART0 = 1;		/// enable constant for USART0

        static const unsigned char USART1 = 2;		/// enable constant for USART1

        static const unsigned char SD_CARD = 4;	/// enable constant for SD_CARD

        static const unsigned char WIFI = 8;		/// enable constant for WIFI

        static const unsigned char GPRS = 16;		/// enable constant for GPRS


};

#endif
