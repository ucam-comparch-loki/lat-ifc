# Loki accelerator template (LAT) interface

A driver for the experimental machine learning accelerator built into the `accelerator` branch of [lokisim](https://github.com/ucam-comparch-loki/lokisim/tree/accelerator).

By default, this library assumes that Loki has been configured with two cores and one accelerator on each tile. The configuration of the accelerator itself, however, should not matter.

![](structure.png)

The computation performed is a generic loop nest, where the innermost loop body is a simple multiply-accumulate. The computation takes two inputs and one output, and they may overlap. This is sufficient for convolution and linear/fully-connected neural network layers.

The two innermost loops are parallelised on the PE array:
* If the accelerator is configured with `--accelerator-broadcast-rows`, the first input should be constant in the penultimate loop
* If the accelerator is configured with `--accelerator-broadcast-columns`, the second input should be constant in the innermost loop

Additional parallelism may be exploited by using multiple accelerator tiles, and coordinating them in software.

## Prerequisites

Requires [libloki](https://github.com/ucam-comparch-loki/libloki).

## Build

Requires Loki compiler.

```
LIBLOKI_DIR=path/to/libloki make
```
