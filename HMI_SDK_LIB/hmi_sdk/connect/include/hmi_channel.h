/**
* @file			hmi_channel
* @brief		hmi会将与sdl交互的数据分类，各种类型的数据通过不同的通道传递（UI、VR、TTS、Button等），该类为所有通过的基类
* @author		fanqiang
* @date			2017-6-21
* @version		A001
* @copyright	ford
*/

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>
#include <string>
#include "socket_manager_interface.h"

namespace hmisdk {
class IMessageInterface;
class ISocketManager;
}

namespace Json {
class Value;
}

class JsonBuffer {
 public:
  JsonBuffer();

  bool getJsonFromBuffer(char *pData, int iLength, Json::Value &output);

 private:
  std::string m_szBuffer;
};


extern Json::Value g_StaticConfigJson;
extern Json::Value g_VehicleInfoJson;
extern Json::Value g_StaticResultJson;

namespace hmisdk {

class Channel: public IChannel {
 public:
  Channel(int startId, std::string Channelname);
  virtual ~Channel();

  static Json::Value ReadSpecifyJson(const char *fileName);
  static void ReadConfigJson();
  void SetCallback(IMessageInterface *pCallback);
  void onReceiveData(void *pData, int iLength);
  int RegisterReqId();
  int UnRegisterRegId();

  std::string getChannelName();
  void setSocketManager(ISocketManager *pManager, void *pHandle = NULL);
  void onOpen();
  void onChannelStatus(bool channelStatus);
  bool getchannelStatus();

 protected:
  void unRegisterComponent();
  void sendError(int resultCode, int id, std::string method, std::string message);
  void onMessage(Json::Value &jsonObj);

  void SubscribeToNotification(std::string notification);
  void UnsubscribeFromNotification(std::string notification);

 public:
  //IMessageCallback
  virtual void onRequest(Json::Value &);
  virtual void onNotification(Json::Value &);
  virtual void onResult(Json::Value &);
  virtual void onRawData(void *p, int iLength);
  virtual void onError(std::string error);
  virtual void sendJson(Json::Value &data);
  virtual void sendError(int id, Json::Value &error);
  virtual void sendResult(int id, Json::Value &result);
  virtual void sendRequest(int id, const std::string mothod, const Json::Value &params = Json::Value::null);
  virtual void sendNotification(const std::string mothod, const Json::Value &params = Json::Value::null);
  virtual void onRegistered();
  virtual void onUnregistered();
  virtual void setStaticResult(std::string attri, std::string ref, Json::Value value);
  virtual void sendResult(int id, std::string ref, Result code = RESULT_SUCCESS);
  virtual void sendError(int id, std::string ref, std::string message, Result code = RESULT_REJECTED);

  int GenerateId();
  std::string MethodName(std::string _mode, Json::Value _method);
 protected:


 protected:
  IMessageInterface *m_pCallback;
  ISocketManager *m_pSocketManager;
  void *m_pHandle;

 private:
  int m_iIDRegRequest;
  int m_iIDUnRegRequest;
  int m_iIDStart;
  int m_iGenerateId;
  std::string m_sComponentName;
  Json::Value  m_StaticResult;
  JsonBuffer m_JsonBuffer;

  bool m_bChannelStatus;

};

}

#endif // CHANNEL_H_
