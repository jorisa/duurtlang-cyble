/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

uint8 notification_enabled = 0; 

uint8 value = 1;
uint8 value_prev = 1;

void BLECallBack(uint32 event, void *eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    
    switch (event)
    {
        /*Handle Connection Events*/ 
        case CYBLE_EVT_STACK_ON:
		{
	        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST); // Start advertising 
            break; 
        }
        
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        {
           CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);  // Restart advertising
           break; 
        }
        
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
        { 
          CyBle_GappStopAdvertisement(); 

           break; 
            
        } 
        
        
        case CYBLE_EVT_GATTS_WRITE_REQ: //Write with response
	    case CYBLE_EVT_GATTS_WRITE_CMD_REQ: //Write without response
        {
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam; //read in event command 
               
            if(wrReqParam->handleValPair.attrHandle == CYBLE_PADDESTOEL_NOTIFY_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)
                {
                     if (0 == (wrReqParam->handleValPair.value.val [CYBLE_PADDESTOEL_NOTIFY_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX]))
                        {
                           notification_enabled = 0;  
                        }
                        
                     else if (1 == (wrReqParam->handleValPair.value.val [CYBLE_PADDESTOEL_NOTIFY_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX]))
                        {
                          notification_enabled = 1; 
                        }
                        
                     else if (4 <= (wrReqParam->handleValPair.value.val [CYBLE_PADDESTOEL_NOTIFY_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX]))
                        {
                          //Invalid PDU, stop notifications
                          notification_enabled = 0; 
                          
                          //Inform the user that they are being stupid
                          CYBLE_GATTS_ERR_PARAM_T error_param;

                          error_param.opcode = CYBLE_GATT_WRITE_REQ;
                          error_param.attrHandle = wrReqParam->handleValPair.attrHandle;
                          error_param.errorCode = 0x04;
                            
                          //Send Error Response 
                          (void)CyBle_GattsErrorRsp(cyBle_connHandle, &error_param);
                        
                        }
                }   
             
            if (event == CYBLE_EVT_GATTS_WRITE_REQ) //If write was a write with response request. 
			    {
	                CyBle_GattsWriteRsp(cyBle_connHandle); //Send response back to Client 
			    }             
           
            break; 
        }
        
        case CYBLE_EVT_GATTS_HANDLE_VALUE_CNF:
        {
            break; 
        }
       
        
    }
      
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    CyBle_Start(BLECallBack);
    CYBLE_GATTS_HANDLE_VALUE_NTF_T notificationHandle; 

    for(;;)
    {
       CyBle_ProcessEvents();
    
        if (notification_enabled == 1) //If notifications enabled, increment the number and send its value as a notification
        {
            value = Button_Read();
            if (value != value_prev) {
            
            //Update Notification handle with new data
            notificationHandle.attrHandle = CYBLE_PADDESTOEL_NOTIFY_CHAR_HANDLE;
            notificationHandle.value.val = &value;
            notificationHandle.value.len = 1;
            // Send Notification
            CyBle_GattsNotification(cyBle_connHandle, &notificationHandle);
            CyDelay(100); //Short delay to make sure the user doesn't get flooded with notifications between when they send the request to disable the notification and when GATT Server actually disables the notification
            value_prev = value;
            }
        }
        
        
    }
}


/* [] END OF FILE */
