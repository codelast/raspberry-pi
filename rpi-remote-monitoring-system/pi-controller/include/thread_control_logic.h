#ifndef	__THREAD_CONTROL_LOGIC_H
#define __THREAD_CONTROL_LOGIC_H

#include <string>
#include <stdio.h>
#include <fstream>
#include <ostream>
#include <sstream>
#include "config.h"
#include "constants.h"
#include "util.h"
#include "motor_control.h"
#include "webcam_control.h"
#include "rapidjson/document.h"
#include "mongoose.h"
#include "base64.h"

/**
 * A thread which implements the main hardware controlling logic.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;
using namespace rapidjson;

MotorControl motorControl;
WebcamControl webcamControl;
extern CConfig gConfig;

/**
 * A specific JSON-RPC method handler to deal with the stepper motor control command.
 * Please refer to the API doc of Mongoose for the detail of the params.
 *
 */
static int rotateMotor(Document &doc, char *buf, int len, struct mg_rpc_request *req) {
  /* process field "direction" */
  Value::ConstMemberIterator itr = doc.FindMember(JSON_FIELD_DIRECTION.c_str());
  int direction;
  if (itr != doc.MemberEnd()) {
    direction = itr->value.GetInt();
    if (direction != CLOCKWISE && direction != COUNTER_CLOCKWISE) {
      LOG(ERROR) << "Invalid [direction] value in JSON-RPC request";
      return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
    }
  } else {
    LOG(ERROR) << "Field [" << JSON_FIELD_DIRECTION << "] missing in JSON-RPC request";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
  }

  /* process field "round" */
  itr = doc.FindMember(JSON_FIELD_ROUND.c_str());
  int round = -1;
  if (itr != doc.MemberEnd()) {
    round = itr->value.GetInt();
    if (round <= 0) {
      LOG(ERROR) << "Invalid [round] value in JSON-RPC request";
      return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
    }
  } else {
    LOG(ERROR) << "Field [" << JSON_FIELD_ROUND << "] missing in JSON-RPC request";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
  }

  LOG(INFO) << "Will rotate motor...";
  for (int i = 0; i < round; i++) {
    //motorControl.rotate(direction, 4);
  }

  // return "T"(refers to "true", bool type) to indicate that the command has been successfully executed
  return mg_rpc_create_reply(buf, len, req, "T");
}

/**
 * A specific JSON-RPC method handler to deal with the image capture command.
 * Please refer to the API doc of Mongoose for the detail of the params.
 *
 */
static int captureImage(Document &doc, char *buf, int len, struct mg_rpc_request *req) {
  LOG(INFO) << "Will capture an image ...";
  string imageFilePath;
  if (!webcamControl.captureImage(gConfig.getFullImageDir(), imageFilePath)) {
    return mg_rpc_create_reply(buf, len, req, "F");  // return "F"(refers to "false")
  }

  /* read the image file to an unsigned char array */
  ifstream ifs(imageFilePath.c_str(), ios_base::in | ios_base::binary);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Failed to open image file << [" << imageFilePath << "]";
    return mg_rpc_create_reply(buf, len, req, "F");
  }
  ifs.seekg(0, ios::end);
  size_t fileSizeInByte = ifs.tellg();
  unsigned char* data = new unsigned char[fileSizeInByte];
  ifs.read(reinterpret_cast<char*>(data), fileSizeInByte);

  /* base64-encod the data we just read */
  string encodedBase64Str = base64_encode(data, fileSizeInByte);
  delete data;
  data = NULL;

  //TODO: fix the bug here, the returned content is not right
  return mg_rpc_create_reply(buf, len , req, "s", encodedBase64Str.c_str());
}

/**
 * Uniform JSON-RPC method handler, when a JSON-RPC request comes, it will first
 * processed by this function, then some other specific method handlers.
 * Please refer to the API doc of Mongoose for the detail of the params.
 *
 */
