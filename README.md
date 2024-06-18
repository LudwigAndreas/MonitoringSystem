# Monitoring System

Implementation of the monitoring system project. The project is a part of the School 21 curriculum.

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installing](#installing)
- [Running the tests](#running-the-tests)
- [Usage](#usage)
- [Built Using](#built-using)
- [Authors](#authors)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## About

Monitoring System is implement a GUI program for simple monitoring of the main indicators of the system as kernels and agents. Agents  collect metrics and pass them to the kernel, which will log these metrics. When critical values are reached, the system sends notifications to the user.

Kernel and agents are implemented as dynamic libraries. The kernel scans the `./agents/` directory for new agents and automatically connects them. The kernel logs the metrics to the `./logs/` directory. The program interface displays the last 1000 lines of the log and all information about connected agents. The interface allows you to get detailed information about the agent, change the configuration of the agent, and disconnect the agent.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need to install the software and how to install them.

```text
cmake >= 3.10
gcc >= 7.5.0
Qt >= 5.12.8
```

```bash
sudo apt-get install cmake
sudo apt-get install gcc
sudo apt-get install qt5-default
```

### Installing

A step by step series of examples that tell you how to get a development env running.

Clone the repository.

```bash
git clone

cd MonitoringSystem
```

Create a build directory and run cmake.

```bash

mkdir build
cd build

cmake ..
```

Build the project.

```bash
make
```

Run the project.

```bash
./MonitoringSystem
```

## Running the tests

Explain how to run the automated tests for this system.

```bash
make test
```

## Usage

After starting the program, the main window will open. The main window has a menu bar with the following items: `Info`, `Log`. The `Info` item opens a main window that displays information about connected agents and contained metrics. The `Log` item opens a window with the last 1000 lines of the log.

The main window has a toolbar with the following buttons: `Add agent`, `Remove agent`, `Agent info`, `Metric info`. The `Add agent` button opens a window for adding a new agent. The `Remove agent` button removes the selected agent. The `Agent info` button opens a window with detailed information about the selected agent. The `Metric info` button opens a window with detailed information about the selected metric. In info section you can chage agent configuration (period, critical value, etc) and disconnect agent.

## Built Using

- [C++ 17](https://isocpp.org/) - Programming language
- [CMake](https://cmake.org/) - Build system
- [Qt](https://www.qt.io/) - GUI framework
- [Google Test](https://google.github.io/googletest/) - Testing framework

## Authors

This project was developed by:

| <img src="https://avatars.githubusercontent.com/u/88089961?v=4" alt="drawing" width="50"/> |[LudwigAndreas](https://github.com/LudwigAndreas)|
| --- | --- |
| <img src="https://avatars.githubusercontent.com/u/57820208?s=60&v=4" alt="drawing" width="50"/> | [AndrefHub](https://github.com/AndrefHub) |

## Acknowledgements

- [School 21](https://21-school.ru/) - Educational institution

## License

This project is licensed under the School 21 License - see the [LICENSE](LICENSE) file for details.
