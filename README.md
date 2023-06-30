# Code
The first one is TCP Reno in Reno.cpp file. It has a class called TCP connections which has private variables such as cwnd, ssthresh, rtt, fast_recovery and sending_rate along with a constructor to set the values, with cwnd and ssthresh given to the constructer and the rest taking a default value. 

onPacketLoss() is called when the receiver detects a packet loss. It updates ssthresh to half of the current congestion window, sets the congestion window to 1, and enables fast recovery mode.

SendData() function is used to simulate sending data packets. If the connection is in fast recovery mode, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by the minimum of 1 and the sending rate. Then, the congestion window is limited by the slow start threshold. Finally, the sending rate is updated to match the current congestion window.

onRTTUpdate() function is called when the round-trip time (RTT) is updated. If the connection is not in fast recovery mode, it checks if the congestion window is less than the slow start threshold. If true, the congestion window is incremented by 1. Otherwise, the congestion window is incremented by 1 / cwnd. If the connection is in fast recovery mode, the congestion window is set to the slow start threshold, and fast recovery mode is disabled. 

In the main() function, an array of TCPConnection objects named connections is created, with each object having a different initial congestion window and slow start threshold values. The code then simulates sending data using the current congestion window and slow start threshold for each connection.

TCP New Reno is the same as TCP Reno with only one minor change. There is an onSelectiveAck() function which is called when the receiver sends a selective acknowledgment (SACK). It updates the SACK information and adjusts the congestion window accordingly. If the connection is in fast recovery mode, the congestion window is set to the sum of the SACK size and the slow start threshold. Otherwise, the congestion window is incremented by the SACK size but limited by the slow start threshold. In the main function, we add a SACK bool vector to simulate the SACK info the receiver sends to us. It'll send it after a packet loss. 

The next one's BBR (Bottleneck Bandwidth and Round-trip time) algorithm is a congestion control algorithm used in TCP (Transmission Control Protocol). It has a class called TCPConnection which has a constructor that initializes the TCP connection with initial values for cwnd and ssthresh.

onPacketLoss(): Handles the packet loss event by adjusting the congestion window size (cwnd) and entering the fast recovery state.

SendData(): Sends data over the TCP connection, adjusting the congestion window size based on the current state.

onRTTUpdate(int new_rtt): Updates the round-trip time (rtt) and adjusts the congestion window size based on the current state.

onSelectiveAck(const std::vector<bool> new_sack_info): Handles the selective acknowledgment event by updating BBR variables and adjusting the congestion window size accordingly.

onPacketSent(): Handles the packet sent event by updating BBR variables, including the sending rate, full bandwidth estimation, minimum RTT, and extra acked bytes. It also handles BBR probing.
 
