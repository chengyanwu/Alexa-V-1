/* ****************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

#include "adc.h"
#include "dma.h"
#include "adc_revb.h"
#include "adc_regs.h"

#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_delay.h"
#include "mxc_sys.h"
#include "mcr_regs.h"
#include "mxc_lock.h"
#include <stdio.h>

// Mask for all Interrupt Enable Fields
#define ADC_IE_MASK (MXC_F_ADC_INTEN_READY       | MXC_F_ADC_INTEN_ABORT    | MXC_F_ADC_INTEN_START_DET | \
                     MXC_F_ADC_INTEN_SEQ_STARTED | MXC_F_ADC_INTEN_SEQ_DONE | MXC_F_ADC_INTEN_CONV_DONE | \
                     MXC_F_ADC_INTEN_CLIPPED     | MXC_F_ADC_INTEN_FIFO_LVL | MXC_F_ADC_INTEN_FIFO_UFL  | \
                     MXC_F_ADC_INTEN_FIFO_OFL )

#define ADC_IF_MASK (MXC_F_ADC_INTFL_READY       | MXC_F_ADC_INTFL_ABORT    | MXC_F_ADC_INTFL_START_DET | \
                     MXC_F_ADC_INTFL_SEQ_STARTED | MXC_F_ADC_INTFL_SEQ_DONE | MXC_F_ADC_INTFL_CONV_DONE | \
                     MXC_F_ADC_INTFL_CLIPPED     | MXC_F_ADC_INTFL_FIFO_LVL | MXC_F_ADC_INTFL_FIFO_UFL  | \
                     MXC_F_ADC_INTFL_FIFO_OFL )

static mxc_adc_complete_cb_t async_callback;
static mxc_adc_conversion_req_t* async_req;
// static volatile uint8_t flag;      //indicates  to irqhandler where to store data

int MXC_ADC_RevB_Init(mxc_adc_req_t *req)
{
    if(req == NULL) {
        return E_NULL_PTR;
    }

    if((req->trackCount<4) || (req->idleCount<17)) {
            return E_BAD_PARAM;
    }
    //Power up to Sleep State
    MXC_ADC->clkctrl |= (req->clock << MXC_F_ADC_CLKCTRL_CLKSEL) & MXC_F_ADC_CLKCTRL_CLKSEL;

    MXC_ADC->clkctrl |= (req->clkdiv << MXC_F_ADC_CLKCTRL_CLKDIV_POS) & MXC_F_ADC_CLKCTRL_CLKDIV;

    MXC_ADC->ctrl0 |= MXC_F_ADC_CTRL0_RESETB;

    //Move to NAP state
    MXC_ADC->ctrl0 |= MXC_F_ADC_CTRL0_BIAS_EN;
    MXC_Delay(500);    

    //calibration
    if(req->cal == MXC_ADC_EN_CAL) {
        MXC_ADC->ctrl0 &= ~MXC_F_ADC_CTRL0_SKIP_CAL;
    }
    else {
        MXC_ADC->ctrl0 |= MXC_F_ADC_CTRL0_SKIP_CAL;
    }

    MXC_ADC->sampclkctrl |= (req->trackCount << MXC_F_ADC_SAMPCLKCTRL_TRACK_CNT_POS) & MXC_F_ADC_SAMPCLKCTRL_TRACK_CNT;
    MXC_ADC->sampclkctrl |= (req->idleCount << MXC_F_ADC_SAMPCLKCTRL_IDLE_CNT_POS) & MXC_F_ADC_SAMPCLKCTRL_IDLE_CNT;

    MXC_ADC->ctrl0 |= MXC_F_ADC_CTRL0_ADC_EN;

    //wait for calibration to complete
    while(!(MXC_ADC->intfl & MXC_F_ADC_INTFL_READY));

    async_callback = NULL;
    
    async_req = NULL;
    
    return E_NO_ERROR;
}

int MXC_ADC_RevB_Shutdown(void)
{   
    if (async_callback != NULL) {
        MXC_FreeLock((uint32_t*) &async_callback);
    }
    
    if (async_req != NULL) {
        MXC_FreeLock((uint32_t*) &async_req);
    }
    
    MXC_ADC->ctrl0 &= ~MXC_F_ADC_CTRL0_ADC_EN;

    return E_NO_ERROR;
}

void MXC_ADC_RevB_EnableInt(uint32_t flags)
{
    MXC_ADC->inten |= (flags & ADC_IE_MASK);
}

void MXC_ADC_RevB_DisableInt(uint32_t flags)
{
    MXC_ADC->inten &= ~(flags & ADC_IE_MASK);
}

int MXC_ADC_RevB_GetFlags(void)
{
    return (MXC_ADC->intfl & ADC_IF_MASK);
}

void MXC_ADC_RevB_ClearFlags(uint32_t flags)
{
    // Write 1 to clear flags
    MXC_ADC->intfl |= (flags & ADC_IF_MASK);
}

void MXC_ADC_RevB_ClockSelect(mxc_adc_clock_t clock)
{
    MXC_ADC->clkctrl |= (clock << MXC_F_ADC_CLKCTRL_CLKSEL) & MXC_F_ADC_CLKCTRL_CLKSEL;
}

int MXC_ADC_RevB_StartConversion(mxc_adc_conversion_req_t *req)
{
    if (req->channel > MXC_ADC_CH_8) {
        return E_BAD_PARAM;
    }

    // store conversion req
    async_req = req;

    //number of samples to average
    MXC_ADC->ctrl1 |= req->avg_number;

    // select between software and hardware trigger
    if(req->trig == MXC_ADC_TRIG_SOFTWARE) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_TRIG_MODE;
    }
    else {
        MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_TRIG_MODE; 
    }

    // select between atomic and continuous conversion mode
    if(req->mode == MXC_ADC_ATOMIC_CONV) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_CNV_MODE;
    }
    else {
        return E_BAD_PARAM;
        // MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_CNV_MODE;
        // MXC_ADC_RevB_StartContinuousConversion();
    }

    MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_START;

    while((MXC_ADC->intfl & MXC_F_ADC_INTFL_SEQ_DONE) == 0);
    
    return E_NO_ERROR;    
}

int MXC_ADC_RevB_StartConversionAsync(mxc_adc_conversion_req_t *req, mxc_adc_complete_cb_t callback)
{
    if (req->channel > MXC_ADC_CH_8) {
        return E_BAD_PARAM;
    }

    if(callback == NULL) {
        return E_BAD_PARAM;
    }

    while (MXC_GetLock((uint32_t*) &async_callback, (uint32_t) callback) != E_NO_ERROR);

    //number of samples to average
    MXC_ADC->ctrl1 |= req->avg_number;

    // select between software and hardware trigger
    if(req->trig == MXC_ADC_TRIG_SOFTWARE) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_TRIG_MODE;
    }
    else {
        MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_TRIG_MODE; 
    }

    // Clear interrupt flags
    MXC_ADC_RevB_ClearFlags(ADC_IF_MASK);

    // select between atomic and continuous conversion mode
    if(req->mode == MXC_ADC_ATOMIC_CONV) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_CNV_MODE;
        MXC_ADC_RevB_EnableInt(MXC_F_ADC_INTEN_SEQ_DONE);        
    }
    else {
        MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_CNV_MODE;
        MXC_ADC_RevB_EnableInt(MXC_F_ADC_INTEN_SEQ_DONE | MXC_F_ADC_INTEN_CONV_DONE);        
    }

    MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_START;
    
    return E_NO_ERROR;  
}

int MXC_ADC_RevB_StartConversionDMA(mxc_adc_conversion_req_t *req, uint16_t* data, void (*callback)(int, int))
{
    if (req->channel > MXC_ADC_CH_8) {
        return E_BAD_PARAM;
    }

    if(callback == NULL) {
        return E_BAD_PARAM;
    }

    if (data == NULL) {
        return E_NULL_PTR;
    }

    uint8_t channel;
    mxc_dma_config_t config;    
    mxc_dma_srcdst_t srcdst;

    //number of samples to average
    MXC_ADC->ctrl1 |= req->avg_number;

    // select between software and hardware trigger
    if(req->trig == MXC_ADC_TRIG_SOFTWARE) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_TRIG_MODE;
    }
    else {
        MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_TRIG_MODE; 
    }

    // Clear interrupt flags
    MXC_ADC_RevB_ClearFlags(ADC_IF_MASK);

    // select between atomic and continuous conversion mode
    if(req->mode == MXC_ADC_ATOMIC_CONV) {
        MXC_ADC->ctrl1 &= ~MXC_F_ADC_CTRL1_CNV_MODE;
    }
    else {
        MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_CNV_MODE;
    }

    MXC_ADC->fifodmactrl |= MXC_F_ADC_FIFODMACTRL_FLUSH;
    MXC_ADC->fifodmactrl |= MXC_S_ADC_FIFODMACTRL_DATA_FORMAT_DATA_STATUS;
    MXC_ADC->fifodmactrl |= (1 << MXC_F_ADC_FIFODMACTRL_THRESH_POS);
        
    channel = MXC_DMA_AcquireChannel();
    
    config.reqsel = MXC_S_DMA_CTRL_REQUEST_ADC;
    config.ch = channel;
    
    config.srcwd = MXC_DMA_WIDTH_HALFWORD;
    config.dstwd = MXC_DMA_WIDTH_HALFWORD;
    
    config.srcinc_en = 0;
    config.dstinc_en = 0;
    
    srcdst.ch   = channel;
    srcdst.dest = data;
    srcdst.len  = 2;
    
    MXC_DMA_ConfigChannel(config, srcdst);
    
    MXC_DMA_SetCallback(channel, callback);

    MXC_DMA->ch[channel].ctrl |= 2 << MXC_F_DMA_CTRL_BURST_SIZE_POS;
      
    MXC_DMA_EnableInt(channel);

    MXC_ADC->ctrl1 |= MXC_F_ADC_CTRL1_START;
    
    MXC_DMA_Start(channel);
    MXC_DMA->ch[channel].ctrl |= MXC_F_DMA_CTRL_CTZ_IE;
    MXC_ADC->fifodmactrl |= MXC_F_ADC_FIFODMACTRL_DMA_EN;

    return E_NO_ERROR;
}

int MXC_ADC_RevB_Handler(void)
{
    uint32_t flags;
    uint16_t data;
    int error;
    
    if ((error = MXC_ADC_RevB_GetData(&data)) != E_NO_ERROR) {
        return error;
    }

    mxc_adc_conversion_req_t *temp = async_req;

    flags = MXC_ADC_RevB_GetFlags();

    if (flags & MXC_F_ADC_INTEN_SEQ_DONE) {
        mxc_adc_complete_cb_t cb = async_callback;
        MXC_ADC_RevB_ClearFlags(MXC_F_ADC_INTEN_SEQ_DONE);
        if(temp->mode == MXC_ADC_ATOMIC_CONV) {
            MXC_ADC_RevB_DisableInt(MXC_F_ADC_INTEN_SEQ_DONE);
            MXC_FreeLock((uint32_t*) &async_callback);
        }
        //read data
        (cb)(NULL, data);
    }
    
    if (flags & MXC_F_ADC_INTEN_CONV_DONE) {
        mxc_adc_complete_cb_t cb = async_callback;
        MXC_FreeLock((uint32_t*) &async_callback);
        MXC_ADC_RevB_ClearFlags(MXC_F_ADC_INTEN_CONV_DONE);
        MXC_ADC_RevB_DisableInt(MXC_F_ADC_INTEN_CONV_DONE);
        //read data
        (cb)(NULL, data);
    }  

    return E_NO_ERROR;  
}

// ************************************* Function to Read ADC Data *******************************************
int MXC_ADC_RevB_GetData(uint16_t* outdata)
{
    if(MXC_ADC->data & MXC_F_ADC_DATA_INVALID) {
        return E_INVALID;
    }

    *outdata = (MXC_ADC->data & MXC_F_ADC_DATA_DATA);

    return E_NO_ERROR;
}
