# Scheme Interpreter

A Scheme interpreter written in C, including a lexer, recursive-descent parser, and tree-walking evaluator.

## Features

- **Arithmetic**: `+`, `-`, `*`, `/`
- **Comparisons**: `=`, `<`, `>`, `<=`, `>=`
- **Booleans**: `and`, `or`, `not`, `#t` / `#f`
- **Conditionals**: `if`
- **Variables**: `define`
- **Functions**: `lambda` with multiple parameters, recursion, and parameter scoping
- **REPL and file execution**, plus a `--test` mode for running an automated test suite
- **Error recovery**: malformed input (missing arguments, unbound symbols, unbalanced parens, etc.) prints an error and returns to the prompt instead of crashing

## Build

```bash
make
```

This produces a `scheme` executable in the project root. Requires `gcc` and a POSIX-compliant environment (developed and tested on WSL).

## Run

**Interactive REPL:**
```bash
./scheme
```

**Run a `.scm` file:**
```bash
./scheme examples/double-investment.scm
```

**Run the automated test suite:**
```bash
./scheme --test tests.scm tests.expected
```

## Examples

Example programs live in [`examples/`](./examples).

Included Examples:
- **`basics.scm`** — arithmetic, multi-argument comparisons, boolean operators, and conditionals.
- **`functions.scm`** — defining functions with `lambda`, composing them, passing expressions as arguments, and parameters shadowing global variables.
- **`recursion.scm`** — factorial, fibonacci, accumulator-style recursion, and two functions that call each other (mutual recursion).
- **`double-investment.scm`** — a program that solves how many years it takes $1000 at 7% interest to double. Prints a single number.
- **`collatz.scm`** — counts the steps in a Collatz sequence.

## Testing

This project includes 136 test cases covering arithmetic, comparisons, booleans, conditionals, variables, lambdas, recursion (including a factorial and fibonacci implementation), multi-line expressions, and malformed-input error handling.

To run the tests:

```bash
./scheme --test tests.scm tests.expected
```

136 tests pass, zero memory errors and zero leaks under Valgrind

## How It Works

```
source text → lexer → tokens → parser → "Nest" trees → evaluator → result
```

- **`lexer.c`** turns raw text into an array of tokens (parentheses, numbers, symbols).
- **`parser.c`** takes those tokens and builds a tree of `Nest` structs (which include an operator and a list of operands).
- **`eval.c`** walks the tree recursively. Most operators evaluate all their operands first, then apply the operation. `lambda` and `if` are handled specially.
- **Environments** are a chain of variable-binding scopes. Every function call gets a fresh environment whose parent is the enclosing scope, which gives shadowing and variable lookup.
- **`free.c`** manually walks and frees every structure the interpreter allocates. No garbage collector.

## Known Limitations
- **No garbage collector.** Memory is freed manually and explicitly. Error-recoveries triggered via `longjmp` can leak the current expression's memory.
- **Numbers are stored as strings internally** rather than a dedicated numeric type, formatted with `%.6g`. Very large or precise values can lose accuracy.
- **No tail-call optimization.** Recursive functions use the C call stack. Deep recursion can eventually overflow it.
- **Names do not support embedded digits** (e.g. `var2` tokenizes as `var` and `2` separately). The lexer doesn't currently allow digits inside identifier names once a symbol has started.
- **No closures over local variables.** A lambda can read globals and its own parameters, but a lambda returned from another lambda cannot capture the enclosing call's locals.

## To Do
- Add `let`, `cond`, strings, and list primitives (`cons`, `car`, `cdr`, `list`).
- Allow digits within identifier names (e.g. `var2`) in the lexer.
- Closure support for environments that outlive the call that created them.
- Eliminate the error-path memory leak by tracking and freeing in-progress allocations before `longjmp`.