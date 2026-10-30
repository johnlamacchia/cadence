; Auto-generated winmm.dll forwarding stubs
; Each export jumps through a function pointer table filled at runtime

.data
public real_funcs
real_funcs dq 180 dup(0)

.code

proxy_CloseDriver proc
    jmp qword ptr [real_funcs + 0]
proxy_CloseDriver endp

proxy_DefDriverProc proc
    jmp qword ptr [real_funcs + 8]
proxy_DefDriverProc endp

proxy_DriverCallback proc
    jmp qword ptr [real_funcs + 16]
proxy_DriverCallback endp

proxy_DrvGetModuleHandle proc
    jmp qword ptr [real_funcs + 24]
proxy_DrvGetModuleHandle endp

proxy_GetDriverModuleHandle proc
    jmp qword ptr [real_funcs + 32]
proxy_GetDriverModuleHandle endp

proxy_OpenDriver proc
    jmp qword ptr [real_funcs + 40]
proxy_OpenDriver endp

proxy_PlaySound proc
    jmp qword ptr [real_funcs + 48]
proxy_PlaySound endp

proxy_PlaySoundA proc
    jmp qword ptr [real_funcs + 56]
proxy_PlaySoundA endp

proxy_PlaySoundW proc
    jmp qword ptr [real_funcs + 64]
proxy_PlaySoundW endp

proxy_SendDriverMessage proc
    jmp qword ptr [real_funcs + 72]
proxy_SendDriverMessage endp

proxy_WOWAppExit proc
    jmp qword ptr [real_funcs + 80]
proxy_WOWAppExit endp

proxy_auxGetDevCapsA proc
    jmp qword ptr [real_funcs + 88]
proxy_auxGetDevCapsA endp

proxy_auxGetDevCapsW proc
    jmp qword ptr [real_funcs + 96]
proxy_auxGetDevCapsW endp

proxy_auxGetNumDevs proc
    jmp qword ptr [real_funcs + 104]
proxy_auxGetNumDevs endp

proxy_auxGetVolume proc
    jmp qword ptr [real_funcs + 112]
proxy_auxGetVolume endp

proxy_auxOutMessage proc
    jmp qword ptr [real_funcs + 120]
proxy_auxOutMessage endp

proxy_auxSetVolume proc
    jmp qword ptr [real_funcs + 128]
proxy_auxSetVolume endp

proxy_joyConfigChanged proc
    jmp qword ptr [real_funcs + 136]
proxy_joyConfigChanged endp

proxy_joyGetDevCapsA proc
    jmp qword ptr [real_funcs + 144]
proxy_joyGetDevCapsA endp

proxy_joyGetDevCapsW proc
    jmp qword ptr [real_funcs + 152]
proxy_joyGetDevCapsW endp

proxy_joyGetNumDevs proc
    jmp qword ptr [real_funcs + 160]
proxy_joyGetNumDevs endp

proxy_joyGetPos proc
    jmp qword ptr [real_funcs + 168]
proxy_joyGetPos endp

proxy_joyGetPosEx proc
    jmp qword ptr [real_funcs + 176]
proxy_joyGetPosEx endp

proxy_joyGetThreshold proc
    jmp qword ptr [real_funcs + 184]
proxy_joyGetThreshold endp

proxy_joyReleaseCapture proc
    jmp qword ptr [real_funcs + 192]
proxy_joyReleaseCapture endp

proxy_joySetCapture proc
    jmp qword ptr [real_funcs + 200]
proxy_joySetCapture endp

proxy_joySetThreshold proc
    jmp qword ptr [real_funcs + 208]
proxy_joySetThreshold endp

proxy_mciDriverNotify proc
    jmp qword ptr [real_funcs + 216]
proxy_mciDriverNotify endp

proxy_mciDriverYield proc
    jmp qword ptr [real_funcs + 224]
proxy_mciDriverYield endp

proxy_mciExecute proc
    jmp qword ptr [real_funcs + 232]
