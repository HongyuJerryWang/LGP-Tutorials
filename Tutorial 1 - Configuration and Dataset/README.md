# LGP Tutorial 1: Configuration and Dataset

> *Running LGP on your own configuration and dataset without doing any programming

## Setup

This tutorial is based on the [tutorial](https://github.com/JedS6391/LGP-Tutorial/tree/develop/src/main/kotlin/lgp/tutorial/linearRegression) made by Jed Simson. Using the program from the tutorial, we can perform LGP without any programming.

Please setup by building the JAR file:

```
# Clone this repository
git clone https://github.com/JedS6391/LGP-Tutorial.git

# Enter the repository directory
cd LGP-Tutorial

# Compile the JAR using the gradle wrapper. This command will take a long time
# the first time it is run as it gathers dependencies.
./gradlew buildJar

# Check the JAR was built
ls build/libs
```

Assuming the steps above are performed, we should be in the directory named "LGP-Tutorial". We will make our own configuration and dataset files, as well as run LGP on them, in this directory.

## configuration.json

The configuration determines the environment in which your LGP runs. To better understand how LGP works, please refer to [this page](http://lgp-tutorial.jedsimson.co.nz/lgp/lgp.html) by Jed Simson.

Here we try to make our own json file for our configuration.

Below is a skeleton file.

```
{
    "initialMinimumProgramLength": ,
    "initialMaximumProgramLength": ,
    "operations": [
        
    ],
    "constants": [],
    "populationSize": ,
    "generations": ,
    "numOffspring": ,
    "numFeatures": 
}
```

All the options and their brief explanations are shown below
Please note that you don't have to specify them all while using Kotlin instead of Java, as there are default values for them.
You can also find the explanation and the code [here](https://github.com/JedS6391/LGP/blob/master/src/main/kotlin/lgp/core/environment/config/Configuration.kt)

Option | Explanation
--- | ---
initialMinimumProgramLength | The minimum length of a program generated during population initialisation.
initialMaximumProgramLength | The maximum length of a program generated during population initialisation.
minimumProgramLength | The lower bound for program length during the evolution process.
maximumProgramLength | The upper bound for program length during the evolution process.
operations | A collection of fully-qualified class names specifying the classes of [lgp.core.evolution.instructions.Operation]s to be loaded into an [lgp.core.environment.Environment]. Please refer to the [implementation](https://github.com/JedS6391/LGP/tree/master/src/main/kotlin/lgp/lib/operations) for available operations. In a later tutorial we'll discuss how to make our own operations.
constantsRate | The probability with which instructions in an LGP program will contain a constant value.
constants | Any values to be used as constants in an LGP program.
numCalculationRegisters | How many additional calculation registers should be provided to an LGP program.
populationSize | How many individuals should be generated in the initial population.
numFeatures | The number of features in the data set (i.e. the number of input registers that should be made available).
crossoverRate | The frequency with which crossover should occur.
microMutationRate | The frequency with which micro-mutations should occur.
macroMutationRate | The frequency with which macro-mutations should occur.
generations | Number of generations to evolve.
numOffspring | Number of individuals that should be taken from the population in each generation.
branchInitialisationRate | How often branches should be included in evolved programs.
stoppingCriterion | Determines the threshold for stopping the evolutionary process. When a solution with a fitness of [stoppingCriterion] is found, the search will stop. By default, we don't stop until the fitness is minimised perfectly.
numberOfRuns | Provides the ability to pass in the number of runs as a configuration parameter instead of a hard-coded value in the problem definition. The parameter is not used by default anywhere in the system, but consumers can choose to use it.

Below is a sample file that we'll use for this tutorial.

```
{
    "initialMinimumProgramLength": 20,
    "initialMaximumProgramLength": 40,
    "operations": [
        "lgp.lib.operations.Addition",
	"lgp.lib.operations.Subtraction",
	"lgp.lib.operations.Multiplication",
	"lgp.lib.operations.IfGreater",
	"lgp.lib.operations.IfLessThanOrEqualTo"
    ],
    "constants": [0.0, 1.0],
    "populationSize": 500,
    "generations": 1000,
    "numOffspring": 10,
    "numFeatures": 2,
    "branchInitialisationRate": 0.1
}
```

Some times these parameters require quite a bit tweaking for the program to get a good result, please adjust these parameters in the context of your dataset.

So we save our configuration file as "configuration1.json". If we want another configuration set, we can make another configuration file (e.g. "configuration2.json"), and we'll decide which to use in our command line argument.

## dataset.csv

The dataset is the data that you'd like to train your LGP with. Each sample in the dataset contains one or more features and a label.

Suppose we have some data representing the non-linear function that y equals x_0 to the power of x_1. Please note, the number of features (x_0 and x_1) conforms with "numFeatures" (2) in the configuration.

Please note, the program we use treat the features and the label as double, so please use double values for this program. In a later tutorial we'll discuss how to make your own LGP that suits your data best.

x_0 | x_1 | y
--- | --- | ---
1.0 | 1.0 | 1.0
1.0 | 2.0 | 1.0
1.0 | 3.0 | 1.0
2.0 | 1.0 | 2.0
2.0 | 2.0 | 4.0
2.0 | 3.0 | 8.0
3.0 | 1.0 | 3.0
3.0 | 2.0 | 9.0
3.0 | 3.0 | 27.0

we can store these data points in a CSV file.

```
x_0,x_1,y
1.0,1.0,1.0
1.0,2.0,1.0
1.0,3.0,1.0
2.0,1.0,2.0
2.0,2.0,4.0
2.0,3.0,8.0
3.0,1.0,3.0
3.0,2.0,9.0
3.0,3.0,27.0
```

So we save our dataset file as "dataset1.csv". Just like the configuration, if we want another dataset, we can make another dataset file (e.g. "dataset2.json"), and we'll decide which to use in our command line argument.

## Run LGP on our own configuration and dataset

We can run the program using this format.

```
kotlin -cp build/libs/LGP-Tutorial.jar: lgp.tutorial.linearRegression.Main CONFIGURATION_FILENAME DATASET_FILENAME
```

So we run the program on the files we just created.

```
kotlin -cp build/libs/LGP-Tutorial.jar: lgp.tutorial.linearRegression.Main configuration1.json dataset1.csv
```

Now we have tested our own configuration and dataset, I got these results:
Please note that due to the nature of LGP, the result is likely to differ each time.

```
Results:
Run 1 (best fitness = 18.88888888888889)
r[4] = r[5] + 1.0
if (1.0 > r[2])
if (0.0 > r[6])
if (1.0 < f1)
r[4] = 0.0 * f0
r[11] = f0 * f1
r[0] = r[4] * r[11]


Stats (last generation only):

generation = 999
bestFitness = 18.88888888888889
meanFitness = 95.58888888888902
standardDeviationFitness = 34.531889359200235
meanProgramLength = 55.712
meanEffectiveProgramLength = 5.446

0 of 9 test cases passed.
        Expected: 1.0, Actual: 2.0
  	Expected: 1.0, Actual: 4.0
  	Expected: 1.0, Actual: 6.0
	Expected: 2.0, Actual: 4.0
  	Expected: 4.0, Actual: 8.0
  	Expected: 8.0, Actual: 12.0
  	Expected: 3.0, Actual: 6.0
  	Expected: 9.0, Actual: 12.0
  	Expected: 27.0, Actual: 18.0
```

Hmm, maybe not the best we can expect, but we haven't adjusted our hyperparameters for the data yet. We'll discuss how to work with the hyperparameters to make LGP work more efficiently in a later tutorial.

Two files are generated by the program, "linear_regression_experiment.c" and "result.csv".

"linear_regression_experiment.c" is the best solution found by LGP, now as a C program, which takes the feature(s) as command line arguments, and will print its prediction of the label for you.

"results.csv" is the statistics collected in the running process, we can analyse it to get some ideas on how to tweak the hyperparameters and change our program to make it perform better. We'll discuss more about that in a later tutorial.

For now, we have successfully run LGP using our own configuration and dataset without doing any programming.
