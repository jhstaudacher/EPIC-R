library(Rmpfr)

#' @export
ComputePowerIndex <- function(index, quota, weights = NULL, weightsfile = "", precoalitions = NULL, filterNullPlayers = FALSE, verbose = FALSE, weightedMajorityGame = FALSE, useGMPTypes = FALSE, inputFloatWeights = FALSE) {
  string_results = ComputePowerIndexAdapter(index, weights, quota, weightsfile, precoalitions, filterNullPlayers, verbose, weightedMajorityGame, inputFloatWeights)
  
  if(!useGMPTypes) {
    double_results = as.double(string_results)
    return(double_results)
  }
  else {
    mpfr_results = Rmpfr::mpfr(string_results)
    return(mpfr_results)
  }
}
