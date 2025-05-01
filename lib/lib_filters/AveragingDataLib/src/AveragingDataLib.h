#ifndef DATA_SMOOTHING2_H
#define DATA_SMOOTHING2_H

/**
 * @file Averaging_Data.h
 * @brief Header-only template class to store recent numeric values and compute smoothed averages.
 * 
 * This class supports:
 * - Configurable sliding window (FIFO buffer) of recent data
 * - Hard limits to reject invalid data
 * - Standard averaging, SD-based filtering, and weighted averaging
 * - Useful accessors like min/max, latest, raw data
 * 
 * @tparam TYPE Numeric data type (e.g., uint16_t, float)
 * 
 * ---------------------------------------------------------------------------
 * UPDATE HISTORY
 * ---------------------------------------------------------------------------
 * - Initial version with float-based storage                          (???)      
 * - Switched to templated internal buffer using std::vector<TYPE>    (20Apr25)  
 * - Added Mean method with SD-based and weighted filtering           (20Apr25)
 * - Added helpers: GetMinMax, Latest, and raw data access            (20Apr25)
 */

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

template<typename TYPE>
class Averaging_Data {
public:
    // --- CONFIGURATION ---
    uint16_t window_size = 1;   ///< Number of values to store in the FIFO buffer

    // --- TIME TRACKING (Optional Use) ---
    uint32_t tResetPeriod = millis(); ///< Tracks time since last full buffer cycle (optional)

    // --- LIMIT STRUCTURE FOR HARD FILTERING ---
    struct LIMIT {
        bool enabled = false;
        TYPE lower;  ///< Lower bound (inclusive)
        TYPE upper;  ///< Upper bound (inclusive)
    } limit;

    // --- CONTAINER FOR RAW DATA ---
    std::vector<TYPE> data_v;

    // --- CONSTRUCTORS / DESTRUCTOR ---
    Averaging_Data() {}

    Averaging_Data(uint16_t _window_size) {
        window_size = _window_size;
        limit.enabled = false;
    }

    ~Averaging_Data() {
        data_v.clear();
    }

    // --- MAIN API METHODS ---

    /**
     * @brief Adds a new value to the buffer (FIFO).
     *        Respects hard boundary limits if enabled.
     * 
     * @param value New data point to store
     * @return true  if added
     * @return false if rejected due to hard limits
     */
    bool Add(TYPE value) {
        if (limit.enabled && !IsWithinLimits(value, limit.lower, limit.upper)) {
            return false; // Rejected
        }

        if (data_v.size() < window_size) {
            data_v.push_back(value);
        } else {
            data_v.erase(data_v.begin());   // Remove oldest
            data_v.push_back(value);        // Add newest
        }

        return true;
    }

    /**
     * @brief Computes the mean using different methods.
     * 
     * @param method Select averaging method:
     *               0 - All values (simple mean)
     *               1 - Reject values outside N std dev
     *               2 - Weighted mean based on Gaussian decay
     * @param std_dev_multiplier Number of standard deviations for filtering (method 1/2)
     * @return TYPE Smoothed average
     */
    TYPE Mean(uint8_t method = 0, float std_dev_multiplier = 2.0f) {
        if (data_v.empty()) return 0;

        const size_t N = data_v.size();
        float mean = 0.0f;

        // Compute basic mean
        for (TYPE val : data_v) {
            mean += static_cast<float>(val);
        }
        mean /= static_cast<float>(N);

        if (method == 0) {
            return static_cast<TYPE>(mean); // Simple average
        }

        // Compute standard deviation
        float sum_sq_diff = 0.0f;
        for (TYPE val : data_v) {
            float diff = static_cast<float>(val) - mean;
            sum_sq_diff += diff * diff;
        }
        float std_dev = std::sqrt(sum_sq_diff / static_cast<float>(N));
        if (std_dev < 1e-6f) return static_cast<TYPE>(mean); // Avoid division by 0

        if (method == 1) {
            // Filtered mean within X std deviations
            float lower = mean - std_dev * std_dev_multiplier;
            float upper = mean + std_dev * std_dev_multiplier;

            float sum = 0.0f;
            size_t count = 0;
            for (TYPE val : data_v) {
                float fval = static_cast<float>(val);
                if (fval >= lower && fval <= upper) {
                    sum += fval;
                    count++;
                }
            }

            return count > 0 ? static_cast<TYPE>(sum / static_cast<float>(count)) : static_cast<TYPE>(mean);
        }

        if (method == 2) {
            // Weighted mean using Gaussian decay
            float weighted_sum = 0.0f;
            float total_weight = 0.0f;

            for (TYPE val : data_v) {
                float fval = static_cast<float>(val);
                float z = (fval - mean) / (std_dev * std_dev_multiplier);
                float weight = std::exp(-0.5f * z * z); // Gaussian kernel
                weighted_sum += fval * weight;
                total_weight += weight;
            }

            return total_weight > 0.0f ? static_cast<TYPE>(weighted_sum / total_weight) : static_cast<TYPE>(mean);
        }

        return static_cast<TYPE>(mean); // fallback
    }

