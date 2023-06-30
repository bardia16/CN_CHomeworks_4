#include <iostream> 
#include <cmath> 
#include <algorithm> 
#include <vector> 
 
class TCPConnection { 
private: 
    int cwnd; 
    int ssthresh; 
    int rtt; 
    bool retransmitting; 
    bool fast_recovery; 
    int sending_rate; 
    double bbr_gain; 
    double bbr_cwnd_gain; 
    double bbr_pacing_gain; 
    double bbr_full_bw; 
    double bbr_min_rtt; 
    double bbr_last_sample; 
    double bbr_extra_acked; 
    double bbr_btlbw; 
    double bbr_window_gain; 
    double bbr_probe_rtt_start_ts; 
    double bbr_probe_rtt_done_ts; 
    double bbr_probe_rtt_rounds; 
 
public: 
    TCPConnection(int initial_cwnd = 1, int initial_ssthresh = 65535) { 
        cwnd = initial_cwnd; 
        ssthresh = initial_ssthresh; 
        rtt = 0; 
        retransmitting = false; 
        fast_recovery = false; 
        sending_rate = 1; 
        bbr_gain = 2.89; 
        bbr_cwnd_gain = 1.0 / bbr_gain; 
        bbr_pacing_gain = 1.0 / (2.0 - 1.0 / bbr_gain); 
        bbr_full_bw = 0.0; 
        bbr_min_rtt = 0.0; 
        bbr_last_sample = 0.0; 
        bbr_extra_acked = 0.0; 
        bbr_btlbw = 0.0; 
        bbr_window_gain = 1.0; 
        bbr_probe_rtt_start_ts = 0.0; 
        bbr_probe_rtt_done_ts = 0.0; 
        bbr_probe_rtt_rounds = 0.0; 
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
 
    void onSelectiveAck(const std::vector<bool> new_sack_info) { 
        // Update BBR variables 
        double acked_bytes = std::count(new_sack_info.begin(), new_sack_info.end(), true); 
        double expected_rtt = std::max(double(rtt), bbr_min_rtt); 
        double rtt_fraction = rtt / expected_rtt; 
        double btlbw_fraction = bbr_btlbw / sending_rate; 
        double window_fraction = cwnd / ssthresh; 
        double extra_acked_fraction = bbr_extra_acked / cwnd; 
 
        // Compute BBR pacing rate 
        double bbr_pacing_rate = std::min(double(sending_rate), bbr_full_bw * btlbw_fraction * bbr_pacing_gain); 
 
        // Compute BBR congestion window 
        if (btlbw_fraction > window_fraction) { 
            bbr_window_gain = bbr_cwnd_gain; 
        } 
        else { 
            bbr_window_gain = 1.0 / bbr_cwnd_gain; 
        } 
        double bbr_window = bbr_window_gain * btlbw_fraction * sending_rate * expected_rtt; 
 
        // Update congestion window based on BBR 
        if (fast_recovery) { 
            cwnd = std::max(cwnd, int(acked_bytes + bbr_extra_acked)); 
        } 
        else if (rtt_fraction > 0.0 && window_fraction > 0.0) { 
            double bbr_increment = bbr_gain * acked_bytes / cwnd; 
            double bbr_decrement = bbr_gain * extra_acked_fraction; 
            double bbr_delta = bbr_increment - bbr_decrement; 
            cwnd = std::max(cwnd + int(bbr_delta), int(bbr_window)); 
        } 
 
        // Reset extra acked bytes and BBR probe round counter 
        bbr_extra_acked = 0.0; 
        bbr_probe_rtt_rounds = 0.0; 
    } 
 
    void onPacketSent() { 
        // Update BBR pacing rate 
        double bbr_pacing_rate = std::min(double(sending_rate), bbr_full_bw * bbr_btlbw * bbr_pacing_gain); 
 
        // Delay BBR probe if necessary 
        if (bbr_probe_rtt_rounds > 0.0) { 
            double now = /* get current time */; 
            double elapsed = now - bbr_probe_rtt_start_ts; 
            double remaining = bbr_probe_rtt_done_ts - now; 
            if (elapsed < remaining) { 
                // Delay sending packet
                return; 
            } 
            else { 
                // End BBR probe 
                bbr_probe_rtt_rounds = 0.0; 
            } 
        } 
 
        // Send packet 
        // ... 
 
        // Update BBR variables 
        double now = /* get current time */; 
        double send_elapsed = now - bbr_last_sample; 
        bbr_last_sample = now; 
        double send_rate = 1.0 / send_elapsed; 
        bbr_full_bw = std::max(bbr_full_bw, send_rate); 
        bbr_min_rtt = (bbr_min_rtt > 0.0) ? std::min(int(bbr_min_rtt), rtt) : rtt; 
        bbr_extra_acked += std::max(0.0, double(acked_bytes - cwnd)); 
 
        // Start BBR probe if necessary 
        if (bbr_probe_rtt_rounds == 0.0 && send_rate > bbr_full_bw * bbr_btlbw) { 
            bbr_probe_rtt_start_ts = now; 
            bbr_probe_rtt_done_ts = now + bbr_min_rtt * 1.5; 
            bbr_probe_rtt_rounds = 1.0; 
        } 
    } 
 
    int getCwnd() const { 
        return cwnd; 
    } 
 
    int getSsthresh() const { 
        return ssthresh; 
    } 
};