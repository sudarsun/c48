#ifndef _DATASOURCE_
#define _DATASOURCE_

#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Typedefs.h"
class Instance;
class Instances;


/// <summary>
/// Utility routines for the converter package.
/// </summary>
class DataSource {
	/// <summary>
	/// Helper class for loading data from files. Before the data can be read again, one has to
	/// call the <code>reset</code> method. The data source can also be initialized
	/// with an Instances object, in order to provide a unified interface to files
	/// and already loaded datasets.
	/// </summary>
private:
	/// <summary>
	/// Describe variable <code>msourceFileData</code> here.
	/// </summary>
	string mSourceFileData;
	string mSourceFileName;

	/// <summary>
	/// Input stream for names file
	/// </summary>
	std::fstream mNamesReader;

	/// <summary>
	/// Input stream for data file
	/// </summary>
	std::fstream mDataReader;

	/// <summary>
	/// Holds the filestem.
	/// </summary>
	string mFileStem;

	/// <summary>
	/// Number of attributes in the data (including ignore and label attributes).
	/// </summary>
	int mNumAttribs;

	/// <summary>
	/// Which attributes are ignore or label. These are *not* included in the arff
	/// representation.
	/// </summary>
	bool_array mIgnore;

	Instances *mStructure;

	Instances *getInstance(std::fstream&);
	Instance *getInstance(string);
	string removeTrailingPeriod(string &val);
	void readHeader(std::fstream&);

public:

	DataSource(const string &location);
	Instances *getDataSet(int classIndex);
	Instances *getStructure(int classIndex);
	bool hasElements(Instances *structure);
	Instance *nextElement(Instances *dataset);
	void reset();
	Instances *getStructure();
	Instances *getDataSet();
	Instance *getNextInstance(Instances *structure);
};

#endif	// _DATASOURCE_