    /**
     * @brief Clears the data buffer.
     */
    void Clear() {
        data_v.clear();
    }

    /**
     * @brief Enables or disables hard filtering (reject outside range).
     * @param state true to enable, false to disable
     */
    void SetBoundaryEnabled(bool state) {
        limit.enabled = state;
    }

    /**
     * @brief Sets the upper and lower boundaries for hard filtering.
     * @param lower Minimum allowed value
     * @param upper Maximum allowed value
     */
    void SetBoundaryLimits(TYPE lower, TYPE upper) {
        limit.lower = lower;
        limit.upper = upper;
        limit.enabled = true;
    }

    /**
     * @brief Gets the most recent data point added.
     * @return TYPE Last value, or 0 if buffer is empty
     */
    TYPE Latest() const {
        return data_v.empty() ? 0 : data_v.back();
    }

    /**
     * @brief Returns a reference to the raw buffer.
     * @return const std::vector<TYPE>& Raw buffer
     */
    const std::vector<TYPE>& GetRawData() const {
        return data_v;
    }

    /**
     * @brief Computes the current minimum and maximum in the buffer.
     * @return std::pair<TYPE, TYPE> {min, max}
     */
    std::pair<TYPE, TYPE> GetMinMax() const {
        if (data_v.empty()) {
            return std::make_pair(static_cast<TYPE>(0), static_cast<TYPE>(0));
        }
    
        // Use std::minmax_element and access the .first and .second
        std::pair<typename std::vector<TYPE>::const_iterator,
                  typename std::vector<TYPE>::const_iterator> minmax_it = 
            std::minmax_element(data_v.begin(), data_v.end());
    
        return std::make_pair(*minmax_it.first, *minmax_it.second);
    }
    

private:
    /**
     * @brief Checks if a value lies within configured limits.
     * @param value Input value
     * @param lower Lower bound
     * @param upper Upper bound
     * @return true if within range
     * @return false otherwise
     */
    bool IsWithinLimits(TYPE value, TYPE lower, TYPE upper) {
        return (value >= lower) && (value <= upper);
    }
};

#endif // DATA_SMOOTHING_H




// #ifndef DATA_SMOOTHING_H
// #define DATA_SMOOTHING_H

// /**
//  * Helper class that stores data, returning the mean value.
//  * Accepts any numeric data type
//  * */
// #include <vector>
// /**
//  * Creating a class that holds an array for averaging.
//  * Templates are used, so the type of data can be specified
//  * Helper functions will calculate average and return it
//  * "Add()" will add to array
//  * "Mean()" will return average
//  * "Clear()" will reset array values and index
//  * "NoiseFilter_AsPercent(X)" will check array (after 100% full) and reject values which deviate by X percent of range
//  * "NoiseFilter" method 1, ignores values beyond mean (store so its not aslways computed), method 2 only computes mean within SD of X
//  * "SetRange()" will set max/min values, default is take in all
//  * */

// template<typename TYPE>
// class Averaging_Data{
//   public:

//     uint16_t window_size = 1;

//     /**
//      * Requied before buffer has been filled up once (use vectors will allow FIFO)
//      * */
//     uint16_t indexes_used = 0;
//     uint16_t index = 0;
//     /**
//      * Recording how long between buffer being reset
//      * */
//     uint32_t tResetPeriod = millis();
//     // TYPE* data = nullptr;
//     struct LIMIT{
//       bool enabled = false;
//       TYPE lower;
//       TYPE upper;
//     }limit;

//     std::vector<float> data_v;

//     Averaging_Data(){};