proxy_mciExecute endp

proxy_mciFreeCommandResource proc
    jmp qword ptr [real_funcs + 240]
proxy_mciFreeCommandResource endp

proxy_mciGetCreatorTask proc
    jmp qword ptr [real_funcs + 248]
proxy_mciGetCreatorTask endp

proxy_mciGetDeviceIDA proc
    jmp qword ptr [real_funcs + 256]
proxy_mciGetDeviceIDA endp

proxy_mciGetDeviceIDFromElementIDA proc
    jmp qword ptr [real_funcs + 264]
proxy_mciGetDeviceIDFromElementIDA endp

proxy_mciGetDeviceIDFromElementIDW proc
    jmp qword ptr [real_funcs + 272]
proxy_mciGetDeviceIDFromElementIDW endp

proxy_mciGetDeviceIDW proc
    jmp qword ptr [real_funcs + 280]
proxy_mciGetDeviceIDW endp

proxy_mciGetDriverData proc
    jmp qword ptr [real_funcs + 288]
proxy_mciGetDriverData endp

proxy_mciGetErrorStringA proc
    jmp qword ptr [real_funcs + 296]
proxy_mciGetErrorStringA endp

proxy_mciGetErrorStringW proc
    jmp qword ptr [real_funcs + 304]
proxy_mciGetErrorStringW endp

proxy_mciGetYieldProc proc
    jmp qword ptr [real_funcs + 312]
proxy_mciGetYieldProc endp

proxy_mciLoadCommandResource proc
    jmp qword ptr [real_funcs + 320]
proxy_mciLoadCommandResource endp

proxy_mciSendCommandA proc
    jmp qword ptr [real_funcs + 328]
proxy_mciSendCommandA endp

proxy_mciSendCommandW proc
    jmp qword ptr [real_funcs + 336]
proxy_mciSendCommandW endp

proxy_mciSendStringA proc
    jmp qword ptr [real_funcs + 344]
proxy_mciSendStringA endp

proxy_mciSendStringW proc
    jmp qword ptr [real_funcs + 352]
proxy_mciSendStringW endp

proxy_mciSetDriverData proc
    jmp qword ptr [real_funcs + 360]
proxy_mciSetDriverData endp

proxy_mciSetYieldProc proc
    jmp qword ptr [real_funcs + 368]
proxy_mciSetYieldProc endp

proxy_midiConnect proc
    jmp qword ptr [real_funcs + 376]
proxy_midiConnect endp

proxy_midiDisconnect proc
    jmp qword ptr [real_funcs + 384]
proxy_midiDisconnect endp

proxy_midiInAddBuffer proc
    jmp qword ptr [real_funcs + 392]
proxy_midiInAddBuffer endp

proxy_midiInClose proc
    jmp qword ptr [real_funcs + 400]
proxy_midiInClose endp

proxy_midiInGetDevCapsA proc
    jmp qword ptr [real_funcs + 408]
proxy_midiInGetDevCapsA endp

proxy_midiInGetDevCapsW proc
    jmp qword ptr [real_funcs + 416]
proxy_midiInGetDevCapsW endp

proxy_midiInGetErrorTextA proc
    jmp qword ptr [real_funcs + 424]
proxy_midiInGetErrorTextA endp

proxy_midiInGetErrorTextW proc
    jmp qword ptr [real_funcs + 432]
proxy_midiInGetErrorTextW endp

proxy_midiInGetID proc
    jmp qword ptr [real_funcs + 440]
proxy_midiInGetID endp

proxy_midiInGetNumDevs proc
    jmp qword ptr [real_funcs + 448]
proxy_midiInGetNumDevs endp

proxy_midiInMessage proc
    jmp qword ptr [real_funcs + 456]
proxy_midiInMessage endp

proxy_midiInOpen proc
    jmp qword ptr [real_funcs + 464]
proxy_midiInOpen endp

