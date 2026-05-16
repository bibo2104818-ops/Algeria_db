
# Algeria_db

A C-based database implementation featuring data structures, binary tree operations, and GUI interface for managing Algerian personalities database.

## Overview

Algeria_db is a project that implements a custom database system in C with support for advanced data structures, tree-based indexing, and a graphical user interface. It provides functionality for storing, retrieving, and managing records related to Algerian personalities.

## Project Structure

```
Algeria_db/
├── main.c                    # Main entry point
├── gui.c & gui.h            # Graphical User Interface implementation
├── list.c & list.h          # Linked list data structure
├── binary_tree.c & binary_tree.h  # Binary search tree implementation
├── recursion.c & recursion.h  # Recursive utility functions
├── stack.c & stack.h        # Stack data structure
├── types.h                  # Type definitions and structures
├── dark_style.css           # UI styling
├── style.css                # Additional styling
├── algerian_personalities.txt  # Data file with personalities database
└── LICENSE                  # MIT License
```

## Key Components

### Core Data Structures

- **Binary Tree** (`binary_tree.c/h`): BST implementation for efficient searching and indexing
- **Linked List** (`list.c/h`): Dynamic list structure for sequential data storage
- **Stack** (`stack.c/h`): LIFO data structure for stack-based operations
- **Recursion** (`recursion.c/h`): Recursive algorithms and utility functions

### User Interface

- **GUI Module** (`gui.c/h`): Graphical interface for user interaction
- **Styling** (`style.css`, `dark_style.css`): Theme and appearance customization

### Data Management

- **Types** (`types.h`): Central definitions for all data structures and types
- **Data File** (`algerian_personalities.txt`): Database of Algerian personalities

## Build System

The project uses **CMake** for multi-platform compilation. A `CMakeLists.txt` configuration file enables building on Windows, macOS, and Linux.

### Building the Project

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the executable
./Algeria_db
```

### Build Requirements

- C compiler (GCC, Clang, or MSVC)
- CMake 3.10 or higher
- Standard C libraries

## Features

- **Binary Search Tree Operations**: Insert, search, delete, and traverse records
- **Linked List Management**: Dynamic data collection and manipulation
- **Stack-based Algorithms**: Support for recursive operations
- **GUI Interface**: User-friendly interaction with the database
- **Data Persistence**: Load and manage personality records from file
- **Recursive Algorithms**: Advanced problem-solving capabilities

## Language Composition

- **C**: 94.0% (Core implementation)
- **CSS**: 6.0% (Styling)

## Usage

### Running the Application

After building, execute the compiled binary:

```bash
./Algeria_db
```

The GUI interface will open, allowing you to:
- Browse Algerian personalities
- Search records using the binary tree index
- Perform CRUD operations (Create, Read, Update, Delete)
- Navigate through the database

### Example Operations

```c
// Binary Tree Search
Node* result = search_tree(root, personality_id);

// Insert into Linked List
insert_list(list, new_personality);

// Stack Operations
push(stack, personality);
pop(stack);
```

## Data Format

The `algerian_personalities.txt` file contains records with the following structure:
```
name | field | contribution | birth_year | region
```

## Documentation

### Header Files

- **types.h**: Contains all structure definitions and type declarations
- **gui.h**: GUI function prototypes and constants
- **binary_tree.h**: BST operations interface
- **list.h**: Linked list operations interface
- **stack.h**: Stack operations interface
- **recursion.h**: Recursive function declarations

### Code Organization

Each `.c` file contains implementation details with clear function comments and logical organization. The modular design allows for easy maintenance and extension.

## License

This project is licensed under the **MIT License** - see the LICENSE file for details.

## Repository Information

- **Repository**: [bibo2104818-ops/Algeria_db](https://github.com/bibo2104818-ops/Algeria_db)
- **Access**: Public
- **Language**: C (94.0%), CSS (6.0%)
- **Last Updated**: Recent commits

## Contributing

To contribute to this project:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Submit a Pull Request

## Getting Started

### Prerequisites

- GCC/Clang compiler
- CMake build system
- Basic understanding of data structures and C programming

### Quick Start

```bash
# Clone the repository
git clone https://github.com/bibo2104818-ops/Algeria_db.git
cd Algeria_db

# Build
mkdir build && cd build
cmake ..
cmake --build .

# Run
./Algeria_db
```

## Architecture Overview

```
┌─────────────────────────────────────┐
│      GUI Interface (gui.c)          │
├─────────────────────────────────────┤
│  Binary Tree │ Linked List │ Stack  │
│  (binary_tree.c) (list.c) (stack.c) │
├─────────────────────────────────────┤
│  Core Types & Recursion             │
│  (types.h, recursion.c)             │
├─────────────────────────────────────┤
│  Data Layer (algerian_personalities) │
└─────────────────────────────────────┘
```

## Performance Considerations

- **Binary Tree**: O(log n) average search, O(n) worst case
- **Linked List**: O(n) search, O(1) insertion at head
- **Stack**: O(1) push/pop operations

## Troubleshooting

### Build Issues

If you encounter build errors:
1. Ensure CMake is properly installed: `cmake --version`
2. Verify C compiler is available: `gcc --version` or `clang --version`
3. Check that all source files are in the correct directory

### Runtime Issues

- Ensure `algerian_personalities.txt` exists in the working directory
- Verify file permissions for data file access
- Check console output for specific error messages

## Future Enhancements

- Database persistence to file/SQLite
- Search optimization with indexing
- Extended GUI features
- Network capabilities
- Unit test suite

## Contact & Support

For issues, questions, or contributions, please visit the [GitHub repository](https://github.com/bibo2104818-ops/Algeria_db).

---

**Version**: 1.0  
**Last Updated**: 2026  
**Status**: Active Development
