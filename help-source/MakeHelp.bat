@echo off
REM -- Zuerst Zuordnungsdatei der mit Microsoft Visual C++ erzeugten resource.h erstellen
echo // Von MAKEHELP.BAT erzeugte Hilfe-Zuordnungsdatei

echo // Befehle (ID_* and IDM_*)
E:\programme\devstudio\vc\bin\makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 ..\HPetriSim\resource.h >>"HPSim.hm"

echo // Eingabeaufforderungen (IDP_*)
E:\programme\devstudio\vc\bin\makehm IDP_,HIDP_,0x30000 ..\HPetriSim\resource.h >>"HPSim.hm"

echo // Ressourcen (IDR_*)
E:\programme\devstudio\vc\bin\makehm IDR_,HIDR_,0x20000 ..\HPetriSim\resource.h >>"HPSim.hm"

echo // Dialogfelder (IDD_*)
E:\programme\devstudio\vc\bin\makehm IDD_,HIDD_,0x20000 ..\HPetriSim\resource.h >>"HPSim.hm"

echo // Rahmen-Steuerelemente (IDW_*)
E:\programme\devstudio\vc\bin\makehm IDW_,HIDW_,0x50000 ..\HPetriSim\resource.h >>"HPSim.hm"

echo // Komponenten (CG_*)
E:\programme\devstudio\vc\bin\makehm CG_,HIDW_,0x20000 ..\HPetriSim\resource.h >>"HPSim.hm"