#include <loki/channels.h>
#include <loki/channel_io.h>
#include <loki/channel_map_table.h>

#include "lat/run.h"

void lat_accelerate(const lat_parameters_t* params) {

  // Send parameters to accelerator.
  channel_t accelerator = loki_mcast_address(MULTICAST_CORE_2, 0, 0);
  set_channel_map(2, accelerator);

  // Slightly messy at the moment. `params` contains dynamically-sized arrays,
  // so we can't just send a big contiguous block of data.
  // Also, we don't need to send the address of any array, only its content.
  loki_send(2, params->notification_address);
  loki_send(2, params->loop_count);
  loki_send_data(params->loops, params->loop_count * sizeof(loop_iteration_t), 2);
  loki_send_data(params->iteration_counts, params->loop_count * sizeof(uint32_t), 2);
  loki_send_data(&params->in1, 3 * sizeof(memory_location_t), 2);

}

void lat_sync(const lat_parameters_t* params) {
  channel_t returnAddr = params->notification_address;
  enum Channels buffer = (returnAddr >> 2) & 0x7;

  // We could also check that this is the correct core, but that seems
  // unnecessary.

  loki_receive_token(buffer);
}

bool lat_poll(const lat_parameters_t* params) {
  channel_t returnAddr = params->notification_address;
  enum Channels buffer = (returnAddr >> 2) & 0x7;

  // We could also check that this is the correct core, but that seems
  // unnecessary.

  return loki_test_channel(buffer);
}
