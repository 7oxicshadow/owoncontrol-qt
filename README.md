# owoncontrol-qt
Owon oscilloscope USB interface in QT

This is the next step from my "owoncontrol" project and provides a simple
user interface for the end user to allow control of an SDS7102v oscilloscope
via USB.

This has been created using qt-creator and has a dependancy on libusb.

Whilst functional there are a couple of known functions that do not have
any effect.

I have added the ability to take screen captures from the oscilloscope
thanks to the work done by bjohnh. Reference project available here:

https://github.com/bjonnh/owon-sds7102-protocol

The only thing that is missing more than anything is the ability to have a
live update. I think it is possible to get raw data from the scope to
display traces on the screen but at this moment in time I am unsure of the
way to tackle it.

Build
-----
Requires qt development files and libusb.

Goto the root directory and run qmake to generate a makefile.
Run make to build.

Additional
----------
The application will need to be run as root otherwise it will be unable to
access the oscilloscope via usb.

Disclaimer
----------
Use this software at your own risk. Whilst it should not be possible to 
damage any hardware as a result of using this tool, it has not undergone
any rigorous testing and software bugs can and will exist.
