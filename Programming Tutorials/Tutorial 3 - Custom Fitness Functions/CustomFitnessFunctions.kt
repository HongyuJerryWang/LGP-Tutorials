import lgp.core.evolution.fitness.*

object CustomFitnessFunctions {

    /**
     * Large constant value for fitness in the cases when either a program is
     * un-evaluated or the fitness computation overflows.
     */
    const val UNDEFINED_FITNESS: Double = 10e9

    /**
     * Mean absolute error fitness function.
     *
     * Simply sums up the absolute differences between the outputs and takes the mean.
     */
    @JvmStatic
    val binaryMAE: FitnessFunction<Double> = object : FitnessFunction<Double>() {

        override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
            val fitness = cases.zip(outputs).map { (case, actual) ->
                val expected = case.target

                Math.abs(actual.zip(expected).map { (actualValue, expectedValue) ->
                    actualValue - expectedValue
                }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
                    moreSignificantBit * 2.0 + lessSignificantBit
                })
            }.sum()

            return when {
                fitness.isFinite() -> fitness / outputs.size.toDouble()
                else               -> UNDEFINED_FITNESS
            }
        }
    }

    /**
     * Sum-of-squared errors fitness function.
     *
     * Simply calculates the sum of the squared error between the actual and expected outputs.
     */
    @JvmStatic
    val binarySSE: FitnessFunction<Double> = object : FitnessFunction<Double>() {

        override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
            val fitness = cases.zip(outputs).map { (case, actual) ->
                val expected = case.target

                Math.pow(actual.zip(expected).map { (actualValue, expectedValue) ->
                    actualValue - expectedValue
                }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
                    moreSignificantBit * 2.0 + lessSignificantBit
                }, 2.0)
            }.sum()

            return when {
                fitness.isFinite() -> fitness
                else               -> UNDEFINED_FITNESS
            }
        }
    }

    /**
     * Mean-Squared Error fitness function for programs that operate on doubles.
     *
     * Calculates the sum of squared errors and then takes the mean.
     */
    @JvmStatic
    val binaryMSE: FitnessFunction<Double> = object : FitnessFunction<Double>() {

        override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
            val fitness = cases.zip(outputs).map { (case, actual) ->
                val expected = case.target

                Math.pow(actual.zip(expected).map { (actualValue, expectedValue) ->
                    actualValue - expectedValue
                }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
                    moreSignificantBit * 2.0 + lessSignificantBit
                }, 2.0)
            }.sum()

            return when {
                fitness.isFinite() -> ((1.0 / cases.size.toDouble()) * fitness)
                else               -> UNDEFINED_FITNESS
            }
        }
    }

    /**
     * Root Mean-Squared Error fitness function for programs that operate on doubles.
     *
     * Essentially operates by computing the MSE, then taking the square-root of the result.
     */
    @JvmStatic
    val binaryRMSE: FitnessFunction<Double> = object : FitnessFunction<Double>() {

        override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
            val fitness = cases.zip(outputs).map { (case, actual) ->
                val expected = case.target

                Math.pow(actual.zip(expected).map { (actualValue, expectedValue) ->
                    actualValue - expectedValue
                }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
                    moreSignificantBit * 2.0 + lessSignificantBit
                }, 2.0)
            }.sum()

            val mse = ((1.0 / cases.size.toDouble()) * fitness)

            return when {
                fitness.isFinite() -> Math.sqrt(mse)
                else               -> UNDEFINED_FITNESS
            }
        }
    }

}
