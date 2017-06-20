Chister is an application designed for automated testing of diced ASIC chips. It controls a probe station to move from chip to chip and sends a signal to a control FPGA to execute a predefined test procedure once a chip is located. Since the chips are already diced, image analysis using OpenCV is performed to correctly align each chip with the probe needles.

The current iteration is designed for testing ATLAS ITk Strip Detector's Hybrid Controller Chips. Work is underway to parametrize the procedure.

**SLC7 Requirements**
- cmake
- qt5-qtbase-devel
- qt5-qtmultimedia-devel
- qt5-qtserialport-devel
- opencv-devel
