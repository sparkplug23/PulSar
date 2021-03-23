
// #ifdef ENABLE_DEVFEATURE_RULE_ENGINE

  
//     #define D_MAX_RULES 10

//     struct RULES{
//       bool enabled = true;
//       struct EVENT{
//         uint16_t trigger_function_id = 999; // not active
//         struct SOURCE{
//           uint16_t module_id;
//           uint16_t function_id;
//           //advanced method
//           uint16_t encoding = 0; // unspecified
//           uint8_t *value.data = nullptr;
//           uint8_t buffer_len = 0;
//           //simple method
//           uint8_t index;
//           uint8_t state;
//         }source;
//         struct DESTINATION{
//           uint16_t module_id;
//           uint16_t function_id;
//           //advanced method
//           uint16_t encoding = 0; // unspecified
//           uint8_t *value.data = nullptr;
//           uint8_t buffer_len = 0;
//           //simple method
//           uint8_t index;
//           uint8_t state;
//         }destination;
//       }event;
//     }rules[D_MAX_RULES];
//     uint8_t rules_active_index = 0;

//     // Rules to match above, now the storage of the event itself so it can be matched
//     struct EVENT_TRIGGERED{

//       // what should be passed into the rules for checking against source, needed for multiple button/sensors
//       uint16_t module_id;
//       uint16_t function_id;
//       //advanced method
//       uint16_t encoding = 0; // unspecified
//       uint8_t *value.data = nullptr;
//       uint8_t buffer_len = 0;
//       //simple method
//       uint8_t index; //ie sensor number
//       uint8_t state;


//     }Event;

//     //move into class
//     void Event_Reset(){
//       Event.module_id = 0;
//       Event.function_id = 0;
//       Event.index = 0;
//       Event.state = 0;
//     };
//     void Event_Add(uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
//       Event.module_id = _module_id;
//       // function_id = _function_id;
//       Event.index = _index;
//       Event.state = _state;
//     };
//     void Event_Encoding(uint8_t encoding){
//         Event.encoding = encoding;
//     };



    void Tasker_Rules_Interface(uint16_t function);
    void Tasker_Rules_Init();

// #endif // ENABLE_DEVFEATURE_RULE_ENGINE