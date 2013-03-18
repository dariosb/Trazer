; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Trazer"
!define PRODUCT_VERSION "2.1"
!define PRODUCT_PUBLISHER "Vortex Technology Solutions"
!define PRODUCT_WEB_SITE "http://www.vxtsolutions.com.ar"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\trazer.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
;!insertmacro MUI_PAGE_LICENSE "c:\ruta\ala\licencia\TuLicenciaDeSoftware.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\trazer.exe"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\doc\Trazer Reference Manual.pdf"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Trazer_2_1_w32install.exe"
InstallDir "$PROGRAMFILES\Trazer"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Principal" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "trazer.exe"
  CreateDirectory "$SMPROGRAMS\Trazer"
  CreateShortCut "$SMPROGRAMS\Trazer\Trazer.lnk" "$INSTDIR\trazer.exe"
  CreateShortCut "$DESKTOP\Trazer.lnk" "$INSTDIR\trazer.exe"
  File "trazer.cfg"
SectionEnd

Section "dlls" SEC02
  ExecWait '"dll\vcredist_x86.exe"'
  SetOutPath "$WINDIR\system32"
  SetOverwrite off
  File "dll\dwmapi.dll"
  File "dll\msvcp90.dll"
  File "dll\msvcr90.dll"
SectionEnd

Section "Docs" SEC03
  SetOutPath "$INSTDIR\doc"
  SetOverwrite ifnewer
  File "doc\Trazer Reference Manual.pdf"
SectionEnd

Section "install" SEC04
  SetOutPath "$INSTDIR\install"
  File "dll\vcredist_x86.exe"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Trazer\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Trazer\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\trazer.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\trazer.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "La desinstalaci�n de $(^Name) finaliz� satisfactoriamente."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "�Est� completamente seguro que desea desinstalar $(^Name) junto con todos sus componentes?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\install\vcredist_x86.exe"
  Delete "$INSTDIR\doc\Trazer Reference Manual.pdf"
  Delete "$WINDIR\system32\msvcr90.dll"
  Delete "$WINDIR\system32\msvcp90.dll"
  Delete "$WINDIR\system32\dwmapi.dll"
  Delete "$INSTDIR\trazer.cfg"
  Delete "$INSTDIR\trazer.exe"

  Delete "$SMPROGRAMS\Trazer\Uninstall.lnk"
  Delete "$SMPROGRAMS\Trazer\Website.lnk"
  Delete "$DESKTOP\Trazer.lnk"
  Delete "$SMPROGRAMS\Trazer\Trazer.lnk"

  RMDir "$WINDIR\system32"
  RMDir "$SMPROGRAMS\Trazer"
  RMDir "$INSTDIR\install"
  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
