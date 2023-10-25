#ifndef INTERFACES_H_
#define INTERFACES_H_

class Interfaces
{
public:
  Interfaces() = default;
  ~Interfaces() = default;

  Interfaces(Interfaces&&) = delete;
  Interfaces(const Interfaces&) = delete;
  Interfaces& operator=(Interfaces&&) = delete;
  Interfaces& operator=(const Interfaces&) = delete;

  /**
   * Plugin X interfaces
   */
  static int  xxx_read();
  static bool xxx_write(int value);

  /**
   * Plugin Y interfaces
   */
  static float yyy_read();
  static bool  yyy_write(float value);
};

#endif /* INTERFACES_H_ */
