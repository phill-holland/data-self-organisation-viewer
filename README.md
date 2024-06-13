<b>Example of Vulkan Boiler Plate Initalisation and Example Triangle</b>

This has been primarily written on an Ubuntu based development environment!

To Run (within VSCode);

<ul>
<li><b>Ensure project is open within the development container</b></li>
<li><b>Hit F5</b></li>
</ul>

This application was written in C++ using VSCode and includes a devcontainer setup file, which should create a self-contained environment with all the below requirements installed;

<ul>
<li>CMake</li>
<li>Vulkan</li>
<li>Shaders</li>
</ul>

It should be possible to build and run the code with alternative development environments, outside of VSCode.

Host Machine Requirements;

To download VSCode;

https://code.visualstudio.com/download

The VSCode development container plugin is installed;

https://code.visualstudio.com/docs/remote/containers

Docker must also be installed;

https://docs.docker.com/get-docker/

This application, however is configured with linux based containers, and will not work correctly on Windows without modification.

<b>Installing Vulkan Shaders</b>

See below for how to install the Vulkan-SDK (without it, the code will not compile!)

https://vulkan.lunarg.com/doc/view/latest/linux/getting_started_ubuntu.html


