/*
 * PrinterLib.cpp
 *
 *  Created on: Apr 4, 2025
 *      Author: Mediatek
 */


#include "PrinterLib.hpp"

Printer::Printer(std::string initialMessage) {
    baseMessage = initialMessage + "\n";  // Append newline
    printCount = 0;                       // Initialize counter
}

std::string Printer::getFormattedMessage() {
    printCount++;  // Increment counter
    return baseMessage + "Print Count: " + std::to_string(printCount);
}

void Printer::resetCount() {
    printCount = 0;
}

int Printer::getCount() {
    return printCount;
}
