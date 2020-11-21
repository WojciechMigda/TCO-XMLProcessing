#pragma once

#ifndef APP_SRC_ERROR_PRINTER_HPP_
#define APP_SRC_ERROR_PRINTER_HPP_

#include <string>


/*
 * Convenience function to handle error printing in `Either` processing
 * pipelines.
 * Because of the convenience of using `Either` errors can simply be passed
 * as strings. Such approach can be found in code written in FP languages, such
 * as scala.
 * `error_printer` also transforms error message into error code that will
 * be returned by the program.
 */
int error_printer(std::string && error_message);


#endif /* APP_SRC_ERROR_PRINTER_HPP_ */
