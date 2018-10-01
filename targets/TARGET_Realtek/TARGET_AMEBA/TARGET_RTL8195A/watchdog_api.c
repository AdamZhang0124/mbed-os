/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "device.h"
#include "watchdog_api.h"

#if DEVICE_WATCHDOG

extern VOID WDGInitial(u32 Period);
extern VOID WDGStart(VOID);
extern VOID WDGStop(VOID);
extern VOID WDGRefresh(VOID);
//extern VOID WDGIrqInitial(VOID);
//extern VOID WDGIrqCallBackReg(VOID *CallBack, u32   Id);


#define maxTimeout         ((uint32_t)10000) //Watchdog Maximum timeout is set as 10 seconds (max possible value 8191s)
#define calibrationFactor  ((float)0.94) //Watchdog timeout calibration value  
#define updateConfig       ((bool)true) //Watchdog configuration can be updated after the watchdog has been started
#define disableWatchdog    ((bool)true)  //Watchdog can be stopped after it is started without a reset

uint32_t wdt_timeout;
uint32_t temp; 

/**
 *  @brief   Initial the watch dog time setting
 *
 *  @param   timeout_ms: the watch-dog timer timeout value, in ms.
 *           default action of timeout is to reset the whole system.
 *  @return  None
 *
 */

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{	
	wdt_timeout = config->timeout_ms;
	temp = wdt_timeout / calibrationFactor;
	
	WDGInitial(temp);
	WDGStart();
	
	if(config->timeout_ms > maxTimeout || config->timeout_ms == 0){
		return WATCHDOG_STATUS_INVALID_ARGUMENT;
	}
	else {
		return WATCHDOG_STATUS_OK;
	}
}



void hal_watchdog_kick(void)
{
	WDGRefresh();
}



watchdog_status_t hal_watchdog_stop(void)
{
    WDGStop();
	return WATCHDOG_STATUS_OK; //return WATCHDOG_STATUS_NOT_SUPPORTED if wdt is unable to stop 
}


//--- revese deduct time
uint32_t hal_watchdog_get_reload_value(void)
{
	return wdt_timeout;
}

 
watchdog_features_t hal_watchdog_get_platform_features(void)
{
	 const watchdog_features_t wd_features = {
            .max_timeout = maxTimeout,
            .update_config = updateConfig,
            .disable_watchdog = disableWatchdog
    };
	
	return wd_features;
}

/**
 *  @brief   Switch the watchdog timer to interrupt mode and
 *           register a watchdog timer timeout interrupt handler.
 *           The interrupt handler will be called when the watch-dog
 *           timer is timeout.
 *
 *  @param   handler: the callback function for WDT timeout interrupt.
 *           id: the parameter for the callback function
 *  @return  None
 *
 */
/*
void watchdog_irq_init(wdt_irq_handler handler, uint32_t id)
{
    WDGIrqCallBackReg((VOID*)handler, (u32)id);
    WDGIrqInitial();
}
*/
#endif /* DEVICE_WATCHDOG */
