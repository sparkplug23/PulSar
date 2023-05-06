/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"


/**
 * @brief 
 * 
 * If modem connection is handled by another task pinned to the other core, then any slow downs will not hold the main processing back
 * Leave this until I better understand pinning tasks and can make them work well
 * 
 */

#ifdef USE_MODULE_NETWORK_CELLULAR





#endif // USE_MODULE_NETWORK_CELLULAR