#include <string.h>

#include <cstdlib>
#include <iostream>

//#####################
// TODO:
// query error codes
//#####################

#include "CoriFlowComHandler.h"

#include "CoriFlow.h"

///
///
///
CoriFlow::CoriFlow( const ioport_t ioPort )
  : VCoriFlow(ioPort),
    uDelay_(250000)
{
  comHandler_ = new CoriFlowComHandler( ioPort );
  Device_Init();
}

CoriFlow::~CoriFlow()
{
  delete comHandler_;
}

bool CoriFlow::IsCommunication( void ) const
{
  return isCommunication_;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

float CoriFlow::getTemp( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":06030421412147" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  
  return ToFloat(buffer);
}

float CoriFlow::getPres( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":06030421412148" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToFloat(buffer);
}

float CoriFlow::getMeasure( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":06030401210120" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToInt(buffer);
}



/// ///////////////////////////////////////////////////////////////////
/// some additional stuff
/// ///////////////////////////////////////////////////////////////////

float CoriFlow::getCapacity( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":0603040141014D" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToFloat(buffer);
}

std::string CoriFlow::getUnit( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":0703040160017F07" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToUnit(buffer);
}


std::string CoriFlow::getFluid( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":070304016101710A" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToUnit(buffer);
}

std::string CoriFlow::getTag( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":0703047161716610" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToUnit(buffer);
}


std::string CoriFlow::setInit( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":050301000A40" ); //set to 64 -> write permission
  
  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return buffer;
}


float CoriFlow::getDensity( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":06030474417404F" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToFloat(buffer);
}

std::string CoriFlow::setCapacity( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":080301014D457A0000" ); //set to 4000 must set INIT first

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return buffer;
}


std::string CoriFlow::setTag( void ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::test] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( ":0A03027166056E696C7300" );//set tag to "nils"

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return buffer;
}
/// //////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////

///
/// returns success flag
///
void CoriFlow::StripBuffer( char* buffer ) const {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[CoriFlow::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  #endif

  for( unsigned int c = 0; c < strlen( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
}

float CoriFlow::ToFloat(const char* buffer) const
{
  std::string temp = buffer;
//   std::cout << " > " << buffer << std::endl;
  temp.erase(0,11);
  temp.insert(0, "0x");
  const char *t = temp.c_str();

  unsigned int num;
  float f;
  sscanf(t, "%x", &num);  // assuming you checked input
  f = *((float*)&num);
//   printf("the hexadecimal 0x%08x becomes %.3f as a float\n", num, f);
  
  return f;
}

float CoriFlow::ToInt(const char* buffer) const
{
  std::string temp = buffer;
//   std::cout << " > " << buffer << std::endl;
  temp.erase(0,11);
  temp.insert(0, "0x");
  const char *t = temp.c_str();

  unsigned int num;
  int f;
  sscanf(t, "%x", &num);  // assuming you checked input
  f = *((int*)&num);
//   printf("the hexadecimal 0x%08x becomes %.3f as a float\n", num, f);

  return f;
}

std::string CoriFlow::ToUnit(const char* buffer) const
{
  std::string temp = buffer;
//   std::cout << " > " << buffer << std::endl;
  temp.erase(0,13);
  temp.insert(0, "0x");
  const char *t = temp.c_str();

  std::string newString;
  for(int i=0; i< 9; i+=2)
  {
    std::string byte = temp.substr(i,2);
    char chr = (char) (int)strtol(byte.c_str(), nullptr, 16);
    newString.push_back(chr);
  }
//   std::cout << " temp > " << temp << std::endl;

  return newString;
}

///
/// read back software version
/// to check communication with device
///
void CoriFlow::Device_Init( void ) {

  #ifdef __CORIFLOW_DEBUG
  std::cout << "[CoriFlow::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( ":0703047161716610" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );

  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp = ToUnit(buffer);
//   std::cout<<"status = "<<temp<<std::endl;
  
  if (!temp.compare("nils")) {
    std::cerr << " [CoriFlow::Device_Init] ** ERROR: Device communication problem." << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;
}