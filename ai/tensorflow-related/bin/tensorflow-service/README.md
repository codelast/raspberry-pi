A simple, not elegant demo on how to run TensorFlow as a service, to initialize the model once, and warm-up the model several times before serving any actual client request, which will significantly reduce the consuming time of one prediction.

Follow these steps to run the test:
(1) start the TensorFlow service

```Bash
./run-tensorflow-service.sh
```
And then wait the successfully initialization of the service.

(2) run the client to send request to TensorFlow service

```Bash
./client.sh /root/raspberry-pi/ai/tensorflow-related/resource/test-images/mobike.jpg
```

On the client side, the console output looks like:

> mountain bike, all-terrain bike, off-roader (score = 0.56671)
> tricycle, trike, velocipede (score = 0.12035)
> bicycle-built-for-two, tandem bicycle, tandem (score = 0.08768)
> lawn mower, mower (score = 0.00651)
> alp (score = 0.00387)
> Prediction used time:4.171393394470215 Seconds
