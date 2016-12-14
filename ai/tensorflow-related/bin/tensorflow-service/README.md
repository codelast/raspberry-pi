A simple, not elegant demo on how to run TensorFlow as a service, to initialize the model once, and warm-up the model several times before serving any actual client request, which will significantly reduce the consuming time of one prediction.

***这里用简单、但不优雅的代码演示了如何把TensorFlow运行为一个服务，在真正服务客户端的请求之前只初始化一次模型、预热几次模型，从而极大地减少后面的一次预测所消耗的时间。***

Follow these steps to run the test:

***按下面的步骤来运行测试程序：***

(1) start the TensorFlow service / ***启动TensorFlow服务***

```Bash
./run-tensorflow-service.sh
```
And then wait the successfully initialization of the service.

***并等待服务初始化完成。***

(2) run the client to send request to TensorFlow service / ***运行客户端，向TensorFlow服务发送请求***

```Bash
./client.sh /root/raspberry-pi/ai/tensorflow-related/resource/test-images/mobike.jpg
```

On the client side, the console output looks like:

***在客户端，你会看到类似于下面的命令行输出：***

> mountain bike, all-terrain bike, off-roader (score = 0.56671)</br>
> tricycle, trike, velocipede (score = 0.12035)</br>
> bicycle-built-for-two, tandem bicycle, tandem (score = 0.08768)</br>
> lawn mower, mower (score = 0.00651)</br>
> alp (score = 0.00387)</br>
> Prediction used time:4.171393394470215 Seconds</br>
