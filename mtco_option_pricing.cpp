#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include "mtco_option_pricing.h"

// Function to generate normally distributed (Gaussian) random numbers
double generateGaussianNoise(double mean, double stddev) {
    static std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(generator);
}

// Function to calculate the payoff of a European call option (not early)
double callOptionPayoff(double curMarketPrice, double strikePrice) {
    return std::max(curMarketPrice - strikePrice, 0.0);
}

// Function to calculate the payoff of a European put option (not early)
double putOptionPayoff(double curMarketPrice, double strikePrice) {
    return std::max(strikePrice - curMarketPrice, 0.0);
}

// Monte Carlo Simulation for European option pricing
double monteCarloOptionPricing(double initialStockPrice, double strikePrice, double riskFreeRate, double sigma, double timeToMaturity, int numSimulations, bool isCallOption) {
    double payoffSum = 0.0;

    for(int i = 0; i < numSimulations; ++i) {
        // Generate a random price path
        double ST = initialStockPrice * std::exp((riskFreeRate - 0.5 * sigma * sigma) * timeToMaturity + sigma * std::sqrt(timeToMaturity) * generateGaussianNoise(0.0, 1.0));

        // Calculate the payoff for this path
        double payoff = isCallOption ? callOptionPayoff(ST, strikePrice) : putOptionPayoff(ST, strikePrice);

        // Accumulate the payoff
        payoffSum += payoff;
    }

    // Calculate the average payoff and discount it to present value
    double averagePayoff = payoffSum / static_cast<double>(numSimulations);
    return std::exp(-riskFreeRate * timeToMaturity) * averagePayoff;
}

double getDoubleInput()
{
    double input;
    std::cin >> input;
    return input;
}

int getIntInput()
{
    int input;
    std::cin >> input;
    return input;
}

int main() {
    // Option parameters (make dynamic)
    std::cout << "Initial stock price (double; ex: 100.0): ";
    double initialStockPrice = getDoubleInput();
    std::cout << "Strike price (double; ex: 100.0): ";
    double strikePrice = getDoubleInput();
    std::cout << "Risk-free rate (double; ex: 0.05): ";
    double riskFreeRate = getDoubleInput();
    std::cout << "Volatility/sigma (double; ex: 0.2): ";
    double sigma = getDoubleInput();
    std::cout << "Time to maturity (double; ex: 1.0): ";
    double timeToMaturity = getDoubleInput();
    std::cout << "Number of Simulations (int; ex: 100000): ";
    int numSimulations = getIntInput();

    // Calculate option prices
    double callPrice = monteCarloOptionPricing(initialStockPrice, strikePrice, riskFreeRate, sigma, timeToMaturity, numSimulations, true);
    double putPrice = monteCarloOptionPricing(initialStockPrice, strikePrice, riskFreeRate, sigma, timeToMaturity, numSimulations, false);

    // Output the results
    std::cout << "European Call Option Price: " << callPrice << std::endl;
    std::cout << "European Put Option Price: " << putPrice << std::endl;

    return 0;
}