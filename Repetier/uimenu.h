/*
    This file is part of Repetier-Firmware.

    Repetier-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Repetier-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Repetier-Firmware.  If not, see <http://www.gnu.org/licenses/>.

*/
#if !defined(_UI_MENU_H)
#define _UI_MENU_H

/*moved to uilang.h
#define cUP "\001"
#define cDEG "\002"
#define cSEL "\003"
#define cUNSEL "\004"
#define cTEMP "\005"
#define cFOLD "\006"
#define cARROW "\176"
*/
/*
The menu configuration uses dynamic strings. These dynamic strings can contain
a placeholder for special values. During print these placeholder are exchanged
by their current value. Everything else is printed exactly as written.

A placeholder always has 3 chars. It starts with a % followed by 2 characters
defining the value. You can use any placeholder in any position, also it doesn't
always make sense.

Special Characters
 constant   description
 cUP        Folder up arrow
 cDEG       Degree mark
 cSEL       Selected
 cUNSEL     Unselected
 cTEMP      Thermometer symbol
 cFOLD      Folder symbol

List of placeholder:
%%% : The % char
%% :  The % char (also)

%?<c> : Conditional. Print c if current char is not c. Allows avoiding duplicate character, like space

acceleration
%ax : X acceleration during print moves
%ay : Y acceleration during print moves
%az : Z acceleration during print moves
%aX : X acceleration during travel moves
%aY : Y acceleration during travel moves
%aZ : Z acceleration during travel moves
%aj : Max. jerk
%aJ : Max. Z-jerk

debug
%do : Debug echo state.
%di : Debug info state.
%de : Debug error state.
%dd : Debug dry run state.

extruder
%ec : Current extruder temperature
%eIc : Current extruder temperature integer (shorter)
%eb : Current heated bed temperature
%e0..9 : Temp. of extruder 0..9
%er : Extruder relative mode
%Ec : Target temperature of current extruder
%Eb : Target temperature of heated bed
%E0-9 : Target temperature of extruder 0..9

feed rate
%fx : Max. feedrate x direction
%fy : Max. feedrate y direction
%fz : Max. feedrate z direction
%fe : Max. feedrate current extruder
%fX : Homing feedrate x direction
%fY : Homing feedrate y direction
%fZ : Homing feedrate z direction
%Fs : Fan speed
%Fi : ignore M106 commands state

inactivity
%is : Stepper inactive time in minutes
%ip : Max. inactive time in minutes

random stuff
%os : Status message
%oe : Error message
%oB : Buffer length
%om : Speed multiplier
%of : flow multiplier
%oc : Connection baudrate
%o0..9 : Output level extruder 0..9 is % including %sign.
%oC : Output level current extruder
%ob : Output level heated bed
%PN : Printer name
%on : current extruder number (1,2,3...)
%oS : servo position
%oY : babysteps counter
%oC : Bed coating thickness

stops
%sx : State of x min endstop.
%sX : State of x max endstop.
%sy : State of y min endstop.
%sY : State of y max endstop.
%sz : State of z min endstop.
%sZ : State of z max endstop.

steps
%Sx : Steps per mm x direction
%Sy : Steps per mm y direction
%Sz : Steps per mm z direction
%Se : Steps per mm current extruder

totals
%Ut : Shows printing time
%Uf : Shows total filament usage

extruder position
%x0 : X position
%x1 : Y position
%x2 : Z position
%x3 : Current extruder position
%x4 : Printed since temperature on in meters (for filament usage)

extruder parameters
%X0..9 : Extruder selected marker
%Xi : PID I gain
%Xp : PID P gain
%Xd : PID D gain
%Xm : PID drive min
%XM : PID drive max
%XD : PID max
%Xw : Extruder watch period in seconds
%Xh : Extruder heat manager (BangBang/PID)
%Xa : Advance K value
%Xl : Advance L value
%Xx : x offset in steps
%Xy : y offset in steps
%Xf : Extruder max. start feedrate
%XF : Extruder max. feedrate
%XA : Extruder max. acceleration

delta stuff
%y0-3 : same as %y0-3 back calculated from delta position steps
%Y0-3 : raw delta position steps (no round off error to display it)
%yD : delta printer low tower distance
%YL : delta print envelope radius Limit
%yx : low towers x offset mm
%yy : low towers y offset mm
%Yx : low towers x offset steps
%Yy : low towers y offset steps
%yX : high (Z) tower x offset mm
%yY : high (Z) tower y offset mm
%YX : high (Z) tower x offset steps
%YY : high (Z) tower y offset steps
*/

#if UI_DISPLAY_TYPE != NO_DISPLAY


// Define precision for temperatures. With small displays only integer values fit.
#ifndef UI_TEMP_PRECISION
#if UI_COLS>16
#define UI_TEMP_PRECISION 1
#else
#define UI_TEMP_PRECISION 0
#endif
#endif

/* ============= PAGES DEFINITION =============

If you are not iside a menu, the firmware displays pages with information.
Especially if you have only a small display it is convenient to have
more then one information page.
*/

/* Define your pages using dynamic strings. To define a page use
UI_PAGE4(name,row1,row2,row3,row4);
for 4 row displays and
UI_PAGE2(name,row1,row2);
for 2 row displays. You can add additional pages or change the default pages like you want.
*/

#if UI_ROWS>=6 && UI_DISPLAY_TYPE == DISPLAY_U8G

 //graphic main status

   UI_PAGE6(ui_page1,"\xa %e0/%E0\xb0 X:%x0",
   #if NUM_EXTRUDER > 1 && MIXING_EXTRUDER == 0
     "\xa %e1/%E1\xb0 Y:%x1",
   #elif HAVE_HEATED_BED
     "\xe %eb/%Eb\xb0 Y:%x1",
  #else
     "             Y:%x1",
   #endif
   #if HAVE_HEATED_BED && NUM_EXTRUDER > 1 && MIXING_EXTRUDER == 0
     "\xe %eb/%Eb\xb0 Z:%x2",
   #else
     "Flow:\xfd %of%%%   Z:%x2",
   #endif
   "Mul: %om%%% \xfd   E: %x4m", "Buf: %oB", "%os")

  #if EEPROM_MODE != 0
    UI_PAGE4(ui_page2,UI_TEXT_PRINT_TIME,"%Ut",UI_TEXT_PRINT_FILAMENT,"%Uf m")
    #define UI_PRINTTIME_PAGES ,&ui_page2
    #define UI_PRINTTIME_COUNT 1
  #else
    #define UI_PRINTTIME_PAGES
    #define UI_PRINTTIME_COUNT 0
  #endif
  /*
  Merge pages together. Use the following pattern:
  #define UI_PAGES {&name1,&name2,&name3}
  */
  #define UI_PAGES {&ui_page1 UI_PRINTTIME_PAGES}
  // How many pages do you want to have. Minimum is 1.
  #define UI_NUM_PAGES 1+UI_PRINTTIME_COUNT

#elif UI_ROWS >= 4
 #if HAVE_HEATED_BED
 #if NUM_EXTRUDER > 0
//   UI_PAGE4(ui_page1,cTEMP "%ec/%Ec" cDEG "B%eB/%Eb" cDEG,"Z:%x2  Buf : %oB","Mul: %om   Flow: %of","%os")
   UI_PAGE4(ui_page1,cTEMP "%ec/%Ec" cDEG "B%eB/%Eb" cDEG,"Z:%x2  Buf : %oB","Mul: %om   E:%x4","%os")
#else
//   UI_PAGE4(ui_page1,"B%eB/%Eb" cDEG,"Z:%x2  Buf : %oB","Mul: %om   Flow: %of","%os")
   UI_PAGE4(ui_page1,"B%eB/%Eb" cDEG,"Z:%x2  Buf : %oB","Mul: %om   E:%x4","%os")
