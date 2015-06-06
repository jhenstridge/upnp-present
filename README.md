UPnP Present
============

This is an Ubuntu Touch app that can send photos from your phone to a
UPnP media renderer device such as a television.

Building
--------

The easiest way to build the package to run on the phone is to use the
Ubuntu SDK.

After creating the click chroot, you will need to install the
libgupnp-1.0-dev:armhf and libgupnp-av-1.0-dev:armhf packages.

Once that is done, simply open the top level CMakeLists.txt file as a
project, and use the "Publish" tab to build and install the package on
the phone.

Tested Renderers
----------------

Sony Bravia KDL-46W4500
Rygel 0.24.2 (must have the rygel-playbin plugin package installed)
Kodi 14.2 (must switch on UPnP Client in settings)
