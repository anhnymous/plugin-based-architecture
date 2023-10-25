#ifndef LOGCOMMONDEFS_H_
#define LOGCOMMONDEFS_H_

typedef uint8_t level;
namespace LogLevel
{
  static constexpr level None = 0;
  static constexpr level Debug = 10;
  static constexpr level Info = 20;
  static constexpr level Warning = 100;
  static constexpr level Error = 200;
  static constexpr level Critical = 250;
  static constexpr level Incident = 255;
}

typedef uint16_t option;
namespace LogOption
{
  static constexpr option Off = 0b0;
  static constexpr option Console = 0b1;
  static constexpr option File = 0b10;
  static constexpr option IncludeExecTime = 0b100;
  static constexpr option UseColor = 0b1000;
}

typedef uint8_t LogCategory;
#define kTechnical 0x01
#define kIncident  0x02

#endif /* LOGCOMMONDEFS_H_ */
