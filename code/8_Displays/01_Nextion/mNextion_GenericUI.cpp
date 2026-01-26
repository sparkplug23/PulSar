#include "mNextion.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

/****
 * Generic items likely added to most nextion pages
 * If addressed by name, then they will either be used of ignored. 
 * I do think the page name is required, but if they get those from the header, it will just set them on the current page.
 * Need a template or filesystem file that describes the webpages. 
 * *  Try to find filesystem method first, then fallback to hardcoded template if not found.
 */

void mNextion::Show_ConnectionWorking()
{
  Command_SplashPage("message", 2);
  
  nextionSetAttr("message.main.txt", "\"Computer connected!\"");
  uint16_t colour565 = RGB888_to_RGB565(0,255,255);
  nextionSetAttr("message.main.bco", colour565);

}


void mNextion::Show_ConnectionNotWorking()
{
  nextionSendCmd("page message");
  nextionSetAttr("message.main.txt", "\"Connection Lost!\"");
  nextionSetAttr("message.main.bco", NEXTION_16BIT_COLOUR_RED_STR);
}



void mNextion::nextionUpdateProgress(const unsigned int &progress, const unsigned int &total)
{
  uint8_t progressPercent = (float(progress) / float(total)) * 100;
  nextionSetAttr("p[0].b[4].val", String(progressPercent).c_str());
}







#endif  // USE_MODULE_DISPLAYS_NEXTION