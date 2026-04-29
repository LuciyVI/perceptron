# Perceptron

Small C demo that trains a single-layer perceptron for the boolean function
`!x1 && x2 && (!x3 || x4)`.

## Build

```sh
clang perceptron.c -lm -o main
```

No third-party libraries are required.

## Run

```sh
./main
```

## Output

- Prints two ASCII graphs with the training error by epoch.
- Writes `data.json` for the step activation run.
- Writes `data2.json` for the smooth activation run.
