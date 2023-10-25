### System Components
In our system, we have 4 components.

1. `feel-features`
- Simulate features/applications that Users want to install to their system.

2. `feel-core`
- Responsible for loading/unloading Plugins, receives requests from and sends reponses
back to `feel-features`.

3. `fips` (not implemented yet in phase 1)
- Send notifications to `feel-core` to load/unload needed plugins based on events of
features installation or uninstallation. The communication channel between `fips` and
`feel-core` can be REST APIs.

4. `feel-plugins`
- Linux shared libs, aka .so shared library files which provide functionalities needed
by `feel-features`.


### System Design & Software Design
#### [Basic System Design](png/SysD - Basic Design.PNG)

#### [Detail System Design](png/SysD - Detail Design.PNG)

#### [Feel Feature] (png/SofD - Feel Feature.PNG)

#### [Feel Core & Feel Plugins interfacing](png/SofD - Feel Core - Feel Plugins.PNG)
