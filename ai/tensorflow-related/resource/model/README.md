To run the programs successfully, you should download the TensorFlow Inception model and extract the package to this directory.
Because the model file size is a bit large so I didn't add it to repo.
NOTE that the model url link may change over time, so the following commands are not guaranteed to be correct.

```Bash
mkdir ~/raspberry-pi/ai/tensorflow-related/resource/model
cd ~/raspberry-pi/ai/tensorflow-related/resource/model
wget http://download.tensorflow.org/models/image/imagenet/inception-2015-12-05.tgz
tar xf inception-2015-12-05.tgz
```
