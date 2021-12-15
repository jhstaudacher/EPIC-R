library(Rmpfr)

#' @export
ComputePowerIndex <- function(index, weights, quota, precoalitions = NULL, filterNullPlayers = FALSE, verbose = FALSE, weightedMajorityGame = FALSE, useGMPTypes = FALSE) {
  string_results = ComputePowerIndexAdapter(index, weights, quota, precoalitions, filterNullPlayers, verbose, weightedMajorityGame)
  
  if(!useGMPTypes) {
    double_results = as.double(string_results)
    return(double_results)
  }
  else {
    mpfr_results = Rmpfr::mpfr(string_results)
    return(mpfr_results)
  }
}
