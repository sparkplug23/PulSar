#ifndef DATA_SMOOTHING_H
#define DATA_SMOOTHING_H


#include<limits>

#define DEBUG_SINGLEFILE_DECOUNTER



template <typename T>
class Decounter{
    private:
    T counter;       

    public:
    Decounter()
    {
        counter = 0; 
    };
    
    /**
     * @brief Returns true if timer is active
     * 
     * @return true  = ticking is active
     * @return false = ticking is not running
     */
    bool UpdateTick()
    {
        
        // ALOG_INF( PSTR(D_LOG_GARAGE D_JSON_COMMAND_NVALUE_K("limit")), std::numeric_limits<T>::max());
        /**
         * @brief Is ticking
         * counter set to max to describe disabled counter
         **/
        if(counter == std::numeric_limits<T>::max()) // disabled 
        { 
            //ALOG_INF( PSTR("Not Counting") );
            return false;
        }else
        if((counter > 0)&&(counter != std::numeric_limits<T>::max()))
        { 
            counter--;
            //ALOG_INF( PSTR(D_LOG_GARAGE D_JSON_COMMAND_NVALUE_K("counter")), counter);
            return true;
        }else 
        if(counter == 0)
        {
            counter = std::numeric_limits<T>::max(); // Not active at max value
            //ALOG_INF( PSTR(D_LOG_GARAGE D_JSON_COMMAND_NVALUE_K("counter")), counter);
        return false;
        }else{
            //ALOG_ERR( PSTR("Unknown State") );
            return false;

        }
    }

    
    void Start(T v)
    {
        counter = v;
    }

    void Stop()
    {
        counter = std::numeric_limits<T>::max();
    }

    T Value()
    {
        return counter;
    }

};


#endif

