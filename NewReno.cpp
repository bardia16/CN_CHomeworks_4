#include <iostream> 
#include <vector> 
 
class TCPConnection { 
private: 
    int cwnd; 
    int ssthresh; 
    int rtt; 
    bool retransmitting; 
    bool fast_recovery; 
    int sending_rate; 
    std::vector<bool> sack_info; 
 
public: 
    TCPConnection(int initial_cwnd = 1, int initial_ssthresh = 65535) { 
        cwnd = initial_cwnd; 
        ssthresh = initial_ssthresh; 
        rtt = 0; 
        retransmitting = false; 
        fast_recovery = false; 
        sending_rate = 1; 
    } 
 
    void onPacketLoss() { 
        ssthresh = std::max(2, cwnd / 2); 
        cwnd = 1; 
        fast_recovery = true; 
    } 
 
    void SendData() { 
        if (fast_recovery) { 
            cwnd += 1; 
        } 
        else { 
            cwnd += std::min(1, sending_rate); 
        } 
        cwnd = std::min(cwnd, ssthresh); 
        sending_rate = cwnd; 
    } 
 
    void onRTTUpdate(int new_rtt) { 
        rtt = new_rtt; 
        if (!fast_recovery) { 
            if (cwnd < ssthresh) { 
                cwnd += 1; 
            } 
            else { 
                cwnd += 1 / cwnd; 
            } 
        } 
        else { 
            cwnd = ssthresh; 
            fast_recovery = false; 
        } 
    } 
 
    void onSelectiveAck(const std::vector<bool>& new_sack_info) { 
        sack_info = new_sack_info; 
        int sack_size = sack_info.size();
        if (fast_recovery) { 
            cwnd = sack_size + ssthresh; 
        } 
        else { 
            cwnd = std::min(cwnd + sack_size, ssthresh); 
        } 
    } 
 
    int getCwnd() const { 
        return cwnd; 
    } 
 
    int getSsthresh() const { 
        return ssthresh; 
    } 
    int getRTT() {
        return rtt;
    }
};

int main() {
    int index = 0;
    // Create an array of TCPConnection objects with different initial congestion window and slow start threshold values
    TCPConnection connections[] = {
        TCPConnection(10, 5),
        TCPConnection(5, 2),
        TCPConnection(5, 10),
        TCPConnection(2, 5)
    };

    // Simulate sending data using current congestion window and slow start threshold for each connection
    for (auto& conn : connections) {
        index += 1;
        std::cout<< "After rtt update"<<std::endl << std::endl;
        std::cout << "CWND: " << conn.getCwnd() << ", ssthresh: " << conn.getSsthresh() << ", RTT: " << conn.getRTT() << std::endl << std::endl;
        std::cout << "Sending data using current congestion window and slow start threshold for connection " << index << "...\n\n";
        conn.SendData();
        std::cout << "CWND: " << conn.getCwnd() << ", ssthresh: " << conn.getSsthresh() << ", RTT: " << conn.getRTT() << std::endl << std::endl;


        // Simulate packet loss for each connection
        std::cout << "Packet loss detected for connection " << index << "\n\n";
        conn.onPacketLoss();

        // Simulate selective acknowledgment (SACK) for each connection
        std::cout << "Selective acknowledgment received for connection " << index << "\n\n";
        std::vector<bool> new_sack_info = {true, true, false, true};  // Example SACK information
        conn.onSelectiveAck(new_sack_info);
        
        // Simulate sending data again with updated congestion window and slow start threshold for each connection
        std::cout << "Sending data again using updated congestion window and slow start threshold for connection " << index << "...\n\n";
        conn.SendData();
        
        std::cout << "CWND: " << conn.getCwnd() << ", ssthresh: " << conn.getSsthresh() << ", RTT: " << conn.getRTT() << std::endl << std::endl;
    }

    return 0;
}