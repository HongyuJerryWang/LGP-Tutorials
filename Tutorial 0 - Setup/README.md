# LGP Tutorial 0: Setup

> *Setting up Kotlin and the LGP API

## Kotlin

The system is built using Kotlin and the easiest way to use it is through the Kotlin API.

If you don't have some of the tools on your UNIX based system, please first install them with:

```
sudo apt install curl
sudo apt install upzip
sudo apt install zip
sudo apt install gnome-terminal
```

According to [https://kotlinlang.org/docs/tutorials/command-line.html](https://kotlinlang.org/docs/tutorials/command-line.html), the simplest way to install Kotlin on UNIX based systems such as OS X, Linux, Cygwin, FreeBSD and Solaris is by using SDKMAN!. Simply run the following in a terminal and follow any instructions:

```
curl -s https://get.sdkman.io | bash
```

Next open a new terminal:

```
gnome-terminal
```

Install Kotlin in the newly-opened terminal with:

```
sdk install kotlin
```

Please refer to the same page for other ways to install Kotlin using the Command Line.

Please refer to [this page](https://kotlinlang.org/docs/tutorials/getting-started.html) if you prefer to work with IntelliJ IDEA.

## LGP API

The core API comes as a JAR file, which can be either built or downloaded.

### Building the API

Please go to the [LGP repository](https://github.com/JedS6391/LGP), and clone it to your local computer. Using the Command Line, change into the directory

```
cd LGP
```

Then build the API with

```
./gradlew coreJar
```

This might take a which for it to gather all the dependencies required.

In the *LGP* directory, you should now see a *build* directory, please go inside it, then go inside the *libs* directory, please look for the JAR file for the core API, e.g. **LGP-core-1.2.jar**. Move the JAR file to the directory you'd like to work in, and rename the file **LGP.jar** for simplicity.

### Downloading the API

Please download the newest version from the [releases](https://github.com/JedS6391/LGP/releases) page. For example, as of when this tutorial is written, the top version on the page is 1.2.1, so please download the **LGP-core-1.2.jar** file in the *Assets* of version 1.2.1. Move the JAR file to the directory you'd like to work in, and rename the file **LGP.jar** for simplicity.

## Done

Now with both Kotlin and the LGP API installed, we are ready to work with LGP.
