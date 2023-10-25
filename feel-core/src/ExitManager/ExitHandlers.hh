#ifndef EXITHANDLERS_H_
#define EXITHANDLERS_H_

class ExitHandler {
public:
  using exit_handler_t = void (*)();

  explicit ExitHandler() = delete;
  ~ExitHandler() = default;
  static void stopLogger(void);
};

#endif /* EXITHANDLERS_H_ */
