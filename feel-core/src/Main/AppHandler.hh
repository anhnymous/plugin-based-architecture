#ifndef APPHANDLER_H_
#define APPHANDLER_H_

class AppHandler
{
public:
  enum class RetCode
  {
    INIT_OK,
    INIT_ERROR
  };

public:
  explicit AppHandler(void) = default;
  virtual ~AppHandler() = default;

  virtual bool isMatchedArguments(int argc, char** argv) = 0;
  virtual RetCode start(int argc, char** argv) = 0;
};

#endif /* APPHANDLER_H_ */