proxy_midiInPrepareHeader proc
    jmp qword ptr [real_funcs + 472]
proxy_midiInPrepareHeader endp

proxy_midiInReset proc
    jmp qword ptr [real_funcs + 480]
proxy_midiInReset endp

proxy_midiInStart proc
    jmp qword ptr [real_funcs + 488]
proxy_midiInStart endp

proxy_midiInStop proc
    jmp qword ptr [real_funcs + 496]
proxy_midiInStop endp

proxy_midiInUnprepareHeader proc
    jmp qword ptr [real_funcs + 504]
proxy_midiInUnprepareHeader endp

proxy_midiOutCacheDrumPatches proc
    jmp qword ptr [real_funcs + 512]
proxy_midiOutCacheDrumPatches endp

proxy_midiOutCachePatches proc
    jmp qword ptr [real_funcs + 520]
proxy_midiOutCachePatches endp

proxy_midiOutClose proc
    jmp qword ptr [real_funcs + 528]
proxy_midiOutClose endp

proxy_midiOutGetDevCapsA proc
    jmp qword ptr [real_funcs + 536]
proxy_midiOutGetDevCapsA endp

proxy_midiOutGetDevCapsW proc
    jmp qword ptr [real_funcs + 544]
proxy_midiOutGetDevCapsW endp

proxy_midiOutGetErrorTextA proc
    jmp qword ptr [real_funcs + 552]
proxy_midiOutGetErrorTextA endp

proxy_midiOutGetErrorTextW proc
    jmp qword ptr [real_funcs + 560]
proxy_midiOutGetErrorTextW endp

proxy_midiOutGetID proc
    jmp qword ptr [real_funcs + 568]
proxy_midiOutGetID endp

proxy_midiOutGetNumDevs proc
    jmp qword ptr [real_funcs + 576]
proxy_midiOutGetNumDevs endp

proxy_midiOutGetVolume proc
    jmp qword ptr [real_funcs + 584]
proxy_midiOutGetVolume endp

proxy_midiOutLongMsg proc
    jmp qword ptr [real_funcs + 592]
proxy_midiOutLongMsg endp

proxy_midiOutMessage proc
    jmp qword ptr [real_funcs + 600]
proxy_midiOutMessage endp

proxy_midiOutOpen proc
    jmp qword ptr [real_funcs + 608]
proxy_midiOutOpen endp

proxy_midiOutPrepareHeader proc
    jmp qword ptr [real_funcs + 616]
proxy_midiOutPrepareHeader endp

proxy_midiOutReset proc
    jmp qword ptr [real_funcs + 624]
proxy_midiOutReset endp

proxy_midiOutSetVolume proc
    jmp qword ptr [real_funcs + 632]
proxy_midiOutSetVolume endp

proxy_midiOutShortMsg proc
    jmp qword ptr [real_funcs + 640]
proxy_midiOutShortMsg endp

proxy_midiOutUnprepareHeader proc
    jmp qword ptr [real_funcs + 648]
proxy_midiOutUnprepareHeader endp

proxy_midiStreamClose proc
    jmp qword ptr [real_funcs + 656]
proxy_midiStreamClose endp

proxy_midiStreamOpen proc
    jmp qword ptr [real_funcs + 664]
proxy_midiStreamOpen endp

proxy_midiStreamOut proc
    jmp qword ptr [real_funcs + 672]
proxy_midiStreamOut endp

proxy_midiStreamPause proc
    jmp qword ptr [real_funcs + 680]
proxy_midiStreamPause endp

proxy_midiStreamPosition proc
    jmp qword ptr [real_funcs + 688]
proxy_midiStreamPosition endp

proxy_midiStreamProperty proc
    jmp qword ptr [real_funcs + 696]
proxy_midiStreamProperty endp

proxy_midiStreamRestart proc
    jmp qword ptr [real_funcs + 704]
proxy_midiStreamRestart endp

proxy_midiStreamStop proc
    jmp qword ptr [real_funcs + 712]