#endif
   //UI_PAGE4(ui_page1,UI_TEXT_PAGE_EXTRUDER,UI_TEXT_PAGE_BED,UI_TEXT_PAGE_BUFFER,"%os");
 #else
 #if NUM_EXTRUDER > 0
   UI_PAGE4(ui_page1,UI_TEXT_PAGE_EXTRUDER,"Z:%x2 mm",UI_TEXT_PAGE_BUFFER,"%os")
   #else
   UI_PAGE4(ui_page1,"","Z:%x2 mm",UI_TEXT_PAGE_BUFFER,"%os")
   #endif
 #endif
  UI_PAGE4(ui_page2,"X:%x0 mm","Y:%x1 mm","Z:%x2 mm","%os")
//UI_PAGE4(ui_page2,"dX:%y0 mm %sX","dY:%y1 mm %sY","dZ:%y2 mm %sZ","%os");
 #if NUM_EXTRUDER>0
    UI_PAGE4(ui_page3,UI_TEXT_PAGE_EXTRUDER1
 #else
    UI_PAGE4(ui_page3
 #endif
 #if NUM_EXTRUDER>1 && MIXING_EXTRUDER == 0
   ,UI_TEXT_PAGE_EXTRUDER2
 #endif
 #if NUM_EXTRUDER>2 && MIXING_EXTRUDER == 0
  ,UI_TEXT_PAGE_EXTRUDER3
 #endif
 #if HAVE_HEATED_BED
   ,UI_TEXT_PAGE_BED
 #endif
 #if (NUM_EXTRUDER >= 3 && MIXING_EXTRUDER == 0 && !HAVE_HEATED_BED) || (NUM_EXTRUDER==2 && MIXING_EXTRUDER == 0 && HAVE_HEATED_BED==true)
   ,"%os"
 #elif (NUM_EXTRUDER == 2 && MIXING_EXTRUDER == 0) || ((NUM_EXTRUDER == 1 || MIXING_EXTRUDER == 1) && HAVE_HEATED_BED)
   ,"","%os"
 #elif (NUM_EXTRUDER == 1 || MIXING_EXTRUDER == 1) || (NUM_EXTRUDER == 0 &&  HAVE_HEATED_BED)
   ,"","","%os"
 #elif NUM_EXTRUDER == 0
   ,"","","","%os"
 #endif
 )
 #if EEPROM_MODE != 0
  UI_PAGE4(ui_page4,UI_TEXT_PRINT_TIME,"%Ut",UI_TEXT_PRINT_FILAMENT,"%Uf m")
  #define UI_PRINTTIME_PAGES ,&ui_page4
  #define UI_PRINTTIME_COUNT 1
 #else
  #define UI_PRINTTIME_PAGES
  #define UI_PRINTTIME_COUNT 0
 #endif
/*
Merge pages together. Use the following pattern:
#define UI_PAGES {&name1,&name2,&name3}
*/
 #define UI_PAGES {&ui_page1, &ui_page2, &ui_page3 UI_PRINTTIME_PAGES}
// How many pages do you want to have. Minimum is 1.
 #define UI_NUM_PAGES 3+UI_PRINTTIME_COUNT
#else
#if HAVE_HEATED_BED
UI_PAGE2(ui_page1,UI_TEXT_PAGE_EXTRUDER,UI_TEXT_PAGE_BED)
#else
UI_PAGE2(ui_page1,UI_TEXT_PAGE_EXTRUDER,"%os")
#endif
UI_PAGE2(ui_page2,"X:%x0 Y:%x1","%os")
UI_PAGE2(ui_page3,"Z:%x2 mm","%os")
/*
Merge pages together. Use the following pattern:
#define UI_PAGES {&name1,&name2,&name3}
*/
#define UI_PAGES {&ui_page1,&ui_page2,&ui_page3}
// How many pages do you want to have. Minimum is 1.
#define UI_NUM_PAGES 3
#endif
/* ============ MENU definition ================

The menu works the same as pages. In addion you need to define what the lines do
or where to jump to. For that reason, the menu structure needs to be entered in
reverse order. Starting from the leaves, the menu structure is defined.
*/

/*
At first define all actions available in the menu. The actions define, what the
next/previous button will do. All menu actions work the same:
next/previous changes the value
ok sets the value if not already done and goes back to previous menu.
*/

// Error menu

UI_MENU_ACTION2(ui_menu_error,UI_ACTION_DUMMY,UI_TEXT_ERROR,"%oe")

// Filament change wizard

#if FEATURE_RETRACTION
#if UI_ROWS >= 4
UI_WIZARD4(ui_wiz_filamentchange, UI_ACTION_WIZARD_FILAMENTCHANGE, UI_TEXT_WIZ_CH_FILAMENT1, UI_TEXT_WIZ_CH_FILAMENT2, UI_TEXT_WIZ_CH_FILAMENT3, UI_TEXT_CLICK_DONE)
UI_WIZARD4(ui_wiz_jamwaitheat, UI_ACTION_WIZARD_JAM_WAITHEAT, UI_TEXT_WIZ_WAITTEMP1, UI_TEXT_WIZ_WAITTEMP2,"",cTEMP "%ec/%Ec" cDEG)
UI_WIZARD4(ui_wiz_jamreheat, UI_ACTION_WIZARD_JAM_REHEAT, UI_TEXT_WIZ_REHEAT1, UI_TEXT_WIZ_REHEAT2,"",cTEMP "%ec" cDEG)
#else
UI_WIZARD2(ui_wiz_filamentchange, UI_ACTION_WIZARD_FILAMENTCHANGE, UI_TEXT_WIZ_CH_FILAMENT1, UI_TEXT_CLICK_DONE)
UI_WIZARD2(ui_wiz_jamwaitheat, UI_ACTION_WIZARD_JAM_WAITHEAT, UI_TEXT_WIZ_WAITTEMP1, UI_TEXT_WIZ_WAITTEMP2)
UI_WIZARD2(ui_wiz_jamreheat, UI_ACTION_WIZARD_JAM_REHEAT, UI_TEXT_WIZ_REHEAT1, UI_TEXT_WIZ_REHEAT2)
#endif
#endif

// **** Positions submenus

#if UI_ROWS >= 4
UI_MENU_ACTION4C(ui_menu_xpos,UI_ACTION_XPOSITION,UI_TEXT_ACTION_XPOSITION4)
UI_MENU_ACTION4C(ui_menu_ypos,UI_ACTION_YPOSITION,UI_TEXT_ACTION_YPOSITION4)
UI_MENU_ACTION4C(ui_menu_zpos,UI_ACTION_ZPOSITION,UI_TEXT_ACTION_ZPOSITION4)
UI_MENU_ACTION4C(ui_menu_zpos_notest,UI_ACTION_ZPOSITION_NOTEST,UI_TEXT_ACTION_ZPOSITION4)
UI_MENU_ACTION4C(ui_menu_xpos_fast,UI_ACTION_XPOSITION_FAST,UI_TEXT_ACTION_XPOSITION_FAST4)
UI_MENU_ACTION4C(ui_menu_ypos_fast,UI_ACTION_YPOSITION_FAST,UI_TEXT_ACTION_YPOSITION_FAST4)
UI_MENU_ACTION4C(ui_menu_zpos_fast,UI_ACTION_ZPOSITION_FAST,UI_TEXT_ACTION_ZPOSITION_FAST4)
UI_MENU_ACTION4C(ui_menu_zpos_fast_notest,UI_ACTION_ZPOSITION_FAST_NOTEST,UI_TEXT_ACTION_ZPOSITION_FAST4)
#define EPOS_ROWS UI_TEXT_ACTION_EPOSITION_FAST2,UI_TEXT_PAGE_EXTRUDER,"%Uf m " UI_TEXT_PRINTED
UI_MENU_ACTION4C(ui_menu_epos,UI_ACTION_EPOSITION,EPOS_ROWS)
#else
UI_MENU_ACTION2C(ui_menu_xpos,UI_ACTION_XPOSITION,UI_TEXT_ACTION_XPOSITION2)
UI_MENU_ACTION2C(ui_menu_ypos,UI_ACTION_YPOSITION,UI_TEXT_ACTION_YPOSITION2)
UI_MENU_ACTION2C(ui_menu_zpos,UI_ACTION_ZPOSITION,UI_TEXT_ACTION_ZPOSITION2)
UI_MENU_ACTION2C(ui_menu_zpos_notest,UI_ACTION_ZPOSITION_NOTEST,UI_TEXT_ACTION_ZPOSITION2)
UI_MENU_ACTION2C(ui_menu_xpos_fast,UI_ACTION_XPOSITION_FAST,UI_TEXT_ACTION_XPOSITION_FAST2)
UI_MENU_ACTION2C(ui_menu_ypos_fast,UI_ACTION_YPOSITION_FAST,UI_TEXT_ACTION_YPOSITION_FAST2)
UI_MENU_ACTION2C(ui_menu_zpos_fast,UI_ACTION_ZPOSITION_FAST,UI_TEXT_ACTION_ZPOSITION_FAST2)
UI_MENU_ACTION2C(ui_menu_zpos_fast_notest,UI_ACTION_ZPOSITION_FAST_NOTEST,UI_TEXT_ACTION_ZPOSITION_FAST2)
UI_MENU_ACTION2C(ui_menu_epos,UI_ACTION_EPOSITION,UI_TEXT_ACTION_EPOSITION_FAST2)
#endif

/*
Next step is to define submenus leading to the action.
*/

// **** Positionening menu
UI_MENU_ACTIONCOMMAND(ui_menu_back,UI_TEXT_BACK,UI_ACTION_BACK)
#if UI_HAS_BACK_KEY==0
#define UI_MENU_ADDCONDBACK &ui_menu_back,
#define UI_MENU_BACKCNT 1
#else
#define UI_MENU_ADDCONDBACK
#define UI_MENU_BACKCNT 0
#endif
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_home_all,UI_TEXT_HOME_ALL,UI_ACTION_HOME_ALL,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_home_x,UI_TEXT_HOME_X,UI_ACTION_HOME_X,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_home_y,UI_TEXT_HOME_Y,UI_ACTION_HOME_Y,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_home_z,UI_TEXT_HOME_Z,UI_ACTION_HOME_Z,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONSELECTOR(ui_menu_go_xpos,UI_TEXT_X_POSITION,ui_menu_xpos)
UI_MENU_ACTIONSELECTOR(ui_menu_go_ypos,UI_TEXT_Y_POSITION,ui_menu_ypos)
UI_MENU_ACTIONSELECTOR(ui_menu_go_zpos,UI_TEXT_Z_POSITION,ui_menu_zpos)
UI_MENU_ACTIONSELECTOR(ui_menu_go_zpos_notest,UI_TEXT_Z_POSITION,ui_menu_zpos_notest)
UI_MENU_ACTIONSELECTOR(ui_menu_go_epos,UI_TEXT_E_POSITION,ui_menu_epos)
#if !UI_SPEEDDEPENDENT_POSITIONING
UI_MENU_ACTIONSELECTOR(ui_menu_go_xfast,UI_TEXT_X_POS_FAST,ui_menu_xpos_fast)
UI_MENU_ACTIONSELECTOR(ui_menu_go_yfast,UI_TEXT_Y_POS_FAST,ui_menu_ypos_fast)
UI_MENU_ACTIONSELECTOR(ui_menu_go_zfast,UI_TEXT_Z_POS_FAST,ui_menu_zpos_fast)
UI_MENU_ACTIONSELECTOR(ui_menu_go_zfast_notest,UI_TEXT_Z_POS_FAST,ui_menu_zpos_fast_notest)
#define UI_SPEED 2
#define UI_SPEED_X ,&ui_menu_go_xfast,&ui_menu_go_xpos
#define UI_SPEED_Y ,&ui_menu_go_yfast,&ui_menu_go_ypos
#define UI_SPEED_Z ,&ui_menu_go_zfast,&ui_menu_go_zpos
#define UI_SPEED_Z_NOTEST ,&ui_menu_go_zfast_notest,&ui_menu_go_zpos_notest
#else
#define UI_SPEED 1
#define UI_SPEED_X ,&ui_menu_go_xpos
#define UI_SPEED_Y ,&ui_menu_go_ypos
#define UI_SPEED_Z ,&ui_menu_go_zpos
#define UI_SPEED_Z_NOTEST ,&ui_menu_go_zpos_notest
#endif
#if FEATURE_SERVO > 0 && UI_SERVO_CONTROL > 0
  UI_MENU_CHANGEACTION(ui_menu_servopos, UI_TEXT_SERVOPOS, UI_ACTION_SERVOPOS)
  #define SERVOPOS_COUNT 1
  #define SERVOPOS_ENTRY ,&ui_menu_servopos
#else
  #define SERVOPOS_COUNT 0
  #define SERVOPOS_ENTRY
#endif

#if DRIVE_SYSTEM != DELTA     //Positioning menu for non-delta
#define UI_MENU_POSITIONS {UI_MENU_ADDCONDBACK &ui_menu_home_all,&ui_menu_home_x,&ui_menu_home_y,&ui_menu_home_z UI_SPEED_X UI_SPEED_Y UI_SPEED_Z ,&ui_menu_go_epos SERVOPOS_ENTRY}
UI_MENU(ui_menu_positions,UI_MENU_POSITIONS,5 + 3 * UI_SPEED + UI_MENU_BACKCNT + SERVOPOS_COUNT)
#else                   //Positioning menu for delta (removes individual x,y,z homing)
#define UI_MENU_POSITIONS {UI_MENU_ADDCONDBACK &ui_menu_home_all  UI_SPEED_X UI_SPEED_Y UI_SPEED_Z ,&ui_menu_go_epos SERVOPOS_ENTRY}
UI_MENU(ui_menu_positions,UI_MENU_POSITIONS,2 + 3 * UI_SPEED + UI_MENU_BACKCNT + SERVOPOS_COUNT)
#endif

// **** Delta calibration menu
#if Z_HOME_DIR > 0
UI_MENU_ACTIONCOMMAND(ui_menu_set_measured_origin,UI_TEXT_SET_MEASURED_ORIGIN,UI_ACTION_SET_MEASURED_ORIGIN)
#define UI_MENU_DELTA {UI_MENU_ADDCONDBACK &ui_menu_home_all UI_SPEED_Z_NOTEST,&ui_menu_set_measured_origin}
UI_MENU(ui_menu_delta,UI_MENU_DELTA,2 + UI_SPEED + UI_MENU_BACKCNT)
#endif

// **** Bed leveling menu
#ifdef SOFTWARE_LEVELING
UI_MENU_ACTIONCOMMAND(ui_menu_set_p1,UI_TEXT_SET_P1,UI_ACTION_SET_P1)
UI_MENU_ACTIONCOMMAND(ui_menu_set_p2,UI_TEXT_SET_P2,UI_ACTION_SET_P2)
UI_MENU_ACTIONCOMMAND(ui_menu_set_p3,UI_TEXT_SET_P3,UI_ACTION_SET_P3)
UI_MENU_ACTIONCOMMAND(ui_menu_calculate_leveling,UI_TEXT_CALCULATE_LEVELING,UI_ACTION_CALC_LEVEL)
#define UI_MENU_LEVEL {UI_MENU_ADDCONDBACK &ui_menu_set_p1,&ui_menu_set_p2,&ui_menu_set_p3,&ui_menu_calculate_leveling UI_SPEED_X UI_SPEED_Y UI_SPEED_Z}
UI_MENU(ui_menu_level,UI_MENU_LEVEL,4+3*UI_SPEED+UI_MENU_BACKCNT)
#endif

// **** Extruder menu

UI_MENU_CHANGEACTION(ui_menu_ext_temp0,UI_TEXT_EXTR0_TEMP,UI_ACTION_EXTRUDER0_TEMP)
UI_MENU_CHANGEACTION(ui_menu_ext_temp1,UI_TEXT_EXTR1_TEMP,UI_ACTION_EXTRUDER1_TEMP)
#if NUM_EXTRUDER>2 && MIXING_EXTRUDER == 0
UI_MENU_CHANGEACTION(ui_menu_ext_temp2,UI_TEXT_EXTR2_TEMP,UI_ACTION_EXTRUDER2_TEMP)
#endif
UI_MENU_CHANGEACTION(ui_menu_bed_temp, UI_TEXT_BED_TEMP,UI_ACTION_HEATED_BED_TEMP)
UI_MENU_ACTIONCOMMAND(ui_menu_ext_sel0,UI_TEXT_EXTR0_SELECT,UI_ACTION_SELECT_EXTRUDER0)
UI_MENU_ACTIONCOMMAND(ui_menu_ext_sel1,UI_TEXT_EXTR1_SELECT,UI_ACTION_SELECT_EXTRUDER1)
#if NUM_EXTRUDER>2 && MIXING_EXTRUDER == 0
UI_MENU_ACTIONCOMMAND(ui_menu_ext_sel2,UI_TEXT_EXTR2_SELECT,UI_ACTION_SELECT_EXTRUDER2)
#endif
UI_MENU_ACTIONCOMMAND(ui_menu_ext_off0,UI_TEXT_EXTR0_OFF,UI_ACTION_EXTRUDER0_OFF)
UI_MENU_ACTIONCOMMAND(ui_menu_ext_off1,UI_TEXT_EXTR1_OFF,UI_ACTION_EXTRUDER1_OFF)
#if NUM_EXTRUDER>2 && MIXING_EXTRUDER == 0
UI_MENU_ACTIONCOMMAND(ui_menu_ext_off2,UI_TEXT_EXTR2_OFF,UI_ACTION_EXTRUDER2_OFF)
#endif
UI_MENU_ACTIONCOMMAND(ui_menu_ext_origin,UI_TEXT_EXTR_ORIGIN,UI_ACTION_RESET_EXTRUDER)
#if NUM_EXTRUDER==2 && MIXING_EXTRUDER == 0
#define UI_MENU_EXTCOND &ui_menu_ext_temp0,&ui_menu_ext_temp1,&ui_menu_ext_off0,&ui_menu_ext_off1,&ui_menu_ext_sel0,&ui_menu_ext_sel1,
#define UI_MENU_EXTCNT 6
#elif NUM_EXTRUDER > 2 && MIXING_EXTRUDER == 0
#define UI_MENU_EXTCOND &ui_menu_ext_temp0,&ui_menu_ext_temp1,&ui_menu_ext_temp2,&ui_menu_ext_off0,&ui_menu_ext_off1,&ui_menu_ext_off2,&ui_menu_ext_sel0,&ui_menu_ext_sel1,&ui_menu_ext_sel2,
#define UI_MENU_EXTCNT 9
#else
#define UI_MENU_EXTCOND &ui_menu_ext_temp0,&ui_menu_ext_off0,
#define UI_MENU_EXTCNT 2
#endif
#if HAVE_HEATED_BED
#define UI_MENU_BEDCOND &ui_menu_bed_temp,
#define UI_MENU_BEDCNT 1
#else
#define UI_MENU_BEDCOND
#define UI_MENU_BEDCNT 0
#endif

#define UI_MENU_EXTRUDER {UI_MENU_ADDCONDBACK UI_MENU_BEDCOND UI_MENU_EXTCOND &ui_menu_go_epos,&ui_menu_ext_origin}
UI_MENU(ui_menu_extruder,UI_MENU_EXTRUDER,UI_MENU_BACKCNT+UI_MENU_BEDCNT+UI_MENU_EXTCNT+2)

// **** SD card menu

// **** Quick menu
#if PS_ON_PIN > -1
UI_MENU_ACTIONCOMMAND(ui_menu_quick_power,UI_TEXT_POWER,UI_ACTION_POWER)
#define MENU_PSON_COUNT 1
#define MENU_PSON_ENTRY ,&ui_menu_quick_power
#else
#define MENU_PSON_COUNT 0
#define MENU_PSON_ENTRY
#endif
#if CASE_LIGHTS_PIN >= 0
UI_MENU_ACTIONCOMMAND(ui_menu_toggle_light,UI_TEXT_LIGHTS_ONOFF,UI_ACTION_LIGHTS_ONOFF)
#define UI_TOOGLE_LIGHT_ENTRY ,&ui_menu_toggle_light
#define UI_TOGGLE_LIGHT_COUNT 1
#else
#define UI_TOOGLE_LIGHT_ENTRY
#define UI_TOGGLE_LIGHT_COUNT 0
#endif
UI_MENU_ACTIONCOMMAND(ui_menu_quick_preheat_pla,UI_TEXT_PREHEAT_PLA,UI_ACTION_PREHEAT_PLA)
UI_MENU_ACTIONCOMMAND(ui_menu_quick_preheat_abs,UI_TEXT_PREHEAT_ABS,UI_ACTION_PREHEAT_ABS)
UI_MENU_ACTIONCOMMAND(ui_menu_quick_cooldown,UI_TEXT_COOLDOWN,UI_ACTION_COOLDOWN)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_quick_origin,UI_TEXT_SET_TO_ORIGIN,UI_ACTION_SET_ORIGIN,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_quick_stopstepper,UI_TEXT_DISABLE_STEPPER,UI_ACTION_DISABLE_STEPPER,0,MENU_MODE_PRINTING)
#if FEATURE_BABYSTEPPING
UI_MENU_CHANGEACTION(ui_menu_quick_zbaby,UI_TEXT_Z_BABYSTEPPING,UI_ACTION_Z_BABYSTEPS)
#define BABY_CNT 1
#define BABY_ENTRY ,&ui_menu_quick_zbaby
#else
#define BABY_CNT 0
#define BABY_ENTRY
#endif
UI_MENU_CHANGEACTION(ui_menu_quick_speedmultiply,UI_TEXT_SPEED_MULTIPLY,UI_ACTION_FEEDRATE_MULTIPLY)
UI_MENU_CHANGEACTION(ui_menu_quick_flowmultiply,UI_TEXT_FLOW_MULTIPLY,UI_ACTION_FLOWRATE_MULTIPLY)
#ifdef DEBUG_PRINT
UI_MENU_ACTIONCOMMAND(ui_menu_quick_debug,"Write Debug",UI_ACTION_WRITE_DEBUG)
#define DEBUG_PRINT_COUNT 1
#define DEBUG_PRINT_EXTRA ,&ui_menu_quick_debug
#else
#define DEBUG_PRINT_COUNT 0
#define DEBUG_PRINT_EXTRA
#endif
#if FEATURE_RETRACTION
UI_MENU_ACTIONCOMMAND(ui_menu_quick_changefil,UI_TEXT_CHANGE_FILAMENT,UI_ACTION_WIZARD_FILAMENTCHANGE)
#define UI_CHANGE_FIL_CNT 1
#define UI_CHANGE_FIL_ENT ,&ui_menu_quick_changefil
#else
#define UI_CHANGE_FIL_CNT 0
#define UI_CHANGE_FIL_ENT
#endif