static int methodHandler(char *buf, int len, struct mg_rpc_request *req) {
  /* use RapidJSON to parse the JSON string in the JSON-RPC call */
  string json(req->message->ptr, req->message->len);      // whole RPC message
  string methodName(req->method->ptr, req->method->len);  // method name, e.g. "rotateMotor"
  LOG(INFO) << "Whole JSON string in the reqeust: [" << json << "]";
  Document doc;
  if (doc.Parse(json.c_str()).HasParseError()) {
    LOG(ERROR) << "Invalid JSON string in the request!";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_PARSE_ERROR);
  }

  /* process field "id" */
  Value::ConstMemberIterator itr = doc.FindMember(JSON_FIELD_ID.c_str());
  int id = -1;
  if (itr != doc.MemberEnd()) {
    id = itr->value.GetInt();
  } else {
    LOG(ERROR) << "Field [" << JSON_FIELD_ID << "] missing in JSON-RPC request";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
  }

  /* process field "validate" */
  itr = doc.FindMember(JSON_FIELD_VALIDATE.c_str());
  string validate;
  if (itr != doc.MemberEnd()) {
    validate = itr->value.GetString();
    /* validate the request */
    string legalValidateStr = CUtil::generateValidateStr(gConfig.getHashWorkFactor(), id, gConfig.getToken());
    if (legalValidateStr != validate) {
      LOG(ERROR) << "Invalid JSON-RPC request due to illegal [validate] field, it should be: " << legalValidateStr;
      return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
    }
  } else {
    LOG(ERROR) << "Field [" << JSON_FIELD_VALIDATE << "] missing in JSON-RPC request";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
  }

  /* process other fields according to specific RPC method */
  if (RPC_METHOD_NAME_ROTATE_MOTOR == methodName) {
    return rotateMotor(doc, buf, len, req);
  } else if (RPC_METHOD_NAME_CAPTURE_IMAGE == methodName) {
    return captureImage(doc, buf, len, req);
  } else {
    LOG(ERROR) << "Unknown method name: [" << methodName << "]";
    return mg_rpc_create_std_error(buf, len, req, JSON_RPC_INVALID_PARAMS_ERROR);
  }
}

/**
 * HTTP event handler, it defines the handlers for all kinds of JSON-RPC request methods.
 * Please refer to the API doc of Mongoose for the detail of the params.
 *
 */
static void httpEventHandler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  static const char *methods[] = {RPC_METHOD_NAME_ROTATE_MOTOR.c_str(), RPC_METHOD_NAME_CAPTURE_IMAGE.c_str(), NULL};
  static mg_rpc_handler_t handlers[] = {methodHandler, methodHandler, NULL};
  char buf[100];

  switch (ev) {
  case NS_HTTP_REQUEST:
    {
      LOG(INFO) << "Received a JSON-RPC request, will process it...";
      mg_rpc_dispatch(hm->body.p, hm->body.len, buf, sizeof(buf),
                      methods, handlers);
      mg_printf(nc, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n"
                "Content-Type: application/json\r\n\r\n%s",
                (int) strlen(buf), buf);
      nc->flags |= NSF_SEND_AND_CLOSE;
    }
  default:
    break;
  }
}

/**
 * Thread function, to start a JSON-RPC server, init stepper motor controller,
 * webcam controller, etc.
 *
 */
void* threadCmdExec(void*) {
  /* initialize stepper motor controller */
  unsigned int pins[4] = {gConfig.getPinA(), gConfig.getPinB(), gConfig.getPinC(), gConfig.getPinD()};
  if (!motorControl.init(pins)) {
    LOG(ERROR) << "Failed to initialize stepper motor controller!";
    return NULL;
  }

  /* initialize webcam controller */
  if (!webcamControl.init(gConfig.getWebcamIndex())) {
    LOG(ERROR) << "Failed to initialize webcam controller!";
    return NULL;
  }

  /* create and configure the JSON-RPC server */
  struct mg_mgr mgr;
  struct mg_connection *nc;

  char port[8];
  memset(port, 0, sizeof(port));
  sprintf(port, "%d", gConfig.getListenPort());

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, port, httpEventHandler);
  mg_set_protocol_http_websocket(nc);

  LOG(INFO) << "Start JSON-RPC server on port [" << port << "]";

  while (gConfig.getThreadRunning()) {
    mg_mgr_poll(&mgr, 1000);
  }

  /* cleanup */
  mg_mgr_free(&mgr);
  webcamControl.cleanup();

  return NULL;
}

#endif