proxy_midiStreamStop endp

proxy_mixerClose proc
    jmp qword ptr [real_funcs + 720]
proxy_mixerClose endp

proxy_mixerGetControlDetailsA proc
    jmp qword ptr [real_funcs + 728]
proxy_mixerGetControlDetailsA endp

proxy_mixerGetControlDetailsW proc
    jmp qword ptr [real_funcs + 736]
proxy_mixerGetControlDetailsW endp

proxy_mixerGetDevCapsA proc
    jmp qword ptr [real_funcs + 744]
proxy_mixerGetDevCapsA endp

proxy_mixerGetDevCapsW proc
    jmp qword ptr [real_funcs + 752]
proxy_mixerGetDevCapsW endp

proxy_mixerGetID proc
    jmp qword ptr [real_funcs + 760]
proxy_mixerGetID endp

proxy_mixerGetLineControlsA proc
    jmp qword ptr [real_funcs + 768]
proxy_mixerGetLineControlsA endp

proxy_mixerGetLineControlsW proc
    jmp qword ptr [real_funcs + 776]
proxy_mixerGetLineControlsW endp

proxy_mixerGetLineInfoA proc
    jmp qword ptr [real_funcs + 784]
proxy_mixerGetLineInfoA endp

proxy_mixerGetLineInfoW proc
    jmp qword ptr [real_funcs + 792]
proxy_mixerGetLineInfoW endp

proxy_mixerGetNumDevs proc
    jmp qword ptr [real_funcs + 800]
proxy_mixerGetNumDevs endp

proxy_mixerMessage proc
    jmp qword ptr [real_funcs + 808]
proxy_mixerMessage endp

proxy_mixerOpen proc
    jmp qword ptr [real_funcs + 816]
proxy_mixerOpen endp

proxy_mixerSetControlDetails proc
    jmp qword ptr [real_funcs + 824]
proxy_mixerSetControlDetails endp

proxy_mmDrvInstall proc
    jmp qword ptr [real_funcs + 832]
proxy_mmDrvInstall endp

proxy_mmGetCurrentTask proc
    jmp qword ptr [real_funcs + 840]
proxy_mmGetCurrentTask endp

proxy_mmTaskBlock proc
    jmp qword ptr [real_funcs + 848]
proxy_mmTaskBlock endp

proxy_mmTaskCreate proc
    jmp qword ptr [real_funcs + 856]
proxy_mmTaskCreate endp

proxy_mmTaskSignal proc
    jmp qword ptr [real_funcs + 864]
proxy_mmTaskSignal endp

proxy_mmTaskYield proc
    jmp qword ptr [real_funcs + 872]
proxy_mmTaskYield endp

proxy_mmioAdvance proc
    jmp qword ptr [real_funcs + 880]
proxy_mmioAdvance endp

proxy_mmioAscend proc
    jmp qword ptr [real_funcs + 888]
proxy_mmioAscend endp

proxy_mmioClose proc
    jmp qword ptr [real_funcs + 896]
proxy_mmioClose endp

proxy_mmioCreateChunk proc
    jmp qword ptr [real_funcs + 904]
proxy_mmioCreateChunk endp

proxy_mmioDescend proc
    jmp qword ptr [real_funcs + 912]
proxy_mmioDescend endp

proxy_mmioFlush proc
    jmp qword ptr [real_funcs + 920]
proxy_mmioFlush endp

proxy_mmioGetInfo proc
    jmp qword ptr [real_funcs + 928]
proxy_mmioGetInfo endp

proxy_mmioInstallIOProcA proc
    jmp qword ptr [real_funcs + 936]
proxy_mmioInstallIOProcA endp

proxy_mmioInstallIOProcW proc
    jmp qword ptr [real_funcs + 944]
proxy_mmioInstallIOProcW endp

proxy_mmioOpenA proc
    jmp qword ptr [real_funcs + 952]
proxy_mmioOpenA endp

