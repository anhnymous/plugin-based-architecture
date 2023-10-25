#ifndef PLUGINY_H_
#define PLUGINY_H_

class PluginY
{
public:
  PluginY() = default;
  ~PluginY() = default;

  PluginY(PluginY&&) = delete;
  PluginY(const PluginY&) = delete;
  PluginY& operator=(PluginY&&) = delete;
  PluginY& operator=(const PluginY&) = delete;

  static float yyy_read();
  static bool  yyy_write(float value);

};

#endif /* PLUGINY_H_ */
