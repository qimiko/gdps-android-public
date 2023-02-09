#pragma once
#ifndef _PROTOCOLS_FLALERTLAYERPROTOCOL_HPP
#define _PROTOCOLS_FLALERTLAYERPROTOCOL_HPP

class FLAlertLayer;

class FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer*, bool btn) = 0;
};

#endif