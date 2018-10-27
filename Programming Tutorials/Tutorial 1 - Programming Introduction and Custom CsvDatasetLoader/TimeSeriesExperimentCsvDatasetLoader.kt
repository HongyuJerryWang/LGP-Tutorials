import com.opencsv.CSVReader
import lgp.core.environment.ComponentLoaderBuilder
import lgp.core.environment.config.Configuration
import lgp.core.environment.dataset.*
import lgp.core.modules.ModuleInformation
import java.io.FileReader
import java.io.Reader

/**
 * Exception given when a CSV file that does not match the criteria the system expects
 * is given to a [TimeSeriesExperimentCsvDatasetLoader] instance.
 */
class InvalidCsvFileException(message: String) : Exception(message)

/**
 * Loads a collection of samples and their target values from a CSV file.
 *
 * @property reader A reader that will provide the contents of a CSV file.
 * @property windowSize The number of previous values we want to provide to LGP as features.
 */
class TimeSeriesExperimentCsvDatasetLoader constructor(
        val reader: Reader,
        val windowSize: Int
) : DatasetLoader<Double>{

    public override fun load(): Dataset<Double> {
        val reader = CSVReader(this.reader)
        val lines: List<Array<String>> = reader.readAll()

        val times: List<Double> = lines.map { line ->
            line[2].toDouble()
        }

        var timeMin = times.get(0)
        var timeMax = times.get(0)

        for (i in 1..(times.size - 1)) {
            if (times.get(i) < timeMin) { timeMin = times.get(i) }
            if (times.get(i) > timeMax) { timeMax = times.get(i) }
        }

        val normalizedTimes: List<Double> = times.map { time ->
            (time - timeMin) / (timeMax - timeMin)
        }

        val values: List<Double> = lines.map { line ->
            line[3].toDouble()
        }

        var valueMin = values.get(0)
        var valueMax = values.get(0)

        for (value: Double in values) {
            if (value < valueMin) { valueMin = value }
            if (value > valueMax) { valueMax = value }
        }

        val normalizedValues: List<Double> = values.map { value ->
            (value - valueMin) / (valueMax - valueMin)
        }

        // Make sure there is data before we continue.
        if (lines.size < windowSize + 1)
            throw InvalidCsvFileException("CSV file should have more data rows.")

        // Parse features and target values individually.
        var features: MutableList<Sample<Double>> = ArrayList()

        var targets: MutableList<List<Double>> = ArrayList()

        for (i in windowSize..(lines.size - 1)) {
            val temp = MutableList(windowSize, { j -> Feature(name = "t-" + (windowSize - j), value = normalizedValues.get(i - windowSize + j)) })
            temp.add(Feature(name = "time", value = normalizedTimes.get(i)))
            features.add(Sample(temp))
            targets.add(listOf(normalizedValues.get(i)))
        }

        return Dataset(features, targets)
    }

    override val information = ModuleInformation(
        description = "A custom loader for the time series data."
    )
}
