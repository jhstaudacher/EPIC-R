# EPIC R Package - Efficient Power Index Computation

[[_TOC_]]

# Setup

## Prerequisites

[R](https://cran.rstudio.com/), [RStudio IDE](https://rstudio.com/products/rstudio/download/), C++ Compiler, [GNU make](https://www.gnu.org/software/make/), [GMP (Gnu Multiple Precision Arithmetic Library)](https://gmplib.org/), [Autoconf](https://www.gnu.org/software/autoconf/), [Git](https://git-scm.com/) (optional)

### Prerequisites for Windows

* Download and install [Rtools](https://cran.r-project.org/bin/windows/Rtools/)

### Prerequisites for Linux

* All the needed tools can be installed using the distributions package manager (e.g. for apt package manager run: `apt install gcc git make libgmp-dev`).

## Installation

1. Download the repository. This can be done in two different ways:    
    1. Clone the repository using git (`git clone ...`)
    1. Download the repository as ZIP archive using the download dropdown menu on the Repository overview. The archive must get extracted after download.
    

1. For Linux: Run `autoconf` in the repository to generate the `configure` file from the `configure.ac` template. Then run `./configure`.

    It is also possible to manually create the `Makevars` file by adding the following contents and inserting the path to the GMP installation:
    ```
    GMP_DIR = *Insert the path to your GMP directory here*

    CXX_STD=@CXX_STD@ 

    PKG_LIBS = -L$(GMP_DIR)/.libs -lgmpxx -lgmp
    ```


2. This directory can be opened in R Studio as a project: "File" -> "Open Project" -> Navigate to the downloaded repository -> Open "EfficientPowerIndices.Rproj"
3. Now the R package can be built and installed in the upper left hand side in the "Build" Tab using the "Install and Restart" button.

# Usage

After installing, the package can be included in R Scripts using `library(EfficientPowerIndices)`

The R package offers the function `ComputePowerIndex` which expects the following parameters:

## Required parameters
| name | type | description |
| ------ | -------- | ----------- |
| index | index shortcut as String (see Available indices) | the index to compute |
| weights or weightsfile | numeric vector or .csv file | the player weights for the weighted voting Game (*1)|
| quota | numeric | the quota for the weighted voting Game (*2)    |


(*1) The weights must be specified either by a numeric vector or a .csv file. The .csv file must contain one weight per line. The weights must all be integers unless you set the inputFloatWeights flag.

(*2) When setting the quota to 0, the first line of the weights file will be interpreted as the quota to use. 


## Optional parameters


| name | type | description |
| ------ | -------- | ----------- |
| precoalitions | List of integer vectors | specifies the structure of the precoalitions (*3) |
| verbose | boolean | adds extra output |
| filterNullPlayers | boolean| excludes null player from the calculation (not compatible with all indices!) |
| weightedMajorityGame | boolean | uses a weighted majority game (i.e. the weight sum must be greater than the quota for a coalition to be considered a winning coalition) for the underlying calculations |
| useGMPTypes | boolean | return the results as GNU MPFR types|
| inputFloatWeights | boolean | reads the weights from the weightfile as floating point numbers|

(*3) When the weights are specified using a weightsfile the structure of the precoalitions will be generated from this file and the precoalitions specified using the parameter will be ignored. The weightsfile has to be written accordingly. If defined in the weightsfile, the players in the same precoalition should all be in the same line and seperated by ",". A new line is a new precoalition. 

When using the precoalitions parameter the precoalitions are structured using a list of integer vectors. Player numbers are from 1 to n. For example precoalitions = list(c(1,3), c(2,4), c(5)) would be a preacoalition between player 1 and 3, player 2 and 4 while player 5 is alone.




## Available indices

| shortcut | name |
| -------- | ---- |
| `ABZ` | Absolute Banzhaf |
| `APG` | Absolute Public Good |
| `APIG` | Absolute Power Index G (based on the Dubey-Shapley identity) |
| `BZ` | Banzhaf |
| `BO` | Banzhaf Owen |
| `BZB` | Banzhaf from below |
| `CC` | Coleman Collective |
| `CI` | Coleman Initiative |
| `CP` | Coleman Preventive |
| `DP` | Deegan Packel |
| `FT` | Felsenthal |
| `HN` | Harsanyi Nash |
| `J` | Johnston |
| `KB` | Koenig Braeuninger (based on the Dubey-Shapley identity) |
| `KBPH` | Koenig Braeuninger (based on Raw Public Help Theta) |
| `N` | Nevison (based on the Dubey-Shapley identity)|
| `NPH` | Nevison (based on Raw Public Help Theta) |
| `O` | Owen |
| `PG` | Public Good |
| `PHT` | Public Help Theta (based on the Dubey-Shapley identity) |
| `PHTPH` | Public Help Theta (based on Raw Public Help Theta) |
| `PHX` | Public Help Xi |
| `PIF` | Power Index F |
| `PIG` | Power Index G (based on the Dubey-Shapley identity)|
| `PIGPH` | Power Index G (based on Raw Public Help Theta) |
| `RA` | Rae |
| `RBZ` | Raw Banzhaf |
| `RBZB` | Raw Banzhaf from below |
| `RDP` | Raw Deegan Packel |
| `RFT` | Raw Felsenthal |
| `RJ` | Raw Johnston |
| `RPG` | Raw Public Good |
| `RPHT` | Raw Public Help Theta |
| `RPHTSD` | Raw Public Help Theta (based on the Dubey-Shapley identity) |
| `RPIG` | Raw Power Index G |
| `RPIF` | Raw Power Index F |
| `RSH` | Raw Shapley Shubik |
| `SCB` | Symmetric Coalitional Banzhaf |
| `RSHB` | Raw Shapley Shubik from below |
| `SH` | Shapley Shubik |
| `SHB` | Shapley Shubik from below |
| `T` | Tijs |
| `W` | Number of winning coalitions |
| `WM` | Number of minimal winning coalitions |
| `WS` | Number of minimal winning coalitions of smallest cardinality |

## Example

For a simple example we want to calculate the Banzhaf index for a simple 5-player Game with the weights (9, 5, 3, 1, 1) and quota 11. 

`library(EfficientPowerIndices)`

`ComputePowerIndex(index = "BZ", quota = 11, weights = c(9, 5, 3, 1, 1), verbose = TRUE)`

By setting the verbose parameter to `TRUE` we get extra output like the raw Banzhaf values and the total number of swings (sum over the raw Banzhaf values).






