/***************************************************************************************
* COMPONENT: feel-plugins
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: PluginX.hh
* License: GPLv3
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************/

#ifndef PLUGINPROVIDER_H_
#define PLUGINPROVIDER_H_

#include <vector>
#include <string>
#include <iostream>
using giid = std::string;
using string = std::string;

/**
 * This class is to provide Plugin template to plugin users.
 */
class PluginProvider
{
public:
  PluginProvider() = default;
  virtual ~PluginProvider() = default;

  PluginProvider(PluginProvider&&) = delete;
  PluginProvider(const PluginProvider&) = delete;
  PluginProvider& operator=(PluginProvider&&) = delete;
  PluginProvider& operator=(const PluginProvider&) = delete;

  /**
   * Get all giids this plugin supports.
   */
  virtual std::vector<giid> get_giids() const noexcept = 0;

  /**
   * Read interface
   */
  virtual string read(const string& giid) const noexcept = 0;

  /**
   * Write interface
   */
  virtual bool write(const string& giid, const string& value) = 0;
};

using create_f  = PluginProvider* (*)();
using destroy_f = void (*)(PluginProvider*);

extern "C" PluginProvider* create(void);
extern "C" void destroy(PluginProvider*);

#endif /* PLUGINPROVIDER_H_ */
