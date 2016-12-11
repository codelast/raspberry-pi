#!/bin/bash
# A script to run the TensorFlow service.

CURRENT_DIR=`dirname "$0"`
export TENSORFLOW_RELATED_HOME=`cd "$CURRENT_DIR/../.."; pwd`

PYTHON_BIN=`which python3.5`
if [ $? -ne 0 ]; then
    echo "Python 3.5 not found, quit"
    exit 1
fi

WARM_UP_IMAGE_FILE_PATH=$TENSORFLOW_RELATED_HOME/resource/test-images/ubike.jpg

# start the service
$PYTHON_BIN $TENSORFLOW_RELATED_HOME/bin/tensorflow-service/tensorflow_service.py --model_dir $TENSORFLOW_RELATED_HOME/resource/model --warm_up_image_file $WARM_UP_IMAGE_FILE_PATH
