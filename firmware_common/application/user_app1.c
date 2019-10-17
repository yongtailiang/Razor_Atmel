/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{ u8 au8Message[] = "(Have fun!)"; 
LCDMessage(LINE1_START_ADDR, au8Message);
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */

static void UserApp1SM_Idle(void)
{
static u16 u16BlinkCount=0;
 static u8 u8Counter=1;
 static u16 u8Return=0;
 int i=0,j=0;
 static u16 au16NotesRight[] = {F5, F5, F5, F5, F5, E5, D5, E5, F5, G5, A5, A5, A5, A5, A5, G5, F5, G5, A5, 
A5S, C6, F5, F5, D6, C6, A5S, A5, G5, F5, NO, NO}; 
 static u16 au16DurationRight[] = {QN, QN, HN, EN, EN, EN, EN, EN, EN, QN, QN, QN, HN, EN, EN, EN, EN, EN, EN, 
QN, HN, HN, EN, EN, EN, EN, QN, QN, HN, HN, FN}; 
 static u16 au16NoteTypeRight[] = {RT, RT, HT, RT, RT, RT, RT, RT, RT, RT, RT, RT, HT, RT, RT, RT, RT, RT, RT, 
RT, RT, HT, RT, RT, RT, RT, RT, RT, RT, HT, HT};
  static u8 u8IndexRight = 0; 
 static u32 u32RightTimer = 0; 
 static u16 u16CurrentDurationRight = 0; 
 static u16 u16NoteSilentDurationRight = 0; 
 static bool bNoteActiveNextRight = TRUE; 
 u8 u8CurrentIndex;
 if(IsTimeUp(&u32RightTimer, (u32)u16CurrentDurationRight)) 
 { 
 u32RightTimer = G_u32SystemTime1ms; 
 u8CurrentIndex = u8IndexRight; 
 /* Set up to play current note */ 
 if(bNoteActiveNextRight) 
 { 
 if(au16NoteTypeRight[u8CurrentIndex] == RT) 
 { u16CurrentDurationRight = au16DurationRight[u8CurrentIndex] - 
REGULAR_NOTE_ADJUSTMENT; 
 u16NoteSilentDurationRight = REGULAR_NOTE_ADJUSTMENT; 
 bNoteActiveNextRight = FALSE;
 } /* end RT case */ 
 else if(au16NoteTypeRight[u8CurrentIndex] == ST) 
 { u16CurrentDurationRight = STACCATO_NOTE_TIME; 
 u16NoteSilentDurationRight = au16DurationRight[u8CurrentIndex] - 
STACCATO_NOTE_TIME; 
 bNoteActiveNextRight = FALSE;
 } /* end ST case */ 
 else if(au16NoteTypeRight[u8CurrentIndex] == HT) 
 { u16CurrentDurationRight = au16DurationRight[u8CurrentIndex];
 u16NoteSilentDurationRight = 0; 
 bNoteActiveNextRight = TRUE; 
 u8IndexRight++; 
 if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16) ) 
 { 
 u8IndexRight = 0; 
 }
 }

 if(au16NotesRight[u8CurrentIndex] != NO) 
 { 
 PWMAudioSetFrequency(BUZZER1, au16NotesRight[u8CurrentIndex]); 
 PWMAudioOn(BUZZER1); 
 } 
 else 
 { 
 PWMAudioOff(BUZZER1); 
 } 
 } 
 else 
 { PWMAudioOff(BUZZER1); 
 u32RightTimer = G_u32SystemTime1ms; 
 u16CurrentDurationRight = u16NoteSilentDurationRight;
 bNoteActiveNextRight = TRUE; 
 u8IndexRight++; 
 if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16) ) 
 { 
 u8IndexRight = 0; 
 } 
 } 

 } 
 
 
 
 
 
 if(u8Counter!=10000)
 { 
   u16BlinkCount++;

  for(i=0;i<100;i++)
   {
     if(u8Counter==1)
     {
     LedOn(RED);
     LedOff(GREEN);
     LedOff(YELLOW);
     LedOff(ORANGE);
     }   
    else
     LedOff(RED);
    if(u8Counter==2)
    {
    LedOn(ORANGE);
    LedOff(RED);
    LedOff(GREEN);
    LedOff(YELLOW);
    }
   else
     LedOff(ORANGE);
    if(u8Counter==3)
    {
    LedOn(YELLOW);
    LedOff(ORANGE);
    LedOff(GREEN);
    LedOff(RED);
    }
   else
     LedOff(YELLOW);
    if(u8Counter==4)
    {
    LedOn(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    }
   else
     LedOff(GREEN);
  }

  {   if(WasButtonPressed(BUTTON3))
 {
 u8Return=1;
 ButtonAcknowledge(BUTTON3);
 }
 if(WasButtonPressed(BUTTON2))
 {
 u8Return=2;
  ButtonAcknowledge(BUTTON2);
 }
 if(WasButtonPressed(BUTTON1))
 {
 u8Return=3;
  ButtonAcknowledge(BUTTON1);
 }
 if(WasButtonPressed(BUTTON0))
 {
 u8Return=4;
  ButtonAcknowledge(BUTTON0);
 }
  }
   if(u16BlinkCount==350)
{u16BlinkCount=0;
   if (u8Return==u8Counter)
  {
    i=u8Counter;
    u8Counter=rand()%4+1;
    while(i==u8Counter)
    {u8Counter=rand()%4+1;}
   
  }
  else
  {  
   LedOn(WHITE);
   u8Counter=10000;   
  }
}
}
}
 /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
