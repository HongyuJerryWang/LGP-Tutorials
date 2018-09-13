import lgp.core.environment.*
import lgp.core.environment.config.JsonConfigurationLoader
import lgp.core.environment.constants.DoubleConstantLoader
import lgp.core.environment.dataset.*
import lgp.core.environment.operations.DefaultOperationLoader
import lgp.core.evolution.*
import lgp.core.evolution.fitness.FitnessFunctions
import lgp.core.evolution.fitness.SingleOutputFitnessContext
import lgp.core.evolution.population.*
import lgp.lib.BaseInstructionGenerator
import lgp.lib.BaseProgramGenerator
import java.io.BufferedReader
import java.io.FileReader

/**
 * Defines what a solution for this problem looks like.
 */
class TimeSeriesExperimentSolution(
    // The problems description
    override val problem: String,
    // The results as given by a Trainer instance
    val result: TrainingResult<Double>,
    // The training dataset
    val dataset: Dataset<Double>
) : Solution<Double>

/**
 * Defines the problem.
 *
 * @param configurationFilename The name of a JSON file where configuration information can be found.
 * @param datasetFilename The name of a CSV file where a data set for the problem can be found.
 */
class TimeSeriesExperiment(
    private val configurationFilename: String,
    private val datasetFilename: String,
    private val windowSize: Int,
    private val evolutionaryAlgorithm: String,
    private val numIslands: Int,
    private val migrationInterval: Int,
    private val migrationSize: Int
) : Problem<Double>() {

    // 1. Give the problem a name and simple description.
    override val name = "Time Series Experiment"
    override val description = Description(
        "An example time series problem definition for the LGP tutorial."
    )

    // 2. Define the necessary dependencies to build a problem.
    // Load configuration from a JSON file.
    override val configLoader = JsonConfigurationLoader(filename = this.configurationFilename)
    // To prevent reloading configuration in this module.
    private val configuration = this.configLoader.load()
    // Load constants from the configuration as double values.
    override val constantLoader = DoubleConstantLoader(constants = this.configuration.constants)
    // Load operations from the configuration (operations are resolved using their class name).
    override val operationLoader = DefaultOperationLoader<Double>(operationNames = this.configuration.operations)
    // Set the default value of any registers to 1.0.
    override val defaultValueProvider = DefaultValueProviders.constantValueProvider(1.0)
    // Use the mean-squared error fitness function.
    override val fitnessFunction = FitnessFunctions.MSE
    // Define the modules to be used for the core evolutionary operations.
    override val registeredModules = ModuleContainer<Double>(modules = mutableMapOf(
            // Generate instructions using the built-in instruction generator.
            CoreModuleType.InstructionGenerator to { environment ->
                BaseInstructionGenerator(environment)
            },
            // Generate programs using the built-in programs generator.
            CoreModuleType.ProgramGenerator to { environment ->
                BaseProgramGenerator(
                    environment,
                    sentinelTrueValue = 1.0, // Determines the value that represents a boolean "true".
                    outputRegisterIndex = 0  // Which register should be read for a programs output.
                )
            },
            // Perform selection using the built-in tournament selection.
            CoreModuleType.SelectionOperator to { environment ->
                TournamentSelection(environment, tournamentSize = 2)
            },
            // Combine individuals using the linear crossover operator.
            CoreModuleType.RecombinationOperator to { environment ->
                LinearCrossover(
                    environment,
                    maximumSegmentLength = 6,
                    maximumCrossoverDistance = 5,
                    maximumSegmentLengthDifference = 3
                )
            },
            // Use the built-in macro-mutation operator.
            CoreModuleType.MacroMutationOperator to { environment ->
                MacroMutationOperator(
                    environment,
                    insertionRate = 0.67,
                    deletionRate = 0.33
                )
            },
            // Use the built-in micro-mutation operator.
            CoreModuleType.MicroMutationOperator to { environment ->
                MicroMutationOperator(
                    environment,
                    registerMutationRate = 0.5,
                    operatorMutationRate = 0.5,
                    constantMutationFunc = ConstantMutationFunctions.randomGaussianNoise(this.environment)
                )
            },
            // Use the single-output fitness context -- meaning that program fitness will be evaluated
            // using a single program output and the fitness function specified earlier in this definition.
            CoreModuleType.FitnessContext to { environment ->
                SingleOutputFitnessContext(environment)
            }
        )
    )

    // 3. Describe how to initialise the problem's environment.
    override fun initialiseEnvironment() {
        this.environment = Environment(
            this.configLoader,
            this.constantLoader,
            this.operationLoader,
            this.defaultValueProvider,
            this.fitnessFunction,
            // Collect results and output them to the file "result.csv".
            ResultAggregators.BufferedResultAggregator(
                ResultOutputProviders.CsvResultOutputProvider(
                    filename = "results.csv"
                )
            )
        )

        this.environment.registerModules(this.registeredModules)
    }

    // 4. Specify which evolution model should be used to solve the problem.
    override fun initialiseModel() {
        if (this.evolutionaryAlgorithm == "IslandMigration")
            this.model = Models.IslandMigration(
                this.environment,
                Models.IslandMigration.IslandMigrationOptions(
                    numIslands = this.numIslands,
                    migrationInterval = this.migrationInterval,
                    migrationSize = this.migrationSize
                )
            )
        else if (this.evolutionaryAlgorithm == "MasterSlave")
            this.model = Models.MasterSlave(this.environment)
        else
            this.model = Models.SteadyState(this.environment)
    }

    // 5. Describe the steps required to solve the problem using the definition given above.
    override fun solve(): TimeSeriesExperimentSolution {

        // Load the data set
        val csvLoader = TimeSeriesExperimentCsvDatasetLoader(
            reader = BufferedReader(FileReader(this.datasetFilename)),
            windowSize = this.windowSize
        )

        val dataset = csvLoader.load()

        // Print details about the data set and configuration before beginning the evolutionary process.
        println("\nDataset details:")
        println("numFeatures = ${dataset.numFeatures()}, numSamples = ${dataset.numSamples()}")

        println()

        println(this.configuration)

        // Train using the built-in sequential trainer.
        val trainer = Trainers.SequentialTrainer(
            this.environment,
            this.model,
            runs = this.configuration.numberOfRuns
        )

        // Return a solution to the problem.
        return TimeSeriesExperimentSolution(
            problem = this.name,
            result = trainer.train(dataset),
            dataset = dataset
        )
    }
}
