# Socket
Use C/C++ to pratice socket program. It's runable in Linux

# How to close a port
sudo netstat -ap | grep :<port_number>
--> tcp  0  0 *:8000   *:* LISTEN  4683/procHoldingPort
--> protocol  IPAddr:port   *:* LISTEN  pid/name_of_process

kill  <pid>
or kill -9 <pid>
--> kill -9 4683 

# How to close a process
• Ctrl-C sends an INT signal ("interrupt", SIGINT)
• Ctrl-Z sends a TSTP signal ("terminal stop", SIGTSTP)
• Ctrl-\ sends a QUIT signal (SIGQUIT)