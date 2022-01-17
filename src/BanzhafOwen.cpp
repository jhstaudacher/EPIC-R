#include "BanzhafOwen.h"

#include "Logging.h"

#include <cmath>

epic::index::BanzhafOwen::BanzhafOwen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::BanzhafOwen::~BanzhafOwen() {
	mCalculator->free_largeNumber(mTmp);
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::BanzhafOwen::calculate() {
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	longUInt totalWeight = mGame.getWeightSum();
	longUInt quota = mGame.getQuota();

	ArrayOffset<lint::LargeNumber> c(totalWeight + 1, quota);
	mCalculator->allocInit_largeNumberArray(c.getArrayPointer(), c.getNumberOfElements());
	mCalculator->assign_one(c[totalWeight]);
	generalizedBackwardCountingPerWeight(c, mPartW, mNbPart);

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, quota);
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cw2(totalWeight + 1, quota);
	mCalculator->alloc_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cwi(totalWeight + 1, quota);
	mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	auto banzhafsInternal = new lint::LargeNumber[mMaxPartSize];
	mCalculator->alloc_largeNumberArray(banzhafsInternal, mMaxPartSize);

	bigFloat ExternalMultiplier;
	{
		mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, mNbPart - 1);
		ExternalMultiplier = 1 / bigFloat(mBigTmp);
	}

	auto winternal = new longUInt[mMaxPartSize];

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < mNbPart; i++) {
		coalitionsContainingPlayerFromAbove(cw, c, mPartW[i]);

		//replicate vector c onto cw
		for (longUInt ii = quota; ii <= totalWeight; ii++) {
			//cw2[i] = cw[i];
			mCalculator->assign(cw2[ii], cw[ii]);
		}

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		if (nbPlayersInParti > 1) {
			for (longUInt x = 0; x < nbPlayersInParti; ++x) {
				winternal[x] = mGame.getWeights()[mGame.getPrecoalitions()[i][x]];
				mCalculator->assign_zero(banzhafsInternal[x]);
			}

			generalizedBackwardCountingPerWeight(cw2, winternal, nbPlayersInParti);

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				coalitionsContainingPlayerFromAbove(cwi, cw2, winternal[ii]);

				longUInt min = std::min(quota + winternal[ii] - 1, totalWeight);
				for (longUInt iii = quota; iii <= min; iii++) {
					mCalculator->plusEqual(banzhafsInternal[ii], cwi[iii]);
				}

				bigFloat InternalMultiplier;
				{ // InternalMultiplier = 2^{nbPlayerInParti - 1}
					mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, nbPlayersInParti - 1);
					InternalMultiplier = 1 / bigFloat(mBigTmp);
				}
				mCalculator->to_bigInt(&mBigTmp, banzhafsInternal[ii]);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * mBigTmp;
			}
		} else {
			//get sum of vector
			mCalculator->assign_zero(mTmp);
			longUInt min = std::min(quota + mPartW[i] - 1, totalWeight);
			for (longUInt ii = quota; ii <= min; ++ii) {
				mCalculator->plusEqual(mTmp, cw[ii]);
			}
			bigInt banzhafs_external;
			mCalculator->to_bigInt(&banzhafs_external, mTmp);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}

	delete[] winternal;
	mCalculator->free_largeNumberArray(banzhafsInternal);
	delete[] banzhafsInternal;

	mCalculator->free_largeNumberArray(cw2.getArrayPointer());
	mCalculator->free_largeNumberArray(cwi.getArrayPointer());
	mCalculator->free_largeNumberArray(c.getArrayPointer());
	mCalculator->free_largeNumberArray(cw.getArrayPointer());

	return solution;
}

std::string epic::index::BanzhafOwen::getFullName() {
	return "BanzhafOwen";
}

epic::longUInt epic::index::BanzhafOwen::getMemoryRequirement() {
	bigInt memory = (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 4; // c, cw, cw2, cwi
	memory += mMaxPartSize * mCalculator->getLargeNumberSize();											   // banzhafInternal
	memory += mMaxPartSize * c_sizeof_longUInt;															   // winternal
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}