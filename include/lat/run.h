#include <stdbool.h>
#include "types.h"

// Accelerate the specified computation on the Loki Accelerator Template.
// Computation proceeds asynchronously - this core can continue to do other work
// and call `lat_sync` or `lat_poll` when ready to use the result.
// WARNING: this function changes output channel 2.
void lat_accelerate(const lat_parameters_t* params);

// Stall this core until the accelerator signals that it has completed its
// computation.
// This core must be the one specified by `notificationAddress` in `params`.
void lat_sync(const lat_parameters_t* params);

// Check whether the accelerator has finished its computation.
// If so, call `lat_sync` to perform any cleanup.
// This core must be the one specified by `notificationAddress` in `params`.
bool lat_poll(const lat_parameters_t* params);

// Execute the specified computation on a Loki core.
// Useful for debugging and benchmarking, though it will not be as efficient as
// executing native code.
void lat_compare(const lat_parameters_t* params);
