/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * URL: https://github.com/martinruenz/gnuplot-cpp
 * AUTHOR: Martin Rünz, 2015
 * Modifications: Lennart Steffen, 2022
 */

#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <stdexcept>

class GnuplotPipe {
public:
	GnuplotPipe() = default;

	bool isOpen() const { return pipe != nullptr; }

	void open(bool persist = true) {
		if ( isOpen() )
			return;
		pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
		if (!pipe)
			throw std::runtime_error("Failed to open gnuplot pipe!");
	}

	inline virtual ~GnuplotPipe(){
		if (pipe) pclose(pipe);
	}

	void sendLine(const std::string& text, bool useBuffer = false){
		if (!pipe) return;
		if (useBuffer)
			buffer.push_back(text + "\n");
		else
			fputs((text + "\n").c_str(), pipe);
	}

	void sendEndOfData(unsigned repeatBuffer = 1){
		if (!pipe) return;
		for (unsigned i = 0; i < repeatBuffer; i++) {
			for (auto& line : buffer) fputs(line.c_str(), pipe);
			fputs("\n", pipe);
		}
		fflush(pipe);
		buffer.clear();
	}

	void sendNewDataBlock(){
		sendLine("\n", !buffer.empty());
	}

	void writeBufferToFile(const std::string& fileName){
		std::ofstream fileOut(fileName);
		for (auto& line : buffer) fileOut << line;
		fileOut.close();
	}

private:
	GnuplotPipe(GnuplotPipe const&) = delete;
	void operator=(GnuplotPipe const&) = delete;

	FILE* pipe {nullptr};
	std::vector<std::string> buffer;
};
