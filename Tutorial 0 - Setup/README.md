# LGP Tutorial 0: Setup

> *Setting up Kotlin and the LGP API

## Kotlin

The system is built using Kotlin and the easiest way to use it is through the Kotlin API.

If you don't have curl on your UNIX based system, please first install curl with:

```
sudo apt install curl
```

According to [https://kotlinlang.org/docs/tutorials/command-line.html](https://kotlinlang.org/docs/tutorials/command-line.html), the simplest way to install Kotlin on UNIX based systems such as OS X, Linux, Cygwin, FreeBSD and Solaris is by using SDKMAN!. Simply run the following in a terminal and follow any instructions:

```
curl -s https://get.sdkman.io | bash
```

Next open a new terminal and install Kotlin with:

```
sdk install kotlin
```

Please refer to the same page for other ways to install Kotlin using the Command Line.

Please refer to [this page](https://kotlinlang.org/docs/tutorials/getting-started.html) if you prefer to work with IntelliJ IDEA.

## LGP API

The core API comes as a JAR file, which can be either built or downloaded.

### Building the API


If you don't have git on your UNIX based system, please install git with:

```
sudo apt install git
```

The API is in this [repository](https://github.com/JedS6391/LGP), please clone it to the local directory of your choice.

```
git clone https://github.com/JedS6391/LGP.git
```

Using the Command Line, change into the directory

```
cd LGP
```

To see if you have Java installed

```
java -version
```

If you don't have Java, please install Java with

```
sudo apt install default-jre
```

and then

```
sudo apt install default-jdk
```

With Java installed, build the API with

```
./gradlew coreJar
```

This might take a while for it to gather all the dependencies required.

In the *LGP* directory, you should now see a *build* directory, please go inside it, then go inside the *libs* directory, please look for the JAR file for the core API, e.g. **LGP-core-2.0.jar**. Move the JAR file to the directory you'd like to work in (here we'll call this directory **LGP-Tutorials**), and rename the file **LGP.jar** for simplicity.

### Downloading the API

Alternatively you can download the newest version from the [releases](https://github.com/JedS6391/LGP/releases) page. For example, as of when this tutorial is written, the top version on the page is 2.0, so please download the **LGP-core-2.0.jar** file in the *Assets* of version 2.0. Move the JAR file to the directory you'd like to work in (here we'll call this directory **LGP-Tutorials**), and rename the file **LGP.jar** for simplicity.

## Additional libraries

Please download **argparser.jar** and **xenocom.jar** from the root of this repository into the directory **LGP-Tutorials**. Our later tutorials will make use of these libraries.

Please note, [argparser](https://github.com/xenomachina/kotlin-argparser) and [xenocom](https://github.com/xenomachina/xenocom) are API's on GitHub. They were used to build the jar files provided here. You can clone them and build the jar files yourself.

## Check

Please download MyProblem.kt from this repository to a sub-directory of **LGP-Tutorials**, e.g. **Tutorial0Setup**. Then run this command in **Tutorial0Setup**

```
kotlinc -cp ../LGP.jar -no-jdk -no-stdlib MyProblem.kt
```

Then run this command

```
kotlin -cp ../LGP.jar:. MyProblemKt
```

If the items are installed correctly, you should see the following output

```
My Problem
Description(description=A simple example problem definition)
```

Now with both Kotlin and the LGP API installed, we are ready to work with LGP.
