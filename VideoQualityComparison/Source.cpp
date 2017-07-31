/***
This code was used orginal to build my first large expected output data set, looking into b and r frames. However I was not able to find any relationships while traing so I decided to move to a less complex idea

**/


//TrainningSet* train = new TrainningSet();
//VideoQualityComparison* vid = new VideoQualityComparison();
//VideoData* video = new VideoData("D:/VideoTestDataSet/Comparision/pair18org.mov", "D:/VideoTestDataSet/Comparision/pair18re2.mp4", 0, 0, 0, 0, 0);
//string in = "HandBrakeCLI -f av_mp4 -E copy -e x264 --no-loose-crop --encoder-preset fast --encoder-level 4.0 --encoder-profile main --no-two-pass -b 1000 --stop-at duration:15 --crop 0:0:0:0 ";
//double quality = 0.0, temp = 0.0;
/*
VideoAnalysis* vidAnl = new VideoAnalysis();
ofstream  pixelChangefile;
pixelChangefile.open("videoPixelAnalysis.txt", ios_base::app);
//int bestR, bestB;
for (int i = 164; i <= 191; i++){
cout << "Analysing: " << i << "..." << endl;
stringstream spath;
spath << "D:/VideoTestDataSet/allVideos/pair" << i << "org.*";
String pathOrg = spath.str();
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile(pathOrg.c_str(), &FindFileData);
if (hFind == INVALID_HANDLE_VALUE){
cout << "file not found" << endl;

}
else{
spath.str("");
spath << "D:/VideoTestDataSet/allVideos/" << FindFileData.cFileName;
pathOrg = spath.str();
VideoCapture* cap = new VideoCapture(pathOrg);
pixelChangefile << cap->get(CAP_PROP_FRAME_COUNT) - 1 << ",";
double temp[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT] = { {} };
vidAnl->scaleDownAndAnalysis(cap, temp);
for (int x = 0; x < SCALE_DOWN_WIDTH; x++){
for (int y = 0; y < SCALE_DOWN_HEIGHT; y++){
pixelChangefile << temp[x][y] << ",";
}
}
}
pixelChangefile << endl;
}

/*
else{
spath.str("");
spath << in << " ";
spath << " -i D:/VideoTestDataSet/allVideos/" << FindFileData.cFileName;

for (int r = 1; r <= 6; r++){
for (int b = 1; b <= 16; b++){
spath << " -x ref=" << r << ":bframes=" << b << " -o D:/VideoTestDataSet/allVideos/testing.mp4";
pathOrg = spath.str();
system(pathOrg.c_str());
video->setPathOrg("D:/VideoTestDataSet/allVideos/" + string(FindFileData.cFileName));
video->setPathRe("D:/VideoTestDataSet/allVideos/testing.mp4");
temp = vid->compareQuality(video);
if (temp > quality){
bestR = r;
bestB = b;
quality = temp;
}
}
}

trainfile << "Video: " << i << ", Quality: " << quality << ", ref: " << bestR << ", b: " << bestB << endl;
quality = 0.0;
}
cout << i << endl;
}
trainfile.close();*/