# 2024_01 System Programming Team Project
## How to Run (Client)
install [wiringpi](https://github.com/WiringPi/WiringPi)

compile with makefile in client folder

run with ./Client <server_ip> <server_port>



on fatal errors:

stop the program with provided executable (./stop)

re-start with ./Client <server_ip> <server_port> <x_coordinates> <y_coordinates> <facing_direction>

where the coordinates are the node the bot is looking at

facing_direction is 0: east(+y), 1: south(+x), 2: west(-y), 3: north(-x)

## code by team 모루카
+ 202211127 윤명진
+ 202111126 이나애
+ 202111150 이진희
+ 202111176 조수빈
### notice
Server code might be outdated. Using official up-to-date code is recommended
