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
#ifndef PLUGINX_H_
#define PLUGINX_H_

#include "PluginProvider.hh"

class PluginX: public PluginProvider
{
public:
  PluginX() = default;
  virtual ~PluginX() = default;

  PluginX(PluginX&&) = delete;
  PluginX(const PluginX&) = delete;
  PluginX& operator=(PluginX&&) = delete;
  PluginX& operator=(const PluginX&) = delete;

  virtual std::vector<giid> get_giids() const noexcept = 0;

  /**
   * giid/001
   */
  virtual string read(const string& giid) const noexcept = 0;
  
  /**
   * giid/002
   */
  virtual bool write(const string& giid, const string& value) = 0;
};

#endif /* PLUGINX_H_ */
