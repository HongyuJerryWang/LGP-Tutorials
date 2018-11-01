import lgp.core.evolution.fitness.*

object CustomFitnessFunctions {

    @JvmStatic
    val match: FitnessFunction<Double> = object : FitnessFunction<Double>() {

        override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
            return cases.zip(outputs).filter { (case, actual) ->
                !actual.equals(case.target)
            }.count().toDouble()
        }
    }

}
