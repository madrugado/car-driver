#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

namespace deviceCommand
{  
  enum Command
  {
    SET_DUTY_CYCLE  = '1',
    SET_DEFAULTS    = '2',
    SET_WFC_TIMEOUT = '3',
    RESET_WFC_TIMER = '4',
    GET_SONAR_IL    = '9' // get impulse length from sonar
  };
}  

namespace deviceRetCode
{
  enum RetCode
  {
	OOK = 'o', //OK is causing error for some reason
	BAD_ARGS = 'b',
	UNKNOWN_COMMAND = 'u'
  };

}

#endif //COMMANDS_H_INCLUDED