proxy_mmioOpenW proc
    jmp qword ptr [real_funcs + 960]
proxy_mmioOpenW endp

proxy_mmioRead proc
    jmp qword ptr [real_funcs + 968]
proxy_mmioRead endp

proxy_mmioRenameA proc
    jmp qword ptr [real_funcs + 976]
proxy_mmioRenameA endp

proxy_mmioRenameW proc
    jmp qword ptr [real_funcs + 984]
proxy_mmioRenameW endp

proxy_mmioSeek proc
    jmp qword ptr [real_funcs + 992]
proxy_mmioSeek endp

proxy_mmioSendMessage proc
    jmp qword ptr [real_funcs + 1000]
proxy_mmioSendMessage endp

proxy_mmioSetBuffer proc
    jmp qword ptr [real_funcs + 1008]
proxy_mmioSetBuffer endp

proxy_mmioSetInfo proc
    jmp qword ptr [real_funcs + 1016]
proxy_mmioSetInfo endp

proxy_mmioStringToFOURCCA proc
    jmp qword ptr [real_funcs + 1024]
proxy_mmioStringToFOURCCA endp

proxy_mmioStringToFOURCCW proc
    jmp qword ptr [real_funcs + 1032]
proxy_mmioStringToFOURCCW endp

proxy_mmioWrite proc
    jmp qword ptr [real_funcs + 1040]
proxy_mmioWrite endp

proxy_mmsystemGetVersion proc
    jmp qword ptr [real_funcs + 1048]
proxy_mmsystemGetVersion endp

proxy_sndPlaySoundA proc
    jmp qword ptr [real_funcs + 1056]
proxy_sndPlaySoundA endp

proxy_sndPlaySoundW proc
    jmp qword ptr [real_funcs + 1064]
proxy_sndPlaySoundW endp

proxy_timeBeginPeriod proc
    jmp qword ptr [real_funcs + 1072]
proxy_timeBeginPeriod endp

proxy_timeEndPeriod proc
    jmp qword ptr [real_funcs + 1080]
proxy_timeEndPeriod endp

proxy_timeGetDevCaps proc
    jmp qword ptr [real_funcs + 1088]
proxy_timeGetDevCaps endp

proxy_timeGetSystemTime proc
    jmp qword ptr [real_funcs + 1096]
proxy_timeGetSystemTime endp

proxy_timeGetTime proc
    jmp qword ptr [real_funcs + 1104]
proxy_timeGetTime endp

proxy_timeKillEvent proc
    jmp qword ptr [real_funcs + 1112]
proxy_timeKillEvent endp

proxy_timeSetEvent proc
    jmp qword ptr [real_funcs + 1120]
proxy_timeSetEvent endp

proxy_waveInAddBuffer proc
    jmp qword ptr [real_funcs + 1128]
proxy_waveInAddBuffer endp

proxy_waveInClose proc
    jmp qword ptr [real_funcs + 1136]
proxy_waveInClose endp

proxy_waveInGetDevCapsA proc
    jmp qword ptr [real_funcs + 1144]
proxy_waveInGetDevCapsA endp

proxy_waveInGetDevCapsW proc
    jmp qword ptr [real_funcs + 1152]
proxy_waveInGetDevCapsW endp

proxy_waveInGetErrorTextA proc
    jmp qword ptr [real_funcs + 1160]
proxy_waveInGetErrorTextA endp

proxy_waveInGetErrorTextW proc
    jmp qword ptr [real_funcs + 1168]
proxy_waveInGetErrorTextW endp

proxy_waveInGetID proc
    jmp qword ptr [real_funcs + 1176]
proxy_waveInGetID endp

proxy_waveInGetNumDevs proc
    jmp qword ptr [real_funcs + 1184]
proxy_waveInGetNumDevs endp

proxy_waveInGetPosition proc
    jmp qword ptr [real_funcs + 1192]
proxy_waveInGetPosition endp

proxy_waveInMessage proc
    jmp qword ptr [real_funcs + 1200]
