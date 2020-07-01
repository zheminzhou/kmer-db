/*
This file is a part of Kmer-db software distributed under GNU GPL 3 licence.
The homepage of the Kmer-db project is http://sun.aei.polsl.pl/REFRESH/kmer-db

Authors: Sebastian Deorowicz, Adam Gudys, Maciej Dlugosz, Marek Kokot, Agnieszka Danek

*/

#pragma once
#include "input_file.h"
#include <stdexcept>
#include <functional>

// *****************************************************************************************
//

class Params {
public:


	static const string MODE_BUILD;
	static const string MODE_MINHASH;
	static const string MODE_ALL_2_ALL;
	static const string MODE_NEW_2_ALL;
	static const string MODE_ONE_2_ALL;
	static const string MODE_DISTANCE;

	static const string SWITCH_KMC_SAMPLES;
	static const string SWITCH_MINHASH_SAMPLES;
	static const string SWITCH_MULTISAMPLE_FASTA;
	static const string SWITCH_PHYLIP_OUT;

	static const string COMPACT_DB;
	
	static const string OPTION_FRACTION;
	static const string OPTION_FRACTION_START;
	static const string OPTION_LENGTH;
	static const string OPTION_VERBOSE;
	static const string OPTION_DEBUG;
	static const string OPTION_THREADS;
	static const string OPTION_READER_THREADS;
	static const string OPTION_BUFFER;
	

};

class Console
{
public:
	using metric_fun_t = std::function<double(size_t, size_t, size_t, int)>;
	
	Console();

	int parse(int argc, char** argv);

protected:
	int numThreads;
	int numReaderThreads;
	int cacheBufferMb;
	bool multisampleFasta;

	std::map<std::string, metric_fun_t> availableMetrics;

	int runBuildDatabase(const std::string& multipleSamples, const std::string dbFilename, 
		InputFile::Format inputFormat, double fraction, double fractionStart, uint32_t kmerLength);
	int runAllVsAll(const std::string& dbFilename, const std::string& similarityFile);
	int runNewVsAll(const std::string& dbFilename, const std::string& multipleSamples, const std::string& similarityFilename, InputFile::Format inputFormat);
	int runOneVsAll(const std::string& dbFilename, const std::string& singleKmcSample, const std::string& similarityFilename, InputFile::Format inputFormat);

	int runMinHash(const std::string& multipleKmcSamples, InputFile::Format inputFormat, double fraction, uint32_t kmerLength);
	int runDistanceCalculation(const std::string& similarityFilename, const std::vector<string>& metricNames, bool usePhylip);
	int runDistanceCalculationPhylip(const std::string& similarityFilename, const std::vector<string>& metricNames);

	int runListPatterns(const std::string& dbFilename, const std::string& patternFile);
	int runAnalyzeDatabase(const std::string& multipleKmcSamples, const std::string& dbFilename);

	void showInstructions();

	bool findSwitch(std::vector<std::string>& params, const std::string& name) {
		auto it = find(params.begin(), params.end(), name); // verbose mode
		if (it != params.end()) {
			params.erase(it);
			return true;
		}

		return false;
	}

	template <typename T>
	bool findOption(std::vector<std::string>& params, const std::string& name, T& v) {
		auto prevToEnd = std::prev(params.end());
		auto it = find(params.begin(), prevToEnd, name); // verbose mode
		if (it != prevToEnd) {
			std::istringstream iss(*std::next(it));
			if (iss >> v) {
				params.erase(it, it + 2);
				return true;
			}
		}

		return false;
	}
};