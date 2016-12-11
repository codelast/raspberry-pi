To run the programs successfully, you should download the TensorFlow Inception model and extract the package to this directory.
Because the model file size is a bit large so I didn't add it to repo.
NOTE that the model url link may change over time, so the following commands are not guaranteed to be correct.

为了能成功运行该项目下的所有程序，你应该先下载TensorFlow的Inception模型，并解压到该目录下。
由于模型文件比较大，所以我没有把它添加到代码库中。
注意：随着时间的推移，下面命令中的模型URL链接可能会更改，所以不保证正确。

```Bash
mkdir ~/raspberry-pi/ai/tensorflow-related/resource/model
cd ~/raspberry-pi/ai/tensorflow-related/resource/model
wget http://download.tensorflow.org/models/image/imagenet/inception-2015-12-05.tgz
tar xf inception-2015-12-05.tgz
```
