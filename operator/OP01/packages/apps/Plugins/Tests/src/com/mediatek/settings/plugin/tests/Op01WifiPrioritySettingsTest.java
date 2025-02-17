/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

package com.mediatek.settings.plugin.tests;

import android.content.Context;
import android.app.ActionBar;
import android.app.Activity;
import android.app.Instrumentation;
import android.test.ActivityInstrumentationTestCase2;
import android.view.KeyEvent;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.provider.Settings.System;

import android.content.res.Resources;

import com.mediatek.op01.plugin.WifiPrioritySettings;

import java.util.List;
import com.jayway.android.robotium.solo.Solo;
import com.mediatek.xlog.Xlog;

public class Op01WifiPrioritySettingsTest extends
        ActivityInstrumentationTestCase2<WifiPrioritySettings> {
    public static final String TAG = "Op01WifiPrioritySettingsTest";
    private static final int FIVE_THOUSANDS_MILLISECOND = 5000;
    private static final int TWO_THOUSANDS_MILLISECOND = 2000;
    static final String MTKCMCCAUTO = "CMCC-AUTO";
    static final String MTKCMCCEdu = "CMCC-EDU";

    private WifiPrioritySettings mActivity;
    private Instrumentation mInst;
    private Solo mSolo;
    private WifiManager mWifiManager;
    private List<WifiConfiguration> mConfigs;

    public Op01WifiPrioritySettingsTest() {
        super("com.mediatek.op01.plugin", WifiPrioritySettings.class);
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        Xlog.i(TAG, "setUp()");
        setActivityInitialTouchMode(false);
        mActivity = this.getActivity();
        mInst = this.getInstrumentation();
        mSolo = new Solo(mInst, mActivity);

        mWifiManager = (WifiManager) mActivity.getSystemService(Context.WIFI_SERVICE);
        mWifiManager.setWifiEnabled(true);
        //System.putInt(mActivity.getContentResolver(), System.WIFI_PRIORITY_TYPE, System.WIFI_PRIORITY_TYPE_MAMUAL);
        mSolo.sleep(FIVE_THOUSANDS_MILLISECOND);
    }

    @Override
    public void tearDown() throws Exception {
        Xlog.d(TAG, "tearDown");
        super.tearDown();
        try {
            mSolo.finishOpenedActivities();
        } catch(Exception e) {
            Xlog.d(TAG, "tearDown exception");;
        }
    }

    public void test01_WifiPrioritySettingsTest() {
        Xlog.i(TAG, "test01_WifiPrioritySettingsTest() begin");

        mSolo.sleep(TWO_THOUSANDS_MILLISECOND);
        if (mSolo.searchText(MTKCMCCAUTO)) {
            mSolo.clickOnText(MTKCMCCAUTO);
            mSolo.sleep(TWO_THOUSANDS_MILLISECOND);
            mSolo.clickOnText("2");
            mSolo.sleep(TWO_THOUSANDS_MILLISECOND);

            boolean set = false; 
            mConfigs = mWifiManager.getConfiguredNetworks();
            int configuredApCount = mConfigs.size();
            if (mConfigs != null) {
                for (WifiConfiguration config : mConfigs) {
                    Xlog.i(TAG, "config.SSID" + config.SSID);
                    Xlog.i(TAG, "config.priority" + config.priority);
                    if (MTKCMCCAUTO.equals(removeDoubleQuotes(config.SSID))) {
                        if (config.priority == (configuredApCount-1)) {
                            set = true;
                        }
                    }
                }
            }
            assertTrue(set);
        } else {
            Xlog.i(TAG, "AP doesn't exist");
            assertTrue(false);
        }
    }

    public void test02_WifiPrioritySettingsTest() {
        Xlog.i(TAG, "test02_WifiPrioritySettingsTest() begin");

        mSolo.sleep(TWO_THOUSANDS_MILLISECOND);
        if (mSolo.searchText(MTKCMCCAUTO)) {
            mSolo.clickOnText(MTKCMCCAUTO);
            mSolo.sleep(TWO_THOUSANDS_MILLISECOND);
            mSolo.clickOnText("1");
            mSolo.sleep(TWO_THOUSANDS_MILLISECOND);

            boolean set = false; 
            mConfigs = mWifiManager.getConfiguredNetworks();
            int configuredApCount = mConfigs.size();
            Xlog.i(TAG, "configuredApCount" + configuredApCount);
            if (mConfigs != null) {
                for (WifiConfiguration config : mConfigs) {
                    Xlog.i(TAG, "config.SSID" + config.SSID);
                    Xlog.i(TAG, "config.priority" + config.priority);
                    if (MTKCMCCAUTO.equals(removeDoubleQuotes(config.SSID))) {
                        Xlog.i(TAG, "aaaa");
                        //priority = configuredApCount - order + 1;
                        if (config.priority == configuredApCount) {
                            Xlog.i(TAG, "bbbb");
                            set = true;
                        }
                    }
                }
            }
            assertTrue(set);
        } else {
            Xlog.i(TAG, "AP doesn't exist");
            assertTrue(false);
        }
    }

    private String removeDoubleQuotes(String string) {
        int length = string.length();
        if ((length > 1) && (string.charAt(0) == '"')
                && (string.charAt(length - 1) == '"')) {
            return string.substring(1, length - 1);
        }
        return string;
    }

}
