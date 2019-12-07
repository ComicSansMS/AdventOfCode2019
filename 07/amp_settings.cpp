#include <amp_settings.hpp>

//#include <range/v3/algorithm/permutation.hpp>
//#include <range/v3/view/iota.hpp>

#include <algorithm>
#include <cassert>
#include <numeric>

int determineMaxSignal(IntegerProgram const& p)
{
    std::array<int, 5> phases;
    std::iota(begin(phases), end(phases), 0);
    int max_signal = 0;
    do {
        int signal = 0;
        for (auto const& phase : phases) {
            IntegerProgram ip = p;
            ip.input.push_back(phase);
            ip.input.push_back(signal);
            executeProgram(ip);
            assert(ip.pc == -1);
            assert(ip.output.size() == 1);
            signal = ip.output.front();
        }
        max_signal = std::max(max_signal, signal);
    } while (std::next_permutation(begin(phases), end(phases)));
    return max_signal;
}

int determineMaxSignalWithFeedback(IntegerProgram const& p)
{
    int constexpr n_amps = 5;
    std::array<int, n_amps> phases;
    std::iota(begin(phases), end(phases), 5);
    int max_signal = 0;
    do {
        int signal = 0;
        std::vector<IntegerProgram> amps(n_amps, p);
        for (int i = 0; i < n_amps; ++i) { amps[i].input.push_back(phases[i]); }
        int done_count = 0;
        for (int current_amp = 0; done_count != n_amps; current_amp = ((current_amp + 1) % n_amps)) {
            IntegerProgram& ip = amps[current_amp];
            ip.input.push_back(signal);
            assert(ip.pc >= 0);
            executeProgram(ip);
            assert(ip.output.size() == 1);
            signal = ip.output.front();
            ip.output.clear();
            if (ip.pc == -1) {
                ++done_count;
            } else {
                ip.pc = ip.resume_point;
            }
        }
        max_signal = std::max(max_signal, signal);
    } while (std::next_permutation(begin(phases), end(phases)));
    return max_signal;
}
