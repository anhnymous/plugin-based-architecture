#ifndef PLUGINX_H_
#define PLUGINX_H_

class PluginX
{
public:
  PluginX() = default;
  ~PluginX() = default;

  PluginX(PluginX&&) = delete;
  PluginX(const PluginX&) = delete;
  PluginX& operator=(PluginX&&) = delete;
  PluginX& operator=(const PluginX&) = delete;

  /**
   * giid/001
   */
  static int  xxx_read();

  /**
   * giid/002
   */
  static bool xxx_write(int value);
};

#endif /* PLUGINX_H_ */
