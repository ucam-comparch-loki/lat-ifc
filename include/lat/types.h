// Data types used to communicate with the accelerator unit.

#ifndef LAT_IFC_TYPES_H
#define LAT_IFC_TYPES_H

#include <loki/channels.h>

typedef int32_t data_t;

// There is scope to compress the following structs by using smaller datatypes,
// but I believe the overhead to be negligible compared to the amount of work
// performed using the parameters.

// Location of data in memory.
typedef struct {
  // An encoded ChannelMapEntry telling where to look in the memory system.
  // This includes details such as which memory banks to access, and whether to
  // access them in cache or scratchpad modes.
  channel_t memory_config;

  data_t*  address;
} memory_location_t;

// Details of how one loop iterates through the data. Each loop may iterate (or
// not) through each of the two input arrays and one output array.
// All distances are in bytes.
typedef struct {
  size_t in1_stride;
  size_t in2_stride;
  size_t out_stride;
} loop_iteration_t;

// All information required to specify a computation. This is the struct to be
// sent to the accelerator. Computation begins when the final parameter arrives.
// TODO: allow pieces of the struct to be reused (useful for tiled computation).
//   * Input/output locations change every tile
//   * Iteration counts usually constant, but may vary from tile to tile
//   * Loop nest is constant for a whole layer
typedef struct {
  // The accelerator will send a token to this address when computation is
  // finished.
  channel_t notification_address;

  // Loop nest.
  uint32_t loop_count;
  loop_iteration_t* loops;

  // Number of iterations of each loop.
  uint32_t* iteration_counts;

  // Details of where to find the input/output data.
  memory_location_t in1;
  memory_location_t in2;
  memory_location_t out;
} lat_parameters_t;

#endif // include guard
