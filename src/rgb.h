#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

class rgb 
{
	private:
		uint8_t r, g, b, a; // Color components

	public:
		rgb(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) 
			: r(r), g(g), b(b), a(a) {}

		// Getters
		uint8_t getR() const { return r; }
		uint8_t getG() const { return g; }
		uint8_t getB() const { return b; }
		uint8_t getA() const { return a; }

		// Setters
		void setR(uint8_t r) { this->r = r; }
		void setG(uint8_t g) { this->g = g; }
		void setB(uint8_t b) { this->b = b; }
		void setA(uint8_t a) { this->a = a; }
};