# Questions
1) The difference between congestion control and flow control: Congestion control is a collection of techniques used to decrease or prevent congestion in a network. Congestion control mechanisms work at the network layer and are primarily focused on maintaining the stability and efficiency of the overall network. On the other hand, Flow control is related to managing data transmission between a sender and a receiver. Flow control mechanisms operate at the transport layer and are primarily concerned with regulating the flow of data between individual communication endpoints
2) TCP New Reno is a congestion control algorithm used in TCP to manage network congestion and ensure reliable data transmission. It is an enhancement of the original TCP Reno algorithm, aiming to improve performance in scenarios with multiple packet losses. It combines fast recovery and fast retransmit techniques to minimize the impact of packet losses on the network throughput and responsiveness. By responding quickly to losses and adjusting the congestion window appropriately, TCP New Reno aims to achieve better performance and fairness in various network conditions. The main difference is when the sender receives three or more duplicate ACKs for the same sequence number, it assumes that a segment has been lost and performs a retransmission of that segment without waiting for the retransmission timeout (RTO) timer to expire. The cwnd is set to the number of unacknowledged packets plus three.
3) BBR (Bottleneck Bandwidth and Round-trip time) algorithm is a congestion control algorithm used in TCP (Transmission Control Protocol) to improve network performance and reduce latency. It works by estimating the bottleneck bandwidth and round-trip time of the network and using this information to adjust the sending rate of the TCP connection. Overall, the BBR algorithm is designed to provide high throughput and low latency in modern networks with large bandwidth-delay products. It is particularly effective in networks with high bandwidth, long round-trip times, and variable link capacities.
4) When TCP Reno experiences packet loss, it assumes that the network is congested and reacts by cutting the congestion window size in half. While in TCP New Reno, after the first loss, it reduces the congestion window by a smaller amount, typically by a factor of 2. After subsequent losses within the same RTT, it reduces the congestion window more significantly to control the sending rate. After detecting packet loss, TCP Reno enters a fast recovery phase. During this phase, instead of going back to the slow-start phase, it increases the congestion window linearly to recover the lost sending rate. TCP New Reno also incorporates fast recovery but extends it to handle multiple losses within a single RTT more effectively. It maintains a separate state to keep track of the number of duplicate ACKs received, enabling it to recover from multiple losses without unnecessarily reducing the congestion window. Both TCP Reno and TCP New Reno do not explicitly estimate the available bandwidth or round-trip time of the network path. BBR is specifically designed to estimate the available bottleneck bandwidth and RTT. It achieves this by carefully measuring the delivery rate of packets and the minimum round-trip time. BBR utilizes this information to adaptively adjust the sending rate to fully utilize available bandwidth while keeping the network buffer occupancy low. Both TCP Reno and TCP New Reno primarily focus on throughput and react to packet loss events as indications of congestion. They aim to maximize throughput while maintaining fairness. BBR places emphasis on both throughput and latency. By estimating available bandwidth and RTT, it strives to achieve high throughput while minimizing queuing delays and buffer bloat.
5) a
6) TCP Vegas: TCP Vegas is an alternative congestion control algorithm that focuses on minimizing queuing delays rather than reacting solely to packet loss. It estimates the available bandwidth by measuring the queuing delay and adjusts the sending rate accordingly to maintain a low delay. It has a couple of advantages. TCP Vegas is designed to prioritize minimizing queuing delays. It estimates the available bandwidth by measuring the queuing delay instead of relying solely on packet loss as an indication of congestion. This approach allows it to adjust the sending rate proactively, resulting in lower queuing delays and improve application responsiveness. TCP Vegas aims to achieve fairness among flows sharing the same network link. By focusing on queuing delay rather than packet loss, it can respond to congestion earlier and more accurately. This proactive behavior helps ensure that competing flows have a fair share of the available bandwidth, leading to improved fairness in resource allocation. TCP Vegas exhibits better stability characteristics in some network scenarios. By reacting to congestion early and reducing the sending rate more gradually, it can avoid sudden and drastic rate reductions that may cause instability in the network. TCP Vegas is particularly well-suited for high-speed networks where queuing delays play a significant role. In such environments, the emphasis on reducing queuing delays can lead to improved performance compared to loss-based congestion control algorithms like TCP Reno and TCP New Reno.
TCP Cubic: TCP Cubic is a widely deployed congestion control algorithm that aims to provide a scalable and stable throughput in high-speed and long-distance networks. It utilizes a cubic function to estimate the congestion window size based on the round-trip time and reacts to congestion signals by reducing the sending rate. TCP Cubic is designed to provide better scalability and stability in high-speed and long-distance networks. It utilizes a cubic function to estimate the congestion window size, allowing it to scale the sending rate more efficiently compared to the linear increase of TCP Reno and TCP New Reno. This scalability is particularly beneficial in networks with large bandwidths and long round-trip times. TCP Cubic aims to achieve fairness among multiple flows competing for network bandwidth. It incorporates a fairness component that considers the number of competing flows and adjusts the sending rate accordingly. This fairness mechanism helps prevent a single flow from dominating the available bandwidth, ensuring equitable sharing among different flows. TCP Cubic is designed to be responsive to network conditions and congestion signals. It reacts to congestion events by reducing the congestion window size, which helps alleviate congestion and mitigate further network issues. By adapting the sending rate based on congestion feedback, TCP Cubic can quickly respond to changes in network conditions. TCP Cubic exhibits better stability characteristics compared to TCP Reno and TCP New Reno in some scenarios. It avoids overly aggressive rate reductions after congestion events, resulting in more stable network behavior. This stability can lead to improved network performance and reduced packet loss.
