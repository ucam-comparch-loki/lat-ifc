// Data types used to communicate with the accelerator unit.
//
// TODO: generalise from convolution to a nested loop.

#ifndef ACCELERATOR_TYPES_H
#define ACCELERATOR_TYPES_H

typedef int32_t data_t;

// There is scope to compress the following structs by using smaller datatypes,
// but I believe the overhead to be negligible compared to the amount of work
// performed using the parameters.

// Size and shape of convolution.
typedef struct {
  uint32_t batchSize;
  uint32_t inChannels;
  uint32_t outChannels;
  uint32_t imageWidth;
  uint32_t imageHeight;
  uint32_t filterWidth;
  uint32_t filterHeight;

  // Channels (both in and out) partitioned into this many groups: default 1.
  uint32_t groups;
} conv_shape_t;


// Details of 4D activations in memory.
typedef struct {
  // An encoded ChannelMapEntry telling which memory to access.
  uint32_t memoryConfigEncoded;

  data_t*  address;

  // Distance (in bytes) between elements in each dimension. Negative offsets
  // are allowed (makes rotation/transpose trivial).
  int32_t  batchSkip;
  int32_t  channelSkip;
  int32_t  columnSkip;
  int32_t  rowSkip;
} activation_config_t;


// Details of 4D weights in memory.
typedef struct {
  // An encoded ChannelMapEntry telling which memory to access.
  uint32_t memoryConfigEncoded;

  data_t*  address;

  // Distance (in bytes) between elements in each dimension. Negative offsets
  // are allowed (makes rotation/transpose trivial).
  int32_t  inChannelSkip;
  int32_t  outChannelSkip;
  int32_t  columnSkip;
  int32_t  rowSkip;

  // Not sure if this can be removed and replaced by a channelSkip and the
  // group size. It's certainly easier for it to remain separate.
  int32_t  groupSkip;
} filter_config_t;


// All information required to specify a convolution. This is the struct to be
// sent to the accelerator.
typedef struct {
  // Size and shape of the computation.
  conv_shape_t shape;

  // Other details of how convolution is to be performed.
  uint32_t stride;    // Distance between filter windows: default 1.
  uint32_t dilation;  // Distance between filter elements: default 0.

  // Data position and layout in memory.
  activation_config_t input;
  activation_config_t output;
  filter_config_t     filters;

  // Encoded network address to send to when computation is finished.
  uint32_t notificationAddress;
} lat_parameters_t;

#endif
