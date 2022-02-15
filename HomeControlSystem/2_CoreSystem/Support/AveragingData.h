#pragma once

/**
 * Helper class that stores data, returning the mean value.
 * Accepts any numeric data type
 * */

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
class AVERAGING_DATA{
  public:

    uint16_t window_size = 1;
    uint16_t indexes_used = 0;
    uint16_t index = 0;
    TYPE* data = nullptr;
    struct LIMIT{
      bool enabled = false;
      TYPE lower;
      TYPE upper;
    }limit;

    AVERAGING_DATA(){};

    AVERAGING_DATA(uint16_t _window_size){
      window_size = _window_size;
      bool result = allocateData(_window_size);
      limit.enabled = false;
      // if(result){
      //     Serial.println("result");
      // }
      // for(int i=0;i<_window_size;i++){data[i] = i;}
    };

    ~AVERAGING_DATA(){
      deallocateData();
    }

    bool allocateData(uint16_t len){
      Serial.println("allocateData");
      if (data && _dataLen == len) return true; //already allocated
      deallocateData();
      data = new (std::nothrow) TYPE[len];
      if (!data) return false; //allocation failed
      _dataLen = len;
      for(int i=0;i<_dataLen;i++){data[i] = 0;}
      //   memset(data, 0, len*sizeof(TYPE));
      return true;
    }

    void deallocateData(){
      delete[] data;
      data = nullptr;
      _dataLen = 0;
    };

    bool Add(TYPE value){
      
      // Check if we exceed boundaries
      if(limit.enabled){
        if(!IsWithinLimits(value, limit.lower, limit.upper)){
          return false;
        }
      }

      if(index >= _dataLen){
        index = 0; //reset index
      }
      data[index++] = value;
      if(indexes_used<window_size){
        indexes_used++; // Add total values used
      }
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
      index = 0;
      for(int i=0;i<_dataLen;i++){data[i] = 0;}
    }

    TYPE Mean(uint8_t method = 0){
      TYPE mean_value = 0;
      if(!index){
        return 0; // no data
      }

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

          mean_value = data[0];
          for(int i=1;i<indexes_used-1;i++){
            mean_value += data[i];
            mean_value /= 2;
          }


        }break;
        case 1: // ignore values which exceed 2 standard deviations

        break;
      }
      
      return mean_value;
      
    }

    inline TYPE& operator[] (uint8_t x) __attribute__((always_inline))
    {
      return data[x];
    }
      
private:
    uint16_t _dataLen = 0;

};