#define UI_MENU_QUICK {UI_MENU_ADDCONDBACK &ui_menu_home_all BABY_ENTRY ,&ui_menu_quick_speedmultiply,&ui_menu_quick_flowmultiply UI_TOOGLE_LIGHT_ENTRY UI_CHANGE_FIL_ENT,&ui_menu_quick_preheat_pla,&ui_menu_quick_preheat_abs,&ui_menu_quick_cooldown,&ui_menu_quick_origin,&ui_menu_quick_stopstepper MENU_PSON_ENTRY DEBUG_PRINT_EXTRA}
UI_MENU(ui_menu_quick,UI_MENU_QUICK,8+BABY_CNT+UI_MENU_BACKCNT+MENU_PSON_COUNT+DEBUG_PRINT_COUNT+UI_TOGGLE_LIGHT_COUNT+UI_CHANGE_FIL_CNT)

// **** Bed Coating Menu

#if UI_BED_COATING
UI_MENU_ACTION2C(ui_menu_nocoating_action,  UI_ACTION_DUMMY,UI_TEXT_BED_COATING_SET UI_TEXT_NOCOATING )
UI_MENU_ACTION2C(ui_menu_buildtak_action,  UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET UI_TEXT_BUILDTAK)
UI_MENU_ACTION2C(ui_menu_kapton_action,  UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET UI_TEXT_KAPTON)
UI_MENU_ACTION2C(ui_menu_bluetape_action, UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET UI_TEXT_BLUETAPE)
UI_MENU_ACTION2C(ui_menu_pettape_action, UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET UI_TEXT_PETTAPE)
UI_MENU_ACTION2C(ui_menu_gluestick_action, UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET UI_TEXT_GLUESTICK)
UI_MENU_ACTION2C(ui_menu_coating_custom, UI_ACTION_DUMMY, UI_TEXT_BED_COATING_SET " %oCmm")

UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_nocoating,UI_TEXT_NOCOATING, UI_ACTION_NOCOATING,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_buildtak,UI_TEXT_BUILDTAK, UI_ACTION_BUILDTAK,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_kapton,UI_TEXT_KAPTON, UI_ACTION_KAPTON,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_bluetape,UI_TEXT_BLUETAPE, UI_ACTION_BLUETAPE,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_pettape,UI_TEXT_PETTAPE, UI_ACTION_PETTAPE,0,MENU_MODE_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_adjust_gluestick,UI_TEXT_GLUESTICK, UI_ACTION_GLUESTICK,0,MENU_MODE_PRINTING)
UI_MENU_CHANGEACTION_FILTER(ui_menu_adjust_custom,UI_TEXT_COATING_CUSTOM,UI_ACTION_COATING_CUSTOM,0,MENU_MODE_PRINTING)
#define UI_MENU_ADJUST {UI_MENU_ADDCONDBACK &ui_menu_adjust_nocoating,&ui_menu_adjust_buildtak,&ui_menu_adjust_kapton,&ui_menu_adjust_bluetape,&ui_menu_adjust_pettape,&ui_menu_adjust_gluestick,&ui_menu_adjust_custom}
UI_MENU(ui_menu_adjust,UI_MENU_ADJUST,7+UI_MENU_BACKCNT)
#define UI_MENU_COATING_CNT 1
#define UI_MENU_COATING_COND &ui_menu_prepare,
UI_MENU_SUBMENU_FILTER(ui_menu_prepare, UI_TEXT_BED_COATING, ui_menu_adjust,0,MENU_MODE_PRINTING)

