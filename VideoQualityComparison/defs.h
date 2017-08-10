#pragma once

#define FramesToCompare 1800
#define TESTING false
#define VIDEO_TRAIN_SET_SIZE 6000

#define VIDEO_CHECK_SET_SIZE 3000
#define MAX_THREADS 1

#define TRAINNING_TEST_INPUT_PATH "trainning_input_test_data.dat"
#define TRAINNING_CHECK_INPUT_PATH "trainning_input_check_data.dat"


#define CHECK_OUTPUT_ENCODING "check_encoding_output.dat"
#define CHECK_OUTPUT_PROFILE "check_profile_output.dat"
#define CHECK_OUTPUT_FRAMERATE "check_framerate_output.dat" 
#define CHECK_OUTPUT_BITRATE "check_bitrate_output.dat"


#define TRAINNING_OUTPUT_ENCODING "trainning_encoding_output.dat"
#define TRAINNING_OUTPUT_PROFILE "trainning_profile_output.dat"
#define TRAINNING_OUTPUT_FRAMERATE "trainning_framerate_output.dat"
#define TRAINNING_OUTPUT_BITRATE "trainning_bitrate_output.dat"

#define REBUILD_TRAINNING_DATA false

//this is for 16x9 videos scales, also determine base input nodes (ie 9216 nodes is values are 128x72)
#define SCALE_DOWN_HEIGHT 9
#define SCALE_DOWN_WIDTH 16

/*constants in the encoding cmd: 
-f is the output container as mp4
-E to prevent discrepency's cause by audio, it is kept the same as source
-e codec to use for encoding is h264
-no-loose-crop prevents pixels from being removed ie black bars on top and bottem of image
-encoder-profile is set to main, can be high and would improve overal quality to size but much slower
-no-two-pass again for speed of encoding, enabling takes 2x longer to render
*/
#define VIDEO_ENCODING_COMMAND "HandBrakeCLI -f av_mp4 -E copy -e x264 --no-loose-crop --encoder-profile main --no-two-pass"
#define LEARNING_RATE 0.000001
#define MOMENTUM 0.000005
#define BFRAMEMAX 16
#define REFFRAMEMAX 6
#define ENCODERMAX 6
#define MAXPROFILE 3
#define MAXFRAMERATE 8
//defined by classes
#define VIDEODATABASELOCATION "D:/VideoTestDataSet/allVideos/"
#define INPUT_SIZE 9