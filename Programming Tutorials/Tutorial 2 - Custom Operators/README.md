# LGP Programming Tutorial 2: Custom Operators

> *Making custom operators for a specific problem

## Overview

We will perform LGP on the Iris dataset, from the [UCI Machine Learning Repository](https://archive.ics.uci.edu/ml/datasets/iris).

We'll do a binary classification this time, predicting whether a flower is Iris Setosa or not. While classifying all three species is more of a wrap-around problem, which can be solved by splicing three (or two) such classification programs together, one for each species, and having one final algorithm to process the predictions of these programs and giving one final result.

In this repository is the processed Iris dataset, where Iris Setosa is marked 1 and the other species are marked 0.

We will need three program files:
Main.kt to run the program
CustomOperatorsExperiment.kt to define the problem, i.e. how LGP should be setup and run
CustomOperators.kt to define the custom operators

## Custom Operators

We'll make some custom operators for this problem: a CustomMacroMutationOperator, a CustomMicroMutationOperator and a ternary operator called Median to return the median of three operands.

The default macro and micro mutation operators in the LGP API do not mutate instructions deemed ineffective, i.e. instructions that have no influence on the final result, but as we do crossovers, swapping parts of the programs, ineffective instructions might become effective as the context will have changed. Therefore, we'll make custom macro and micro mutation operators that don't ignore ineffective instructions.

We'll add Median to our operations and see if it helps with the classification task at hand.

## Running

Please make a new directory and have a copy of your **LGP.jar**, **argparser.jar** and **xenocom.jar** in it, or alternatively please link to their locations when you compile and run your program.

Download the following file from this repository into the directory: **Main.kt**, **CustomOperatorsExperiment.kt**, **CustomOperators.kt**, **configuration.json** and **dataset.csv**.

Compile

```
kotlinc -cp LGP.jar:argparser.jar:xenocom.jar -no-stdlib *.kt
```

Run

```
kotlin -cp LGP.jar:argparser.jar:xenocom.jar:. Main configuration.json dataset.csv
```

## Analysis

The **Main.kt** and **CustomOperatorsExperiment.kt** files are very similar to our files in the last tutorial, please just make sure you indeed use the custom operators you want to use.

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

Please also make sure to add the Median operator to the list of operations in our configuration file.

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

```
private enum class MacroMutationType {
    Insertion,
    Deletion
}
```

A macro mutation on a program is either inserting a new instruction or deleting an existing instruction.

```
val mutationType = if (random.nextDouble() `<` this.insertionRate) {
    MacroMutationType.Insertion
} else {
    MacroMutationType.Deletion
}
```

We determine whether an insertion or a deletion should be performed, by the probabilities given to them.

```
val mutationPoint = random.randInt(0, programLength - 1)
```

We randomly select an instruction at a position i (mutation point).

```
if (programLength `<` maximumProgramLength &&
        (mutationType == MacroMutationType.Insertion || programLength == minimumProgramLength)) {
    val instruction = this.instructionGenerator.generateInstruction()

    instruction.destination = random.randInt(0, individual.registers.calculationRegisters.last())
    individual.instructions.add(mutationPoint, instruction)
}
```

We can only perform an insertion if the maximum program length hasn't been reached, and either the mutation type is insertion or no deletion can be performed. We randomly generate an instruction and insert it into the mutation point.

```
else if (programLength > minimumProgramLength &&
            (mutationType == MacroMutationType.Deletion || programLength == maximumProgramLength)) {
    individual.instructions.removeAt(mutationPoint)
}
```

We delete the instruction at the mutation point if otherwise.

### CustomMicroMutationOperator

```
private enum class MicroMutationType {
    Register,
    Operator,
    Constant
}
```

A micro mutation is changing a register or the operator of an instruction, or changing a constant value of the program.

```
val instruction = random.choice(individual.instructions)
```

We randomly select an instruction to mutate.

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

We determine which micro mutation will be performed.

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

To perform a register mutation, we randomly select a register. If it's the destination register, we randomly change it to one of the input or culculation registers, but NOT a constant register so the constants don't get overriden. If it's an operand register, we change it to an input, calculation or constant register.

```
MicroMutationType.Operator -> {
    // 4. If operator mutation then select a different instruction operation randomly
    val operation = random.choice(this.operations)

    // Assure that the arity of the new operation matches with the number of operands the instruction has.
    // If the arity of the operations is the same, then nothing needs to be done.
    if (instruction.operands.size > operation.arity.number) {
        // If we're going to a reduced arity instruction, we can just truncate the operands
        instruction.operands = instruction.operands.slice(0 until operation.arity.number)
    } else if (instruction.operands.size `<` operation.arity.number) {
        // Otherwise, if we're increasing the arity, just add random input
        // and calculation registers until the arity is met.
        while (instruction.operands.size `<` operation.arity.number) {
            val register = if (random.nextDouble() `<` constantsRate) {
            registerGenerator.next(RegisterType.Constant).first()
            } else {
                registerGenerator.next(
                    a = RegisterType.Input,
                    b = RegisterType.Calculation,
                    predicate = { random.nextDouble() `<` 0.5 }
                ).first()
            }

            instruction.operands.add(register.index)
        }
    }

    instruction.operation = operation
}
```

To perform an operator mutation, we copy the operation from a random instruction in the program to this instruction, deleting operands if we need fewer of them, and adding random operands if we need more.

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

To perform a constant mutation, we randomly select a constant register and mutate its value with our constantMutationFunc.

### Median

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

Define a ternary operation, i.e. an operation with 3 operands.

```
class Median : TernaryOperation<Double>(
        func = { args: Arguments<Double> ->
            Math.max(Math.min(args.get(0), args.get(1)), Math.min(Math.max(args.get(0), args.get(1)), args.get(2)))
        }
)
```

The function for Median gets the median value of 3 values.

```
override val representation = "Median"

override val information = ModuleInformation(
    description = "An operation for getting the median of three Double arguments."
)
```

Give some information regarding the operator. The representation is used to help format the program in the command line interface, but as it's an operator of a custom arity, it cannot be displayed, so the representation will have no practical effect.

```
override fun toString(operands: MutableList<RegisterIndex>, destination: RegisterIndex): String {
    return "r[" + destination + "] = max(min(r[" + operands[0] + "], r[" + operands[1] + "]), min(max(r[" + operands[0] + "], r[" + operands[1] + "]), r[" + operands[2] + "]))"
}
```

We define how it will be translated into a C program instruction.

## Results

Please take a look at the files generated by the program: **results.csv**, **testcases.txt** and **custom_operators_experiment.c**. Feel free to compare your results to the results in this repository, and compile and run the C program. Please refer to the Non-Programming Tutorial 2 for how to tweak the hyperparameters and the evolutionary algorithm.
