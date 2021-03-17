#include "EfficientPowerIndicesInterface.h"
#include "UserInputHandler.h"
#include "FloatToIntRepresentation.h"
#include "SystemControlUnit.h"
#include "IndexFactory.h"
#include "DataOutput.h"
#include <vector>
#include <cmath>
#include "types.h"


// function to convert Rcpp string to c
const char* crs(Rcpp::String element);

// function to validate the name of the power index
bool validateIndex(std::string index);

// function to validate the vector of weights
bool validateWeights(Rcpp::NumericVector weights);

// function to check if all elements of a Rcpp::NumericVector are integers
bool checkIfWeightsVectorIsInteger(Rcpp::NumericVector weights);

// function to check if quota is an integer
bool checkIfQuotaIsInteger(double quota);

// function to validate the quota of the game
bool validateQuota(double quota);


// [[Rcpp::export]]
std::vector<double> ComputePowerIndex(Rcpp::String index, Rcpp::NumericVector weights, double quota, bool filterNullPlayers=false, bool verbose = false, bool weightedMajorityGame = false) {
  
	std::string s_index = crs(index);

	if(!epic::index::IndexFactory::validateIndex(s_index)){
	  Rcpp::Rcout << "The following indices are available:" << std::endl;
	  epic::index::IndexFactory::printIndexList(Rcpp::Rcout);
	  Rcpp::stop("Invalid Index.");
	}	
  
	if(!validateWeights(weights)){

		Rcpp::stop("The weights vector needs to contain elements >= 0.");

	}	

	if(!validateQuota(quota)){

		Rcpp::stop("The quota of the game needs to be larger than 0.");

	}		
			
	// convert Rcpp::NumericVector to appropriate std::vector
	// If the weights vector passed from R only contains integers the conversion is simple
	// In all other cases an integer representation is found for the weights
	std::vector<epic::longUInt> v_weights = Rcpp::as<std::vector<epic::longUInt>>(weights);
	//epic::longUInt v_quota = Rcpp::as<epic::longUInt>(quota);
	epic::longUInt v_quota = static_cast<epic::longUInt>(quota);
	
	if(!checkIfWeightsVectorIsInteger(weights) || !checkIfQuotaIsInteger(quota)){
	  
	  std::vector<double> floats = Rcpp::as<std::vector<double>>(weights);
	  floats.push_back(quota);
	  // scaling weights to an integer representation
	  UpscaleFloatToIntAndReturnMultiplicator(floats,v_weights);
	  v_quota = v_weights.back();	
	  v_weights.pop_back();	
	  
	}
	if (weightedMajorityGame) {
	  v_quota = v_quota + 1;
	}
	
	// pass the arguments to the UserInputHandler
	epic::io::UserInputHandler userInputHandler(s_index, v_weights, v_quota, epic::io::OutputType::R, filterNullPlayers, verbose);
	
	
	std::vector<double> results;
	results.clear();
  
  try {
  
	// start the computation
	epic::SystemControlUnit systemControlUnit(&userInputHandler);
  
	results = systemControlUnit.handleRReturnValue();	
  	

  } catch(std::exception& e){
    
    forward_exception_to_r(e);
  }
  
  return results;	
	
}

bool validateWeights(Rcpp::NumericVector weights){

	for (int i = 0; i<weights.size(); i++){
	  
		if (weights[i] < 0)
				return false;
	}
	
	return true;
	
}

bool checkIfWeightsVectorIsInteger(Rcpp::NumericVector weights){
  
  bool isInteger = true;
  
  for (int i = 0; i<weights.size(); i++){
   
    if (trunc(weights[i]) != weights[i])
      isInteger = false;
  }
  
  return isInteger;
  
  
}


bool checkIfQuotaIsInteger(double quota){
  
  bool isInteger = true;
    
  if (trunc(quota) != quota)
    isInteger = false;
  
  return isInteger;
  
}

bool validateQuota(double quota){
	
	if (quota > 0){

        return true;
    }
    else{
        return false;
    }

}

const char* crs(Rcpp::String element){

	// convert Rcpp string to c
	return (element.get_cstring());
}
