# Stack Visualization with SDL2

This repository contains a program for visualizing stack operations using the SDL2 library in C. The program demonstrates the structure and behavior of a stack through graphical visualization.

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Installation](#installation)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [File Descriptions](#file-descriptions)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Features

- **Stack Visualization**: Graphically visualize the structure and operations of a stack.
- **Operation Demonstration**: Visualize stack operations like push, pop, and peek.
- **Interactive Interface**: Perform stack operations interactively.
- **Customizable**: Easily modify stack properties and visualization parameters.

## Demo

### Watch the Demo Video
[Watch the demo video](https://youtu.be/kEAUIdTImso?si=DfYFKEodvghKKOXt)

## Installation

To set up the stack visualization program on your local machine, follow these steps:

1. **Clone the Repository**: Download the repository to your local machine using git.
    ```sh
    git clone <repo>
    ```

2. **Navigate to the Project Directory**: Change your current directory to the project's root directory.
    ```sh
    cd <repo-directory>
    ```

3. **Install Dependencies**: Ensure you have SDL2 installed on your system. You can install SDL2 using package managers like `apt` for Ubuntu or `brew` for macOS.
    ```sh
    # For Ubuntu
    sudo apt-get install libsdl2-dev

    # For macOS
    brew install sdl2
    ```

4. **Compile the Program**: Use `gcc` to compile the program.
    ```sh
    gcc -o stack_visualization main.c junzhe_utilities.c stack_utilities.c -lSDL2 -lm
    ```

5. **Run the Program**: Execute the compiled program to start the visualization.
    ```sh
    ./stack
    ```

## Usage

Once the program is running, use the following controls to interact with the stack:

- **push**: Push a value onto the stack.
- **peek**: Peek a value from the stack which does not remove it.
- **pop**: Pop a value from the stack.
- **refresh**: Refresh the menu.
- **quit**: Quit the program.

## Code Structure

The project consists of the following files and directories:

### File Descriptions

- `LICENSE`: The license file containing the terms under which the project's source code can be used.
- `UbuntuMono-Regular.ttf`: A TrueType font file used for rendering text in the SDL2 window.
- `junzhe_utilities.c`: Implementation file containing general utility functions used throughout the project.
- `junzhe_utilities.h`: Header file declaring the general utility functions.
- `main.c`: The main entry point of the program, containing the core logic for initializing SDL2, handling events, and managing the visualization loop.
- `stack_utilities.c`: Implementation file for the stack operations, including push, pop, and peek functions.
- `stack_utilities.h`: Header file defining the stack data structure and declaring stack-related functions.

## Contributing

Contributions are welcome! If you have any ideas for improvements or find any bugs, feel free to open an issue or submit a pull request. Please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a new Pull Request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgements

- Developed by [Junzhe Wang]()https://github.com/J-M-W0.
- Special thanks to the SDL2 community for their support and resources.
- Inspired by various stack visualization tools and resources.
