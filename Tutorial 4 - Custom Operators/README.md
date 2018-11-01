# LGP Programming Tutorial 2: Custom Operators

> *Making custom operators for a specific problem

## Overview

We will perform LGP on the Iris dataset, from the [UCI Machine Learning Repository](https://archive.ics.uci.edu/ml/datasets/iris), to classify the species of a flower using the length and width of its sepals and petals.

We will need three program files:
1. Main.kt to run the program
2. CustomOperatorsExperiment.kt to define the problem, i.e. how LGP should be setup and run
3. CustomOperators.kt to define the custom mutation operators and the custom instruction operators

## Custom Operators

We'll make some custom operators for this problem: a CustomMacroMutationOperator, a CustomMicroMutationOperator and a ternary instruction operator called Median to return the median of three operands.

The default macro and micro mutation operators in the LGP API do not mutate instructions deemed ineffective, i.e. instructions that have no influence on the final result, but as we do crossovers, swapping parts of the programs, ineffective instructions might become effective as the context will have changed. Therefore, we'll make custom macro and micro mutation operators that don't ignore ineffective instructions.

We'll add Median to our operations and see if it helps with the classification task at hand.

## Running

Please download **Main.kt**, **CustomOperators.kt**, **CustomOperatorsExperiment.kt**, **configuration.kt** and **dataset.csv** from this repository into a sub-directory of **LGP-Tutorials**, e.g. **Tutorial4**.

In **Tutorial4**, compile:

```
export CLASSPATH=../LGP.jar:../argparser.jar:../xenocom.jar
kotlinc -cp $CLASSPATH -no-stdlib *.kt
```

This problem is somewhat more challenging than the problems in our previous tutorials, partially because it's a classification problem, so the more advanced IslandMigration algorithm is recommended, also in **configuration.json** we should allow deeper and wider searches into the search space. Run with the command below, it may run for a while:

```
kotlin -cp $CLASSPATH:. Main configuration.json dataset.csv IslandMigration 4 10 10
```

## Analysis

When there are categorical inputs or outputs, they need to be vectorized in one-hot encoding for the training process. Please fill in featuresBeingCategorical and outputsBeingCategorical accordingly in **configuration.json**:

```
"featuresBeingCategorical": [false, false, false, false],
"outputsBeingCategorical": [true]
```

Call the *vectorize* method:

```
private val vectorization = CsvDatasetLoader.vectorize(this.datasetFilename, this.configuration.featuresBeingCategorical.map { value -> value.toBoolean() }, this.configuration.outputsBeingCategorical.map { value -> value.toBoolean() })
private val inputVectorization = vectorization.first
private val outputVectorization = vectorization.second
```

This information will help encode the data. Here are three sample rolls from the original dataset:

sepal_length | sepal_width | petal_length | petal_width | species
---|---|---|---|---
5.1 | 3.5 | 1.4 | 0.2 | Iris-setosa
7.0 | 3.2 | 4.7 | 1.4 | Iris-versicolor
6.3 | 3.3 | 6.0 | 2.5 | Iris-virginica

It will be encoded into:

sepal_length | sepal_width | petal_length | petal_width | species_being_Iris-setosa | species_being_Iris-versicolor | species_being_Iris-virginica
---|---|---|---|---|---|---
5.1 | 3.5 | 1.4 | 0.2 | 1.0 | 0.0 | 0.0
7.0 | 3.2 | 4.7 | 1.4 | 0.0 | 1.0 | 0.0
6.3 | 3.3 | 6.0 | 2.5 | 0.0 | 0.0 | 1.0

This vectorization will be used in multiple places to ensure the correct training of the program and the production of the resulting C program:

```
CoreModuleType.ProgramGenerator to { environment ->
    BaseProgramGenerator(
        environment,
        sentinelTrueValue = 1.0, // Determines the value that represents a boolean "true".
        outputRegisterIndices = outputVectorization.flatten().map { pair -> pair.first }  // Which registers should be read for a programs output.
    )
}
```

```
this.environment = Environment(
    this.configLoader,
    this.constantLoader,
    this.operationLoader,
    this.defaultValueProvider,
    this.fitnessFunction,
    this.inputVectorization,
    // Collect results and output them to the file "result.csv".
    ResultAggregators.BufferedResultAggregator(
        ResultOutputProviders.CsvResultOutputProvider(
            filename = "results.csv"
        )
    )
)
```

```
val featureIndices = 0 until inputVectorization.count()
val targetIndices = inputVectorization.count() until (inputVectorization.count() + outputVectorization.count())

val csvLoader = CsvDatasetLoader(
    reader = BufferedReader(FileReader(this.datasetFilename)),
    featureParseFunction = ParsingFunctions.vectorizedDoubleFeatureParsingFunction(featureIndices, inputVectorization),
    targetParseFunction = ParsingFunctions.vectorizedDoubleTargetParsingFunction(targetIndices, outputVectorization)
)
```

```
class CustomOperatorsExperimentSolution(
    override val problem: String,
    val result: TrainingResult<Double>,
    val dataset: Dataset<Double>,
    val inputVectorization: List<List<Pair<Int, String?>>>,
    val outputVectorization: List<List<Pair<Int, String?>>>
) : Solution<Double>
```

```
return CustomOperatorsExperimentSolution(
    problem = this.name,
    result = trainer.train(dataset),
    dataset = dataset,
    inputVectorization = this.inputVectorization,
    outputVectorization = this.outputVectorization
)
```

```
val translated = ExtendedProgramTranslator(true, solution.inputVectorization, solution.outputVectorization).translate(best as BaseProgram<Double>)
```

The vectorization mechanism is compatible with non-categorical inputs and outputs (e.g. numeric values). In fact, the program for the non-programming tutorial is integrated with vectorization as well, and can deal with both categorical and non-categorical values.

Besides that, the **Main.kt** and **CustomOperatorsExperiment.kt** files are very similar to our files in the last tutorial, please just make sure you indeed use the custom mutation operators you want to use:

```
// Use the custom macro-mutation operator.
CoreModuleType.MacroMutationOperator to { environment ->
    CustomMacroMutationOperator(
        environment,
        insertionRate = 0.67,
        deletionRate = 0.33
    )
},
// Use the custom micro-mutation operator.
CoreModuleType.MicroMutationOperator to { environment ->
    CustomMicroMutationOperator(
        environment,
        registerMutationRate = 0.5,
        operatorMutationRate = 0.5,
        constantMutationFunc = ConstantMutationFunctions.randomGaussianNoise(this.environment)
    )
}
```

Please also make sure to add the Median instruction operator to the list of operations in our configuration file:

```
"operations": [
    "lgp.lib.operations.Addition",
    "lgp.lib.operations.Subtraction",
    "lgp.lib.operations.Multiplication",
    "lgp.lib.operations.Division",
    "Median"
]
```

Let's have a look at the custom operators in our **CustomOperators.kt**.

### CustomMutationOperator

A macro mutation on a program is either inserting a new instruction or deleting an existing instruction:

```
private enum class MacroMutationType {
    Insertion,
    Deletion
}
```

We determine whether an insertion or a deletion should be performed, by the probabilities given to them:

```
val mutationType = if (random.nextDouble() < this.insertionRate) {
    MacroMutationType.Insertion
} else {
    MacroMutationType.Deletion
}
```

We randomly select an instruction at a position i (mutation point):

```
val mutationPoint = random.randInt(0, programLength - 1)
```

We can only perform an insertion if the maximum program length hasn't been reached, and either the mutation type is insertion or no deletion can be performed. We randomly generate an instruction and insert it into the mutation point:

```
if (programLength < maximumProgramLength &&
        (mutationType == MacroMutationType.Insertion || programLength == minimumProgramLength)) {
    val instruction = this.instructionGenerator.generateInstruction()

    instruction.destination = random.randInt(0, individual.registers.calculationRegisters.last())
    individual.instructions.add(mutationPoint, instruction)
}
```

We delete the instruction at the mutation point if otherwise:

```
else if (programLength > minimumProgramLength &&
            (mutationType == MacroMutationType.Deletion || programLength == maximumProgramLength)) {
    individual.instructions.removeAt(mutationPoint)
}
```

### CustomMicroMutationOperator

A micro mutation is changing a register or the instruction operator of an instruction, or changing a constant value of the program:

```
private enum class MicroMutationType {
    Register,
    Operator,
    Constant
}
```

We randomly select an instruction to mutate:

```
val instruction = random.choice(individual.instructions)
```

We determine which micro mutation will be performed:

```
val p = random.nextDouble()
val mutationType = when {
    (p < registerMutationRate) -> {
        MicroMutationType.Register
    }
    (registerMutationRate <= p && p <= (registerMutationRate + operatorMutationRate)) -> {
        MicroMutationType.Operator
    }
    else -> {
        MicroMutationType.Constant
    }
}
```

To perform a register mutation, we randomly select a register. If it's the destination register, we randomly change it to one of the input or culculation registers, but NOT a constant register so the constants don't get overriden. If it's an operand register, we change it to an input, calculation or constant register:

```
MicroMutationType.Register -> {
    val registerPositions = mutableListOf(instruction.destination)
    registerPositions.addAll(instruction.operands)

    // Randomly select a register position destination | operand
    val register = random.choice(registerPositions)

    if (register == instruction.destination) {
        instruction.destination = random.randInt(0, individual.registers.calculationRegisters.last())
    } else {
        // If operand register then select a different constant | register with probability
        // p_const | 1 - p_const
        val operand = instruction.operands.indexOf(register)

        val replacementRegister = if (random.nextDouble() < constantsRate) {
            registerGenerator.next(RegisterType.Constant).first()
        } else {
            registerGenerator.next(
                a = RegisterType.Input,
                b = RegisterType.Calculation,
                predicate = { random.nextDouble() < 0.5 }
            ).first()
        }

        instruction.operands[operand] = replacementRegister.index
    }
}
```

To perform an instruction operator mutation, we copy the operation from a random instruction in the program to this instruction, deleting operands if we need fewer of them, and adding random operands if we need more:

```
MicroMutationType.Operator -> {
    // 4. If operator mutation then select a different instruction operation randomly
    val operation = random.choice(this.operations)

    // Assure that the arity of the new operation matches with the number of operands the instruction has.
    // If the arity of the operations is the same, then nothing needs to be done.
    if (instruction.operands.size > operation.arity.number) {
        // If we're going to a reduced arity instruction, we can just truncate the operands
        instruction.operands = instruction.operands.slice(0 until operation.arity.number)
    } else if (instruction.operands.size < operation.arity.number) {
        // Otherwise, if we're increasing the arity, just add random input
        // and calculation registers until the arity is met.
        while (instruction.operands.size < operation.arity.number) {
            val register = if (random.nextDouble() < constantsRate) {
            registerGenerator.next(RegisterType.Constant).first()
            } else {
                registerGenerator.next(
                    a = RegisterType.Input,
                    b = RegisterType.Calculation,
                    predicate = { random.nextDouble() < 0.5 }
                ).first()
            }

            instruction.operands.add(register.index)
        }
    }

    instruction.operation = operation
}
```

To perform a constant mutation, we randomly select a constant register and mutate its value with our constantMutationFunc:

```
else -> {
    // Change constant c through a standard deviation σ_const
    // from the current value: c := c + N(0, σ_const)
    // NOTE: We allow for different types of mutation of values
    // depending on the type, but in general a standard deviation
    // should be used.

    // Use the first operand that refers to a constant register.
    val register = random.randInt(individual.registers.constantRegisters.first(), individual.registers.constantRegisters.last())
    val oldValue = individual.registers[register]

    // Compute a new value using the current constant register value.
    val newValue = this.constantMutationFunc(oldValue)

    // Because each individual has its own register set, we can
    // just overwrite the constant register value.
    individual.registers.overwrite(register, newValue)
}
```

### Median

Define a ternary operation, i.e. an operation with 3 operands:

```
enum class CustomArity(override val number: Int) : Arity {
    Ternary(3);
}

abstract class TernaryOperation<T>(func: (Arguments<T>) -> T) : Operation<T>(CustomArity.Ternary, func) {
    override fun execute(arguments: Arguments<T>): T {
        return when {
            arguments.size() != this.arity.number -> throw ArityException("TernaryOperation takes 3 argument but was given ${arguments.size()}.")
            else -> this.func(arguments)
        }
    }
}
```

The function for Median gets the median value of 3 values:

```
class Median : TernaryOperation<Double>(
        func = { args: Arguments<Double> ->
            Math.max(Math.min(args.get(0), args.get(1)), Math.min(Math.max(args.get(0), args.get(1)), args.get(2)))
        }
)
```

We need to give the instruction operator some description. The representation is used to help format the program in the command line interface:

```
override val representation = "Median"

override val information = ModuleInformation(
    description = "An operation for getting the median of three Double arguments."
)
```

We define how it will be translated into a C program instruction:

```
override fun toString(operands: MutableList<RegisterIndex>, destination: RegisterIndex): String {
    return "r[" + destination + "] = max(min(r[" + operands[0] + "], r[" + operands[1] + "]), min(max(r[" + operands[0] + "], r[" + operands[1] + "]), r[" + operands[2] + "]))"
}
```

## Results

Please take a look at the files generated by the program: **results.csv**, **testcases.txt** and **custom_operators_experiment.c**. Feel free to compare your results to the results in this repository, and compile and run the C program.
