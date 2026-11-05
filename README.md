# CC105 Week 8 Laboratory

## Stack-Based Expression Application

### Project Description

This project is a C++17 console application that uses a Stack Abstract
Data Type (ADT) to check brackets, convert infix expressions into
postfix notation, and evaluate arithmetic expressions.

### Features

1.  **Check Brackets** -- Checks whether parentheses `()`, square
    brackets `[]`, and curly brackets `{}` are balanced.
2.  **Convert and Evaluate Expression** -- Converts infix expressions
    into postfix notation and calculates the result.
3.  **Stack Demonstration** -- Demonstrates push, pop, peek, size, and
    empty-stack operations.
4.  **Error Handling** -- Detects mismatched brackets, invalid operator
    sequences, missing operators, unsupported characters, empty groups,
    and division by zero.

### Implementation Details

-   Programming language: C++17
-   Data structure: Stack ADT
-   Stack implementation: Dynamic array
-   Initial array capacity: 2 elements
-   Resizing: Capacity doubles when the array becomes full
-   Expression conversion: Infix to postfix
-   Expression evaluation: Postfix evaluation using `Stack<double>`

The stack implementation includes `push`, `pop`, `peek`, `isEmpty`, and
`size` operations. It also includes a destructor to release allocated
memory and disables copying to prevent unsafe shallow copies.

### How to Run

1.  Open a C++17 compiler, such as Programiz Online C++ Compiler.
2.  Open or paste the contents of `main.cpp`.
3.  Select the C++17 language standard, if the compiler provides that
    option.
4.  Run the program.
5.  Choose an option from the menu:
    -   `1` -- Check brackets
    -   `2` -- Convert and evaluate an expression
    -   `3` -- Demonstrate stack operations
    -   `0` -- Exit

### Sample Expressions

  Infix Expression        Postfix Expression     Result
  ----------------------- -------------------- --------
  `(12 + 3) * 2`          `12 3 + 2 *`               30
  `8 + 2 * 5`             `8 2 5 * +`                18
  `20 / 5 / 2`            `20 5 / 2 /`                2
  `((2 + 3) * (4 + 5))`   `2 3 + 4 5 + *`            45

### Supported Input

-   Nonnegative, multi-digit integer operands
-   Addition (`+`), subtraction (`-`), multiplication (`*`), and
    division (`/`)
-   Parentheses, square brackets, and curly brackets for grouping
-   Spaces between tokens are allowed

### Limitations

-   Decimal operands are not supported.
-   Unary plus and minus are not supported.
-   Implicit multiplication, such as `2(3 + 4)`, is not supported.
-   Only the four basic arithmetic operators are supported.

### Conclusion

This laboratory demonstrates how stacks can be used to check balanced
brackets, convert infix expressions into postfix notation, and evaluate
postfix expressions. The dynamic array implementation also demonstrates
how a stack can grow when more elements are added and how allocated
memory is released.
