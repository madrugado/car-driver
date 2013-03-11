#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

namespace deviceCommand
{  
  enum Command
  {
    SET_DUTY_CYCLE  = '1',
    SET_DEFAULTS    = '2',
    SET_WFC_TIMEOUT = '3',
    RESET_WFC_TIMER = '4'
  };
}  

namespace deviceRetCode
{
  enum RetCode
  {
    OK              = 'o',
    BAD_ARGS        = 'b',
    UNKNOWN_COMMAND = 'u'
  };
}

#endif //COMMANDS_H_INCLUDED
