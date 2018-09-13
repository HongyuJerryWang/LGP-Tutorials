import lgp.core.evolution.fitness.FitnessFunction
import lgp.core.evolution.fitness.FitnessCase
import lgp.core.environment.dataset.Dataset

import lgp.lib.BaseProgram
import lgp.lib.BaseProgramTranslator
import lgp.lib.BaseProgramSimplifier

import com.xenomachina.argparser.*

import java.io.File

class Arguments(parser: ArgParser) {

    val configurationFilename by parser.positional(
        name = "CONFIGURATION_FILENAME",
        help = "A file that contains configuration information for the system.")

    val datasetFilename by parser.positional(
        name = "DATASET_FILENAME",
        help = "A CSV file that contains a data set to run the experiment on.")

    val windowSize by parser.positional(
        name = "WINDOW_SIZE",
	help = "The size of the window for the time series.").default("3")

    val evolutionaryAlgorithm by parser.positional(
        name = "EVOLUTIONARY_ALGORITHM",
	help = "The evolutionary algorithm used by LGP.").default("SteadyState")

    val numIslands by parser.positional(
        name = "NUM_ISLANDS",
	help = "The number of islands the population should be split into, for IslandMigration.").default("4")

    val migrationInterval by parser.positional(
        name = "MIGRATION_INTERVAL",
	help = "The interval that solutions are migrated with, for IslandMigration.").default("10")

    val migrationSize by parser.positional(
        name = "MIGRATION_SIZE",
	help = "How many solutions migrate between islands at each interval, for IslandMigration.").default("10")

    init {
        try {
	    parser.force()
        } catch (ex: SystemExitException) {
            println("Invalid arguments: ${ex.message}")
            System.exit(ex.returnCode)
        }
    }

}

class Main {
    companion object Main {

        private lateinit var configurationFilename: String
        private lateinit var datasetFilename: String
        private lateinit var windowSize: String
        private lateinit var evolutionaryAlgorithm: String
        private lateinit var numIslands: String
        private lateinit var migrationInterval: String
        private lateinit var migrationSize: String

        private val verboseMSE = object : FitnessFunction<Double>() {

            override fun fitness(outputs: List<Double>, cases: List<FitnessCase<Double>>): Double {
                val difference = cases.zip(outputs).map { (case, actual) ->
                    val expected = case.target
                    Math.abs(actual - expected)
                }.average()
                File("testcases.txt").bufferedWriter().use { out ->
                    out.write("The average difference is ${difference}.\n")
                    cases.zip(outputs).map { (expected, actual) ->
                        out.write("\tExpected: ${expected.target}, Actual: $actual\n")
                    }
                }
                return 0.0
            }
        }

        @JvmStatic
        fun main(args: Array<String>) {
            this.processArguments(args)
            this.runExperiment()
        }

        private fun processArguments(args: Array<String>) {
            val arguments = Arguments(ArgParser(args))

            this.configurationFilename = arguments.configurationFilename
            this.datasetFilename = arguments.datasetFilename
            this.windowSize = arguments.windowSize
            this.evolutionaryAlgorithm = arguments.evolutionaryAlgorithm
            this.numIslands = arguments.numIslands
            this.migrationInterval = arguments.migrationInterval
            this.migrationSize = arguments.migrationSize
        }

        private fun runExperiment() {
            val problem = TimeSeriesExperiment(
                configurationFilename = this.configurationFilename,
                datasetFilename = this.datasetFilename,
                windowSize = this.windowSize.toInt(),
                evolutionaryAlgorithm = this.evolutionaryAlgorithm,
                numIslands = this.numIslands.toInt(),
                migrationInterval = this.migrationInterval.toInt(),
                migrationSize = this.migrationSize.toInt()
            )
            val translator = BaseProgramTranslator<Double>(includeMainFunction = true)
            val simplifier = BaseProgramSimplifier<Double>()

            problem.initialiseEnvironment()
            problem.initialiseModel()

            println("Running...")

            val solution = problem.solve()
            val dataset = solution.dataset

            println("Results:")

            solution.result.evaluations.forEachIndexed { run, res ->
                println("Run ${run + 1} (best fitness = ${res.best.fitness})")
                println(simplifier.simplify(res.best as BaseProgram<Double>))

                println("\nStats (last generation only):\n")

                for ((k, v) in res.statistics.last().data) {
                    println("$k = $v")
                }
                println("")
            }

            val bestPrograms = solution.result.evaluations.map { res -> res.best as BaseProgram<Double> }
            val best = bestPrograms.minBy(BaseProgram<Double>::fitness)
            this.printPassedTestCases(best as BaseProgram<Double>, dataset)
            File("time_series_experiment.c").bufferedWriter().use { out ->
                val translated = translator.translate(best)
                out.write(translated)
            }
        }

        private fun printPassedTestCases(program: BaseProgram<Double>, dataset: Dataset<Double>) {
            // Setup program for testing.
            program.findEffectiveProgram()
            val fitnessCases = dataset.inputs.zip(dataset.outputs).map { (features, target) ->
                FitnessCase(features, target)
            }

            // Collect the results of the program for each fitness case.
            val outputs = fitnessCases.map { case ->
                // Make sure the registers are in a default state
                program.registers.reset()

                // Load the case
                program.registers.writeInstance(case.features)

                // Run the program...
                program.execute()

                // ... and gather a result from the programs specified output register.
                program.registers[program.outputRegisterIndex]
            }

            // Execute fitness function and discard the result
            this.verboseMSE(outputs, fitnessCases)
        }
    }
}
