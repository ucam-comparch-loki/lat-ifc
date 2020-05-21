#include <stdbool.h>
#include "types.h"

#ifndef LAT_IFC_RUN_H
#define LAT_IFC_RUN_H

// Accelerate the specified computation on the Loki Accelerator Template.
// Computation proceeds asynchronously - this core can continue to do other work
// and call `lat_sync` or `lat_poll` when ready to use the result.
// WARNING: this function changes output channel 2.
void lat_accelerate(const lat_parameters_t* params);

// Stall this core until the accelerator signals that it has completed its
// computation.
// This core must be the one specified by `notification_address` in `params`.
void lat_sync(const lat_parameters_t* params);

// Check whether the accelerator has finished its computation.
// If so, call `lat_sync` to perform any cleanup.
// This core must be the one specified by `notification_address` in `params`.
bool lat_poll(const lat_parameters_t* params);

#endif // include guard
