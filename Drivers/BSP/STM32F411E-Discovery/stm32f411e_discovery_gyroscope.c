/**
  ******************************************************************************
  * @file    stm32f411e_discovery_gyroscope.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the
  *          MEMS gyroscope available on STM32F411E-Discovery Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f411e_discovery_gyroscope.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F411E_DISCOVERY
  * @{
  */  
  
/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE STM32F411E DISCOVERY GYROSCOPE
  * @{
  */


/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_TypesDefinitions STM32F411E DISCOVERY GYROSCOPE Private TypesDefinitions
  * @{
  */ 
/**
  * @}
  */

/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_Defines STM32F411E DISCOVERY GYROSCOPE Private Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_Macros STM32F411E DISCOVERY GYROSCOPE Private Macros
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_Variables STM32F411E DISCOVERY GYROSCOPE Private Variables
  * @{
  */ 
static GYRO_DrvTypeDef *GyroscopeDrv;
/**
  * @}
  */

/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_FunctionPrototypes STM32F411E DISCOVERY GYROSCOPE Private FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F411E_DISCOVERY_GYROSCOPE_Private_Functions STM32F411E DISCOVERY GYROSCOPE Private Functions
  * @{
  */

/**
  * @brief  Set Gyroscope Initialization.
  * @retval GYRO_OK if no problem during initialization
  */
uint8_t BSP_GYRO_Init(void)
{  
  uint8_t ret = GYRO_ERROR;
  uint16_t ctrl = 0x0000;
  GYRO_InitTypeDef         I3G4250D_InitStructure;
  GYRO_FilterConfigTypeDef I3G4250D_FilterStructure = {0,0};
 
  if(I3g4250Drv.ReadID() == I_AM_I3G4250D)
  {
    /* Initialize the Gyroscope driver structure */
    GyroscopeDrv = &I3g4250Drv;

    /* MEMS configuration ----------------------------------------------------*/
    /* Fill the Gyroscope structure */
    I3G4250D_InitStructure.Power_Mode = I3G4250D_MODE_ACTIVE;
    I3G4250D_InitStructure.Output_DataRate = I3G4250D_OUTPUT_DATARATE_1;
    I3G4250D_InitStructure.Axes_Enable = I3G4250D_AXES_ENABLE;
    I3G4250D_InitStructure.Band_Width = I3G4250D_BANDWIDTH_4;
    I3G4250D_InitStructure.BlockData_Update = I3G4250D_BlockDataUpdate_Continous;
    I3G4250D_InitStructure.Endianness = I3G4250D_BLE_LSB;
    I3G4250D_InitStructure.Full_Scale = I3G4250D_FULLSCALE_500;
  
    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl = (uint16_t) (I3G4250D_InitStructure.Power_Mode | I3G4250D_InitStructure.Output_DataRate | \
                      I3G4250D_InitStructure.Axes_Enable | I3G4250D_InitStructure.Band_Width);
  
    ctrl |= (uint16_t) ((I3G4250D_InitStructure.BlockData_Update | I3G4250D_InitStructure.Endianness | \
                        I3G4250D_InitStructure.Full_Scale) << 8);

    /* Configure the Gyroscope main parameters */
    GyroscopeDrv->Init(ctrl);
  
    I3G4250D_FilterStructure.HighPassFilter_Mode_Selection =I3G4250D_HPM_NORMAL_MODE_RES;
    I3G4250D_FilterStructure.HighPassFilter_CutOff_Frequency = I3G4250D_HPFCF_0;
  
    ctrl = (uint8_t) ((I3G4250D_FilterStructure.HighPassFilter_Mode_Selection |\
                       I3G4250D_FilterStructure.HighPassFilter_CutOff_Frequency));
  
    /* Configure the Gyroscope main parameters */
    GyroscopeDrv->FilterConfig(ctrl) ;
  
    GyroscopeDrv->FilterCmd(I3G4250D_HIGHPASSFILTER_ENABLE);
  
    ret = GYRO_OK;
  }
  return ret;
}

/**
  * @brief  Read ID of Gyroscope component.
  * @retval ID
  */
uint8_t BSP_GYRO_ReadID(void)
{
  uint8_t id = 0x00;
  
  if(GyroscopeDrv->ReadID != NULL)
  {
    id = GyroscopeDrv->ReadID();
  }  
  return id;
}

/**
  * @brief  Reboot memory content of Gyroscope.
  */
void BSP_GYRO_Reset(void)
{  
  if(GyroscopeDrv->Reset != NULL)
  {
    GyroscopeDrv->Reset();
  }  
}

/**
  * @brief  Configures INT1 interrupt.
  * @param  pIntConfig: pointer to a L3GD20_InterruptConfig_TypeDef 
  *         structure that contains the configuration setting for the L3GD20 Interrupt.
  */
void BSP_GYRO_ITConfig(GYRO_InterruptConfigTypeDef *pIntConfig)
{
  uint16_t interruptconfig = 0x0000;
  
  if(GyroscopeDrv->ConfigIT != NULL)
  {
    /* Configure latch Interrupt request and axe interrupts */                   
    interruptconfig |= ((uint8_t)(pIntConfig->Latch_Request| \
                                  pIntConfig->Interrupt_Axes) << 8);
    
    interruptconfig |= (uint8_t)(pIntConfig->Interrupt_ActiveEdge);
    
    GyroscopeDrv->ConfigIT(interruptconfig);
  }  
}

/**
  * @brief  Enables INT1 or INT2 interrupt.
  * @param  IntPin: Interrupt pin 
  *      This parameter can be: 
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  */
void BSP_GYRO_EnableIT(uint8_t IntPin)
{
  if(GyroscopeDrv->EnableIT != NULL)
  {
    GyroscopeDrv->EnableIT(IntPin);
  }
}

/**
  * @brief  Disables INT1 or INT2 interrupt.
  * @param  IntPin: Interrupt pin 
  *      This parameter can be: 
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  */
void BSP_GYRO_DisableIT(uint8_t IntPin)
{
  if(GyroscopeDrv->DisableIT != NULL)
  {
    GyroscopeDrv->DisableIT(IntPin);
  }
}
  
/**
  * @brief  Get XYZ angular acceleration.
  * @param  pfData: pointer on floating array
  */
void BSP_GYRO_GetXYZ(float *pfData)
{
  if(GyroscopeDrv->GetXYZ!= NULL)
  {   
    GyroscopeDrv->GetXYZ(pfData);
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     