#else
#define UI_MENU_COATING_CNT 0
#define UI_MENU_COATING_COND
#endif

// **** Fan menu

#if FAN_PIN>-1 && FEATURE_FAN_CONTROL
UI_MENU_CHANGEACTION(ui_menu_fan_fanspeed, UI_TEXT_ACTION_FANSPEED,UI_ACTION_FANSPEED)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_fan_off,UI_TEXT_FAN_OFF,UI_ACTION_FAN_OFF,MENU_MODE_FAN_RUNNING,0)
UI_MENU_ACTIONCOMMAND(ui_menu_fan_25,UI_TEXT_FAN_25,UI_ACTION_FAN_25)
UI_MENU_ACTIONCOMMAND(ui_menu_fan_50,UI_TEXT_FAN_50,UI_ACTION_FAN_50)
UI_MENU_ACTIONCOMMAND(ui_menu_fan_75,UI_TEXT_FAN_75,UI_ACTION_FAN_75)
UI_MENU_ACTIONCOMMAND(ui_menu_fan_full,UI_TEXT_FAN_FULL,UI_ACTION_FAN_FULL)
UI_MENU_ACTIONCOMMAND(ui_menu_fan_ignoreM106,UI_TEXT_IGNORE_M106,UI_ACTION_IGNORE_M106)
#define UI_MENU_FAN {UI_MENU_ADDCONDBACK &ui_menu_fan_fanspeed,&ui_menu_fan_off,&ui_menu_fan_25,&ui_menu_fan_50,&ui_menu_fan_75,&ui_menu_fan_full,&ui_menu_fan_ignoreM106}
UI_MENU(ui_menu_fan,UI_MENU_FAN,7+UI_MENU_BACKCNT)
UI_MENU_SUBMENU(ui_menu_fan_sub,UI_TEXT_FANSPEED,ui_menu_fan)
#define UI_MENU_FAN_COND &ui_menu_fan_sub,
#define UI_MENU_FAN_CNT 1
#else
#define UI_MENU_FAN_COND
#define UI_MENU_FAN_CNT 0
#endif

// **** SD card menu

#if SDSUPPORT

UI_MENU_HEADLINE(ui_menu_sd_askstop_head,UI_TEXT_STOP_PRINT)
UI_MENU_ACTIONCOMMAND(ui_menu_sd_askstop_no,UI_TEXT_NO,UI_ACTION_BACK)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_askstop_yes,      UI_TEXT_YES,     UI_ACTION_SD_STOP | UI_ACTION_TOPMENU,     MENU_MODE_SD_PRINTING, 0)
#define UI_MENU_SD_ASKSTOP {&ui_menu_sd_askstop_head,&ui_menu_sd_askstop_no,&ui_menu_sd_askstop_yes}
UI_MENU(ui_menu_sd_askstop,UI_MENU_SD_ASKSTOP,3)

