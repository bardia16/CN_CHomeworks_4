# Code
The first one is TCP Reno in Reno.cpp file. It has a class called TCP connections which has private variables such as cwnd, ssthresh, rtt, fast_recovery and sending_rate along with a constructor to set the values, with cwnd and ssthresh given to the constructer and the rest taking a default value. 

onPacketLoss() is called when the receiver detects a packet loss. It updates ssthresh to half of the current congestion window, sets the congestion window to 1, and enables fast recovery mode.

SendData() function is used to simulate sending data packets. If the connection is in fast recovery mode, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by the minimum of 1 and the sending rate. Then, the congestion window is limited by the slow start threshold. Finally, the sending rate is updated to match the current congestion window.

onRTTUpdate() function is called when the round-trip time (RTT) is updated. If the connection is not in fast recovery mode, it checks if the congestion window is less than the slow start threshold. If true, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by 1 / cwnd. If the connection is in fast recovery mode, the congestion window is set to the slow start threshold, and fast recovery mode is disabled. 

In the main() function, an array of TCPConnection objects named connections is created, with each object having a different initial congestion window and slow start threshold values. The code then simulates sending data using the current congestion window and slow start threshold for each connection.

TCP New Reno is the same as TCP Reno with only one minor change. There is a onSelectiveAck() function which is called when the receiver sends a selective acknowledgment (SACK). It updates the SACK information and adjusts the congestion window accordingly. If the connection is in fast recovery mode, the congestion window is set to the sum of the SACK size and the slow start threshold. Otherwise, the congestion window is incremented by the SACK size but limited by the slow start threshold. In the main function, we add a SACK bool vector to simulate the SACK info the receiver sends to us. It'll send it after a packet loss. 

The next one's BBR (Bottleneck Bandwidth and Round-trip time) algorithm is a congestion control algorithm used in TCP (Transmission Control Protocol).
 
Estimate the bottleneck bandwidth: The BBR algorithm estimates the bottleneck bandwidth of the network by measuring the sending rate of the TCP connection and comparing it to the maximum sending rate of the bottleneck link. This is done by sending packets at different rates and measuring the round-trip time of the network. The sending rate is increased until the round-trip time stops decreasing, which indicates that the bottleneck link has been saturated. The estimated bottleneck bandwidth is then the sending rate at which the round-trip time stopped decreasing. 
 
Estimate the round-trip time: The BBR algorithm estimates the round-trip time of the network by measuring the time it takes for a packet to travel from the sender to the receiver and back. This is done by sending packets at a high rate and measuring the round-trip time of each packet. The minimum round-trip time observed is used as the estimate of the round-trip time. 
 
Calculate the BBR pacing rate: The BBR algorithm calculates the pacing rate at which packets should be sent based on the estimated bottleneck bandwidth and round-trip time. The pacing rate is the rate at which packets should be sent to maintain a steady flow of data without overloading the network. 
 
Calculate the BBR congestion window: The BBR algorithm calculates the congestion window based on the estimated bottleneck bandwidth and round-trip time. The congestion window is the maximum number of packets that can be sent without causing congestion in the network. 
 
Adjust the sending rate: The BBR algorithm adjusts the sending rate of the TCP connection based on the calculated pacing rate and congestion window. If the pacing rate is lower than the sending rate, the sending rate is decreased to match the pacing rate. If the congestion window is lower than the sending rate, the sending rate is decreased to match the congestion window. 
 
Handle packet loss: The BBR algorithm handles packet loss by reducing the congestion window and entering a congestion control mode. During congestion control, the sending rate is gradually increased until the bottleneck bandwidth is reached again. 
 
# Questions
1) The difference between congestion control and flow control: Congestion control is a collection of techniques used to decrease or prevent congestion in a network. Congestion control mechanisms work at the network layer and are primarily focused on maintaining the stability and efficiency of the overall network. On the other hand, Flow control is related to managing data transmission between a sender and a receiver. Flow control mechanisms operate at the transport layer and are primarily concerned with regulating the flow of data between individual communication endpoints
