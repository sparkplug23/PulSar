#ifndef DATA_SMOOTHING_H
#define DATA_SMOOTHING_H

/**
 * Helper class that stores data, returning the mean value.
 * Accepts any numeric data type
 * */
#include <vector>
/**
 * Creating a class that holds an array for averaging.
 * Templates are used, so the type of data can be specified
 * Helper functions will calculate average and return it
 * "Add()" will add to array
 * "Mean()" will return average
 * "Clear()" will reset array values and index
 * "NoiseFilter_AsPercent(X)" will check array (after 100% full) and reject values which deviate by X percent of range
 * "NoiseFilter" method 1, ignores values beyond mean (store so its not aslways computed), method 2 only computes mean within SD of X
 * "SetRange()" will set max/min values, default is take in all
 * */

template<typename TYPE>
class Averaging_Data{
  public:

    uint16_t window_size = 1;

    /**
     * Requied before buffer has been filled up once (use vectors will allow FIFO)
     * */
    uint16_t indexes_used = 0;
    uint16_t index = 0;
    /**
     * Recording how long between buffer being reset
     * */
    uint32_t tResetPeriod = millis();
    // TYPE* data = nullptr;
    struct LIMIT{
      bool enabled = false;
      TYPE lower;
      TYPE upper;
    }limit;

    std::vector<float> data_v;

    Averaging_Data(){};

    Averaging_Data(uint16_t _window_size){
      window_size = _window_size;
      // bool result = allocateData(_window_size);
      limit.enabled = false;
      // if(result){
      //     Serial.println("result");
      // }
      // for(int i=0;i<_window_size;i++){data[i] = i;}

      // data_v.push_back(1);
      // data_v.push_back(2);
      // data_v.push_back(3);
      // data_v.push_back(4);

      // data_v.erase(data_v.begin());



    };

    ~Averaging_Data(){
      // deallocateData();
      data_v.clear();
    }

    // bool allocateData(uint16_t len){
    //   Serial.println("allocateData");
    //   if (data && _dataLen == len) return true; //already allocated
    //   deallocateData();
    //   data = new (std::nothrow) TYPE[len];
    //   if (!data) return false; //allocation failed
    //   _dataLen = len;
    //   for(int i=0;i<_dataLen;i++){data[i] = 0;}
    //   //   memset(data, 0, len*sizeof(TYPE));
    //   return true;
    // }

    // void deallocateData(){
    //   delete[] data;
    //   data = nullptr;
    //   _dataLen = 0;
    // };

    bool Add(TYPE value){
      
      // Check if we exceed boundaries
      if(limit.enabled){
        if(!IsWithinLimits(value, limit.lower, limit.upper)){ 
          return false;
        }
      }

// Serial.print(data_v.size());
// Serial.println(window_size);

      /**
       * Not yet full, just add into it
       * */
      if(data_v.size() < window_size)
      {

        data_v.push_back(value);
      }
      /**
       * remove first element (ie FIRST IN)
       * add to end
       * */
      else
      {
        data_v.erase(data_v.begin());
        data_v.push_back(value);
      }


      // if(_dataLen == 0)
      // {
      //   return false; // not configured right
      // }
      
      // data[index++] = value;

      // data_v.push_back(value);

      // if(index >= _dataLen){
      //   index = 0; //reset index
      //   tResetPeriod = millis() - tResetPeriod;
      //   // Serial.println("index reset!!");
      // }


      // if(indexes_used<window_size){
      //   indexes_used++; // Add total values used
      // }


      return true; // added
    }
    
    bool IsWithinLimits(TYPE value, TYPE lower, TYPE upper){
      if((value>lower)&&(value<upper)){
        return true;
      }
      return false;
    }

    void SetBoundaryLimits(TYPE lower, TYPE upper){
      limit.lower = lower;
      limit.upper = upper;
      limit.enabled = true;
    }

    void SetBoundaryEnabled(bool state){
      limit.enabled = state;
    }

    void Clear(){
      // index = 0;
      // for(int i=0;i<_dataLen;i++){data[i] = 0;}
      data_v.clear();
    }

    TYPE Mean(uint8_t method = 0){
      TYPE mean_value = 0;
      // if(!index){
      //   return 0; // no data
      // }

      switch(method){
        default:
        case 0:{ // average all

          // mean_value = data[0];
          // if(index>=1){
          //   // Add each value, then divide by 2, this should prevent overflow
          //   for(int i=1;i<indexes_used-1;i++){
          //     mean_value += data[i];
          //     mean_value /= 2;
          //   }
          // }

          // mean_value = data[0];
          // for(int i=1;i<indexes_used-1;i++){
          //   mean_value += data[i];
          //   mean_value /= 2;
          // }

          
          // mean_value = 
          // // static_cast<TYPE>
          // accumulate( data_v.begin(), data_v.end(), 0.0)/data_v.size();    

          // std::vector<float> v{8, 4, 2, 7, 5};
          // auto const a = average(v);
          // Serial.println(mean_value);


        }break;
        case 1: // ignore values which exceed 2 standard deviations

        break;
      }
      
      return mean_value;
      
    }

    
// float average(std::vector<float> const& v){
//     if(v.empty()){
//         return 0;
//     }

//     auto const count = static_cast<float>(v.size());
//     return std::reduce(v.begin(), v.end()) / count;
// }




    // inline TYPE& operator[] (uint8_t x) __attribute__((always_inline))
    // {
    //   return data[x];
    // }
      
private:
    uint16_t _dataLen = 0;

};


#endif

