# LGP Programming Tutorial 3: Custom Fitness Functions

> *Making custom fitness functions for a specific problem

## Overview

We'll perform LGP to simulate a [full adder](https://en.wikipedia.org/wiki/Adder_(electronics)#Full_adder), which takes two input bits (a and b) and one carry-over bit (c_in) as its input, and produces one carry-over bit (c_out) and one sum bit (s) as its output.

The truth table:

a | b | c_in | c_out | s
---|---|---|---|---
0 | 0 | 0 | 0 | 0
0 | 0 | 1 | 0 | 1
0 | 1 | 0 | 0 | 1
0 | 1 | 1 | 1 | 0
1 | 0 | 0 | 0 | 1
1 | 0 | 1 | 1 | 0
1 | 1 | 0 | 1 | 0
1 | 1 | 1 | 1 | 1

We will need three program files:
1. Main.kt to run the program
2. CustomFitnessFunctionsExperiment.kt to define the problem, i.e. how LGP should be setup and run
3. CustomFitnessFunctions.kt to define the custom fitness functions

## Custom Fitness Functions

It is important for an electric circuit to get the output totally right. For example, if the expected output is 1111, the actual output is only good if it's exactly 1111, while 1110 and 0000 are equally bad. So the more test cases the program can get completely right, the better it simulates the circuit we want. We want to penalise each imperfect output equally, no matter how close or how far it is from the expected output. However, there is no fitness function implementing this idea in our LGP API and the closest we can get from the API is counting mismatching bits, but not mismatching outputs. So we'll have to implement our own custom fitness function.

## Running

Please download **Main.kt**, **CustomFitnessFunctions.kt**, **CustomFitnessFunctionsExperiment.kt**, **configuration.kt** and **dataset.csv** from this repository into a sub-directory of **LGP-Tutorials**, e.g. **Tutorial5**.

In **Tutorial5**, compile:

```
export CLASSPATH=../LGP.jar:../argparser.jar:../xenocom.jar
kotlinc -cp $CLASSPATH -no-stdlib *.kt
```

Run:

```
kotlin -cp $CLASSPATH:. Main configuration.json dataset.csv
```

## Analysis

### Main.kt

The file is very similar to the one from our last tutorial, the only significant difference being:

```
private val verboseMatch = object : FitnessFunction<Double>() {

    override fun fitness(outputs: List<List<Double>>, cases: List<FitnessCase<Double>>): Double {
        val numMatches = cases.zip(outputs).filter { (case, actual) ->
            actual.equals(case.target)
        }.count()

        File("testcases.txt").bufferedWriter().use { out ->

            out.write("$numMatches matches out of ${ outputs.count() } cases.\n")

            cases.zip(outputs).map { (expected, actual) ->
                out.write("\tExpected: ${expected.target}, Actual: $actual\n")
            }

        }

        return 0.0
    }
}
```

This is the function that specifies how our testcase.txt file is produced. It is very similar to a fitness function we'd use in our LGP training (being a subclass of FitnessFunction after all), except that instead of returning a calculated fitness, it writes the cases to a file while iterating through them. It also counts how many cases the program gets right.

Let's take a closer look at how the *numMatches* value is calculated in this function.

The *outputs* is a list of outputs, and in an earlier tutorial we mentioned that one output is a vector of values, e.g. here in the context of a full adder `[`1.0, 0.0`]` represents 10 in binary and 2 in decimal:

```
outputs: List<List<Double>>
```

The *cases* is a list of FitnessCase, each FitnessCase represents one sample, which consists of a set of *features* and a *target*, and the *target* here is the expected output, i.e. a vector of values:

```imperfect definition
cases: List<FitnessCase<Double>>
```

So we zip the *cases* with the *outputs* to put each case and its actual output side by side:

```
cases.zip(outputs)
```

We get the expected output:

```
case.target
```

We check if the actual output completely matches the expected output:

```
actual.equals(case.target)
```

We filter out the mismatches:

```
filter { (case, actual) ->
    actual.equals(case.target)
}
```

We count the matches:

```
count()
```

So this is how the *numMatches* value is calculated, our custom fitness functions for binary representations follow the same principle.

Now we work on producing our **testcases.txt** file:

```
File("testcases.txt").bufferedWriter()
```

We record how many cases the program gets right out of all the cases:

```
out.write("$numMatches matches out of ${ outputs.count() } cases.\n")
```

We record each pair of expected and actual values:

```
cases.zip(outputs).map { (expected, actual) ->
    out.write("\tExpected: ${expected.target}, Actual: $actual\n")
}
```

This information can be used for debugging and hyperparameter-tweaking. Of course, in this way we can record all kinds of diagnostic information we want.

This is a verbose fitness function for producing a statistics file instead of a useful fitness value, so we can just return any value as it won't be used:

```
return 0.0
```

### CustomFitnessFunctionsExperiment.kt

This file is very similar to the one from our last tutorial, too, the significant difference being we use the *match* fitness function from our custom fitness functions:

```
override val fitnessFunction = CustomFitnessFunctions.match
```

Also, we shift the output registers to the end of the calculation registers so the input and output registers don't overlap, so LGP can produce a good program more easily:

```
private val outputShift = inputVectorization.flatten().count() + configuration.numCalculationRegisters - 1 - vectorization.second.last().last().first
private val outputVectorization = vectorization.second.map { vector ->
    vector.map { pair -> Pair(pair.first + outputShift, pair.second) }
}
```

### CustomFitnessFunctions.kt

We define our custom *match* fitness function here:

```
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
```

It follows the same idea as our *verboseMatch* above, but produces a valid fitness value, which LGP will try to minimise, so we are counting mismatches instead of matches this time, by filtering out all the matches:

```
!actual.equals(case.target)
```

The fitness function returns a double:

```
FitnessFunction<Double>
```

```
toDouble()
```

## Results

Please take a look at the files generated by the program: **results.csv**, **testcases.txt**, **custom_fitness_functions_experiment.c**. Feel free to compare your results to the results in this repository, and compile and run the C program.

In fact the LGP program in this tutorial can simulate all kinds of circuits, but the basic logic operators of NOT, AND, OR and XOR are too low-level for more complicated circuits, and for LGP to find a good solution in a reasonable amount of time, larger modules are needed as operations, e.g. half adder, full adder, multiplexer, etc.
