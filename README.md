# Code
The first one is TCP Reno in Reno.cpp file. It has a class called TCP connections which has private variables such as cwnd, ssthresh, rtt, fast_recovery and sending_rate along with a constructor to set the values, with cwnd and ssthresh given to the constructer and the rest taking a default value. 

onPacketLoss() is called when the receiver detects a packet loss. It updates ssthresh to half of the current congestion window, sets the congestion window to 1, and enables fast recovery mode.

SendData() function is used to simulate sending data packets. If the connection is in fast recovery mode, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by the minimum of 1 and the sending rate. Then, the congestion window is limited by the slow start threshold. Finally, the sending rate is updated to match the current congestion window.

onRTTUpdate() function is called when the round-trip time (RTT) is updated. If the connection is not in fast recovery mode, it checks if the congestion window is less than the slow start threshold. If true, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by 1 / cwnd. If the connection is in fast recovery mode, the congestion window is set to the slow start threshold, and fast recovery mode is disabled. 

In the main() function, an array of TCPConnection objects named connections is created, with each object having a different initial congestion window and slow start threshold values. The code then simulates sending data using the current congestion window and slow start threshold for each connection.

TCP New Reno is the same as TCP Reno with only one minor change. There is a onSelectiveAck() function which is called when the receiver sends a selective acknowledgment (SACK). It updates the SACK information and adjusts the congestion window accordingly. If the connection is in fast recovery mode, the congestion window is set to the sum of the SACK size and the slow start threshold. Otherwise, the congestion window is incremented by the SACK size but limited by the slow start threshold. In the main function, we add a SACK bool vector to simulate the SACK info the receiver sends to us. It'll send it after a packet loss. 


# Questions
1) The difference between congestion control and flow control: Congestion control is a collection of techniques used to decrease or prevent congestion in a network. Congestion control mechanisms work at the network layer and are primarily focused on maintaining the stability and efficiency of the overall network. On the other hand, Flow control is related to managing data transmission between a sender and a receiver. Flow control mechanisms operate at the transport layer and are primarily concerned with regulating the flow of data between individual communication endpoints
