package com.kyurime.geometryjump

import android.os.Bundle

class GeometryDashActivity : BaseGeometryDashActivity() {
    init
    {
        System.loadLibrary(GJConstants.FMOD_LIB_NAME)
        System.loadLibrary(GJConstants.COCOS_LIB_NAME)
        System.loadLibrary(GJConstants.MOD_CORE_LIB_NAME)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }
}