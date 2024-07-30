# NetworkDetector
Network detector embedded system made with Arduino and AJAX 

The point of this system is to detect if a network is down, it sends a signal to 5V relay, which activates the AJAX to send a notification about the network status.
Using the Arduino R4 Minima combined with Ethernet Shield 2, trough the RJ45 connector it monitors 2 Internet providers (2 ip's which only the provider can access), the system then sends ICMP Ping to each of the ip's, if
it is unsuccessfull 4 times it activates the relay. Made in Arduino IDE , in C++ using andrew-susanto's fork of Blake Foster's ICMP Library https://github.com/andrew-susanto/Arduino-Ethernet-Icmp.
