# LGP Programming Tutorial 2: Custom Operators

> *Making custom fitness functions for a specific problem

## Overview

We'll perform LGP to simulate a binary adder, which takes the bits of two binary numbers as its input, and produces the bits of their sum as its output.

We will need three program files:
Main.kt to run the program
CustomFitnessFunctionsExperiment.kt to define the problem, i.e. how LGP should be setup and run
CustomFitnessFunctions.kt to define the custom fitness functions

## Custom Fitness Functions

We can use the fitness functions included in the API, but they don't diffrentiate a more significant bit from a less significant bit and can only treat each bit individually and uniformly, which is mathematically incorrect (e.g. 1100 is further from 0000 than 0011 from 0000 because the different bits are more significant in the former case. Also 11 is further from 00 than 10 from 01 because in the former case the differences in the bits add up while in the latter they cancel each other out. But the default fitness functions will misjudge these cases to have the same fitness).

To calculate the difference between the actual and the expected values, we need to convert the bits back into numbers and then apply the fitness function we'd like to perform on them.

We'll implement four common fitness functions
1. Mean absolute error
2. Sum of squared errors
3. Mean squared error
4. Root mean squared error

## Running

Please download **Main.kt**, **CustomFitnessFunctions.kt**, **CustomFitnessFunctionsExperiment.kt**, **configuration1.kt**, **configuration2.kt**, **configuration3.kt**, **dataset1.csv** (1-bit adder), **dataset2.csv** (2-bit adder) and **dataset3.csv** (3-bit adder) from this repository into a sub-directory of **LGP-Tutorials**, e.g. **ProgrammingTutorial3CustomFitnessFunctions**.

In **ProgrammingTutorial3CustomFitnessFunctions**, compile

```
kotlinc -cp ../LGP.jar:../argparser.jar:../xenocom.jar -no-stdlib *.kt
```

Run

```
kotlin -cp ../LGP.jar:../argparser.jar:../xenocom.jar:. Main configuration1.json dataset1.csv
```

```
kotlin -cp ../LGP.jar:../argparser.jar:../xenocom.jar:. Main configuration2.json dataset2.csv
```

```
kotlin -cp ../LGP.jar:../argparser.jar:../xenocom.jar:. Main configuration3.json dataset3.csv
```

## Analysis

### Main.kt

The file is very similar to the one from our last tutorial, the only significant difference being

```
private val verboseBinary = object : FitnessFunction<Double>() {

    override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
        val difference = cases.zip(outputs).map { (case, actual) ->
            val expected = case.target
            Math.abs(actual.zip(expected).map { (singleActual, singleExpected) ->
                singleActual - singleExpected
            }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
                moreSignificantBit * 2.0 + lessSignificantBit
            })
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
```

This is the function that specifies how our testcase.txt file is produced. It is very similar to a fitness function we'd use in our LGP training (being a subclass of FitnessFunction after all), except that instead of returning a calculated fitness, it writes the cases to a file while iterating through them. It also calculates the average difference between each actual case and its expected value.

Let's take a closer look at how the *difference* value is calculated in this function.

The *outputs* is a list of outputs, and in an earlier tutorial we mentioned that one output is a vector of values, e.g. here `[`1.0, 0.0, 1.0`]` represents 101 in binary and 5 in decimal.

```
outputs: List<List<Double>>
```

The *cases* is a list of FitnessCase, each FitnessCase represents one sample, which consists of a set of *features* and a *target*, and the *target* here is the expected output, i.e. a vector of values.

```
cases: List<FitnessCase<Double>>
```

So we zip the *cases* with the *outputs* to put each case and its actual output side by side.

```
cases.zip(outputs)
```

We extract the expected output from the case.

```
val expected = case.target
```

We zip the actual output and the expected output to put each actual bit by its expected bit.

```
actual.zip(expected)
```

We calculate the difference in each bit. The differences are signed.

```
map { (singleActual, singleExpected) ->
    singleActual - singleExpected
}
```

We calculate the total difference, i.e. the difference between the two numbers represented by the two vectors of bits.

```
reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
    moreSignificantBit * 2.0 + lessSignificantBit
}
```

We get the absolute value of the difference.

```
Math.abs
```

The procedure is applied to each pair of expected and actual values, to get the difference of each pair.

```
cases.zip(outputs).map { (case, actual) ->
    val expected = case.target
    Math.abs(actual.zip(expected).map { (singleActual, singleExpected) ->
        singleActual - singleExpected
    }.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
        moreSignificantBit * 2.0 + lessSignificantBit
    })
}
```

The average difference is calculated.

```
average()
```

So this is how the *difference* value is calculated, our custom fitness functions for binary representations follow the same principle.

### CustomFitnessFunctionsExperiment.kt

This file is very similar to the one from our last tutorial, too, the only significant difference being

```
override val fitnessFunction = CustomFitnessFunctions.binaryMSE
```

We use the Mean Squared Error from our custom fitness functions.

### CustomFitnessFunctions.kt

We define our custom fitness functions here, they implement the same interface as our verboseBinary in **Main.kt**, and the *fitness* method takes the *outputs* and the *cases* as arguments. These fitness functions follow the same idea as above. One thing to notice is that the calculated value may not be finite, we need to test against that, and if it is not finite, we need to return a default value.

```
const val UNDEFINED_FITNESS: Double = 10e9
```

```
return when {
    fitness.isFinite() -> fitness
    else               -> UNDEFINED_FITNESS
}
```

Please note the bulks of the functions are the same. The numeric difference between an expected value and its actual value is calculated with

```
actual.zip(expected).map { (actualValue, expectedValue) ->
    actualValue - expectedValue
}.reduce { moreSignificantBit: Double, lessSignificantBit: Double ->
    moreSignificantBit * 2.0 + lessSignificantBit
}
```

The procedure is applied to every pair of expected and actual cases with

```
val fitness = cases.zip(outputs).map { (case, actual) ->
```

Let's walkthrough the four fitness functions we have here.

**binaryMAE** gets the absolute error of each case and then calculates the mean.

```
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
```

**binarySSE** gets the squared error of each case and then calculates the sum.

```
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
```

**binaryMSE** gets the squared error of each case and then calculates the mean.

```
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
```

**binaryRMSE** gets the squared error of each case and then calculates the square root of the mean.

```
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
```

## Results

Please take a look at the files generated by the program: **results1.csv**, **results2.csv**, **results3.csv**, **testcases1.txt**, **testcases2.txt**, **testcases3.txt**, **custom_fitness_functions_experiment_1.c**, **custom_fitness_functions_experiment_2.c** and **custom_fitness_functions_experiment_3.c**. Feel free to compare your results to the results in this repository, and compile and run the C program.