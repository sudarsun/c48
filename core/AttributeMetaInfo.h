#ifndef _ATTRIBUTEMETAINFO_
#define _ATTRIBUTEMETAINFO_

#include <string>
#include <limits>

class Attribute;


class AttributeMetaInfo
{

protected:
	int mOrdering;
	bool mIsRegular;
	bool mIsAveragable;
	bool mHasZeropoint;
	double mLowerBound;
	bool mLowerBoundIsOpen;
	double mUpperBound;
	bool mUpperBoundIsOpen;

public:
	AttributeMetaInfo();
	AttributeMetaInfo(Attribute *);

private:
	void setMetadata(Attribute *att);
	void setNumericRange(const std::string &rangeString);
};

#endif	// _ATTRIBUTEMETAINFO_
