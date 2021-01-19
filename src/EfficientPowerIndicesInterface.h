#ifndef EFFICIENT_POWER_INDICES_INTERFACE_H_
#define EFFICIENT_POWER_INDICES_INTERFACE_H_

#include <Rcpp.h>

#if defined(R_BUILD)
 #define STRICT_R_HEADERS
 #include "R.h"
 // textual substitution
 #define std::cout Rcpp::Rcout
#endif

#endif /* EFFICIENT_POWER_INDICES_INTERFACE_H_ */
