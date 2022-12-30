#include "mAddressableLight.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE 

const char* mAddressableLight::PM_MODULE_LIGHTS_ADDRESSABLE_CTR = D_MODULE_LIGHTS_ADDRESSABLE_CTR;
const char* mAddressableLight::PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR = D_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR;


void mAddressableLight::SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware, bool flag_replicate_for_total_pixel_length)
{

  if(flag_replicate_for_total_pixel_length)
  { 
    pCONT_lAni->stripbus->ClearTo(colour_hardware);
  }
  else
  {
    pCONT_lAni->stripbus->SetPixelColor(index, colour_hardware); 
  }

}


RgbcctColor mAddressableLight::GetPixelColorHardware(uint16_t index)
{

  return pCONT_lAni->stripbus->GetPixelColor(index);

}


void mAddressableLight::ShowHardware()
{

  #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    neopixel_runner->execute();   
  #else
      pCONT_lAni->stripbus->Show();
  #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

}


#ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

namespace
{
  void commitTaskProcedure(void *arg)
  {
    CommitParams *params = (CommitParams *)arg;

    while (true)
    {
      while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != 1);
      params->handler();
      xSemaphoreGive(params->semaphore);
    }
  }
} // namespace

void NeoPixelShowTask::begin(CommitHandler handler, uint8_t core_id)
{
  _commit_params.handler = handler;

  if (core_id < 2)
  {
    _commit_params.semaphore = xSemaphoreCreateBinary();

    xTaskCreatePinnedToCore(commitTaskProcedure, /* Task function. */
                            "NeoPixelShow",      /* name of task. */
                            10000,               /* Stack size of task */
                            &_commit_params,     /* parameter of the task */
                            2,                   /* priority of the task */
                            &_commit_task,       /* Task handle to keep track of created task */
                            core_id);            /* pin task to core core_id */
  }
}

void NeoPixelShowTask::execute()
{
  if (_commit_task)
  {
    xTaskNotifyGive(_commit_task);
    while (xSemaphoreTake(_commit_params.semaphore, portMAX_DELAY) != pdTRUE);
  }
  else
  {
    _commit_params.handler();
  }
}

#endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32


int8_t mAddressableLight::Tasker(uint8_t function, JsonParserObject obj){}
void mAddressableLight::parse_JSONCommand(JsonParserObject obj){}

#endif // USE_DRIVER
