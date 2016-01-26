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


/**
 * Helper class for loading data from files. Reads a file that is C45 format. Can take a filestem or filestem with .names or .data appended.
 * Assumes that path/&lt;filestem&gt;.names and path/&lt;filestem&gt;.data exist and contain the names and data respectively.
 */
class DataSource {

private:

    /**
     * The source filename.
     */
    string mSourceFileData;
    string mSourceFileName;

    /**
     * Input stream for names file
     */
    std::fstream mNamesReader;

    /**
     *    Input stream for data file
     */
    std::fstream mDataReader;

    /**
     * Holds the filestem.
     */
    string mFileStem;

    /**
     * Number of attributes in the data (including ignore and label attributes).
     */
    int mNumAttribs;

    /**
     * Which attributes are ignore or label. These are *not* included in the
     * arff representation.
     */
    bool_array mIgnore;

    /** Holds the determined structure (header) of the data set. */
    Instances *mStructure;

    /**
     * Reads an instance using the supplied file stream.
     *
     * @param inStream the file stream to use
     * @return an Instance or null if there are no more instances to read
     * @exception IOException if an error occurs
     */
    Instances *getInstance(std::fstream& inStream) const;

    /**
     * Reads an instance from the given line.
     *
     * @param inLine the file stream to use
     * @return an Instance or null if there are no more instances to read
     * @exception IOException if an error occurs
     */
    Instance *getInstance(string inLine) const;

    /**
     * removes the trailing period
     *
     * @param val the string to work on
     * @return the processed string
     */
    string removeTrailingPeriod(string &val) const;

    /**
     * Reads header (from the names file) using the file stream.
     *
     * @param inStream the file stream to use
     * @exception IOException if an error occurs
     */
    void readHeader(std::fstream& inStream);

public:

    /**
     * Constructor for Data source
     *
     * @throws IOException if something goes wrong
     */
    DataSource(const string &location);

    /**
     * Returns the structure of the data set using the given class index.
     *
     * @param classIndex Class Index
     * @return the structure of the data set as an empty set of Instances
     * @exception IOException if an error occurs
     */
    Instances *getStructure(int classIndex);

    /**
     * Resets the Loader ready to read a new data set or the
     * same data set again.
     *
     * @throws IOException if something goes wrong
     */
    void reset();

    /**
     * Determines and returns (if possible) the structure (internally the
     * header) of the data set as an empty set of instances.
     *
     * @return the structure of the data set as an empty set of Instances
     * @exception IOException if an error occurs
     */
    Instances *getStructure();

    /**
     * Return the full data set. If the structure hasn't yet been determined
     * by a call to getStructure then method should do so before processing
     * the rest of the data set.
     *
     * @return the structure of the data set as an empty set of Instances
     * @exception IOException if there is no source or parsing fails
     */
    Instances *getDataSet();
};

#endif    // _DATASOURCE_
