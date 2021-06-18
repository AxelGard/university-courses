# StarCraft II Python Bot
This repository contains a passive PyCommandCenter-based bot for StarCraft II. It is intended to be a reasonable starting-point for creating your own StarCraft II-playing system.

## Setup for creating your own bot

1. Locate your own fork ("copy") of this project and its repository. If you are participating in TDDD92 or TDDE25, **you should *not* create your own fork!**  Instead, begin by registering your group in Webreg.  We periodically run a script that forks this project for all registered groups, placing projects in the [TDDD92-2020 group](https://gitlab.liu.se/tddd92-2020) or the [TDDE25-2020 group](https://gitlab.liu.se/tdde25-2020).  We will require you to use these projects and repositories.

2. Now clone the repository from Gitlab using the following command: `git clone <path>`, where `<path>` is shown when you press the blue button *Clone* in the top-right corner on your repository page. If you have configured SSH-access, then use the SSH link, otherwise use the HTTP link.

3. Next, setup the library in PyCharm. See [PyCommandCenter's page on PyCharm](https://gitlab.liu.se/starcraft-ai-course/pycommandcenter/blob/master/pycharm.md) for instructions.

4. Everything is now ready for PyCharm: Start PyCharm, and then select "Open" and navigate to the location you cloned your fork of this repository to.

5. In order to run the code, right click the file named "main.py" in the project panel to the right, and select "Debug main". This will run the code in "main.py", and consequently start StarCraft II. You can also edit the code in "main.py" by double-clicking "main.py".
