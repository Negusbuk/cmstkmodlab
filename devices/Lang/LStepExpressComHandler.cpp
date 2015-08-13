#include <string.h>

#include <iostream>

#include "LStepExpressComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
LStepExpressComHandler::LStepExpressComHandler(ioport_t ioPort)
{
  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

LStepExpressComHandler::~LStepExpressComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void LStepExpressComHandler::SendCommand( const char *commandString )
{
  if (!fDeviceAvailable) return;

  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }

  // send feed characters
  SendFeedString();
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
void LStepExpressComHandler::ReceiveString( char *receiveString )
{
  if (!fDeviceAvailable) {
    receiveString[0] = 0;
    return;
  }

  usleep( 10000 );

  int timeout = 0, readResult = 0;

  while ( timeout < 100000 )  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 ) {
      receiveString[readResult] = 0;
      break;
    }
    
    timeout++;
  }
}

//! Open I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[LStepExpressComHandler::OpenIoPort] ** ERROR: could not open device file "
              << fIoPort << "." << std::endl;
    std::cerr << "                               (probably it's not user-writable)."
              << std::endl;
    fDeviceAvailable = false;
    return;
  } else {
    // configure port with no delay
    fcntl( fIoPortFileDescriptor, F_SETFL, FNDELAY );
  }

  fDeviceAvailable = true;
}

//! Initialize I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::InitializeIoPort( void )
{
  if (!fDeviceAvailable) return;

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

#ifndef USE_FAKEIO

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

  // baud rate
  cfsetispeed( &fThisTermios, B115200 );  // input speed
  cfsetospeed( &fThisTermios, B115200 );  // output speed

  // various settings, 8N1 (no parity, 1 stopbit)
  fThisTermios.c_cflag   &= ~PARENB;
  fThisTermios.c_cflag   &= ~PARODD;
  fThisTermios.c_cflag   |=  CS8;
  fThisTermios.c_cflag   |=  HUPCL;
  fThisTermios.c_cflag   &= ~CSTOPB;
  fThisTermios.c_cflag   |=  CREAD;
  fThisTermios.c_cflag   |=  CLOCAL;
  fThisTermios.c_cflag   &= ~CRTSCTS;

  fThisTermios.c_lflag   |=  ISIG;
  fThisTermios.c_lflag   |=  ICANON;
  fThisTermios.c_lflag   |=  ECHO;
  fThisTermios.c_lflag   |=  ECHOE;
  fThisTermios.c_lflag   |=  ECHOK;
  fThisTermios.c_lflag   &= ~ECHONL;
  fThisTermios.c_lflag   |=  IEXTEN;

  fThisTermios.c_iflag   &= ~IGNBRK;
  fThisTermios.c_iflag   &= ~BRKINT;
  fThisTermios.c_iflag   &= ~IGNPAR;
  fThisTermios.c_iflag   &= ~PARMRK;
  fThisTermios.c_iflag   &= ~INPCK;

  // right i/o/l flags ??
  fThisTermios.c_iflag   &= ~ISTRIP;
  fThisTermios.c_iflag   &= ~INLCR;
  fThisTermios.c_iflag   &= ~IGNCR;
  //  fThisTermios.c_iflag   |=  ICRNL; // DO NOT ENABLE!!
  fThisTermios.c_iflag   |=  IXON;
  fThisTermios.c_iflag   &= ~IXOFF;
  fThisTermios.c_iflag   &= ~IUCLC;
  fThisTermios.c_iflag   &= ~IXANY;
  fThisTermios.c_iflag   &= ~IMAXBEL;
  
  fThisTermios.c_iflag   &= ~IUTF8;

  // right i/o/l flags?
  fThisTermios.c_oflag   |=  OPOST;
  fThisTermios.c_oflag   &= ~OLCUC;
  fThisTermios.c_oflag   &= ~OCRNL;
  fThisTermios.c_oflag   |=  ONLCR;
  fThisTermios.c_oflag   &= ~ONOCR;
  fThisTermios.c_oflag   &= ~ONLRET;
  fThisTermios.c_oflag   &= ~OFILL;
  fThisTermios.c_oflag   &= ~OFDEL;

  //   fThisTermios.c_cflag   |=  NL0;
  //   fThisTermios.c_cflag   |=  CR0;
  //   fThisTermios.c_cflag   |=  TAB0;
  //   fThisTermios.c_cflag   |=  BS0;
  //   fThisTermios.c_cflag   |=  VT0;
  //   fThisTermios.c_cflag   |=  FF0;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );
  
#endif
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void LStepExpressComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void LStepExpressComHandler::SendFeedString( void )
{
  if (!fDeviceAvailable) return;

  // feed string is <CR> ; see documentation page 4.1
  char feedString = 13;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

bool LStepExpressComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}