#ifndef	__CONFIG_H
#define __CONFIG_H

#include <string>

/**
 * This class stores the configuration of this program.
 *
 * @author Darran Zhang @ codelast.com
 */

class CConfig
{
 public:
  CConfig();
  virtual ~CConfig();

 private:
  std::string currentAppPath;  // the path of the current running program 

  unsigned int webcamIndex;      // index of the webcam connected to Raspberry Pi
  bool threadRunning;	         // a flag to control the thread running status 
  unsigned int listenPort;       // local HTTP sever listen port
  std::string token;             // token to encrypt the HTTP request
  unsigned int hashWorkFactor;   // hash work factor to calculate the validate string in a HTTP request
  std::string imageDir;          // image file relative dir
  std::string fullImageDir;      // image file absolute dir
  
  /* below are the No. of the GPIO ports on Raspberry Pi used by the stepper motor */
  unsigned int pinA;
  unsigned int pinB;
  unsigned int pinC;
  unsigned int pinD;

 public:
  size_t getExecutablePath(char* buffer, size_t len);

  bool loadConfig(const std::string configFile);

  inline std::string getCurrentAppPath() const { return currentAppPath; }
  inline unsigned int getWebcamIndex() const { return webcamIndex; }
  inline bool getThreadRunning() const { return threadRunning; }
  inline void setThreadRunning(bool val) { threadRunning = val; } 
  inline unsigned int getListenPort() const { return listenPort; }
  inline std::string getToken() const { return token; }
  inline unsigned int getHashWorkFactor() const { return hashWorkFactor; }
  inline std::string getImageDir() const { return imageDir; }
  inline std::string getFullImageDir() const { return fullImageDir; }
  inline unsigned int getPinA() const { return pinA; }
  inline unsigned int getPinB() const { return pinB; }
  inline unsigned int getPinC() const { return pinC; }
  inline unsigned int getPinD() const { return pinD; }
};

#endif
