#include "xhal/XHALInterface.h"

int xhal::XHALInterface::index = 0;

xhal::XHALInterface::XHALInterface(const std::string& board_domain_name):
  m_board_domain_name(board_domain_name),
  isConnected(false)
{
  log4cplus::SharedAppenderPtr myAppender(new log4cplus::ConsoleAppender());
  std::auto_ptr<log4cplus::Layout> myLayout = std::auto_ptr<log4cplus::Layout>(new log4cplus::TTCCLayout());
  myAppender->setLayout( myLayout );
  // Following strange construction is required because it looks like log4cplus was compiled withot c++11 support...
  auto t_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("XHALInterface_"+m_board_domain_name + "_" + std::to_string(index)));
  ++index;
  m_logger = t_logger;
  m_logger.addAppender(myAppender);
  m_logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
  XHAL_DEBUG("XHAL constructor called");
  XHAL_INFO("XHAL Logger tuned up");
  try {
    this->connect();
    XHAL_INFO("XHAL Interface connected");
  }
  catch (xhal::utils::XHALRPCException &e) {
    XHAL_INFO("XHAL Interface failed to connect");
  }
}

xhal::XHALInterface::XHALInterface(const std::string& board_domain_name, log4cplus::Logger& logger):
  m_board_domain_name(board_domain_name),
  m_logger(logger),
  isConnected(false)
{
  m_logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
  XHAL_DEBUG("XHAL constructor called");
  XHAL_INFO("XHAL Logger tuned up, using external logger reference");
  try {
    this->connect();
    XHAL_INFO("XHAL Interface connected");
  }
  catch (xhal::utils::XHALRPCException &e) {
    XHAL_INFO("XHAL Interface failed to connect");
    isConnected = false;
  }
}

xhal::XHALInterface::~XHALInterface()
{
  XHAL_DEBUG("XHAL destructor called");
  this->disconnect();
  //m_logger.shutdown();
}

void xhal::XHALInterface::connect()
{
  try {
    rpc.connect(m_board_domain_name);
    isConnected = true;
    XHAL_INFO("RPC connected");
  }
  catch (wisc::RPCSvc::ConnectionFailedException &e) {
    XHAL_ERROR("Caught RPCErrorException: " << e.message.c_str());
    throw xhal::utils::XHALRPCException("RPC ConnectionFailedException: " + e.message);
  }
  catch (wisc::RPCSvc::RPCException &e) {
    XHAL_ERROR("Caught exception: " << e.message.c_str());
    throw xhal::utils::XHALRPCException("RPC exception: " + e.message);
  }
}

void xhal::XHALInterface::reconnect()
{
  this->connect();
}

void xhal::XHALInterface::disconnect()
{
  try {
    rpc.disconnect();
    XHAL_INFO("RPC disconnected");
    isConnected = false;
  }
  catch (wisc::RPCSvc::NotConnectedException &e) {
    XHAL_INFO("Caught RPCNotConnectedException: " << e.message.c_str());
  }
  catch (wisc::RPCSvc::RPCException &e) {
    XHAL_ERROR("Caught exception: " << e.message.c_str());
    throw xhal::utils::XHALRPCException("RPC exception: " + e.message);
  }
}

void xhal::XHALInterface::loadModule(const std::string& module_name, const std::string& module_version)
{
  try {
    ASSERT(rpc.load_module(module_name, module_version));
  }
  STANDARD_CATCH;
}

void xhal::XHALInterface::setLogLevel(int loglevel)
{
  switch(loglevel)
  {
    case 0:
      m_logger.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
      break;
    case 1:
      m_logger.setLogLevel(log4cplus::WARN_LOG_LEVEL);
      break;
    case 2:
      m_logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
      break;
    case 3:
      m_logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
      break;
    case 4:
      m_logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);
      break;
  }
}
