//******************************************************************************
//******************************************************************************

#ifndef XROUTERERROR_H
#define XROUTERERROR_H

//******************************************************************************
//******************************************************************************
namespace xrouter
{
    enum Error
    {
      SUCCESS = 0,
      UNKNOWN_ERROR = 1002,
      BAD_REQUEST = 1004,
      BAD_CONNECTOR = 1018,
      INVALID_PARAMETERS = 1025,
      BAD_ADDRESS = 1026,
      INSUFFICIENT_FUNDS = 1027,
      INSUFFICIENT_FEE = 1028,
      UNSUPPORTED_BLOCKCHAIN = 1030,
      UNSUPPORTED_SERVICE = 1031,
      NOT_ENOUGH_NODES = 1032,
      MAXFEE_TOO_HIGH = 1033,
      UNAUTHORIZED = 1001,
      INTERNAL_SERVER_ERROR = 1002
    };

} // namespace xrouter

#endif // XROUTERERROR_H