#define UI_MENU_SD_FILESELECTOR {&ui_menu_back}
UI_MENU_FILESELECT(ui_menu_sd_fileselector,UI_MENU_SD_FILESELECTOR,1)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_printfile, UI_TEXT_PRINT_FILE,     UI_ACTION_SD_PRINT,    MENU_MODE_SD_MOUNTED,  MENU_MODE_SD_PRINTING)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_pause,     UI_TEXT_PAUSE_PRINT,    UI_ACTION_SD_PAUSE,    MENU_MODE_SD_PRINTING, MENU_MODE_SD_PAUSED)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_continue,  UI_TEXT_CONTINUE_PRINT, UI_ACTION_SD_CONTINUE, MENU_MODE_SD_PAUSED,   0)
// two versions of stop. Second is with security question since pausing can trigger stop with bad luck!
//UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_stop,      UI_TEXT_STOP_PRINT,     UI_ACTION_SD_STOP,     MENU_MODE_SD_PRINTING, 0)
UI_MENU_SUBMENU_FILTER(ui_menu_sd_stop, UI_TEXT_STOP_PRINT,ui_menu_sd_askstop, MENU_MODE_SD_PRINTING, 0 )
#define SD_PRINTFILE_ENTRY &ui_menu_sd_printfile,
#define SD_PRINTFILE_ENTRY_CNT 1
#if SDCARDDETECT > -1
#define UI_MOUNT_CNT 0
#define UI_MOUNT_CMD
#else
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_unmount,UI_TEXT_UNMOUNT_CARD,UI_ACTION_SD_UNMOUNT,MENU_MODE_SD_MOUNTED,0)
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_mount,UI_TEXT_MOUNT_CARD,UI_ACTION_SD_MOUNT,0,MENU_MODE_SD_MOUNTED)
#define UI_MOUNT_CNT 2
#define UI_MOUNT_CMD ,&ui_menu_sd_unmount,&ui_menu_sd_mount
#endif
UI_MENU_ACTIONCOMMAND_FILTER(ui_menu_sd_delete,UI_TEXT_DELETE_FILE,UI_ACTION_SD_DELETE,MENU_MODE_SD_MOUNTED,MENU_MODE_SD_PRINTING)
#define UI_MENU_SD {UI_MENU_ADDCONDBACK &ui_menu_sd_printfile,&ui_menu_sd_pause,&ui_menu_sd_continue,&ui_menu_sd_stop UI_MOUNT_CMD ,&ui_menu_sd_delete}
UI_MENU(ui_menu_sd, UI_MENU_SD, UI_MENU_BACKCNT + 5 + UI_MOUNT_CNT)
UI_MENU_SUBMENU(ui_menu_sd_sub, UI_TEXT_SD_CARD, ui_menu_sd)

#define UI_MENU_SD_COND &ui_menu_sd_sub,
#define UI_MENU_SD_CNT 1
#else
#define UI_MENU_SD_COND
#define UI_MENU_SD_CNT 0
#define SD_PRINTFILE_ENTRY
#define SD_PRINTFILE_ENTRY_CNT 0
#endif


// **** Debugging menu
UI_MENU_ACTIONCOMMAND(ui_menu_debug_echo,   UI_TEXT_DBG_ECHO,   UI_ACTION_DEBUG_ECHO)
UI_MENU_ACTIONCOMMAND(ui_menu_debug_info,   UI_TEXT_DBG_INFO,   UI_ACTION_DEBUG_INFO)
UI_MENU_ACTIONCOMMAND(ui_menu_debug_error,  UI_TEXT_DBG_ERROR,  UI_ACTION_DEBUG_ERROR)
UI_MENU_ACTIONCOMMAND(ui_menu_debug_dryrun, UI_TEXT_DBG_DRYRUN, UI_ACTION_DEBUG_DRYRUN)

#define UI_MENU_DEBUGGING {UI_MENU_ADDCONDBACK &ui_menu_debug_echo,&ui_menu_debug_info,&ui_menu_debug_error,&ui_menu_debug_dryrun}
UI_MENU(ui_menu_debugging,UI_MENU_DEBUGGING,4 + UI_MENU_BACKCNT)

// **** Acceleration settings
#if DRIVE_SYSTEM != DELTA
UI_MENU_CHANGEACTION(ui_menu_accel_printx,  UI_TEXT_PRINT_X,UI_ACTION_PRINT_ACCEL_X)
UI_MENU_CHANGEACTION(ui_menu_accel_printy,  UI_TEXT_PRINT_Y,UI_ACTION_PRINT_ACCEL_Y)
UI_MENU_CHANGEACTION(ui_menu_accel_printz,  UI_TEXT_PRINT_Z,UI_ACTION_PRINT_ACCEL_Z)
UI_MENU_CHANGEACTION(ui_menu_accel_travelx, UI_TEXT_MOVE_X,UI_ACTION_MOVE_ACCEL_X)
UI_MENU_CHANGEACTION(ui_menu_accel_travely, UI_TEXT_MOVE_Y,UI_ACTION_MOVE_ACCEL_Y)
UI_MENU_CHANGEACTION(ui_menu_accel_travelz, UI_TEXT_MOVE_Z,UI_ACTION_MOVE_ACCEL_Z)
UI_MENU_CHANGEACTION(ui_menu_accel_jerk,    UI_TEXT_JERK,UI_ACTION_MAX_JERK)
UI_MENU_CHANGEACTION(ui_menu_accel_zjerk,   UI_TEXT_ZJERK,UI_ACTION_MAX_ZJERK)
#define UI_MENU_ACCEL {UI_MENU_ADDCONDBACK &ui_menu_accel_printx,&ui_menu_accel_printy,&ui_menu_accel_printz,&ui_menu_accel_travelx,&ui_menu_accel_travely,&ui_menu_accel_travelz,&ui_menu_accel_jerk,&ui_menu_accel_zjerk}
UI_MENU(ui_menu_accel,UI_MENU_ACCEL,8+UI_MENU_BACKCNT)

// **** Feedrates
UI_MENU_CHANGEACTION(ui_menu_feedrate_maxx,  UI_TEXT_FEED_MAX_X,  UI_ACTION_MAX_FEEDRATE_X)
UI_MENU_CHANGEACTION(ui_menu_feedrate_maxy,  UI_TEXT_FEED_MAX_Y,  UI_ACTION_MAX_FEEDRATE_Y)
UI_MENU_CHANGEACTION(ui_menu_feedrate_maxz,  UI_TEXT_FEED_MAX_Z,  UI_ACTION_MAX_FEEDRATE_Z)
UI_MENU_CHANGEACTION(ui_menu_feedrate_homex, UI_TEXT_FEED_HOME_X, UI_ACTION_HOMING_FEEDRATE_X)
UI_MENU_CHANGEACTION(ui_menu_feedrate_homey, UI_TEXT_FEED_HOME_Y, UI_ACTION_HOMING_FEEDRATE_Y)
UI_MENU_CHANGEACTION(ui_menu_feedrate_homez, UI_TEXT_FEED_HOME_Z, UI_ACTION_HOMING_FEEDRATE_Z)
#define UI_MENU_FEEDRATE {UI_MENU_ADDCONDBACK &ui_menu_feedrate_maxx,&ui_menu_feedrate_maxy,&ui_menu_feedrate_maxz,&ui_menu_feedrate_homex,&ui_menu_feedrate_homey,&ui_menu_feedrate_homez}
UI_MENU(ui_menu_feedrate,UI_MENU_FEEDRATE,6 + UI_MENU_BACKCNT)
#else
UI_MENU_CHANGEACTION(ui_menu_accel_printz,UI_TEXT_PRINT_Z_DELTA,UI_ACTION_PRINT_ACCEL_Z)
UI_MENU_CHANGEACTION(ui_menu_accel_travelz,UI_TEXT_MOVE_Z_DELTA,UI_ACTION_MOVE_ACCEL_Z)
UI_MENU_CHANGEACTION(ui_menu_accel_jerk,UI_TEXT_JERK,UI_ACTION_MAX_JERK)
#define UI_MENU_ACCEL {UI_MENU_ADDCONDBACK &ui_menu_accel_printz,&ui_menu_accel_travelz,&ui_menu_accel_jerk}
UI_MENU(ui_menu_accel,UI_MENU_ACCEL,3+UI_MENU_BACKCNT)

// **** Feedrates
UI_MENU_CHANGEACTION(ui_menu_feedrate_maxz,UI_TEXT_FEED_MAX_Z_DELTA,UI_ACTION_MAX_FEEDRATE_Z)
UI_MENU_CHANGEACTION(ui_menu_feedrate_homez,UI_TEXT_FEED_HOME_Z_DELTA,UI_ACTION_HOMING_FEEDRATE_Z)
#define UI_MENU_FEEDRATE {UI_MENU_ADDCONDBACK &ui_menu_feedrate_maxz,&ui_menu_feedrate_homez}
UI_MENU(ui_menu_feedrate,UI_MENU_FEEDRATE,2+UI_MENU_BACKCNT)
#endif

