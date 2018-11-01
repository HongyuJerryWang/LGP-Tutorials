import lgp.core.evolution.fitness.FitnessFunction
import lgp.core.evolution.fitness.FitnessCase
import lgp.core.evolution.model.EvolutionResult
import lgp.core.environment.dataset.Dataset

import lgp.lib.BaseProgram
import lgp.lib.ExtendedProgramTranslator
import lgp.lib.BaseProgramSimplifier

import com.xenomachina.argparser.*

import java.io.File

class Arguments(parser: ArgParser) {
    val configurationFilename by parser.positional(
        name = "CONFIGURATION_FILENAME",
        help = "A file that contains configuration information for the system."
    )

    val datasetFilename by parser.positional(
        name = "DATASET_FILENAME",
        help = "A CSV file that contains a data set to run the experiment on."
    )

    val evolutionaryAlgorithm by parser.positional(
        name = "EVOLUTIONARY_ALGORITHM",
        help = "The evolutionary algorithm used by LGP."
    ).default("SteadyState").addValidator {
        val usingDefault = value == "SteadyState"
        println("Using ${if (usingDefault) "default" else "specified"} evolutionary algorithm ($value)...")
    }

    val numIslands by parser.positional(
        name = "NUM_ISLANDS",
        help = "The number of islands the population should be split into, for IslandMigration."
    ).default("4")

    val migrationInterval by parser.positional(
        name = "MIGRATION_INTERVAL",
        help = "The interval that solutions are migrated with, for IslandMigration."
    ).default("10")

    val migrationSize by parser.positional(
        name = "MIGRATION_SIZE",
        help = "How many solutions migrate between islands at each interval, for IslandMigration."
    ).default("10")

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
        private lateinit var evolutionaryAlgorithm: String
        private lateinit var numIslands: String
        private lateinit var migrationInterval: String
        private lateinit var migrationSize: String

        private val verboseMSE = object : FitnessFunction<Double>() {

            override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
                val difference = cases.zip(outputs).map { (case, actual) ->
                    val expected = case.target
                    actual.zip(expected).map { (singleActual, singleExpected) -> Math.abs(singleActual - singleExpected) }.sum()
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
            this.evolutionaryAlgorithm = arguments.evolutionaryAlgorithm
            this.numIslands = arguments.numIslands
            this.migrationInterval = arguments.migrationInterval
            this.migrationSize = arguments.migrationSize
        }

        private fun runExperiment() {
            val problem = CustomOperatorsExperiment(
                configurationFilename = this.configurationFilename,
                datasetFilename = this.datasetFilename,
                evolutionaryAlgorithm = this.evolutionaryAlgorithm,
                numIslands = this.numIslands.toInt(),
                migrationInterval = this.migrationInterval.toInt(),
                migrationSize = this.migrationSize.toInt()
            )
            val simplifier = BaseProgramSimplifier<Double>()

            problem.initialiseEnvironment()
            problem.initialiseModel()

            println("Running...")

            val solution = problem.solve()
            val dataset = solution.dataset

            println("Results:")

            val printResults : (Int, EvolutionResult<Double>) -> Unit = { run, res ->
                println("Run ${run + 1} (best fitness = ${res.best.fitness})")
                println(simplifier.simplify(res.best as BaseProgram<Double>))

                println("\nStats (last generation only):\n")

                for ((k, v) in res.statistics.last().data) {
                    println("$k = $v")
                }
                println("")

                this.printPassedTestCases(res.best as BaseProgram<Double>, dataset)
            }

            solution.result.evaluations.forEachIndexed(printResults)

            val bestPrograms = solution.result.evaluations.map { res: EvolutionResult<Double> -> res.best as BaseProgram<Double> }
            val best = bestPrograms.minBy(BaseProgram<Double>::fitness)
            File("custom_operators_experiment.c").bufferedWriter().use { out ->
                val translated = ExtendedProgramTranslator(true, solution.inputVectorization, solution.outputVectorization).translate(best as BaseProgram<Double>)
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
                program.outputRegisterIndices.map { index: Int -> program.registers[index] }
            }

            // Execute fitness function and discard the result
            this.verboseMSE(outputs, fitnessCases)
        }
    }
}
