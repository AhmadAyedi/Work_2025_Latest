/*
 * PrinterLib.hpp
 *
 *  Created on: Apr 4, 2025
 *      Author: Mediatek
 */

#ifndef INC_PRINTERLIB_HPP_
#define INC_PRINTERLIB_HPP_

#include <string>

class Printer {
private:
    std::string baseMessage;  // Attribute: base message
    int printCount;           // Attribute: tracks number of prints

public:
    // Constructor
    Printer(std::string initialMessage);

    // Method to get the formatted message
    std::string getFormattedMessage();

    // Method to reset the print counter
    void resetCount();

    // Method to get the current count (for checking in main)
    int getCount();
};

#endif /* INC_PRINTERLIB_HPP_ */
