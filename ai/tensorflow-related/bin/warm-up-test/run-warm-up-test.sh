#!/bin/bash
# A script to run the TensorFlow warm-up test.

CURRENT_DIR=`dirname "$0"`
export TENSORFLOW_RELATED_HOME=`cd "$CURRENT_DIR/../.."; pwd`

PYTHON_BIN=`which python3.5`
if [ $? -ne 0 ]; then
    echo "Python 3.5 not found, quit"
    exit 1
fi

# run the test
$PYTHON_BIN $TENSORFLOW_RELATED_HOME/bin/warm-up-test/warm_up_test.py --model_dir $TENSORFLOW_RELATED_HOME/resource/model --warm_up_image_file /root/raspberry-pi/ai/tensorflow-related/resource/test-images/ubike.jpg --image_file $TENSORFLOW_RELATED_HOME/resource/test-images/mobike.jpg
