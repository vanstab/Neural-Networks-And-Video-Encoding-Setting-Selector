#pragma once

#define FramesToCompare 1800
#define TESTING false
#define MAX_ARRAY 64
#define TRAINNING_INFO_PATH "trainning_data.dat"
#define REBUILD_TRAINNING_DATA false

//this is for 16x9 videos scales, also determine base input nodes (ie 9216 nodes is values are 128x72)
#define SCALE_DOWN_HEIGHT 72
#define SCALE_DOWN_WIDTH 128

/*constants in the encoding cmd: 
-f is the output container as mp4
-E to prevent discrepency's cause by audio, it is kept the same as source
-e codec to use for encoding is h264
-no-loose-crop prevents pixels from being removed ie black bars on top and bottem of image
-encoder-profile is set to main, can be high and would improve overal quality to size but much slower
-no-two-pass again for speed of encoding, enabling takes 2x longer to render
*/
#define VIDEO_ENCODING_COMMAND "HandBrakeCLI -f av_mp4 -E copy -e x264 --no-loose-crop --encoder-profile main --no-two-pass"
#define EPHOC_TRAIN_MAX 100
#define LEARNING_RATE 0.05
#define MOMENTUM 0.5
#define BFRAMEMAX 16
#define REFFRAMEMAX 6
//defined by classes
#define OUTPUT_NEURONS 3

#define HIDDEN_NEURONS 3
#define SAMPLE_COUNT 196
#define MAX_DECIMALS 1000000000000000