import lgp.core.environment.CoreModuleType
import lgp.core.environment.Environment
import lgp.core.evolution.instructions.*
import lgp.core.evolution.population.*
import lgp.core.evolution.registers.Arguments
import lgp.core.evolution.registers.RandomRegisterGenerator
import lgp.core.evolution.registers.RegisterType
import lgp.core.modules.Module
import lgp.core.modules.ModuleInformation

// Used internally to make the macro/micro mutation code a bit clearer.
private enum class MacroMutationType {
    Insertion,
    Deletion
}

private enum class MicroMutationType {
    Register,
    Operator,
    Constant
}

/**
 * A [MutationOperator] implementation that performs macro mutations.
 *
 * For more information, see Algorithm 6.1 from Linear Genetic Programming (Brameier, M., Banzhaf, W. 2001).
 *
 * Note that [insertionRate] + [deletionRate] should be equal to 1.
 *
 * @property insertionRate The probability with which instructions should be inserted.
 * @property deletionRate The probability with which instructions should be deleted.
 */
class CustomMacroMutationOperator<T>(
        environment: Environment<T>,
        val insertionRate: Double,      // p_ins
        val deletionRate: Double        // p_del
) : MutationOperator<T>(environment) {

    init {
        // Give a nasty runtime message if we get invalid parameters.
        assert((insertionRate + deletionRate) == 1.0)
    }

    private val minimumProgramLength = this.environment.configuration.minimumProgramLength
    private val maximumProgramLength = this.environment.configuration.maximumProgramLength
    private val random = this.environment.randomState
    private val instructionGenerator = this.environment.registeredModule<InstructionGenerator<T>>(
            CoreModuleType.InstructionGenerator
    )

    /**
     * Performs a single macro mutation to the individual given.
     */
    override fun mutate(individual: Program<T>) {
        val programLength = individual.instructions.size

        // 1. Randomly select macro mutation type insertion | deletion with probability
        // p_ins | p_del and with p_ins + p_del = 1
        val mutationType = if (random.nextDouble() < this.insertionRate) {
            MacroMutationType.Insertion
        } else {
            MacroMutationType.Deletion
        }

        // 2. Randomly select an instruction at a position i (mutation point) in program gp.
        val mutationPoint = random.randInt(0, programLength - 1)

        // 3. If len(gp) < l_max and (insertion or len(gp) = l_min) then
        if (programLength < maximumProgramLength &&
                (mutationType == MacroMutationType.Insertion || programLength == minimumProgramLength)) {
            val instruction = this.instructionGenerator.generateInstruction()

            instruction.destination = random.randInt(0, individual.registers.calculationRegisters.last())
            individual.instructions.add(mutationPoint, instruction)
        }
        else if (programLength > minimumProgramLength &&
                    (mutationType == MacroMutationType.Deletion || programLength == maximumProgramLength)) {
            // Delete instruction i
            individual.instructions.removeAt(mutationPoint)
        }
    }

    override val information = ModuleInformation("Algorithm 6.1 ((effective) instruction mutation).")
}

/**
 * A [MutationOperator] implementation that performs micro mutations.
 *
 * For more information, see Algorithm 6.2 from Linear Genetic Programming (Brameier, M., Banzhaf, W. 2001).
 *
 * Note that the constant mutation rate is 1 - ([registerMutationRate] - [operatorMutationRate]), which should
 * be taken into account when choosing values for these parameters.
 *
 * @property registerMutationRate The rate with which registers should be mutated.
 * @property operatorMutationRate The rate with which operates should be mutated.
 * @property constantMutationFunc A function that can mutate values in the domain of [T].
 */
class CustomMicroMutationOperator<T>(
        environment: Environment<T>,
        val registerMutationRate: Double,
        val operatorMutationRate: Double,
        val constantMutationFunc: ConstantMutationFunction<T>
) : MutationOperator<T>(environment) {

    private val constantsRate = this.environment.configuration.constantsRate
    private val operations = this.environment.operations
    private val random = this.environment.randomState

    /**
     * Performs a single micro mutation to the individual given.
     */
    override fun mutate(individual: Program<T>) {
        val instruction = random.choice(individual.instructions)

        // 2. Randomly select mutation type register | operator | constant with
        // probability p_regmut | p_opermut | p_constmut and with p_regmut +
        // p_opermut + p_constmut = 1.
        // Assumption: p_constmut = 1 - (p_regmut + p_opermut).
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

        val registerGenerator = RandomRegisterGenerator(this.environment.randomState, individual.registers)

        when (mutationType) {
            // 3. If register mutation then
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
        }
    }

    override val information = ModuleInformation("Algorithm 6.2 ((effective) micro mutation).")
}

enum class CustomArity(override val number: Int) : Arity {
    Ternary(3);
}

/**
 * An operation that has an arity of three (i.e. its function takes 3 arguments).
 */
abstract class TernaryOperation<T>(func: (Arguments<T>) -> T) : Operation<T>(CustomArity.Ternary, func) {

    /**
     * Applies the operations function to the argument(s) given.
     *
     * If the number of arguments given does not match the operations arity,
     * then an exception will be thrown.
     *
     * @param arguments A collection of arguments (should be at most 3).
     * @throws ArityException When the number of arguments does not match the arity.
     * @returns A mapping of the arguments to some value.
     */
    override fun execute(arguments: Arguments<T>): T {
        return when {
            arguments.size() != this.arity.number -> throw ArityException("TernaryOperation takes 3 argument but was given ${arguments.size()}.")
            else -> this.func(arguments)
        }
    }
}

/**
 * Gets the median of three Double arguments.
 */
class Median : TernaryOperation<Double>(
        func = { args: Arguments<Double> ->
            Math.max(Math.min(args.get(0), args.get(1)), Math.min(Math.max(args.get(0), args.get(1)), args.get(2)))
        }
) {

    override val representation = "Median"

    override val information = ModuleInformation(
        description = "An operation for getting the median of three Double arguments."
    )

    override fun toString(operands: MutableList<RegisterIndex>, destination: RegisterIndex): String {
        return "r[" + destination + "] = fmax(fmin(r[" + operands[0] + "], r[" + operands[1] + "]), fmin(fmax(r[" + operands[0] + "], r[" + operands[1] + "]), r[" + operands[2] + "]))"
    }
}
