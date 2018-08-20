import lgp.core.environment.config.Configuration
import lgp.core.evolution.Description
import lgp.lib.BaseProblem
import lgp.lib.BaseProblemParameters

fun main(args: Array<String>) {
    val parameters = BaseProblemParameters(
        name = "My Problem",
        description = Description(
            "A simple example problem definition"
        ),
        // A problem will generally need custom configuration
        config = Configuration()
    )

    val problem = BaseProblem(parameters)

    println(problem.name)
    println(problem.description)
}

