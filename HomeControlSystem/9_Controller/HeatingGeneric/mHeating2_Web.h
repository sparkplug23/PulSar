/*
  mHeating_Commands.cpp - Home Heating Controller

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _DRIVER_INTERFACE_HEATING2_WEB_H
#define _DRIVER_INTERFACE_HEATING2_WEB_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_HEATING2

#ifdef USE_MODULE_NETWORK_WEBSERVER

// no class, include within header class

int test = 0;

int8_t Tasker_Web(uint8_t function);


#endif // USE_MODULE_NETWORK_WEBSERVER







// const char HTTP_BTN_MENU_TIMER[] PROGMEM =
//   "<p><form action='" WEB_HANDLE_HEATING_TIMER "' method='get'><button>" D_CONFIGURE_TIMER "</button></form></p>";

// const char S_CONFIGURE_TIMER[] PROGMEM = D_CONFIGURE_TIMER;

// const char HTTP_TIMER_SCRIPT1[] PROGMEM =
//   "var pt=[],ct=99;"
//   "function ce(i,q){"                                             // Create select option
//     "var o=document.createElement('option');"
//     "o.textContent=i;"
//     "q.appendChild(o);"
//   "}";

// #ifdef USE_SUNRISE
// const char HTTP_TIMER_SCRIPT2[] PROGMEM =
//   "function gt(){"                                                // Set hours and minutes according to mode
//     "var m,p,q;"
//     "m=qs('input[name=\"rd\"]:checked').value;"                   // Get mode
//     "p=pt[ct]&0x7FF;"                                             // Get time
//     "if(m==0){"                                                   // Time is set
//       "so(0);"                                                    // Hide offset span and allow Hour 00..23
//       "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"   // Set hours
//       "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set minutes
//     "}"
//     "if((m==1)||(m==2)){"                                         // Sunrise or sunset is set
//       "so(1);"                                                    // Show offset span and allow Hour 00..11
//       "q=Math.floor(p/60);"                                       // Parse hours
//       "if(q>=12){q-=12;qs('#dr').selectedIndex=1;}"               // Negative offset
//         "else{qs('#dr').selectedIndex=0;}"
//       "if(q<10){q='0'+q;}qs('#ho').value=q;"                      // Set offset hours
//       "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set offset minutes
//     "}"
//   "}"
//   "function so(b){"                                               // Hide or show offset items
//     "o=qs('#ho');"
//     "e=o.childElementCount;"
//     "if(b==1){"
//       "qs('#dr').disabled='';"
//       "if(e>12){for(i=12;i<=23;i++){o.removeChild(o.lastElementChild);}}"  // Create offset hours select options
//     "}else{"
//       "qs('#dr').disabled='disabled';"
//       "if(e<23){for(i=12;i<=23;i++){ce(i,o);}}"                   // Create hours select options
//     "}"
//   "}";
// #endif
// const char HTTP_TIMER_SCRIPT3[] PROGMEM =
//   "function st(){"                                                // Save parameters to hidden area
//     "var i,l,m,n,p,s;"
//     "m=0;s=0;"
//     //"n=1<<31;if(eb('a0').checked){s|=n;}"                         // Get arm
//     //"n=1<<15;if(eb('r0').checked){s|=n;}"                         // Get repeat
//     "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
// #ifdef USE_SUNRISE
//     "m=qs('input[name=\"rd\"]:checked').value;"                   // Check mode
//     "s|=(qs('input[name=\"rd\"]:checked').value<<29);"            // Get mode
// #endif
//     // "if(%d>0){"
//     //   "i=qs('#d1').selectedIndex;if(i>=0){s|=(i<<23);}"           // Get output
//     //   "s|=(qs('#p1').selectedIndex<<27);"                         // Get action
//     // "}else{"
//     //   "s|=3<<27;"                                                 // Get action (rule)
//     // "}"
//     "l=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"
//     "if(m==0){s|=l;}"                                             // Get time
// #ifdef USE_SUNRISE
//     "if((m==1)||(m==2)){"
//       "if(qs('#dr').selectedIndex>0){l+=720;}"                    // If negative offset, add 12h to given offset time
//       "s|=l&0x7FF;"                                               // Save offset instead of time
//     "}"
// #endif
//     "s|=((qs('#mw').selectedIndex)&0x0F)<<11;"                    // Get window minutes
//     "pt[ct]=s;"
//     "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
//   "}";
// const char HTTP_TIMER_SCRIPT4[] PROGMEM =
//   "function ot(t,e){"                                             // Select tab and update elements
//     "var i,n,o,p,q,s;"
//     "if(ct<99){st();}"                                            // Save changes
//     "ct=t;"
//     "o=document.getElementsByClassName('tl');"                    // Restore style to all tabs/buttons
//     "for(i=0;i<o.length;i++){o[i].style.cssText=\"background:#%06x;color:#%06x;font-weight:normal;\"}"  // COLOR_TIMER_TAB_BACKGROUND, COLOR_TIMER_TAB_TEXT
//     "e.style.cssText=\"background:#%06x;color:#%06x;font-weight:bold;\";"  // COLOR_FORM, COLOR_TEXT, Change style to tab/button used to open content
//     "s=pt[ct];"                                                   // Get parameters from array
// #ifdef USE_SUNRISE
//     "p=(s>>29)&3;eb('b'+p).checked=1;"                            // Set mode
//     "gt();"                                                       // Set hours and minutes according to mode
// #else
//     "p=s&0x7FF;"                                                  // Get time
//     "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
//     "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                // Set minutes
// #endif
//     "q=(s>>11)&0xF;if(q<10){q='0'+q;}qs('#mw').value=q;"          // Set window minutes
//     "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
//     "if(%d>0){"
//       "p=(s>>23)&0xF;qs('#d1').value=p+1;"                        // Set output
//       "p=(s>>27)&3;qs('#p1').selectedIndex=p;"                    // Set action
//     "}"
//     "p=(s>>15)&1;eb('r0').checked=p;"                             // Set repeat
//     "p=(s>>31)&1;eb('a0').checked=p;"                             // Set arm
//   "}";
// const char HTTP_TIMER_SCRIPT5[] PROGMEM =
//   "function it(){"                                                // Initialize elements and select first tab
//     "var b,i,o,s;"
//     "pt=eb('t0').value.split(',').map(Number);"                   // Get parameters from hidden area to array
//     "s='';"
//     "for(i=0;i<%d;i++){"
//       "b='';"
//       "if(0==i){b=\" id='dP'\";}"
//       "s+=\"<button type='buttonh' class='tl' onclick='ot(\"+i+\",this)'\"+b+\">\"+(i+1)+\"</button>\""
//     "}"
//     "eb('bt').innerHTML=s;"                                       // Create tabs
//     "if(%d>0){"                                                   // Create Output and Action drop down boxes
//       "eb('oa').innerHTML=\"<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:60px;' id='d1'></select></span>&emsp;<b>" D_TIMER_ACTION "</b>&nbsp;<select style='width:99px;' id='p1'></select>\";"
//       "o=qs('#p1');ce('" D_OFF "',o);ce('" D_ON "',o);ce('" D_TOGGLE "',o);"  // Create offset direction select options
// #if defined(USE_RULES) || defined(USE_SCRIPT)
//       "ce('" D_RULE "',o);"
// #else
//       "ce('" D_BLINK "',o);"
// #endif
//     "}else{"
//       "eb('oa').innerHTML=\"<b>" D_TIMER_ACTION "</b> " D_RULE "\";"  // No outputs but rule is allowed
//     "}";
// const char HTTP_TIMER_SCRIPT6[] PROGMEM =
// #ifdef USE_SUNRISE
//     "o=qs('#dr');ce('+',o);ce('-',o);"                            // Create offset direction select options
// #endif
//     "o=qs('#ho');for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,o);}"     // Create hours select options
//     "o=qs('#mi');for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,o);}"     // Create minutes select options
//     "o=qs('#mw');for(i=0;i<=15;i++){ce((i<10)?('0'+i):i,o);}"     // Create window minutes select options
//     "o=qs('#d1');for(i=0;i<%d;i++){ce(i+1,o);}"                   // Create outputs
//     "var a='" D_DAY3LIST "';"
//     "s='';for(i=0;i<7;i++){s+=\"<input id='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b> \"}"
//     "eb('ds').innerHTML=s;"                                       // Create weekdays
//     "eb('dP').click();"                                           // Get the element with id='dP' and click on it
//   "}"
//   "wl(it);";
// const char HTTP_TIMER_STYLE[] PROGMEM =
//   ".tl{float:left;border-radius:0;border:1px solid #%06x;padding:1px;width:6.25%%;}";  // COLOR_FORM, Border color needs to be the same as Fieldset background color from HTTP_HEAD_STYLE1 (transparent won't work)
// const char HTTP_FORM_TIMER1[] PROGMEM =
//   "<fieldset style='min-width:470px;text-align:center;'>"
//   "<legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='post' action='" WEB_HANDLE_HEATING_TIMER "' onsubmit='return st();'>"
//   "<br><input id='e0' type='checkbox'%s><b>" D_TIMER_ENABLE "</b><br><br><hr>"
//   "<input id='t0' value='";
// const char HTTP_FORM_TIMER2[] PROGMEM =
//   "' hidden><div id='bt'></div><br><br><br>"
//   "<div id='oa' name='oa'></div><br>"
//   "<div>"
//   "<input id='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
//   "<input id='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
//   "</div><br>"
//   "<div>";
  
// const char HTTP_FORM_TIMER2a[] PROGMEM =
//   "<div id='bt2'></div>"
//   "<br><br><br>"
//   "<div id='oa' name='oa'></div>"
//   "<br><br>";

// #ifdef USE_SUNRISE
//   const char HTTP_FORM_TIMER3[] PROGMEM =
//     "<fieldset style='width:%dpx;margin:auto;text-align:left;border:0;'>"
//     "<input id='b0' name='rd' type='radio' value='0' onclick='gt();'><b>" D_TIMER_TIME "</b><br>"
//     "<input id='b1' name='rd' type='radio' value='1' onclick='gt();'><b>" D_SUNRISE "</b> (%s)<br>"
//     "<input id='b2' name='rd' type='radio' value='2' onclick='gt();'><b>" D_SUNSET "</b> (%s)<br>"
//     "</fieldset>"
//     "<p></p>"
//     "<span><select style='width:46px;' id='dr'></select></span>"
//     "&nbsp;";
// #else
//   const char HTTP_FORM_TIMER3[] PROGMEM =
//     "<b>" D_TIMER_TIME "</b>&nbsp;";
// #endif  // USE_SUNRISE

// const char HTTP_FORM_TIMER4[] PROGMEM =
//   "<span><select style='width:60px;' id='ho'></select></span>"
//   "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
//   "<span><select style='width:60px;' id='mi'></select></span>"
//   "&emsp;<b>+/-</b>&nbsp;"
//   "<span><select style='width:60px;' id='mw'></select></span>"
//   "</div><br>"
//   "<div id='ds' name='ds'></div>";



#endif
#endif