//     Averaging_Data(uint16_t _window_size){
//       window_size = _window_size;
//       // bool result = allocateData(_window_size);
//       limit.enabled = false;
//       // if(result){
//       //     Serial.println("result");
//       // }
//       // for(int i=0;i<_window_size;i++){data[i] = i;}

//       // data_v.push_back(1);
//       // data_v.push_back(2);
//       // data_v.push_back(3);
//       // data_v.push_back(4);

//       // data_v.erase(data_v.begin());



//     };

//     ~Averaging_Data(){
//       // deallocateData();
//       data_v.clear();
//     }

//     // bool allocateData(uint16_t len){
//     //   Serial.println("allocateData");
//     //   if (data && _dataLen == len) return true; //already allocated
//     //   deallocateData();
//     //   data = new (std::nothrow) TYPE[len];
//     //   if (!data) return false; //allocation failed
//     //   _dataLen = len;
//     //   for(int i=0;i<_dataLen;i++){data[i] = 0;}
//     //   //   memset(data, 0, len*sizeof(TYPE));
//     //   return true;
//     // }

//     // void deallocateData(){
//     //   delete[] data;
//     //   data = nullptr;
//     //   _dataLen = 0;
//     // };

//     bool Add(TYPE value){
      
//       // Check if we exceed boundaries
//       if(limit.enabled){
//         if(!IsWithinLimits(value, limit.lower, limit.upper)){ 
//           return false;
//         }
//       }

// // Serial.print(data_v.size());
// // Serial.println(window_size);

//       /**
//        * Not yet full, just add into it
//        * */
//       if(data_v.size() < window_size)
//       {

//         data_v.push_back(value);
//       }
//       /**
//        * remove first element (ie FIRST IN)
//        * add to end
//        * */
//       else
//       {
//         data_v.erase(data_v.begin());
//         data_v.push_back(value);
//       }


//       // if(_dataLen == 0)
//       // {
//       //   return false; // not configured right
//       // }
      
//       // data[index++] = value;

//       // data_v.push_back(value);

//       // if(index >= _dataLen){
//       //   index = 0; //reset index
//       //   tResetPeriod = millis() - tResetPeriod;
//       //   // Serial.println("index reset!!");
//       // }


//       // if(indexes_used<window_size){
//       //   indexes_used++; // Add total values used
//       // }


//       return true; // added
//     }
    
//     bool IsWithinLimits(TYPE value, TYPE lower, TYPE upper){
//       if((value>lower)&&(value<upper)){
//         return true;
//       }
//       return false;
//     }

//     void SetBoundaryLimits(TYPE lower, TYPE upper){
//       limit.lower = lower;
//       limit.upper = upper;
//       limit.enabled = true;
//     }

//     void SetBoundaryEnabled(bool state){
//       limit.enabled = state;
//     }

//     void Clear(){
//       // index = 0;
//       // for(int i=0;i<_dataLen;i++){data[i] = 0;}
//       data_v.clear();
//     }

//     TYPE Mean(uint8_t method = 0){
//       TYPE mean_value = 0;
//       // if(!index){
//       //   return 0; // no data
//       // }

//       switch(method){
//         default:
//         case 0:{ // average all

//           // mean_value = data[0];
//           // if(index>=1){
//           //   // Add each value, then divide by 2, this should prevent overflow
//           //   for(int i=1;i<indexes_used-1;i++){
//           //     mean_value += data[i];
//           //     mean_value /= 2;
//           //   }
//           // }

//           // mean_value = data[0];
//           // for(int i=1;i<indexes_used-1;i++){
//           //   mean_value += data[i];
//           //   mean_value /= 2;
//           // }

          
//           // mean_value = 
//           // // static_cast<TYPE>
//           // accumulate( data_v.begin(), data_v.end(), 0.0)/data_v.size();    

//           // std::vector<float> v{8, 4, 2, 7, 5};
//           // auto const a = average(v);
//           // Serial.println(mean_value);


//         }break;
//         case 1: // ignore values which exceed 2 standard deviations

//         break;
//       }
      
//       return mean_value;
      
//     }

    
// // float average(std::vector<float> const& v){
// //     if(v.empty()){
// //         return 0;
// //     }

// //     auto const count = static_cast<float>(v.size());
// //     return std::reduce(v.begin(), v.end()) / count;
// // }




//     // inline TYPE& operator[] (uint8_t x) __attribute__((always_inline))
//     // {
//     //   return data[x];
//     // }
      
// private:
//     uint16_t _dataLen = 0;

// };


// #endif

