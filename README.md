### FFmpeg utility stripRotate

 Using this utility is possible to strip the "rotate" metadata tag or to transpose videos.
 A recent version of FFmpeg should be available setting PATH environment variable.

Usage: stripRotate [args]

If argument is 0,1,2 or 3, transpose is performed:

* 0 = 90CounterCLockwise and Vertical Flip (default)
* 1 = 90Clockwise
* 2 = 90CounterClockwise
* 3 = 90Clockwise and Vertical Flip

If there is no argument the application will strip the "rotate" meta tag when it exists and autorotate video according (fast transcode). This is an essential pre-processing step, as some openCV algorithms, mainly object detection algorithms are not rotation invariant.
The app will always look for INPUT files on the data/files folder and will OUTPUT to data/fixed and data/info folders.

* The utility searches for rotation metadata using ffprobe command, afterwards saves all metadata in Json format in the data/info folder:

ffprobe -v quiet -print_format json -show_format -show_streams "data/files/INPUT.mp4" > "data/info/INPUT.mp4.json"

* Then, after determining orientation if it is not 0 degrees(e.g:no rotate tag), it will strip "rotate" metadata and traspose video accordingly:

ffmpeg -i data/files/INPUT.mp4 -metadata:s:v rotate="0" -c:v libx264 -crf 23 -acodec copy data/fixed/INPUT.mp4

* It can also be used to rotate videos arbitrarely:

example: stripRotate 2, will transpose every file in data/files folder  90 degree CounterClockwise to data/fixed folder using:

ffmpeg -i INPUT.mp4 -vf "transpose=2" -c:v libx264 -crf 23 -acodec copy OUTPUT.mp4