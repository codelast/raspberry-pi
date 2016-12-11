#!/bin/bash
# A script to send request to the TensorFlow service.

if [ $# -lt 1 ]; then
    echo "Usage: <inference-image-file-path>"
    exit 1
fi

INFERENCE_IMAGE_FILE_PATH=$1

# send request to the service
curl "http://127.0.0.1:8080/?image_path=$INFERENCE_IMAGE_FILE_PATH"
