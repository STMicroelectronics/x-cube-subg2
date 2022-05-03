
## <b>Serial Sniffer Readme</b>

This is a simple sniffer. The sniffer runs a device that is connected
to a local USB/serial port and prints out sniffed packets as
pcap-compatible data that can be fed directly into Wireshark. Allows
real-time sniffing.

Prerequisites:
- Cygwin
- Perl
- Whireshark

How to use the sniffer
Once the board is flashed with the proper binary, open a cygwin shell in this folder
(Utilities/PC_Software/SerialSniffer) and issue the following command line:

 ./serialdump-windows.exe -b115200 /dev/ttySX  | ./convert-to-binary | [PATH_TO_]/wireshark.exe -k -i -

(mind the trailing dash!)
ttySx is dependent on which device number your Nucleo board will take.
whireshark.exe must be in path or you must provide the full path.


### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2021 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