// **** General configuration settings

UI_MENU_ACTION2C(ui_menu_stepper2,UI_ACTION_STEPPER_INACTIVE,UI_TEXT_STEPPER_INACTIVE2)
UI_MENU_ACTION2C(ui_menu_maxinactive2,UI_ACTION_MAX_INACTIVE,UI_TEXT_POWER_INACTIVE2)
UI_MENU_CHANGEACTION(ui_menu_general_baud,UI_TEXT_BAUDRATE,UI_ACTION_BAUDRATE)
UI_MENU_ACTIONSELECTOR(ui_menu_general_stepper_inactive,UI_TEXT_STEPPER_INACTIVE,ui_menu_stepper2)
UI_MENU_ACTIONSELECTOR(ui_menu_general_max_inactive,UI_TEXT_POWER_INACTIVE,ui_menu_maxinactive2)
#if FEATURE_AUTOLEVEL
 UI_MENU_ACTIONCOMMAND(ui_menu_toggle_autolevel,UI_TEXT_AUTOLEVEL_ONOFF,UI_ACTION_AUTOLEVEL_ONOFF)
 #define UI_TOOGLE_AUTOLEVEL_ENTRY ,&ui_menu_toggle_autolevel
 #define UI_TOGGLE_AUTOLEVEL_COUNT 1
#else
 #define UI_TOOGLE_AUTOLEVEL_ENTRY
 #define UI_TOGGLE_AUTOLEVEL_COUNT 0
#endif
#define UI_MENU_GENERAL {UI_MENU_ADDCONDBACK &ui_menu_general_baud,&ui_menu_general_stepper_inactive,&ui_menu_general_max_inactive UI_TOOGLE_AUTOLEVEL_ENTRY}
UI_MENU(ui_menu_general,UI_MENU_GENERAL,3+UI_MENU_BACKCNT+UI_TOGGLE_AUTOLEVEL_COUNT)

// **** Extruder configuration

UI_MENU_CHANGEACTION(ui_menu_cext_steps,          UI_TEXT_EXTR_STEPS,              UI_ACTION_EXTR_STEPS)
UI_MENU_CHANGEACTION(ui_menu_cext_start_feedrate, UI_TEXT_EXTR_START_FEED,         UI_ACTION_EXTR_START_FEEDRATE)
UI_MENU_CHANGEACTION(ui_menu_cext_max_feedrate,   UI_TEXT_EXTR_MAX_FEED,           UI_ACTION_EXTR_MAX_FEEDRATE)
UI_MENU_CHANGEACTION(ui_menu_cext_acceleration,   UI_TEXT_EXTR_ACCEL,              UI_ACTION_EXTR_ACCELERATION)
UI_MENU_CHANGEACTION(ui_menu_cext_watch_period,   UI_TEXT_EXTR_WATCH,              UI_ACTION_EXTR_WATCH_PERIOD)
UI_MENU_CHANGEACTION(ui_menu_ext_wait_temp,       UI_TEXT_EXTR_WAIT_RETRACT_TEMP,  UI_ACTION_EXTR_WAIT_RETRACT_TEMP)
UI_MENU_CHANGEACTION(ui_menu_ext_wait_units,      UI_TEXT_EXTR_WAIT_RETRACT_UNITS, UI_ACTION_EXTR_WAIT_RETRACT_UNITS)
#define UI_MENU_ADV_CNT 0
#define UI_MENU_ADVANCE
#if USE_ADVANCE
#define UI_MENU_ADV_CNT 1
#define UI_MENU_ADVANCE ,&ui_menu_cext_advancel
#if ENABLE_QUADRATIC_ADVANCE
#define UI_MENU_ADV_CNT 2
#define UI_MENU_ADVANCE ,&ui_menu_cext_advancel,&ui_menu_cext_advancek
UI_MENU_CHANGEACTION(ui_menu_cext_advancek,UI_TEXT_EXTR_ADVANCE_K,UI_ACTION_ADVANCE_K)
#endif
UI_MENU_CHANGEACTION(ui_menu_cext_advancel,UI_TEXT_EXTR_ADVANCE_L,UI_ACTION_ADVANCE_L)
#endif
UI_MENU_CHANGEACTION(       ui_menu_cext_manager, UI_TEXT_EXTR_MANAGER, UI_ACTION_EXTR_HEATMANAGER)
UI_MENU_CHANGEACTION(       ui_menu_cext_pmax,    UI_TEXT_EXTR_PMAX,    UI_ACTION_PID_MAX)
#if TEMP_PID
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_pgain,   UI_TEXT_EXTR_PGAIN,   UI_ACTION_PID_PGAIN, MENU_MODE_FULL_PID, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_igain,   UI_TEXT_EXTR_IGAIN,   UI_ACTION_PID_IGAIN,  MENU_MODE_FULL_PID, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_dgain,   UI_TEXT_EXTR_DGAIN,   UI_ACTION_PID_DGAIN,  MENU_MODE_FULL_PID, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_dmin,    UI_TEXT_EXTR_DMIN,    UI_ACTION_DRIVE_MIN,  MENU_MODE_FULL_PID, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_dmax,    UI_TEXT_EXTR_DMAX,    UI_ACTION_DRIVE_MAX,  MENU_MODE_FULL_PID, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_pgain_dt,   UI_TEXT_EXTR_DEADTIME,   UI_ACTION_PID_PGAIN, MENU_MODE_DEADTIME, 0)
UI_MENU_CHANGEACTION_FILTER(ui_menu_cext_dmax_dt,    UI_TEXT_EXTR_DMAX_DT,    UI_ACTION_DRIVE_MAX,  MENU_MODE_DEADTIME, 0)
#define UI_MENU_PIDCOND ,&ui_menu_cext_manager,&ui_menu_cext_pgain,&ui_menu_cext_igain,&ui_menu_cext_dgain,&ui_menu_cext_dmin,&ui_menu_cext_dmax, &ui_menu_cext_pgain_dt,&ui_menu_cext_dmax_dt,&ui_menu_cext_pmax
#define UI_MENU_PIDCNT 9
#else
#define UI_MENU_PIDCOND ,&ui_menu_cext_manager, &ui_menu_cext_pmax
#define UI_MENU_PIDCNT 2
#endif
#if NUM_EXTRUDER>2 && MIXING_EXTRUDER == 0
UI_MENU_CHANGEACTION(ui_menu_cext_xoffset,UI_TEXT_EXTR_XOFF,UI_ACTION_X_OFFSET)
UI_MENU_CHANGEACTION(ui_menu_cext_yoffset,UI_TEXT_EXTR_YOFF,UI_ACTION_Y_OFFSET)
#define UI_MENU_CONFEXTCOND &ui_menu_ext_sel0,&ui_menu_ext_sel1,&ui_menu_ext_sel2,&ui_menu_cext_xoffset,&ui_menu_cext_yoffset,
#define UI_MENU_CONFEXTCNT 5
#elif NUM_EXTRUDER>1 && MIXING_EXTRUDER == 0
UI_MENU_CHANGEACTION(ui_menu_cext_xoffset,UI_TEXT_EXTR_XOFF,UI_ACTION_X_OFFSET)
UI_MENU_CHANGEACTION(ui_menu_cext_yoffset,UI_TEXT_EXTR_YOFF,UI_ACTION_Y_OFFSET)
#define UI_MENU_CONFEXTCOND &ui_menu_ext_sel0,&ui_menu_ext_sel1,&ui_menu_cext_xoffset,&ui_menu_cext_yoffset,
#define UI_MENU_CONFEXTCNT 4
#else
#define UI_MENU_CONFEXTCOND
#define UI_MENU_CONFEXTCNT 0
#endif
#define UI_MENU_CEXTR {UI_MENU_ADDCONDBACK UI_MENU_CONFEXTCOND &ui_menu_cext_steps,&ui_menu_cext_start_feedrate,&ui_menu_cext_max_feedrate,&ui_menu_cext_acceleration,&ui_menu_cext_watch_period,&ui_menu_ext_wait_units,&ui_menu_ext_wait_temp UI_MENU_ADVANCE UI_MENU_PIDCOND}
UI_MENU(ui_menu_cextr,UI_MENU_CEXTR,7+UI_MENU_BACKCNT+UI_MENU_PIDCNT+UI_MENU_CONFEXTCNT+UI_MENU_ADV_CNT)

