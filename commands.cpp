/*
 * commands.cpp
 *
 *  Created on: May 13, 2017
 *      Author: tstern
 */
#include "board.h"
#include "commands.h"
#include "command.h"
#include "main.h"
#include "Reset.h"

#define COMMANDS_PROMPT ("ok\r\n")
sCmdUart UsbUart;


CMD_STR(help,"Displays this message");
CMD_STR(version,"Displays the firmware version");
CMD_STR(syslog, "turns on/off syslogging");
CMD_STR(boot, "Enters the bootloader");
CMD_STR(M100, "M100 xx – Turns FET output on where xx is 0-15");
CMD_STR(M101, "M100 xx – Turns FET output off where xx is 0-15");
CMD_STR(M102, "M102 xx ddd – Turns FET output xx (0-15) on with ddd (0-100) duty cycle");
CMD_STR(M104, "M104 xx - Turns FET xx (0-15) on for 100us then back off");
CMD_STR(M106, "M106 ii – Reads Digital input ii (0-5) which are inputs D0-D5 on PCB");


//List of supported commands
sCommand Cmds[] =
{
      COMMAND(help),
	  COMMAND(version),
	  COMMAND(boot),
	  COMMAND(syslog),
	  COMMAND(M100),
	  COMMAND(M101),
	  COMMAND(M102),
	  COMMAND(M104),
	  COMMAND(M106),

      {"",0,""}, //End of list signal
};

static int M106_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int pin;
   float duty;

   if (argc == 1)
   {
	   int val;
	   pin=atoi(argv[0]);
	   if (pin >5 || pin <0)
	   {
		   CommandPrintf(ptrUart, "Wrong pin number");
		   return -1;
	   }
	   pinMode(pin, INPUT);
	   val=digitalRead(pin);
	   LOG("pin %d value is %d", pin,val);
	   CommandPrintf(ptrUart, "%d\n\r", val);
	   return 0;
   }
    CommandPrintf(ptrUart,"usage 'M106 0' ");
    return -1;
}

static int M102_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int pin;
   float duty;

   if (argc == 2)
   {
	   int val;
	   pin=atoi(argv[0]);
	   duty=atof(argv[1]);
	   if (pin >15 || pin <0)
	   {
		   CommandPrintf(ptrUart, "Wrong pin number");
		   return -1;
	   }
	   if (duty>100 || duty<0)
	   {
		   CommandPrintf(ptrUart, "duty must be between 0.0 and 100.0");
		   return -1;
	   }
	   val=int(duty*4096.0+0.5);
	   LOG("PWM value is %d", val);
	   pca9685.setPin(pin, val,true);
	   return 0;
   }
    CommandPrintf(ptrUart,"usage 'on xx' where xx is output to turn on");
    return -1;
}

static int M100_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int pin;

   if (argc == 1)
   {
	   pin=atoi(argv[0]);
	   if (pin >15 || pin <0)
	   {
		   CommandPrintf(ptrUart, "Wrong pin number");
		   return -1;
	   }
	   pca9685.setPinOn(pin);
	   return 0;
   }
    CommandPrintf(ptrUart,"usage 'on xx' where xx is output to turn on");
    return -1;
}

static int M104_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int pin;

   if (argc == 1)
   {
	   pin=atoi(argv[0]);
	   if (pin >15 || pin <0)
	   {
		   CommandPrintf(ptrUart, "Wrong pin number");
		   return -1;
	   }
	   pca9685.setPinOn(pin);
	   delay(100);
	   pca9685.setPinOff(pin);
	   return 0;
   }
    CommandPrintf(ptrUart,"usage 'on xx' where xx is output to turn on");
    return -1;
}

static int M101_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int pin;

   if (argc == 1)
   {
	   pin=atoi(argv[0]);
	   if (pin >15 || pin <0)
	   {
		   CommandPrintf(ptrUart, "Wrong pin number");
		   return -1;
	   }
	   pca9685.setPinOff(pin);
	   return 0;
   }
    CommandPrintf(ptrUart,"usage 'on xx' where xx is output to turn on");
    return -1;
}

static int version_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
	 CommandPrintf(ptrUart, "%s", FW_VERSION);
	 return 0;
}
static int syslog_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   int i;

   if (argc == 0)
   {
	   if (SysLogIsEnabled())
	   {
		   SysLogDisable();
	   }else
	   {
		   SysLogEnable();
		   LOG("logging enabled");
	   }
	   return 0;
   }

   if (argc ==1)
   {
	   i=atoi(argv[0]);
	   if (0 == i)
	   {
		   SysLogDisable();
	   }else
	   {
		   SysLogEnable();
		   LOG("logging enabled");
	   }
	   return 0;
   }
   CommandPrintf(ptrUart,"use with no arguments to toggle, 'syslog 1' -enables, 'syslog 0' -disables");
   return -1;
}

static int boot_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   initiateReset(250);
}


// print out the help strings for the commands
static int help_cmd(sCmdUart *ptrUart,int argc, char * argv[])
{
   sCommand cmd_list;
   int i;

   //now let's parse the command
   i=0;
   memcpy(&cmd_list, &Cmds[i], sizeof(sCommand));
   while(cmd_list.function!=0)
   {

      CommandPrintf(ptrUart,(cmd_list.name));
      CommandPrintf(ptrUart,(" - "));
      CommandPrintf(ptrUart,(cmd_list.help));
      CommandPrintf(ptrUart,("\n\r"));
      i=i+1;
      memcpy(&cmd_list, &Cmds[i], sizeof(sCommand));
   }
   return 0;
}

uint8_t kbhit(void)
{
   return SerialUSB.available();
   //return SerialUSB.peek() != -1;
}
uint8_t getChar(void)
{
   return SerialUSB.read();
}
uint8_t putch(char data)
{
   return SerialUSB.write((uint8_t)data);
}


void commandsInit(void)
{
   CommandInit(&UsbUart, kbhit, getChar, putch ,NULL); //set up the UART structure

   SerialUSB.print("\n\rPower Up\n\r");
   SerialUSB.print(COMMANDS_PROMPT);
}

int commandsProcess(void)
{
   return CommandProcess(&UsbUart,Cmds,' ',COMMANDS_PROMPT);
}



