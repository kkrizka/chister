SET(SRCS main.cpp
  mainwindow.cpp

  NIFrameGrabber.cpp
  DummyFrameGrabber.cpp

  SerialDevice.cpp
  serialconsole.cpp

  ECS02.cpp
  ECS02UI.cpp

  QOpenCVHelpers.cpp

  AnalysisProgram.cpp
  AnalysisProgramGUI.cpp

  ImageScanAnalysis.cpp
  
  SwissHCCAnalysis.cpp
  SwissHCCAnalysisGUI.cpp
  SwissHCC_InfoForm.cpp
  SwissHCC_ConfigureForm.cpp
  SwissHCC_LoadChipsForm.cpp
  SwissHCC_CrossAlignForm.cpp
  SwissHCC_ChipTestForm.cpp
  SwissHCC_SummaryForm.cpp
  MicroZedHCC.cpp)

ADD_EXECUTABLE(chister ${SRCS})
TARGET_LINK_LIBRARIES(chister Qt5::Widgets Qt5::SerialPort ${OpenCV_LIBS})

IF(NIIMAQ_FOUND)
	INCLUDE_DIRECTORIES(${NIIMAQ_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(chister ${NIIMAQ_LIBRARIES})
ENDIF(NIIMAQ_FOUND)