// HeatBed Configuration - use menu actions from extruder configuration
#if HAVE_HEATED_BED
 #if TEMP_PID
  #define UI_MENU_BEDCONF {UI_MENU_ADDCONDBACK &ui_menu_cext_manager,&ui_menu_cext_pgain,&ui_menu_cext_igain,&ui_menu_cext_dgain,&ui_menu_cext_dmin,&ui_menu_cext_dmax,&ui_menu_cext_pmax}
  UI_MENU(ui_menu_bedconf, UI_MENU_BEDCONF, 8)
 #else
  #define UI_MENU_BEDCONF {UI_MENU_ADDCONDBACK &ui_menu_cext_manager, &ui_menu_cext_pmax}
  UI_MENU(ui_menu_bedconf, UI_MENU_BEDCONF, 3)
 #endif
#endif

// **** Configuration menu
UI_MENU_SUBMENU(ui_menu_conf_general, UI_TEXT_GENERAL,      ui_menu_general)
UI_MENU_SUBMENU(ui_menu_conf_accel,   UI_TEXT_ACCELERATION, ui_menu_accel)
UI_MENU_SUBMENU(ui_menu_conf_feed,    UI_TEXT_FEEDRATE,     ui_menu_feedrate)
UI_MENU_SUBMENU(ui_menu_conf_extr,    UI_TEXT_EXTRUDER,     ui_menu_cextr)
#if HAVE_HEATED_BED
 UI_MENU_SUBMENU(ui_menu_conf_bed,    UI_TEXT_HEATING_BED,  ui_menu_bedconf)
 #define UI_MENU_BEDCONF_COND ,&ui_menu_conf_bed
 #define UI_MENU_BEDCONF_CNT 1
#else
 #define UI_MENU_BEDCONF_COND
 #define UI_MENU_BEDCONF_CNT 0
#endif
#if EEPROM_MODE!=0
UI_MENU_ACTIONCOMMAND(ui_menu_conf_to_eeprom,UI_TEXT_STORE_TO_EEPROM,UI_ACTION_STORE_EEPROM)
UI_MENU_ACTIONCOMMAND(ui_menu_conf_from_eeprom,UI_TEXT_LOAD_EEPROM,UI_ACTION_LOAD_EEPROM)
#define UI_MENU_EEPROM_COND ,&ui_menu_conf_to_eeprom,&ui_menu_conf_from_eeprom
#define UI_MENU_EEPROM_CNT 2
UI_MENU_ACTION2C(ui_menu_eeprom_saved,  UI_ACTION_DUMMY, UI_TEXT_EEPROM_STORED)
UI_MENU_ACTION2C(ui_menu_eeprom_loaded, UI_ACTION_DUMMY, UI_TEXT_EEPROM_LOADED)
#else
#define UI_MENU_EEPROM_COND
#define UI_MENU_EEPROM_CNT 0
#endif
#ifdef SOFTWARE_LEVELING
#define UI_MENU_SL_COND ,&ui_menu_conf_level
#define UI_MENU_SL_CNT 1
UI_MENU_SUBMENU(ui_menu_conf_level, UI_TEXT_LEVEL, ui_menu_level)
#else
#define UI_MENU_SL_COND
#define UI_MENU_SL_CNT 0
#endif
#if Z_HOME_DIR > 0
#define UI_MENU_DELTA_COND ,&ui_menu_conf_delta
#define UI_MENU_DELTA_CNT 1
UI_MENU_SUBMENU(ui_menu_conf_delta, UI_TEXT_ZCALIB, ui_menu_delta)
#else
#define UI_MENU_DELTA_COND
#define UI_MENU_DELTA_CNT 0
#endif
#define UI_MENU_CONFIGURATION {UI_MENU_ADDCONDBACK &ui_menu_conf_general,&ui_menu_conf_accel,&ui_menu_conf_feed,&ui_menu_conf_extr UI_MENU_BEDCONF_COND UI_MENU_EEPROM_COND UI_MENU_DELTA_COND UI_MENU_SL_COND}
UI_MENU(ui_menu_configuration,UI_MENU_CONFIGURATION,UI_MENU_BACKCNT+UI_MENU_EEPROM_CNT+UI_MENU_BEDCONF_CNT+UI_MENU_DELTA_CNT+UI_MENU_SL_CNT+4)
// Main menu
UI_MENU_SUBMENU(ui_menu_main1, UI_TEXT_QUICK_SETTINGS,ui_menu_quick)
UI_MENU_SUBMENU(ui_menu_main2, UI_TEXT_POSITION,ui_menu_positions)
UI_MENU_SUBMENU(ui_menu_main3, UI_TEXT_EXTRUDER,ui_menu_extruder)
UI_MENU_SUBMENU(ui_menu_main4, UI_TEXT_DEBUGGING,ui_menu_debugging)
UI_MENU_SUBMENU(ui_menu_main5, UI_TEXT_CONFIGURATION,ui_menu_configuration)
#define UI_MENU_MAIN {UI_MENU_ADDCONDBACK  &ui_menu_main1,SD_PRINTFILE_ENTRY &ui_menu_main2,&ui_menu_main3,UI_MENU_FAN_COND UI_MENU_COATING_COND UI_MENU_SD_COND &ui_menu_main4,&ui_menu_main5}
UI_MENU(ui_menu_main,UI_MENU_MAIN,5+UI_MENU_BACKCNT+UI_MENU_SD_CNT+UI_MENU_FAN_CNT+SD_PRINTFILE_ENTRY_CNT+UI_MENU_COATING_CNT)

/* Define menus accessible by action commands

You can create up to 10 user menus which are accessible by the action commands UI_ACTION_SHOW_USERMENU1 until UI_ACTION_SHOW_USERMENU10
You this the same way as with the menus above or you use one of the above menus. Then add a define like

#define UI_USERMENU1 ui_menu_conf_feed

which assigns the menu stored in ui_menu_conf_feed to the action UI_ACTION_SHOW_USERMENU1. Make sure only to change the numbers and not the name of the define.

When do you need this? You might want a fast button to change the temperature. In the default menu you have no menu
to change the temperature and view it the same time. So you need to make an action menu for this like:
UI_MENU_ACTION4C(ui_menu_extrtemp,UI_ACTION_EXTRUDER0_TEMP,"Temp. 0  :%E0" cDEG,"","","");
Then you assign this menu to a usermenu:
#define UI_USERMENU2 ui_menu_extrtemp

Now you can assign the action  UI_ACTION_SHOW_USERMENU2+UI_ACTION_TOPMENU to a key and that will now show the temperture screen and allows
the change of temperature with the next/previous buttons.

*/
#endif
#endif // __UI_MENU_H
