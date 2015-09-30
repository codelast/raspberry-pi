#!/bin/bash
# A script to test the JSON-RPC server running on pi-controller.

SERVER_ADDRESS=127.0.0.1
SERVER_PORT=56098

which curl
if [ $? -ne 0 ]; then
    echo "*** You must install [curl] before running this test!"
    exit 1
fi

PI_CONTROLLER_PROCESS_NAME=pi_controller
PROCESS_COUNT=`ps -ef | grep pi_controller | grep -v grep | wc -l`
if [ $PROCESS_COUNT -ne 1 ]; then
    echo "*** You must run pi_controller before running this test!"
    exit 1
fi

# rotate the stepper motor
echo "Making JSON-RPC call to pi_controller..."
curl -d '{"id":1443367226,"method":"rotateMotor","direction":1,"round":150,"validate":"38e4de9df62d7a7395340a22226a041944c1fdc570d05c356f9d9053b5271c4d1932d474a2bee342488a379058d96b4e5dcd13c15536566a242c5af70fa97ea7"}' $SERVER_ADDRESS:$SERVER_PORT
echo ""

# capture an image
echo "Making JSON-RPC call to pi_controller..."
curl -d '{"id":1443367226,"method":"captureImage","validate":"38e4de9df62d7a7395340a22226a041944c1fdc570d05c356f9d9053b5271c4d1932d474a2bee342488a379058d96b4e5dcd13c15536566a242c5af70fa97ea7"}' $SERVER_ADDRESS:$SERVER_PORT
echo ""
