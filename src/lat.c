#include <loki/channels.h>
#include <loki/channel_io.h>
#include <loki/channel_map_table.h>

#include "lat/run.h"

void lat_compare(const lat_parameters_t* params) {

  // TODO: stride, dilation, groups.
  // Might remove some of these if I switch to a pure nested-loop interface.

  // TODO: Check that padding is used consistently. Here I assume that image
  // sizes refer to the output.

  // TODO: Check that I've got row and column skips the right way round. Here I
  // Use column skip to bypass one column (and not iterate through a column).

  for (int o=0; o<params->shape.outChannels; o++) {
    for (int y=0; y<params->shape.imageHeight; y++) {
      for (int x=0; x<params->shape.imageWidth; x++) {

        int sum = 0; // (Or bias)

        for (int i=0; i<params->shape.inChannels; i++) {
          for (int fy=0; fy<params->shape.filterHeight; fy++) {
            for (int fx=0; fx<params->shape.filterWidth; fx++) {

              int inPtr = (int)params->input.address +
                          params->input.channelSkip * i +
                          params->input.rowSkip * (y+fy) +
                          params->input.columnSkip * (x+fx);
              int weightPtr = (int)params->filters.address +
                              params->filters.inChannelSkip * i +
                              params->filters.outChannelSkip * o +
                              params->filters.rowSkip * fy +
                              params->filters.columnSkip * fx;

              data_t input = *((const data_t*)inPtr);
              data_t weight = *((const data_t*)weightPtr);
              sum += input * weight;

            }
          }
        }

        // Store result.
        int outPtr = (int)params->output.address +
                     params->output.channelSkip * o +
                     params->output.rowSkip * y +
                     params->output.columnSkip * x;
        *((data_t*)outPtr) += sum;

      }
    }
  }

}

void lat_accelerate(const lat_parameters_t* params) {

  // Send parameters to accelerator.
  channel_t accelerator = loki_mcast_address(MULTICAST_CORE_2, 0, 0);
  set_channel_map(2, accelerator);
  loki_send_data(params, sizeof(lat_parameters_t), 2);

}

void lat_sync(const lat_parameters_t* params) {
  channel_t returnAddr = params->notificationAddress;
  enum Channels buffer = (returnAddr >> 2) & 0x7;

  // We could also check that this is the correct core, but that seems
  // unnecessary.

  loki_receive_token(buffer);
}

bool lat_poll(const lat_parameters_t* params) {
  channel_t returnAddr = params->notificationAddress;
  enum Channels buffer = (returnAddr >> 2) & 0x7;

  // We could also check that this is the correct core, but that seems
  // unnecessary.

  return loki_test_channel(buffer);
}
