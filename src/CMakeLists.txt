set (CMAKE_CXX_STANDARD 11)

SET(SRCS main.cpp
  mainwindow.cpp
  PreferencesDialog.cpp

  DialogMoveTo.cpp

  SerialDevice.cpp
  serialconsole.cpp

  ECS02.cpp
  ECS02UI.cpp

  QOpenCVHelpers.cpp

  AnalysisProgram.cpp
  AnalysisProgramGUI.cpp

  ImageScanAnalysis.cpp
  ImageScanAnalysisGUI.cpp
  ImageScan_StartForm.cpp
  ImageScan_CalibrateForm.cpp
  ImageScan_ScanForm.cpp
  ImageScan_PictureForm.cpp
  
  SwissHCCAnalysis.cpp
  SwissHCCAnalysisGUI.cpp
  SwissHCCPreferencesForm.cpp
  SwissHCCTemplate.cpp
  SwissHCCTemplateLabel.cpp
  SwissHCCTemplateManager.cpp
  SwissHCC_InfoForm.cpp
  SwissHCC_ConfigureForm.cpp
  SwissHCC_LoadChipsForm.cpp
  SwissHCC_CrossAlignForm.cpp
  SwissHCC_ChipTestForm.cpp
  SwissHCC_SummaryForm.cpp
  MicroZedHCC.cpp

  DicedChipAnalysis.cpp
  DicedChipAnalysisGUI.cpp
  DicedChipPreferencesForm.cpp
  DicedChipTemplate.cpp
  DicedChipTemplateLabel.cpp
  DicedChipTemplateManager.cpp
  DicedChip_InfoForm.cpp
  DicedChip_ConfigureForm.cpp
  DicedChip_LoadChipsForm.cpp
  DicedChip_CrossAlignForm.cpp
  DicedChip_ChipTestForm.cpp
  DicedChip_SummaryForm.cpp)

IF(WIN32)
  LIST(APPEND SRCS NIFrameGrabber.cpp)
ELSE()
  LIST(APPEND SRCS V4L2FrameGrabber.cpp)
ENDIF(WIN32)

ADD_EXECUTABLE(chister ${SRCS})
TARGET_LINK_LIBRARIES(chister Qt5::Widgets Qt5::SerialPort ${OpenCV_LIBS})

IF(WIN32 AND NIIMAQ_FOUND)
  INCLUDE_DIRECTORIES(${NIIMAQ_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(chister ${NIIMAQ_LIBRARIES})
ENDIF(WIN32 AND NIIMAQ_FOUND)