proxy_waveInMessage endp

proxy_waveInOpen proc
    jmp qword ptr [real_funcs + 1208]
proxy_waveInOpen endp

proxy_waveInPrepareHeader proc
    jmp qword ptr [real_funcs + 1216]
proxy_waveInPrepareHeader endp

proxy_waveInReset proc
    jmp qword ptr [real_funcs + 1224]
proxy_waveInReset endp

proxy_waveInStart proc
    jmp qword ptr [real_funcs + 1232]
proxy_waveInStart endp

proxy_waveInStop proc
    jmp qword ptr [real_funcs + 1240]
proxy_waveInStop endp

proxy_waveInUnprepareHeader proc
    jmp qword ptr [real_funcs + 1248]
proxy_waveInUnprepareHeader endp

proxy_waveOutBreakLoop proc
    jmp qword ptr [real_funcs + 1256]
proxy_waveOutBreakLoop endp

proxy_waveOutClose proc
    jmp qword ptr [real_funcs + 1264]
proxy_waveOutClose endp

proxy_waveOutGetDevCapsA proc
    jmp qword ptr [real_funcs + 1272]
proxy_waveOutGetDevCapsA endp

proxy_waveOutGetDevCapsW proc
    jmp qword ptr [real_funcs + 1280]
proxy_waveOutGetDevCapsW endp

proxy_waveOutGetErrorTextA proc
    jmp qword ptr [real_funcs + 1288]
proxy_waveOutGetErrorTextA endp

proxy_waveOutGetErrorTextW proc
    jmp qword ptr [real_funcs + 1296]
proxy_waveOutGetErrorTextW endp

proxy_waveOutGetID proc
    jmp qword ptr [real_funcs + 1304]
proxy_waveOutGetID endp

proxy_waveOutGetNumDevs proc
    jmp qword ptr [real_funcs + 1312]
proxy_waveOutGetNumDevs endp

proxy_waveOutGetPitch proc
    jmp qword ptr [real_funcs + 1320]
proxy_waveOutGetPitch endp

proxy_waveOutGetPlaybackRate proc
    jmp qword ptr [real_funcs + 1328]
proxy_waveOutGetPlaybackRate endp

proxy_waveOutGetPosition proc
    jmp qword ptr [real_funcs + 1336]
proxy_waveOutGetPosition endp

proxy_waveOutGetVolume proc
    jmp qword ptr [real_funcs + 1344]
proxy_waveOutGetVolume endp

proxy_waveOutMessage proc
    jmp qword ptr [real_funcs + 1352]
proxy_waveOutMessage endp

proxy_waveOutOpen proc
    jmp qword ptr [real_funcs + 1360]
proxy_waveOutOpen endp

proxy_waveOutPause proc
    jmp qword ptr [real_funcs + 1368]
proxy_waveOutPause endp

proxy_waveOutPrepareHeader proc
    jmp qword ptr [real_funcs + 1376]
proxy_waveOutPrepareHeader endp

proxy_waveOutReset proc
    jmp qword ptr [real_funcs + 1384]
proxy_waveOutReset endp

proxy_waveOutRestart proc
    jmp qword ptr [real_funcs + 1392]
proxy_waveOutRestart endp

proxy_waveOutSetPitch proc
    jmp qword ptr [real_funcs + 1400]
proxy_waveOutSetPitch endp

proxy_waveOutSetPlaybackRate proc
    jmp qword ptr [real_funcs + 1408]
proxy_waveOutSetPlaybackRate endp

proxy_waveOutSetVolume proc
    jmp qword ptr [real_funcs + 1416]
proxy_waveOutSetVolume endp

proxy_waveOutUnprepareHeader proc
    jmp qword ptr [real_funcs + 1424]
proxy_waveOutUnprepareHeader endp

proxy_waveOutWrite proc
    jmp qword ptr [real_funcs + 1432]
proxy_waveOutWrite endp

end